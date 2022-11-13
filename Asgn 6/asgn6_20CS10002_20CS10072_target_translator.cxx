/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/

#include "asgn6_20CS10002_20CS10072_translator.h"
#include <fstream>
#include <sstream>
#include <stack>
using namespace std;

// External variables
extern SymbolTable globalST;
extern SymbolTable* ST;
extern quadArray quadList;

// Declare global variables
vector<string> stringConsts;
map<int, string> labels;
stack<pair<string, int>> parameters;
int labelCount = 0;
string funcRunning = "";
string FileName;


// Add Global Variables and declarations to assembly code.
void genGlobalVar(ofstream& file) {

        for(auto s : globalST.Symbols){

        DataType type = s->type.type;
        SymbolValue* init = s->initVal;
        string name = s->name;

        if(type == CHAR && name[0] != 't') {
            if(init != NULL) {
                file << "\t.globl\t" << name << endl;
                file << "\t.data" << endl;
                file << "\t.type\t" << name << ", @object" << endl;
                file << "\t.size\t" << name << ", 1" << endl;
                file << name << ":" << endl;
                file << "\t.byte\t" << init->c << endl;
            }
            else
                file << "\t.comm\t" << name << ",1,1" << endl;
        }
        else if(type == INT && name[0] != 't') {
            if(init != NULL) {
                file << "\t.globl\t" << name << endl;
                file << "\t.data" << endl;
                file << "\t.align\t4" << endl;
                file << "\t.type\t" << name << ", @object" << endl;
                file << "\t.size\t" << name << ", 4" << endl;
                file << name << ":" << endl;
                file << "\t.long\t" << init->i << endl;
            }
            else
                file << "\t.comm\t" << name << ",4,4" << endl;
        }
    }

}

// Create labels for all strings used throughout the code in assembly code.
// Also print / add the string to those labels.
void genStrLabel(ofstream& file) {

    file << ".section\t.rodata" << endl;
    
    // Maintain a count of number of strings 
    int i = 0;
    
    for(vector<string>::iterator it = stringConsts.begin(); it != stringConsts.end(); it++) {
        file << ".LC" << i++ << ":" << endl;
        file << "\t.string " << *it << endl;
    }

}

// Generate labels for GOTO targets
void genLabel() {
    // int i = 0;
    for(vector<quad>::iterator it = quadList.quads.begin(); it != quadList.quads.end(); it++) {

            opcode op = it->op;
            if(op == GOTO || op ==  GOTO_EQ || op == GOTO_NEQ || op == GOTO_GT || op == GOTO_LT || op ==  GOTO_GTE || op == GOTO_LTE || op ==  IF_GOTO || op == IF_FALSE_GOTO){

            int target = atoi((it->result.c_str()));
            if(!labels.count(target)) {
                string labelName = ".L" + to_string(labelCount++);
                labels[target] = labelName;
            }
            it->result = labels[target];
        }
    }
    
}

// At the beginning of each function, allocate variables on stack and adjust the stack pointer.
// Function Prologue : Entering a function.
void genFuncPrlg(int memBind, ofstream& file) {
    int width = 16;
    file << endl << "\t.text" << endl;
    file << "\t.globl\t" << funcRunning << endl;
    file << "\t.type\t" << funcRunning << ", @function" << endl;
    file << funcRunning << ":" << endl;
    file << "\tpushq\t" << "%rbp" << endl;
    file << "\tmovq\t" << "%rsp, %rbp" << endl;
    file << "\tsubq\t$" << (memBind / width + 1) * width << ", %rsp" << endl;
}

