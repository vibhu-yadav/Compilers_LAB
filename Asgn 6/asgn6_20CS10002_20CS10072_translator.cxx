/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/

#include "asgn6_20CS10002_20CS10072_translator.h"
#include <iomanip>
using namespace std;

// Initialize the global variables
int nextinstr = 0;

// Initialise the Count of Temporaries as 0
// This is static variable to maintain the count of temps across the Symbol tables consistently.
int SymbolTable::tempCount = 0;

quadArray quadList;
SymbolTable globalST;
SymbolTable* ST;


/*
    Implenting Functions defined in header file for SymbolValue
*/
void SymbolValue::setInitVal(int val) {
    this -> c = val;
    this -> i = val;
    this -> f = val;
    this -> p = NULL;
}

void SymbolValue::setInitVal(char val) {
    this -> c = val;
    this -> i = val;
    this -> f = val;
    this -> p = NULL;
}

void SymbolValue::setInitVal(float val) {
    this -> c = val;
    this -> i = val;
    this -> f = val;
    this -> p = NULL;
}


/*
    Implenting Functions defined in header file for Symbol
*/
Symbol::Symbol(){
    this -> nestedST = NULL;
}

/*
    Implenting Functions defined in header file for SymbolTable
*/
SymbolTable::SymbolTable(){
    this -> offset = 0;
}

// Symbol Table Lookup
Symbol* SymbolTable::lookup(string name, DataType t, int pc) {

    if(!table.count(name)) {

        // Make a new variable / Symbol in the Symbol table if it doesn't exist already.
        // Initialise the attributes of the Symbol.
        Symbol* s = new Symbol();
        s->name = name;
        s->type.type = t;
        s->offset = offset;
        s->initVal = NULL;


        if(!pc) {
            s->size = sizeOfType(t);
            offset += s->size;
        }
        else {
            s->size = SIZE_OF_PTR;
            s->type.nextType = t;
            s->type.pointers = pc;
            s->type.type = ARRAY;
        }

        Symbols.push_back(s);
        table[name] = s;
    }

    return table[name];

}

// Search the Symbol Table for a given Symbol / variable
Symbol* SymbolTable::searchGlobal(string name) {

    if(table.count(name))
        return table[name];
    
    return NULL;
}

// Create a new temp variable in the Symbol Table
string SymbolTable::gentemp(DataType t) {

    // Create a new temp and assign it a name
    string tempName = "t" + to_string(this->tempCount);
    this->tempCount++;
    
    // Initialize the Symbol for the temp
    Symbol* s = new Symbol();
    s->name = tempName;
    s->size = sizeOfType(t);
    s->offset = offset;
    s->type.type = t;
    s->initVal = NULL;

    // Update Symbol Table offset and add Symbol to the list
    offset += s->size;
    Symbols.push_back(s);
    
    // Add the temp to Symbol Table
    table[tempName] = s;  
    
    return tempName;
}

// Print the Symbol Table
void SymbolTable::print(string tableName) {

    for(int i = 0; i < 160; i++) cout << '='; 
    cout << endl;

    cout << "Symbol Table: " << setfill(' ') << left << setw(50) << tableName << endl;

    for(int i = 0; i < 160; i++) cout << '='; 
    cout << endl;


    // Table Headers
    cout << setfill(' ') << left << setw(30) <<  "Name";
    cout << left << setw(30) << "Type";
    cout << left << setw(40) << "Initial Value";
    cout << left << setw(20) << "Size";
    cout << left << setw(20) << "Offset";
    cout << left << "Nested Symbol Table" << endl;

    for(int i = 0; i < 160; i++) cout << '='; 
    cout << endl;


    // For storing nested Symbol tables
    typedef pair<string, SymbolTable*> pairStrST;
    vector <pairStrST> tableList;

    // Print the Symbol Table's contents
    for(auto s : Symbols){

        cout << left << setw(30) << s->name;
        cout << left << setw(30) << checkType(s->type);
        cout << left << setw(40) << getInitVal(s);
        cout << left << setw(20) << s->size;
        cout << left << setw(20) << s->offset;
        cout << left;

        // Add name of nested table to table list
        if(s->nestedST != NULL){
            string name = tableName + "." + s->name ;
            cout << name << endl;
            tableList.push_back({name,s->nestedST});
        }
        else
            cout << "NULL" << endl;
    }

    for(int i = 0; i < 160; i++) cout << '='; 
    cout << endl << endl;


    // Print all the nested tables
    vector<pairStrST> :: iterator it;
    for(it = tableList.begin(); it != tableList.end(); it++) {
        
        pairStrST p = *it;
        p.second->print(p.first);
    
    }

}


