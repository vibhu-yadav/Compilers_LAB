/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/

#include <iomanip>
#include "asgn5_20CS10002_20CS10072_translator.h"
using namespace std;

// Global variables 

Symbol* curr_Symbol;
SymbolTable* curr_ST;
SymbolTable* globalST;
QuadArray QuadList;
int num_tables;
string loop_type;

// Stores the type of current variable
string curr_var_type;

/*
    Implenting Functions defined in header file for Type
*/

// Constructor

Type::Type(string __type, Type* __type_of_array, int __size){

    this -> type = __type;
    this -> size = __size;
    this -> type_of_array = __type_of_array;

}

/*
    Implenting Functions defined in header file for Symbol
*/

// Constructor
// t -> type of Symbol, __type -> case of nested array, __size -> size of array
Symbol::Symbol(string __name, string __t, Type* __type_of_array, int __size){

        this -> name = __name;
        this -> type = new Type(__t, __type_of_array, __size);
        this -> size = getSize(this->type);
        this -> value = "-";
        this -> offset = 0;
        this -> nestedTable = NULL;
    
}

// Update when typecasting
Symbol* Symbol::update(Type* __t){
    
        this -> type = __t;
        this -> size = getSize(__t);

        return this;
    
}

/*
    Implenting Functions defined in header file for SymbolTable
*/

SymbolTable::SymbolTable(string __name){
    this -> name = __name;
    this -> num_temp = 0;
}


// Find Symbol in the Symbol table
Symbol* SymbolTable::lookup(string __name) {

    for(list<Symbol>::iterator it = table.begin(); it != table.end(); it++) {
        if(it->name == __name) {
            // it is of type list<Symbol>::iterator
            // Need to return Symbol*, so dereference it and return pointer
            return &(*it);
        }
    }

    // When a Symbol isn't found in the current Symbol table
    // It could still be present in the parent  Symbol table
    
    Symbol* sym = NULL;

    if(this->parent != NULL) {
        sym = this->parent->lookup(__name);
    }

    if(sym != NULL) {
        // If the Symbol is present in some Parent Table
        return sym;
    }

    if(curr_ST == this && sym == NULL) {
        // If the Symbol is not found
        // Create and add it to Symbol Table and return a pointer to it
        Symbol* new_sym = new Symbol(__name);
        this -> table.push_back(*new_sym);
        return &(table.back());
    }

    return NULL;
}


Symbol* SymbolTable::gentemp(Type* t, string initValue) {
    
    // Create a new temporary Symbol and initialise it.
    string name = "t" + convertInt2String(curr_ST->num_temp++);
    Symbol* sym = new Symbol(name);
    sym->type = t;
    sym->value = initValue;         
    sym->size = getSize(t);

    // Add the temporary Symbol to the Symbol table
    curr_ST->table.push_back(*sym);
    return &(curr_ST->table.back());
}

void SymbolTable::print() {
    
    for(int i = 0; i < 160; i++) cout << '='; 
    cout << endl;

    cout << "Symbol Table for " << setfill(' ') << left << setw(50) << this->name;
    cout << "Parent Symbol Table: " << setfill(' ') << left << setw(50) << ((this->parent != NULL) ? this->parent->name : "NULL") << endl;
    
    for(int i = 0; i < 160; i++) cout << '=';
    cout << endl;

    // Columns of the Symbol Table
    // Used setw to align the columns
    cout << setfill(' ') << left << setw(30) <<  "Name";
    cout << left << setw(30) << "Type";
    cout << left << setw(40) << "Value";
    cout << left << setw(20) << "Size";
    cout << left << setw(20) << "Offset";
    cout << left << "Nested Symbol Table" << endl;

    for(int i = 0; i < 160; i++) cout << '=';
    cout << endl;

    list<SymbolTable*> tableList;

    // Print the Symbols in the Symbol table
    for(list<Symbol>::iterator it = this->table.begin(); it != this->table.end(); it++) {
        cout << left << setw(30) << it->name;
        cout << left << setw(30) << checkType(it->type);
        cout << left << setw(40) << (it->value != "" ? it->value : "-");
        cout << left << setw(20) << it->size;
        cout << left << setw(20) << it->offset;
        cout << left;

        if(it->nestedTable != NULL) {
            cout << it->nestedTable->name << endl;
            tableList.push_back(it->nestedTable);
        }
        else {
            cout << "NULL" << endl;
        }
    }

    for(int i = 0; i < 160; i++) cout << '=';
    cout << endl << endl;

    // Print all the nested tables
    for(list<SymbolTable*>::iterator it = tableList.begin(); it != tableList.end(); it++) {
        (*it)->print();
    }

}