// Generate assembly code from Quads
// Converts Three Address Code to assembly code
void quadCode(quad q, ofstream& file) {

    string strLabel = q.result;
    string toPrint1 = "", toPrint2 = "", toPrintRes = "";
    
    int off1 = 0, off2 = 0, offRes = 0;

    // Get Symbol for the three arguments in Symbol Table
    Symbol* loc1 = ST->lookup(q.arg1);
    Symbol* loc2 = ST->lookup(q.arg2);
    Symbol* loc3 = ST->lookup(q.result);
    
    // Search for the three arguments in Global Symbol Table
    Symbol* glb1 = globalST.searchGlobal(q.arg1);
    Symbol* glb2 = globalST.searchGlobal(q.arg2);
    Symbol* glb3 = globalST.searchGlobal(q.result);

    if(ST != &globalST) {

        if(glb1 == NULL)
            off1 = loc1->offset;
        
        if(glb2 == NULL)
            off2 = loc2->offset;
        
        if(glb3 == NULL)
            offRes = loc3->offset;

        if(q.arg1[0] < '0' || q.arg1[0] > '9') {
            if(glb1 != NULL)
                toPrint1 = q.arg1 + "(%rip)";
            else
                toPrint1 = to_string(off1) + "(%rbp)";
        }
        if(q.arg2[0] < '0' || q.arg2[0] > '9') {
            if(glb2 != NULL)
                toPrint2 = q.arg2 + "(%rip)";
            else
                toPrint2 = to_string(off2) + "(%rbp)";
        }
        if(q.result[0] < '0' || q.result[0] > '9') {
            if(glb3 != NULL)
                toPrintRes = q.result + "(%rip)";
            else
                toPrintRes = to_string(offRes) + "(%rbp)";
        }
    }
    else {
        toPrint1 = q.arg1;
        toPrint2 = q.arg2;
        toPrintRes = q.result;
    }

    if(q.result[0] == '.' && q.result[1] == 'L' && q.result[2] == 'C')
        toPrintRes = strLabel;

    if(q.op == ASSIGN) {
        if(q.result[0] != 't' || loc3->type.type == INT || loc3->type.type == POINTER) {
            if(loc3->type.type != POINTER) {
                if(q.arg1[0] < '0' || q.arg1[0] > '9')
                {
                    file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
                    file << "\tmovl\t%eax, " << toPrintRes << endl; 
                }
                else
                    file << "\tmovl\t$" << q.arg1 << ", " << toPrintRes << endl;
            }
            else {
                file << "\tmovq\t" << toPrint1 << ", %rax" << endl;
                file << "\tmovq\t%rax, " << toPrintRes << endl; 
            }
        }
        else {
            int temp = q.arg1[0];
            file << "\tmovb\t$" << temp << ", " << toPrintRes << endl;
        }
    }
    else if(q.op == UNARY_MINUS) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tnegl\t%eax" << endl;
        file << "\tmovl\t%eax, " << toPrintRes << endl;
    }
    else if(q.op == ADD) {
        if(q.arg1[0] > '0' && q.arg1[0] <= '9')
            file << "\tmovl\t$" << q.arg1 << ", %eax" << endl;
        else
            file << "\tmovl\t" << toPrint1 << ", %eax" << endl; 
        if(q.arg2[0] > '0' && q.arg2[0] <= '9')
            file << "\tmovl\t$" << q.arg2 << ", %edx" << endl;
        else
            file << "\tmovl\t" << toPrint2 << ", %edx" << endl; 
        file << "\taddl\t%edx, %eax" << endl;
        file << "\tmovl\t%eax, " << toPrintRes << endl;
    }
    else if(q.op == SUB) {
        if(q.arg1[0] > '0' && q.arg1[0] <= '9')
            file << "\tmovl\t$" << q.arg1 << ", %edx" << endl;
        else
            file << "\tmovl\t" << toPrint1 << ", %edx" << endl; 
        if(q.arg2[0]>'0' && q.arg2[0]<='9')
            file << "\tmovl\t$" << q.arg2 << ", %eax" << endl;
        else
            file << "\tmovl\t" << toPrint2 << ", %eax" << endl; 
        file << "\tsubl\t%eax, %edx" << endl;
        file << "\tmovl\t%edx, %eax" << endl;
        file << "\tmovl\t%eax, " << toPrintRes << endl;
    }
    else if(q.op == MULT) {
        if(q.arg1[0] > '0' && q.arg1[0] <= '9')
            file << "\tmovl\t$" << q.arg1 << ", %eax" << endl;
        else
            file << "\tmovl\t" << toPrint1 << ", %eax" << endl; 
        file << "\timull\t";
        if(q.arg2[0] > '0' && q.arg2[0] <= '9')
            file << "$" << q.arg2 << ", %eax" << endl;
        else
            file << toPrint2 << ", %eax" << endl;
        file << "\tmovl\t%eax, " << toPrintRes << endl;
    }
    else if(q.op == DIV) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tcltd\n\tidivl\t" << toPrint2 << endl;
        file << "\tmovl\t%eax, " << toPrintRes << endl;
    }
    else if(q.op == MOD) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tcltd\n\tidivl\t" << toPrint2 << endl;
        file << "\tmovl\t%edx, " << toPrintRes << endl;
    }
    else if(q.op == GOTO)
        file << "\tjmp\t" << q.result << endl;
    else if(q.op == GOTO_LT) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tcmpl\t" << toPrint2 << ", %eax" << endl;
        file << "\tjge\t.L" << labelCount << endl;
        file << "\tjmp\t" << q.result << endl;
        file << ".L" << labelCount++ << ":" << endl;
    }
    else if(q.op == GOTO_GT) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tcmpl\t" << toPrint2 << ", %eax" << endl;
        file << "\tjle\t.L" << labelCount << endl;
        file << "\tjmp\t" << q.result << endl;
        file << ".L" << labelCount++ << ":" << endl;
    }
    else if(q.op == GOTO_GTE) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tcmpl\t" << toPrint2 << ", %eax" << endl;
        file << "\tjl\t.L" << labelCount << endl;
        file << "\tjmp\t" << q.result << endl;
        file << ".L" << labelCount++ << ":" << endl;
    }
    else if(q.op == GOTO_LTE) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tcmpl\t" << toPrint2 << ", %eax" << endl;
        file << "\tjg\t.L" << labelCount << endl;
        file << "\tjmp\t" << q.result << endl;
        file << ".L" << labelCount++ << ":" << endl;
    }
    else if(q.op == GOTO_GTE) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tcmpl\t" << toPrint2 << ", %eax" << endl;
        file << "\tjl\t.L" << labelCount << endl;
        file << "\tjmp\t" << q.result << endl;
        file << ".L" << labelCount++ << ":" << endl;
    }
    else if(q.op == GOTO_EQ) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        if(q.arg2[0] >= '0' && q.arg2[0] <= '9')
            file << "\tcmpl\t$" << q.arg2 << ", %eax" << endl;
        else
            file << "\tcmpl\t" << toPrint2 << ", %eax" << endl;
        file << "\tjne\t.L" << labelCount << endl;
        file << "\tjmp\t" << q.result << endl;
        file << ".L" << labelCount++ << ":" << endl;
    }
    else if(q.op == GOTO_NEQ) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tcmpl\t" << toPrint2 << ", %eax" << endl;
        file << "\tje\t.L" << labelCount << endl;
        file << "\tjmp\t" << q.result << endl;
        file << ".L" << labelCount++ << ":" << endl;
    }
    else if(q.op == IF_GOTO) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tcmpl\t$0" << ", %eax" << endl;
        file << "\tje\t.L" << labelCount << endl;
        file << "\tjmp\t" << q.result << endl;
        file << ".L" << labelCount++ << ":" << endl;
    }
    else if(q.op == IF_FALSE_GOTO) {
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tcmpl\t$0" << ", %eax" << endl;
        file << "\tjne\t.L" << labelCount << endl;
        file << "\tjmp\t" << q.result << endl;
        file << ".L" << labelCount++ << ":" << endl;
    }
    else if(q.op == ARR_IDX_ARG) {
        file << "\tmovl\t" << toPrint2 << ", %edx" << endl;
        file << "cltq" << endl;
        if(off1 < 0) {
            file << "\tmovl\t" << off1 << "(%rbp,%rdx,1), %eax" << endl;
            file << "\tmovl\t%eax, " << toPrintRes << endl;
        }
        else {
            file << "\tmovq\t" << off1 << "(%rbp), %rdi" << endl;
            file << "\taddq\t%rdi, %rdx" << endl;
            file << "\tmovq\t(%rdx) ,%rax" << endl;
            file << "\tmovq\t%rax, " << toPrintRes << endl;
        }
    }
    else if(q.op == ARR_IDX_RES) {
        file << "\tmovl\t" << toPrint2 << ", %edx" << endl;
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "cltq" << endl;
        if(offRes > 0) {
            file << "\tmovq\t" << offRes << "(%rbp), %rdi" << endl;
            file << "\taddq\t%rdi, %rdx" << endl;
            file << "\tmovl\t%eax, (%rdx)" << endl;
        }
        else
            file << "\tmovl\t%eax, " << offRes << "(%rbp,%rdx,1)" << endl;
    }
    else if(q.op == REFERENCE) {
        if(off1 < 0) {
            file << "\tleaq\t" << toPrint1 << ", %rax" << endl;
            file << "\tmovq\t%rax, " << toPrintRes << endl;
        }
        else {
            file << "\tmovq\t" << toPrint1 << ", %rax" << endl;
            file << "\tmovq\t%rax, " << toPrintRes << endl;
        }
    }
    else if(q.op == DEREFERENCE) {
        file << "\tmovq\t" << toPrint1 << ", %rax" << endl;
        file << "\tmovq\t(%rax), %rdx" << endl;
        file << "\tmovq\t%rdx, " << toPrintRes << endl;
    }
    else if(q.op == LEFT_DEREFERENCE) {
        file << "\tmovq\t" << toPrintRes << ", %rdx" << endl;
        file << "\tmovl\t" << toPrint1 << ", %eax" << endl;
        file << "\tmovl\t%eax, (%rdx)" << endl;
    }
    else if(q.op == PARAM) {
        int paramSize;
        DataType t;
        if(glb3 != NULL)
            t = glb3->type.type;
        else
            t = loc3->type.type;
        if(t == INT)
            paramSize = SIZE_OF_INT;
        else if(t == CHAR)
            paramSize = SIZE_OF_CHAR;
        else
            paramSize = SIZE_OF_PTR;
        stringstream ss;
        if(q.result[0] == '.')
            ss << "\tmovq\t$" << toPrintRes << ", %rax" <<endl;
        else if(q.result[0] >= '0' && q.result[0] <= '9')
            ss << "\tmovq\t$" << q.result << ", %rax" <<endl;
        else {
            if(loc3->type.type != ARRAY) {
                if(loc3->type.type != POINTER)
                    ss << "\tmovq\t" << toPrintRes << ", %rax" <<endl;
                else if(loc3 == NULL)
                    ss << "\tleaq\t" << toPrintRes << ", %rax" <<endl;
                else
                    ss << "\tmovq\t" << toPrintRes << ", %rax" <<endl;
            }
            else {
                if(offRes < 0)
                    ss << "\tleaq\t" << toPrintRes << ", %rax" <<endl;
                else {
                    ss << "\tmovq\t" << offRes << "(%rbp), %rdi" <<endl;
                    ss << "\tmovq\t%rdi, %rax" <<endl;
                }
            }
        }
        parameters.push(make_pair(ss.str(), paramSize));
    }
    else if(q.op == CALL) {
        int numParams = atoi(q.arg1.c_str());
        int totalSize = 0, k = 0;

        // We need different registers based on the parameters
        if(numParams > 6) {
            for(int i = 0; i < numParams - 6; i++) {
                string s = parameters.top().first;
                file << s << "\tpushq\t%rax" << endl;
                totalSize += parameters.top().second;
                parameters.pop();
            }
            file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %r9d" << endl;
            totalSize += parameters.top().second;
            parameters.pop();
            file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %r8d" << endl;
            totalSize += parameters.top().second;				
            parameters.pop();
            file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %rcx" << endl;
            totalSize += parameters.top().second;
            parameters.pop();
            file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %rdx" << endl;
            totalSize += parameters.top().second;
            parameters.pop();
            file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %rsi" << endl;
            totalSize += parameters.top().second;
            parameters.pop();
            file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %rdi" << endl;
            totalSize += parameters.top().second;
            parameters.pop();
        }
        else {
            while(!parameters.empty()) {
                if(parameters.size() == 6) {
                    file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %r9d" << endl;
                    totalSize += parameters.top().second;
                    parameters.pop();
                }
                else if(parameters.size() == 5) {
                    file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %r8d" << endl;
                    totalSize += parameters.top().second;
                    parameters.pop();
                }
                else if(parameters.size() == 4) {
                    file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %rcx" << endl;
                    totalSize += parameters.top().second;
                    parameters.pop();
                }
                else if(parameters.size() == 3) {
                    file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %rdx" << endl;
                    totalSize += parameters.top().second;
                    parameters.pop();
                }
                else if(parameters.size() == 2) {
                    file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %rsi" << endl;
                    totalSize += parameters.top().second;
                    parameters.pop();
                }
                else if(parameters.size() == 1) {
                    file << parameters.top().first << "\tpushq\t%rax" << endl << "\tmovq\t%rax, %rdi" << endl;
                    totalSize += parameters.top().second;
                    parameters.pop();
                }
            }
        }
        file << "\tcall\t" << q.result << endl;
        if(q.arg2 != "")
            file << "\tmovq\t%rax, " << toPrint2 << endl;
        file << "\taddq\t$" << totalSize << ", %rsp" << endl;
    }
    else if(q.op == RETURN) {
        if(q.result != "")
            file << "\tmovq\t" << toPrintRes << ", %rax" << endl;
        file << "\tleave" << endl;
        file << "\tret" << endl;
    }

}