/*
    Implenting Functions defined in header file for Quads
*/
quad::quad(string __res, string __arg1, string __arg2, opcode __op){
    
    this -> op = __op;
    this -> arg1 = __arg1;
    this -> arg2 = __arg2;
    this -> result = __res;
}
// Printing a Three Address Code
string quad::print() {
    
    opcode op = this->op;
    string arg1 = this->arg1;
    string arg2 = this->arg2;
    string result = this->result;

    string out;

    // For Binary Operators
    if(op == ADD || op ==  SUB || op ==  MULT || op ==  DIV || op ==  MOD || op == SL || op == SR || op == LOGIC_AND || op == LOGIC_OR || op == LOGIC_XOR)
    {                 

        string opSymbol;

        if(op == ADD) 
            opSymbol = "+";

        else if(op == SUB) 
            opSymbol = "-";
        
        else if(op == MULT) 
            opSymbol = "*";
        
        else if(op == DIV) 
            opSymbol = "/";
        
        else if(op == MOD) 
            opSymbol = "%";
        
        else if(op == LOGIC_AND) 
            opSymbol = "&";
        
        else if(op == LOGIC_OR) 
            opSymbol = "|";
        
        else if(op == LOGIC_XOR) 
            opSymbol = "^";

        else if(op == SL)
            opSymbol = "<<";
        
        else if(op == SR)
            opSymbol = ">>";

        out = result + " = " + arg1 + " " + opSymbol + " " + arg2;

    }

    // For Unary operators
    else if(op == LOGIC_UNARY_NOT || op == UNARY_PLUS || op == UNARY_MINUS || op ==  REFERENCE || op == DEREFERENCE || op == UNARY_NEG){  

        string opSymbol;

        if(op == LOGIC_UNARY_NOT) 
            opSymbol = "~";
        
        else if(op == UNARY_PLUS)
            opSymbol = "+";
        
        else if(op == UNARY_MINUS) 
            opSymbol = "-";

        else if(op == REFERENCE)
            opSymbol = "&";

        else if(op == DEREFERENCE)
            opSymbol = "*";

        else if(op == UNARY_NEG)
            opSymbol = "!";

        out = result + " = " + opSymbol + arg1;
        
    }
   
    // For Conditional Operators
    if(op == GOTO || op ==  GOTO_EQ || op == GOTO_NEQ || op == GOTO_GT || op == GOTO_LT || op ==  GOTO_GTE || op == GOTO_LTE || op ==  IF_GOTO || op == IF_FALSE_GOTO){

        string opSymbol;

        if(op == GOTO_EQ) 
            opSymbol = "==";
        
        else if(op == GOTO_NEQ) 
            opSymbol = "!=";
        
        else if(op == GOTO_LT) 
            opSymbol = "<";
        
        else if(op == GOTO_GT) 
            opSymbol = ">";
        
        else if(op == GOTO_LTE) 
            opSymbol = "<=";
        
        else if(op == GOTO_GTE) 
            opSymbol = ">=";

        else if(op == IF_GOTO)
            opSymbol = "!= 0";

        else if(op == IF_FALSE_GOTO) 
            opSymbol = "== 0";

        out = "if " + arg1 + " " + opSymbol + " " + arg2 + " goto " + result;

    }

    // For Type Conversion Functions
    else if(op == CtoI || op ==  ItoC || op ==  FtoI || op ==  ItoF || op ==  FtoC || op == CtoF){            

        string opSymbol;

        if(op == CtoI)
            opSymbol = "CharToInt";
        
        else if(op == ItoC)
            opSymbol = "IntToChar";
        
        else if(op == CtoF)
            opSymbol = "CharToFloat";
        
        else if(op == ItoF)
            opSymbol = "IntToFloat";
        
        else if(op == FtoC)
            opSymbol = "FloatToChar";
        
        else if(op == FtoI)
            opSymbol = "FloatToInt";

        out = result + " = " + opSymbol + "(" + arg1 + ")";

    }

    // For Assignment Operator
    else if(op == ASSIGN)                       
        out = result + " = " + arg1;
    
    // For GOTO
    else if(op == GOTO)                         
        out = "goto " + result;
    
    // For Returning value from a function
    else if(op == RETURN)                       
        out = "return " + result;

    // For Function Parameters
    else if(op == PARAM)                        
        out = "param " + result;

    // For Function Calls
    else if(op == CALL) {       

        if(arg2 != "")
            out = arg2 + " = " + "call " + result + ", " + arg1;
        else
            out = "call " + result + ", " + arg1;

    }

    // Indexing Arrays
    else if(op == ARR_IDX_ARG)                  
        out = result + " = " + arg1 + "[" + arg2 + "]";

    else if(op == ARR_IDX_RES)                  
        out = result + "[" + arg2 + "] = " + arg1;

    // For Entering and Exiting a Function
    else if(op == FUNC_BEG)                     
        out = result + ": ";

    else if(op == FUNC_END) 
        out = "function " + result + " ends";  
    
    // For Dereferencing
    else if(op == LEFT_DEREFERENCE)                   
        out = "*" + result + " = " + arg1;   

    return out;
}


