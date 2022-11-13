/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/
#include <iostream>
#include <vector>
#include <list>
using namespace std;

// Sizes of fundamental data types
// These can be changed according to the system architecture by simply modifying these macros.

#define SIZE_OF_VOID 0
#define SIZE_OF_FUNC 0
#define SIZE_OF_CHAR 1
#define SIZE_OF_INT 4
#define SIZE_OF_PTR 4
#define SIZE_OF_FLOAT 8

// Forward Declaration to avoid circular dependency
class Type;

class Symbol;
class SymbolTable;

class Quad;
class QuadArray;

// Extern variables that are used in prototyping
extern Symbol* curr_Symbol;
extern SymbolTable* curr_ST;
extern SymbolTable* globalST;
extern QuadArray QuadList;
extern int num_tables;
extern string loop_type;

extern char* yytext;
extern int yyparse();

/*

Structure of Symbol Table

===================================================================
Name      Type     Initial Value   Size    Offset    Nested Table
===================================================================

*/

class Type {
public:

    // Type and Size of entry element
    // If the element is an array, it'll be of format : array(size, type_of_array)
    string type;
    int size;
    Type* type_of_array;

    // Constructor
    // As a default, it will be a single element and thus wont have a nested Type.
    Type(string __type, Type* __type_of_array = NULL, int __size = 1);
};

class Symbol {
public:

    // Name of the Symbol
    string name;
    // Type of the Symbol
    Type* type;
    // Initial Value of the Symbol
    string value;
    // Size of the Symbol
    int size;
    // Offset of the Symbol
    int offset;
    // Nested Table of the Symbol
    SymbolTable* nestedTable;

    // Constructor
    // Defaults are required to be able to create a Symbol without any arguments (besides name).    
    Symbol(string __name, string __t = "int", Type* __type_of_array = NULL, int __size = 0);

    // Update the Symbol Table
    Symbol* update(Type* __t);
};

class SymbolTable {

public:

    // Name of the Symbol table
    string name;
    // The Symbol Table itself
    int num_temp;
    // The parent Table it may be embedded into
    list<Symbol> table;
    // Number of Temporary Variables
    SymbolTable* parent;

    // Constructor
    SymbolTable(string __name = "NULL");

    Symbol* lookup(string name);
    static Symbol* gentemp(Type* t, string initValue = "");

    void print();
    void update();
};

// Represents a Quad in the Intermediate Code
// Contains at most 3 addresses (result, op1, op2) and a operator
class Quad {

public:
    
    string op;
    string arg1;
    string arg2;
    string result;

    // Constructor
    // The order is chosen as result = arg1 op arg2
    // This is because at times we might not have arg2, and op might be unary or simply assignment
    Quad(string __res, string arg1_, string operation = "=", string arg2_ = "");
    Quad(string __res, int arg1_, string operation = "=", string arg2_ = "");
    Quad(string __res, float arg1_, string operation = "=", string arg2_ = "");

    void print();
};

// Represents a list of Quads
class QuadArray {

public:

    // Vector to allow random access
    vector<Quad> Quads;

    // Print all the Quads
    void print();
};

// Represents arrays of any given type
class Array {

public:

    string array_type;
    // Address of the array
    Symbol* pos;
    // Entry corresponding to the array in the Symbol table
    Symbol* Array;
    // Type of Array
    Type* type;

};

class statement {

public:
    // Nextlist of the statement
    list<int> nextlist;

};


class expression {

public:

    string type;
    // Type of the expression
    Symbol* pos;
    // Pointer to entry in Symbol table 
    list<int> truelist;
    list<int> falselist;
    // Truelist and Falselist for boolean expressions
    list<int> nextlist;
    // Nextlist for statement expressions

};

// Overloaded versions of emit function for bison parser
// Generates a quad of the form: result = arg1 op arg2
// arg2 is optional, thus defaulted to ""
void emit(string op, string result, string arg1 = "", string arg2 = "");
void emit(string op, string result, int arg1, string arg2 = "");
void emit(string op, string result, float arg1, string arg2 = "");

// Generate a new list containing only i
list<int> makelist(int i);

// Merge function to concatenate two lists and return the merged list
list<int> merge(list<int> &list1, list<int> &list2);

// Backpatching function to assign target label
void backpatch(list<int> l, int address);

// Check compatibility of the types 
// Overloaded for Type and Symbols
bool typecheck(Symbol* &s1, Symbol* &s2);
bool typecheck(Type* t1, Type* t2);

// Typecast Symbol s to type t
Symbol* TypeCast(Symbol* s, string t);

// Convert Integer to String
string convertInt2String(int i);

// Convert Float to String
string convertFloat2String(float f);

// Convert Integer to Boolean
expression* convertInt2Bool(expression* expr);

// Convert Boolean to Integer
expression* convertBool2Int(expression* expr);

// Changes the current Symbol table to newTable
void changeTable(SymbolTable* newTable);

// Get the next instruction
int nextinstr();

// Get the size of a type
int getSize(Type* t);

// Check & Print type
string checkType(Type* t);