// The primary function that invokes all other necessary functions to produce the target assembly code
void MakeAsm(ofstream& file) {

    // Add all global variables and String labels to assembly code before beginning the main body.
    genGlobalVar(file);
    genStrLabel(file);

    
    
    // For maintaing the function that program is currently inside
    Symbol* currFunc = NULL;
    // Symbol Table for the function
    SymbolTable* FuncST = NULL;
    
    genLabel();

    for(int i = 0; i < quadList.quads.size(); i++) {

        // We print the quads to help understand where the next lines of assembly code came from
        // and their functionality.
        file << "# " << quadList.quads[i].print() << endl;
        if(labels.count(i))
            file << labels[i] << ":" << endl;

        // Need to perform certain steps whenever entering the function
        if(quadList.quads[i].op == FUNC_BEG) {
            
            i++;
            
            if(quadList.quads[i].op != FUNC_END)
                i--;
            else
                continue;

            // Get/Create Symbol Table for new function. 
            currFunc = globalST.searchGlobal(quadList.quads[i].result);
            FuncST = currFunc->nestedST;
            
            int takingParam = 1, memBind = 16;
            ST = FuncST;
            
            for(int j = 0; j < FuncST->Symbols.size(); j++) {
            
                if(FuncST->Symbols[j]->name == "RETVAL") {
                    takingParam = 0;
                    memBind = 0;
                    if(FuncST->Symbols.size() > j+1)
                        memBind = -FuncST->Symbols[j+1]->size;
                }
            
                else {
            
                    if(!takingParam) {
                        FuncST->Symbols[j]->offset = memBind;
                        if(FuncST->Symbols.size() > j+1)
                            memBind -= FuncST->Symbols[j+1]->size;
                    }
            
                    else {
                        FuncST->Symbols[j]->offset = memBind;
                        memBind += 8;
                    }
                }
            }

            if(memBind >= 0)
                memBind = 0;
            else
                memBind = - memBind;

            funcRunning = quadList.quads[i].result;

            genFuncPrlg(memBind, file);
        }

        // Function Epilogue : Exiting a function
        else if(quadList.quads[i].op == FUNC_END) {
            ST = &globalST;
            funcRunning = "";
            file << "\tleave" << endl;
            file << "\tret" << endl;
            file << "\t.size\t" << quadList.quads[i].result << ", .-" << quadList.quads[i].result << endl;
        }

        if(funcRunning != "")
            quadCode(quadList.quads[i], file);
    }

}

int main(int argc, char* argv[]) {

    // Initialise the current Symbol table as being Global Symbol Table
    ST = &globalST;

    // Parse the C file
    yyparse();

    // Print TAC
    quadList.print();       
    cout << endl;

    // Print the Symbol Tables, starting with the global table
    ST->print("GlobalST");         

    ST = &globalST;

    // Store the Three Address Code as a assembly file
    FileName = "asm_" + string(argv[argc - 1]) + ".s";
    ofstream file;
    file.open(FileName);

    // Generate the target assembly code
    MakeAsm(file);      

    file.close();

    return 0;
}