/*
    Implenting Functions defined in header file for QuadArray
*/
void quadArray::print() {

    for(int i = 0; i < 160; i++) cout << '=';
    cout << endl;

    cout << "Intermediate Representation as Three Address Codes" << endl;

    for(int i = 0; i < 160; i++) cout << '=';
    cout << endl;


    // Print all Quads

    vector<quad> quads = this -> quads;
    
    for(int i = 0; i < this->quads.size(); i++) {

        opcode op = quads[i].op;

        if(op != FUNC_BEG && op != FUNC_END)
            cout << left << setw(4) << i << ":    ";

        else if(op == FUNC_BEG)
            cout << endl << left << setw(4) << i << ": ";
        
        else if(op == FUNC_END)
            cout << left << setw(4) << i << ": ";
        
        cout << quads[i].print() << endl;
    }
    
    cout << endl;
}


/*
    Implenting Constructor defined in header file for Expression
*/
expression::expression(){
    this -> fold = 0;
    this -> folder = NULL;
}


// Emit function to generate a quad and append it to quadlist
// Overloaded for different types of arguments

void emit(string result, string arg1, string arg2, opcode op) {

    quad q(result, arg1, arg2, op);
    quadList.quads.push_back(q);
    
    nextinstr++;

}

void emit(string result, int constant, opcode op) {

    quad q(result, to_string(constant), "", op);
    quadList.quads.push_back(q);
  
    nextinstr++;

}

void emit(string result, char constant, opcode op) {

    quad q(result, to_string(constant), "", op);
    quadList.quads.push_back(q);
   
    nextinstr++;

}

void emit(string result, float constant, opcode op) {

    quad q(result, to_string(constant), "", op);
    quadList.quads.push_back(q);

    nextinstr++;

}

// Make a new list with a single element = i.
list<int> makelist(int i) {

    list<int> l(1, i);
    return l;

}

// Merge two lists
list<int> merge(list<int> l1, list<int> l2) {

    l1.merge(l2);
    return l1;

}

// Function to perform backpatching
void backpatch(list<int> l, int address) {

    string str = to_string(address);
    auto it = l.begin();
    
    for(; it != l.end(); it++)
        quadList.quads[*it].result = str;

}

