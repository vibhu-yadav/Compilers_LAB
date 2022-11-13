/*
    Abhibhu Prakash - 20CS10002
    Vibhu           - 20CS10072
    Compilers Lab   - Assignment 5
*/

#include <iostream>
#include <vector>
#include <list>
#include <map>
using namespace std;

// Sizes of fundamental data types
// These can be changed according to the system architecture by simply modifying these macros.

#define SIZE_OF_VOID 0
#define SIZE_OF_FUNC 0
#define SIZE_OF_CHAR 1
#define SIZE_OF_INT 4
#define SIZE_OF_PTR 8
#define SIZE_OF_FLOAT 8

// Fundamental Data Types
typedef enum {
    VOID,
    BOOL,
    CHAR,
    INT,
    FLOAT,
    ARRAY,
    POINTER,
    FUNCTION
} DataType;

// To depict the various operations
// and help create opcodes for them
typedef enum  {
    
    // Basic Arithmetic and SHifting
    ADD, SUB, MULT, DIV, MOD, SL, SR, 
    // Logical Binary Operations
    LOGIC_AND, LOGIC_OR, LOGIC_XOR, LOGIC_UNARY_NOT,
    // Logical Unary Operations
    UNARY_PLUS, UNARY_MINUS, UNARY_NEG,
    // Referencing and De-referencing
    REFERENCE, DEREFERENCE,  LEFT_DEREFERENCE,
    // GOTO on various cases
    GOTO_EQ, GOTO_NEQ, GOTO_GT,GOTO_LT, GOTO_GTE, 
    GOTO_LTE, IF_GOTO, IF_FALSE_GOTO, 
    // Type Conversion
    CtoI, ItoC, FtoI, ItoF, FtoC ,CtoF, 
    ASSIGN, GOTO, RETURN, PARAM, CALL, 
    // For using array indexing on LHS and RHS
    ARR_IDX_ARG, ARR_IDX_RES, 
    // Function Begin and End
    FUNC_BEG, FUNC_END 
} opcode;

class Symbol;
class Type;
class SymbolValue;
class SymbolTable;

class quad;
class quadArray;


extern char* yytext;
extern int yyparse();


class Type {
public:
    int pointers;
    DataType type;
    DataType nextType;
    vector<int> dims;
};


class SymbolValue {
public:
    int i;
    char c;
    float f;
    void* p;

    void setInitVal(int val);
    void setInitVal(char val);
    void setInitVal(float val);
};

class Symbol {
public:
    string name;
    Type type;
    SymbolValue* initVal;
    int size;
    int offset;
    SymbolTable* nestedST;

    Symbol();
};


class SymbolTable {
public:
    map<string, Symbol*> table;
    vector<Symbol*> Symbols;
    int offset;
    static int tempCount;

    SymbolTable();
    Symbol* lookup(string name, DataType t = INT, int pc = 0);
    Symbol* searchGlobal(string name);
    string gentemp(DataType t = INT);

    void print(string tableName);
};


class quad {
public:
    opcode op;
    string arg1;
    string arg2;
    string result;

    quad(string, string, string, opcode);

    string print();
};


class quadArray {
public:
    vector<quad> quads;

    void print();
};


class param {
public:
    string name;
    Type type;
};


class expression {
public:
    int instr;
    DataType type;
    string loc;
    list<int> truelist;
    list<int> falselist;
    list<int> nextlist;
    int fold;
    string* folder;

    expression();
};


class declaration {
public:
    string name;
    int pointers;
    DataType type;
    DataType nextType;
    vector<int> li;
    expression* initVal;
    int pc;
};

void emit(string result, string arg1, string arg2, opcode op);
void emit(string result, int constant, opcode op);
void emit(string result, char constant, opcode op);
void emit(string result, float constant, opcode op);


list<int> makelist(int i);

list<int> merge(list<int> list1, list<int> list2);

void backpatch(list<int> l, int address);

void convertToType(expression* arg, expression* res, DataType toType);

void convertToType(string t, DataType to, string f, DataType from);

void convertInt2Bool(expression* expr);

int sizeOfType(DataType t);

string checkType(Type t);

string getInitVal(Symbol* s);