void SymbolTable::update() {
    list<SymbolTable*> tableList;
    int off_set;

    // Update the offsets in Symbol Table based on size / width of previous elements
    for(list<Symbol>::iterator it = table.begin(); it != table.end(); it++) {
        if(it == table.begin()) {
            it->offset = 0;
            off_set = it->size;
        }
        else {
            it->offset = off_set;
            off_set = it->offset + it->size;
        }

        if(it->nestedTable != NULL) {
            tableList.push_back(it->nestedTable);
        }
    }

    // Use update recursive to update offsets of nested tables
    for(list<SymbolTable*>::iterator iter = tableList.begin(); iter != tableList.end(); iter++) {
        (*iter)->update();
    }
}

/*
    Implenting Functions defined in header file for Quads
*/
Quad::Quad(string res, string __arg1, string operation, string __arg2) {

    this -> result = res;
    this -> arg1 = __arg1;
    this -> op = operation;
    this -> arg2 = __arg2;

}

Quad::Quad(string res, int __arg1, string operation, string __arg2): result(res), op(operation), arg2(__arg2) {

    this -> result = res;
    arg1 = convertInt2String(__arg1);
    this -> op = operation;
    this -> arg2 = __arg2;
}

Quad::Quad(string res, float __arg1, string operation, string __arg2): result(res), op(operation), arg2(__arg2) {
    
    this -> result = res;
    arg1 = convertFloat2String(__arg1);
    this -> op = operation;
    this -> arg2 = __arg2;

}