// Implementation of the overloaded convertToType functions
void convertToType(expression* arg, expression* res, DataType toType) {
    if(res->type == toType)
        return;

    if(res->type == FLOAT) {
        if(toType == INT)
            emit(arg->loc, res->loc, "", FtoI);
        else if(toType == CHAR)
            emit(arg->loc, res->loc, "", FtoC);
    }
    else if(res->type == INT) {
        if(toType == FLOAT)
            emit(arg->loc, res->loc, "", ItoF);
        else if(toType == CHAR)
            emit(arg->loc, res->loc, "", ItoC);
    }
    else if(res->type == CHAR) {
        if(toType == FLOAT)
            emit(arg->loc, res->loc, "", CtoF);
        else if(toType == INT)
            emit(arg->loc, res->loc, "", CtoI);
    }
}

void convertToType(string t, DataType to, string f, DataType from) {
    if(to == from)
        return;
    
    if(from == FLOAT) {
        if(to == INT)
            emit(t, f, "", FtoI);
        else if(to == CHAR)
            emit(t, f, "", FtoC);
    }
    else if(from == INT) {
        if(to == FLOAT)
            emit(t, f, "", ItoF);
        else if(to == CHAR)
            emit(t, f, "", ItoC);
    }
    else if(from == CHAR) {
        if(to == FLOAT)
            emit(t, f, "", CtoF);
        else if(to == INT)
            emit(t, f, "", CtoI);
    }
}

// Function for Integer to Boolean conversion
void convertInt2Bool(expression* expr) {

    if(expr->type != BOOL) {
    
        expr->type = BOOL;
    
        // Create a TrueList
        expr->truelist = makelist(nextinstr);     
        emit("", "", "", GOTO);

        // Create a FalseList
        expr->falselist = makelist(nextinstr);    
        emit("", expr->loc, "", IF_FALSE_GOTO);
    }

}

// Function to get size of a type
int sizeOfType(DataType t) {
    if(t == VOID)
        return SIZE_OF_VOID;
    else if(t == CHAR)
        return SIZE_OF_CHAR;
    else if(t == INT)
        return SIZE_OF_INT;
    else if(t == POINTER)
        return SIZE_OF_PTR;
    else if(t == FLOAT)
        return SIZE_OF_FLOAT;
    else if(t == FUNCTION)
        return SIZE_OF_FUNC;
    else
        return 0;
}

// Function to check type of a variable
string checkType(Type t) {

    int type = t.type;
    int nextType = t.nextType;

    if(type == VOID)
        return "void";
    
    else if(type == CHAR)
        return "char";
    
    else if(type == INT)
        return "int";
    
    else if(type == FLOAT)
        return "float";
    
    else if(type == FUNCTION)
        return "function";

        // Check Type for Arrays

    else if(type == ARRAY) {  

        string tp = "";
        
        if(nextType == CHAR)
            tp += "char";
        
        else if(nextType == INT)
            tp += "int";
        
        else if(nextType == FLOAT)
            tp += "float";
        

        for(auto dim : t.dims){
            if(!dim)
                tp += "[]";
            else
                tp += "[" + to_string(dim) + "]";
        }
        
        if(t.dims.size() == 0)
            tp += "[]";
        
        return tp;
    }

    // Check Type for Pointers

    else if(type == POINTER) {        
        string tp = "";
        
        if(nextType == CHAR)
            tp += "char";
        
        else if(nextType == INT)
            tp += "int";
        
        else if(nextType == FLOAT)
            tp += "float";

        tp += string(t.pointers, '*');
        
        return tp;
    }

    else
        return "unknown";
}

// Function to get the initial value of a variable
string getInitVal(Symbol* s) {

    int type = s->type.type;
    auto init = s->initVal;

    if(init != NULL) {
        
        if(type == INT)
            return to_string(init->i);
        
        else if(type == CHAR)
            return to_string(init->c);
        
        else if(type == FLOAT)
            return to_string(init->f);
        
        else
            return "-";
    }

    else
        return "-";
}