void Quad::print() {

    if(op == "=")       
        cout << result << " = " << arg1;
    
    else if(op == "*=")
        cout << "*" << result << " = " << arg1;
    
    // Array Indexing
    else if(op == "[]=")
        cout << result << "[" << arg1 << "]" << " = " << arg2;
    
    // Array Indexing
    else if(op == "=[]")
        cout << result << " = " << arg1 << "[" << arg2 << "]";
    
    else if(op == "goto" || op == "param" || op == "return")
        cout << op << " " << result;
    
    else if(op == "call")
        cout << result << " = " << "call " << arg1 << ", " << arg2;
    
    else if(op == "label")
        cout << result << ": ";

    // Unary operators
    else if(op == "= &" || op == "= *" || op == "= -" || op == "= ~" || op == "= !")
        cout << result << " " << op << arg1;

    // Binary Operators
    else if(op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || op == "^" || op == "|" || op == "&" || op == "<<" || op == ">>")
        cout << result << " = " << arg1 << " " << op << " " << arg2;

    // Relational Operators
    else if(op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=")
        cout << "if " << arg1 << " " << op << " " << arg2 << " goto " << result;

    else
        cout << "Unknown Operator";

}

/*
    Implenting Functions defined in header file for QuadArray
*/
void QuadArray::print() {
   
    for(int i = 0; i < 160; i++) cout << '=';
    cout << endl;

    cout << "Intermediate Representation as Three Address Codes" << endl;

    for(int i = 0; i < 160; i++) cout << '=';
    cout << endl;

    int cnt = 0;

    // Print all Quads
    for(vector<Quad>::iterator it = this->Quads.begin(); it != this->Quads.end(); it++, cnt++) {
        if(it->op != "label") {
            cout << left << setw(4) << cnt << ":    ";
            it->print();
        }
        else {
            cout << endl << left << setw(4) << cnt << ": ";
            it->print();
        }
        cout << endl;
    }
}

// Overloaded versions of emit function for bison parser
void emit(string op, string result, string arg1, string arg2) {
    Quad* q = new Quad(result, arg1, op, arg2);
    QuadList.Quads.push_back(*q);
}

void emit(string op, string result, int arg1, string arg2) {
    Quad* q = new Quad(result, arg1, op, arg2);
    QuadList.Quads.push_back(*q);
}

void emit(string op, string result, float arg1, string arg2) {
    Quad* q = new Quad(result, arg1, op, arg2);
    QuadList.Quads.push_back(*q);
}

// Implementation of the makelist function
list<int> makelist(int i) {
    list<int> l(1, i);
    return l;
}

// Implementation of the merge function
list<int> merge(list<int> &list1, list<int> &list2) {
    list1.merge(list2);
    return list1;
}

// Implementation of the backpatch function
void backpatch(list<int> l, int address) {
    string str = convertInt2String(address);
    for(list<int>::iterator it = l.begin(); it != l.end(); it++) {
        QuadList.Quads[*it].result = str;
    }
}

/*
    Implementation of typeCheck function
    Checks if the type of the two arguments are compatible
*/
bool typecheck(Symbol* &s1, Symbol* &s2) {

    Type* t1 = s1->type;
    Type* t2 = s2->type;

    if(typecheck(t1, t2))
        return true;
    else if(s1 = TypeCast(s1, t2->type))
        return true;
    else if(s2 = TypeCast(s2, t1->type))
        return true;
    else
        return false;
}

bool typecheck(Type* t1, Type* t2) {

    if(t1 == NULL && t2 == NULL)
        return true;
    else if(t1 == NULL || t2 == NULL)
        return false;
    else if(t1->type != t2->type)
        return false;

    return typecheck(t1->type_of_array, t2->type_of_array);
}

/*
    Implementation of the TypeCast function
    This function is used to change the type of a Symbol
    to the type of another Symbol
*/
Symbol* TypeCast(Symbol* s, string t) {
    Symbol* temp = SymbolTable::gentemp(new Type(t));

    if(s->type->type == "float") {
        if(t == "int") {
            emit("=", temp->name, "float2int(" + s->name + ")");
            return temp;
        }
        else if(t == "char") {
            emit("=", temp->name, "float2char(" + s->name + ")");
            return temp;
        }
        return s;
    }

    else if(s->type->type == "int") {
        if(t == "float") {
            emit("=", temp->name, "int2float(" + s->name + ")");
            return temp;
        }
        else if(t == "char") {
            emit("=", temp->name, "int2char(" + s->name + ")");
            return temp;
        }
        return s;
    }

    else if(s->type->type == "char") {
        if(t == "float") {
            emit("=", temp->name, "char2float(" + s->name + ")");
            return temp;
        }
        else if(t == "int") {
            emit("=", temp->name, "char2int(" + s->name + ")");
            return temp;
        }
        return s;
    }

    return s;
}

// Integer to String conversion function
string convertInt2String(int i) {
    return to_string(i);
}

// Float to String conversion function
string convertFloat2String(float f) {
    return to_string(f);
}

// Integer to Boolean conversion function
expression* convertInt2Bool(expression* expr) {
    if(expr->type != "bool") {
        expr->falselist = makelist(nextinstr());    // Add falselist for boolean expressions
        emit("==", expr->pos->name, "0");
        expr->truelist = makelist(nextinstr());     // Add truelist for boolean expressions
        emit("goto", "");
    }
    return expr;
}

// Boolean to Integer conversion function
expression* convertBool2Int(expression* expr) {
    if(expr->type == "bool") {
        expr->pos = SymbolTable::gentemp(new Type("int"));
        backpatch(expr->truelist, nextinstr());
        emit("=", expr->pos->name, "true");
        emit("goto", convertInt2String(nextinstr() + 1));
        backpatch(expr->falselist, nextinstr());
        emit("=", expr->pos->name, "false");
    }
    return expr;
}

void changeTable(SymbolTable* newTable) {
    curr_ST = newTable;
}

int nextinstr() {
    return QuadList.Quads.size();
}

// Function to get size of a type
// Implemented for fundamental data types as well as arrays.
int getSize(Type* t) {
    if(t->type == "void")
        return SIZE_OF_VOID;
    else if(t->type == "char")
        return SIZE_OF_CHAR;
    else if(t->type == "int")
        return SIZE_OF_INT;
    else if(t->type == "ptr")
        return SIZE_OF_PTR;
    else if(t->type == "float")
        return SIZE_OF_FLOAT;
    else if(t->type == "array")
        return t->size * getSize(t->type_of_array);
    else if(t->type == "func")
        return SIZE_OF_FUNC;
    else
        return -1;
}

// Function to check type of a variable
string checkType(Type* t) {
    if(t == NULL)
        return "null";
    else if(t->type == "void" || t->type == "char" || t->type == "int" || t->type == "float" || t->type == "bposk" || t->type == "func")
        return t->type;
    else if(t->type == "ptr")
        return "ptr(" + checkType(t->type_of_array) + ")";
    else if(t->type == "array")
        return "arr(" + convertInt2String(t->size) + ", " + checkType(t->type_of_array) + ")";
    else
        return "unknown";
}

int main() {


    // Initially 0 tables
    num_tables = 0;                        
    // Make a Global Symbol Table                       
    globalST = new SymbolTable("Global");  
    // Initialise the current Symbol Table to be the Global Symbol Table
    curr_ST = globalST;                   
    // No loops at the starting.
    loop_type = "";


    /*
    https://stackoverflow.com/questions/41400976/why-does-bison-yacc-print-new-lines-for-apparently-no-reason
    yyparse() generating extra newlines due to missing '\r'
    */
    yyparse();

    globalST->update();
    
    // Print TAC
    QuadList.print();       
    cout << endl;
    
    // Print Symbol Tables
    globalST->print();    

    return 0;
}
