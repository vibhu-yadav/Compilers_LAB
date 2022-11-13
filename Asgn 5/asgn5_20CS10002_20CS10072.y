%{
    /*
        Abhibhu Prakash - 20CS10002
        Vibhu           - 20CS10072
        Compilers Lab   - Assignment 5
    */

    #include <iostream>
    #include "asgn5_20CS10002_20CS10072_translator.h"
    using namespace std;

    extern int yylex();         
    void yyerror(string err);    
    extern char* yytext;       
    extern int yylineno;       
    extern string curr_var_type;   
%}  

%union {

    int intval;             // Integer
    char* strval;           //  String

    int instr;              // Instruction Number
    int ParamCount;         // Function Parameter Count

    char unaryOp;           // Unary operators
    Array* arr;             // Arrays    

    expression* expr;       // Expression
    statement* stmt;        // Statement
    
    Symbol* sym;            // Symbol
    Type* symType;          // Symbol Type
}


// Tokens for keywords
%token AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM EXTERN FLOAT FOR GOTO IF INLINE INT LONG REGISTER RESTRICT RETURN SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE BOOL COMPLEX IMAGINARY

// Tokens for various brackets
%token SQUARE_BRACKET_OPEN SQUARE_BRACKET_CLOSE ROUND_BRACKET_OPEN ROUND_BRACKET_CLOSE CURLY_BRACKET_OPEN CURLY_BRACKET_CLOSE 

// Tokens for operators
%token DOT ARROW INCREMENT DECREMENT BITWISE_AND MULTIPLY ADD SUBTRACT BITWISE_NOR NOT DIVIDE MODULO 

// Tokens for relational operators
%token LEFT_SHIFT RIGHT_SHIFT LESS_THAN GREATER_THAN LESS_THAN_EQUAL GREATER_THAN_EQUAL EQUAL NOT_EQUAL BITWISE_XOR BITWISE_OR 

// Tokens for logical operators
%token LOGICAL_AND LOGICAL_OR QUESTION_MARK COLON SEMICOLON ELLIPSIS 

// Tokens for assignment operators
%token ASSIGN MULTIPLY_ASSIGN DIVIDE_ASSIGN MODULO_ASSIGN ADD_ASSIGN SUBTRACT_ASSIGN LEFT_SHIFT_ASSIGN RIGHT_SHIFT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN COMMA HASH

// Identifiers are treated with type Symbol*
%token <sym> IDENTIFIER

// Integer constants are of type intval
%token <intval> INTEGER_CONSTANT

// Floating constants are of type strval
%token <strval> FLOATING_CONSTANT

// Character constants are of type strval
%token <strval> CHAR_CONSTANT

// String literals are of type strval
%token <strval> STRING_LITERAL

// Start Symbol
%start translation_unit

// Takes care of dangling else
%right THEN ELSE

// Non-terminals of type unaryOp (unary operator)
%type <unaryOp> unary_operator

// Non-terminals of type ParamCount (number of parameters)
%type <ParamCount> argument_expression_list argument_expression_list_opt

// Non-terminals of type expr (denoting expressions)
%type <expr> 
        expression primary_expression  multiplicative_expression additive_expression shift_expression relational_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression assignment_expression expression_statement

// Non-terminals of type stmt (denoting statements)
%type <stmt>
        statement compound_statement loop_statement selection_statement iteration_statement labeled_statement jump_statement block_item block_item_list block_item_list_opt

%type <symType> pointer

// Non-terminals of type symbol
%type <sym> constant initializer
%type <sym> direct_declarator init_declarator declarator

// Non-terminals of array
%type <arr> postfix_expression unary_expression cast_expression

// For backpatching
%type <instr> M 
// For control flow 
%type <stmt> N


%%

primary_expression: 
        IDENTIFIER
        {
            $$ = new expression();  // New expression
            $$->pos = $1;           // Store pointer for new entry in Symbol Table
            $$->type = "non_bool";
        }
        | constant
        {
            $$ = new expression();  // New expression
            $$->pos = $1;           // Store pointer for new entry in Symbol Table
        }
        | STRING_LITERAL
        {
            $$ = new expression();  // New expression
            $$->pos = SymbolTable::gentemp(new Type("ptr"), $1);  // Store into new temp 
            $$->pos->type->type_of_array = new Type("char");
        }
        | ROUND_BRACKET_OPEN expression ROUND_BRACKET_CLOSE
        {
            $$ = $2;    // assignment
        }
        ;

constant: 
        INTEGER_CONSTANT
        {
            $$ = SymbolTable::gentemp(new Type("int"), convertInt2String($1));   // Store into new temp 
            emit("=", $$->name, $1);
        }
        | FLOATING_CONSTANT
        {
            $$ = SymbolTable::gentemp(new Type("float"), string($1));     // Store into new temp 
            emit("=", $$->name, string($1));
        }
        | CHAR_CONSTANT
        {
            $$ = SymbolTable::gentemp(new Type("float"), string($1));     // Store into new temp 
            emit("=", $$->name, string($1));
        }
        ;

postfix_expression: 
        primary_expression
        {
            $$ = new Array();           // new Array
            $$->Array = $1->pos;        // Store address of  primary expression
            $$->type = $1->pos->type;   // Update the type
            $$->pos = $$->Array;
        }
        | postfix_expression SQUARE_BRACKET_OPEN expression SQUARE_BRACKET_CLOSE
        {
            $$ = new Array();               // new Array
            $$->type = $1->type->type_of_array;   // Set type as element type
            $$->Array = $1->Array;          // Copy base
            $$->pos = SymbolTable::gentemp(new Type("int"));  // Store address of new temp
            $$->array_type = "array";              // Set array_type to "array"

            // For an array, size = number of elements * size of each element
            if($1->array_type == "array") {        
                Symbol* sym = SymbolTable::gentemp(new Type("int"));
                int sz = getSize($$->type);
                emit("*", sym->name, $3->pos->name, convertInt2String(sz));
                emit("+", $$->pos->name, $1->pos->name, sym->name);
            }
            else {                          // Calculate size
                int sz = getSize($$->type);
                emit("*", $$->pos->name, $3->pos->name, convertInt2String(sz));
            }
        }
        | postfix_expression ROUND_BRACKET_OPEN argument_expression_list_opt ROUND_BRACKET_CLOSE
        {   
            // Call a function with its name and parameter list
            $$ = new Array();
            $$->Array = SymbolTable::gentemp($1->type);
            emit("call", $$->Array->name, $1->Array->name, convertInt2String($3));
        }
        | postfix_expression DOT IDENTIFIER
        { /* Do Nothing */ }
        | postfix_expression ARROW IDENTIFIER
        { /* Do Nothing */ }
        | postfix_expression INCREMENT
        {   
            $$ = new Array();
            $$->Array = SymbolTable::gentemp($1->Array->type);      // make new temp
            emit("=", $$->Array->name, $1->Array->name);            // assign previous value
            emit("+", $1->Array->name, $1->Array->name, "1");       // Add 1 to it
        }
        | postfix_expression DECREMENT
        {
            $$ = new Array();
            $$->Array = SymbolTable::gentemp($1->Array->type);      // make new temp
            emit("=", $$->Array->name, $1->Array->name);            // assign previous value
            emit("-", $1->Array->name, $1->Array->name, "1");       // Subtract 1 from it
        }
        | ROUND_BRACKET_OPEN type_name ROUND_BRACKET_CLOSE CURLY_BRACKET_OPEN initializer_list CURLY_BRACKET_CLOSE
        { /* Do Nothing */ }
        | ROUND_BRACKET_OPEN type_name ROUND_BRACKET_CLOSE CURLY_BRACKET_OPEN initializer_list COMMA CURLY_BRACKET_CLOSE
        { /* Do Nothing */ }
        ;

argument_expression_list_opt: 
        argument_expression_list
        {
            $$ = $1;    // Optional list, either it's same as list or empty
        }
        | %empty
        {
            $$ = 0;     // Set as 0
        }
        ;

argument_expression_list: 
        assignment_expression
        {
            $$ = 1;                         // Singe Argument
            emit("param", $1->pos->name);   // emit parameter
        }
        | argument_expression_list COMMA assignment_expression
        {
            $$ = $1 + 1;                    // Add another argument
            emit("param", $3->pos->name);   // emit parameter
        }
        ;

unary_expression: 
        postfix_expression
        {
            $$ = $1;    // Same as postfix expression
        }
        | INCREMENT unary_expression
        {
            emit("+", $2->Array->name, $2->Array->name, "1");   // Add 1
            $$ = $2;    // Assign to unary expression increment
        }
        | DECREMENT unary_expression
        {
            emit("-", $2->Array->name, $2->Array->name, "1");   // Subtract 1
            $$ = $2;    // Assign to unary expression decrement
        }
        | unary_operator cast_expression
        {
            // Case of unary operator
            $$ = new Array();
            switch($1) {
                case '&':   // Use Address 
                    $$->Array = SymbolTable::gentemp(new Type("ptr"));    // make pointer temp
                    $$->Array->type->type_of_array = $2->Array->type;                 // Assign corresponding type
                    emit("= &", $$->Array->name, $2->Array->name);              // Emit the Quad
                    break;
                case '*':   // De-referencing
                    $$->array_type = "ptr";
                    $$->pos = SymbolTable::gentemp($2->Array->type->type_of_array);   // make temp of suitable type
                    $$->Array = $2->Array;                                      // Assign
                    emit("= *", $$->pos->name, $2->Array->name);                // Emit the Quad
                    break;
                case '+':   // Unary plus
                    $$ = $2;    // assignment
                    break;
                case '-':   // Unary minus
                    $$->Array = SymbolTable::gentemp(new Type($2->Array->type->type));    // Generate temp of same base type
                    emit("= -", $$->Array->name, $2->Array->name);                              // Emit the Quad
                    break;
                case '~':   // Bitwise not
                    $$->Array = SymbolTable::gentemp(new Type($2->Array->type->type));    // Generate temp of same base type
                    emit("= ~", $$->Array->name, $2->Array->name);                              // Emit the Quad
                    break;
                case '!':   // Logical not 
                    $$->Array = SymbolTable::gentemp(new Type($2->Array->type->type));    // Generate temp of same base type
                    emit("= !", $$->Array->name, $2->Array->name);                              // Emit the Quad
                    break;
            }
        }
        | SIZEOF unary_expression
        { /* Do Nothing */ }
        | SIZEOF ROUND_BRACKET_OPEN type_name ROUND_BRACKET_CLOSE
        { /* Do Nothing */ }
        ;

unary_operator:
        BITWISE_AND
        {
            $$ = '&'; // Logical bitwise AND
        }
        | MULTIPLY
        {
            $$ = '*'; // multiply
        }
        | ADD
        {
            $$ = '+'; // Add
        }
        | SUBTRACT
        {
            $$ = '-'; // Subtract
        }
        | BITWISE_NOR
        {
            $$ = '~'; // Logical Bitwise NOR
        }
        | NOT // Logical NOT
        {
            $$ = '!';
        }
        ;

cast_expression: 
        unary_expression
        {
            $$ = $1;    // assignment
        }
        | ROUND_BRACKET_OPEN type_name ROUND_BRACKET_CLOSE cast_expression
        {
            $$ = new Array();
            $$->Array = TypeCast($4->Array, curr_var_type);    // make new Symbol of suitable type
        }
        ;

multiplicative_expression: 
        cast_expression
        {
            $$ = new expression();          // Generate new expression
            if($1->array_type == "array") {        // array_type "array"
                $$->pos = SymbolTable::gentemp($1->pos->type);  // Generate new temp
                emit("=[]", $$->pos->name, $1->Array->name, $1->pos->name);     // Emit the Quad
            }
            else if($1->array_type == "ptr") {   // array_type "ptr"
                $$->pos = $1->pos;          // Assign the Symbol table entry
            }
            else {
                $$->pos = $1->Array;
            }
        }
        | multiplicative_expression MULTIPLY cast_expression
        {   
            // Multiplication
            if(typecheck($1->pos, $3->Array)) {     // Type Check
                $$ = new expression();                                                  // Generate new expression
                $$->pos = SymbolTable::gentemp(new Type($1->pos->type->type));    // Generate new temp
                emit("*", $$->pos->name, $1->pos->name, $3->Array->name);               // Emit the Quad
            }
            else {
                yyerror("Type Error");
            }
        }
        | multiplicative_expression DIVIDE cast_expression
        {
            // Division
            if(typecheck($1->pos, $3->Array)) {     // Type Check
                $$ = new expression();                                                  // Generate new expression
                $$->pos = SymbolTable::gentemp(new Type($1->pos->type->type));    // Generate new temp
                emit("/", $$->pos->name, $1->pos->name, $3->Array->name);               // Emit the Quad
            }
            else {
                yyerror("Type Error");
            }
        }
        | multiplicative_expression MODULO cast_expression
        {
            // Indicates modulo
            if(typecheck($1->pos, $3->Array)) {     // Type Check
                $$ = new expression();                                                  // Generate new expression
                $$->pos = SymbolTable::gentemp(new Type($1->pos->type->type));    // Generate new temp
                emit("%", $$->pos->name, $1->pos->name, $3->Array->name);               // Emit the Quad
            }
            else {
                yyerror("Type Error");
            }
        }
        ;

additive_expression: 
        multiplicative_expression
        {
            $$ = $1;    // assignment
        }
        | additive_expression ADD multiplicative_expression
        {   
            // Indicates addition
            if(typecheck($1->pos, $3->pos)) {       // Type Check
                $$ = new expression();                                                  // Generate new expression
                $$->pos = SymbolTable::gentemp(new Type($1->pos->type->type));    // Generate new temp
                emit("+", $$->pos->name, $1->pos->name, $3->pos->name);                 // Emit the Quad
            }
            else {
                yyerror("Type Error");
            }
        }
        | additive_expression SUBTRACT multiplicative_expression
        {
            // Indicates subtraction
            if(typecheck($1->pos, $3->pos)) {       // Type Check
                $$ = new expression();                                                  // Generate new expression
                $$->pos = SymbolTable::gentemp(new Type($1->pos->type->type));    // Generate new temp
                emit("-", $$->pos->name, $1->pos->name, $3->pos->name);                 // Emit the Quad
            }
            else {
                yyerror("Type Error");
            }
        }
        ;

shift_expression: 
        additive_expression
        {
            $$ = $1;    // assignment
        }
        | shift_expression LEFT_SHIFT additive_expression
        {
            // Left shift
            if($3->pos->type->type == "int") {      // Type Check for int
                $$ = new expression();                                      // Generate new expression
                $$->pos = SymbolTable::gentemp(new Type("int"));      // Generate new temp
                emit("<<", $$->pos->name, $1->pos->name, $3->pos->name);    // Emit the Quad
            }
            else {
                yyerror("Type Error");
            }
        }
        | shift_expression RIGHT_SHIFT additive_expression
        {
            // Right shift
            if($3->pos->type->type == "int") {      // Type Check for int
                $$ = new expression();                                      // Generate new expression
                $$->pos = SymbolTable::gentemp(new Type("int"));      // Generate new temp
                emit(">>", $$->pos->name, $1->pos->name, $3->pos->name);    // Emit the Quad
            }
            else {
                yyerror("Type Error");
            }
        }
        ;

relational_expression: 
        shift_expression
        {
            $$ = $1;    // assignment
        }
        | relational_expression LESS_THAN shift_expression
        {
            if(typecheck($1->pos, $3->pos)) {                   // Type Check
                $$ = new expression();                          // Make new bool expression
                $$->type = "bool";
                $$->truelist = makelist(nextinstr());           // Create truelist for boolean expression
                $$->falselist = makelist(nextinstr() + 1);      // Create falselist for boolean expression
                emit("<", "", $1->pos->name, $3->pos->name);    // Emit "if x < y goto " , to be updated by backpatch
                emit("goto", "");                               // Emit "goto " , to be updated by backpatch
            }
            else {
                yyerror("Type Error");
            }
        }
        | relational_expression GREATER_THAN shift_expression
        {
            if(typecheck($1->pos, $3->pos)) {                   // Type Check
                $$ = new expression();                          // Make new bool expression
                $$->type = "bool";
                $$->truelist = makelist(nextinstr());           // Create truelist for boolean expression
                $$->falselist = makelist(nextinstr() + 1);      // Create falselist for boolean expression
                emit(">", "", $1->pos->name, $3->pos->name);    // Emit "if x > y goto ", to be updated by backpatch
                emit("goto", "");                               // Emit "goto ", to be updated by backpatch
            }
            else {
                yyerror("Type Error");
            }
        }
        | relational_expression LESS_THAN_EQUAL shift_expression
        {
            if(typecheck($1->pos, $3->pos)) {                   // Type Check
                $$ = new expression();                          // Make new bool expression
                $$->type = "bool";
                $$->truelist = makelist(nextinstr());           // Create truelist for boolean expression
                $$->falselist = makelist(nextinstr() + 1);      // Create falselist for boolean expression
                emit("<=", "", $1->pos->name, $3->pos->name);   // Emit "if x <= y goto ", to be updated by backpatch
                emit("goto", "");                               // Emit "goto ", to be updated by backpatch
            }
            else {
                yyerror("Type Error");
            }
        }
        | relational_expression GREATER_THAN_EQUAL shift_expression
        {
            if(typecheck($1->pos, $3->pos)) {                   // Type Check
                $$ = new expression();                          // Make new bool expression
                $$->type = "bool";
                $$->truelist = makelist(nextinstr());           // Create truelist for boolean expression
                $$->falselist = makelist(nextinstr() + 1);      // Create falselist for boolean expression
                emit(">=", "", $1->pos->name, $3->pos->name);   // Emit "if x >= y goto ", to be updated by backpatch
                emit("goto", "");                               // Emit "goto ", to be updated by backpatch
            }
            else {
                yyerror("Type Error");
            }
        }
        ;

equality_expression: 
        relational_expression
        {
            $$ = $1;    // assignment
        }
        | equality_expression EQUAL relational_expression
        {
            if(typecheck($1->pos, $3->pos)) {                   // Type Check
                convertBool2Int($1);                            // Convert bool to int
                convertBool2Int($3);
                $$ = new expression();                          // Make new bool expression
                $$->type = "bool";
                $$->truelist = makelist(nextinstr());           // Create truelist for boolean expression
                $$->falselist = makelist(nextinstr() + 1);      // Create falselist for boolean expression
                emit("==", "", $1->pos->name, $3->pos->name);   // Emit "if x == y goto ", to be updated by backpatch
                emit("goto", "");                               // Emit "goto ", to be updated by backpatch
            }
            else {
                yyerror("Type Error");
            }
        }
        | equality_expression NOT_EQUAL relational_expression
        {
            if(typecheck($1->pos, $3->pos)) {                   // Type Check
                convertBool2Int($1);                           // Convert bool to int
                convertBool2Int($3);
                $$ = new expression();                          // Make new bool expression
                $$->type = "bool";
                $$->truelist = makelist(nextinstr());           // Create truelist for boolean expression
                $$->falselist = makelist(nextinstr() + 1);      // Create falselist for boolean expression
                emit("!=", "", $1->pos->name, $3->pos->name);   // Emit "if x != y goto ", to be updated by backpatch
                emit("goto", "");                               // Emit "goto ", to be updated by backpatch
            }
            else {
                yyerror("Type Error");
            }
        }
        ;

and_expression: 
        equality_expression
        {
            $$ = $1;    // assignment
        }
        | and_expression BITWISE_AND equality_expression
        {
            if(typecheck($1->pos, $3->pos)) {                               // Type Check
                convertBool2Int($1);                                       // Convert bool to int
                convertBool2Int($3);
                $$ = new expression();
                $$->type = "not_bool";                                      // The new result is not bool
                $$->pos = SymbolTable::gentemp(new Type("int"));      // make new temp
                emit("&", $$->pos->name, $1->pos->name, $3->pos->name);     // Emit the Quad
            }
            else {
                yyerror("Type Error");
            }
        }
        ;

exclusive_or_expression: 
        and_expression
        {
            $$ = $1;    // assignment
        }
        | exclusive_or_expression BITWISE_XOR and_expression
        {
            if(typecheck($1->pos, $3->pos)) {                               // Type Check
                convertBool2Int($1);                                       // Convert bool to int
                convertBool2Int($3);
                $$ = new expression();
                $$->type = "not_bool";                                      // The new result is not bool
                $$->pos = SymbolTable::gentemp(new Type("int"));      // make new temp
                emit("^", $$->pos->name, $1->pos->name, $3->pos->name);     // Emit the Quad
            }
            else {
                yyerror("Type Error");
            }
        }
        ;

inclusive_or_expression: 
        exclusive_or_expression
        {
            $$ = $1;    // assignment
        }
        | inclusive_or_expression BITWISE_OR exclusive_or_expression
        {
            if(typecheck($1->pos, $3->pos)) {                               // Type Check
                convertBool2Int($1);                                       // Convert bool to int
                convertBool2Int($3);
                $$ = new expression();
                $$->type = "not_bool";                                      // The new result is not bool
                $$->pos = SymbolTable::gentemp(new Type("int"));      // make new temp
                emit("|", $$->pos->name, $1->pos->name, $3->pos->name);     // Emit the Quad
            }
            else {
                yyerror("Type Error");
            }
        }
        ;

logical_and_expression: 
        inclusive_or_expression
        {
            $$ = $1;    // assignment
        }
        | logical_and_expression LOGICAL_AND M inclusive_or_expression
            // M is a non-terminal that does not generate any code, but it is used to store the next instruction
            // M is used to store the next instruction, so that we can backpatch the truelist of the expression
        {
            convertInt2Bool($1);                                   // convert from int to bool
            convertInt2Bool($4);
            $$ = new expression();                                  // Make new bool expression for result
            $$->type = "bool";
            backpatch($1->truelist, $3);                            // Backpatching
            $$->truelist = $4->truelist;                            // Generate truelist from truelist of $4
            $$->falselist = merge($1->falselist, $4->falselist);    // Generate falselist by merging the falselists of $1 and $4
        }
        ;

logical_or_expression: 
        logical_and_expression
        {
            $$ = $1;    // assignment
        }
        | logical_or_expression LOGICAL_OR M logical_and_expression
        {
            convertInt2Bool($1);                                   // convert from int to bool
            convertInt2Bool($4);
            $$ = new expression();                                  // Make new bool expression for result
            $$->type = "bool";
            backpatch($1->falselist, $3);                           // Backpatching
            $$->falselist = $4->falselist;                          // Generate falselist from falselist of $4
            $$->truelist = merge($1->truelist, $4->truelist);       // Generate truelist by merging the truelists of $1 and $4
        }
        ;

conditional_expression: 
        logical_or_expression
        {
            $$ = $1;    // assignment
        }
        | logical_or_expression N QUESTION_MARK M expression N COLON M conditional_expression
            // Grammar has been augmmented using M and N as dummy non-terminals.
        {   
            $$->pos = SymbolTable::gentemp($5->pos->type);      // Generate temp for expression
            $$->pos->update($5->pos->type);
            emit("=", $$->pos->name, $9->pos->name);            // Assign the conditional expression
            list<int> l1 = makelist(nextinstr());
            emit("goto", "");                                   // Use GOTO to avoid fall through
            backpatch($6->nextlist, nextinstr());               // Make list with next instruction
            emit("=", $$->pos->name, $5->pos->name);
            list<int> l2 = makelist(nextinstr());               // Make list with next instruction
            l1 = merge(l1, l2);                                 // Merge the two lists
            emit("goto", "");                                   // Use GOTO to avoid fall through
            backpatch($2->nextlist, nextinstr());               // Backpatching
            convertInt2Bool($1);                                // Convert expression to bool
            backpatch($1->truelist, $4);                        // $1 = true, control goes to $4 (expression)
            backpatch($1->falselist, $8);                       // $1 = false, control goes to $8 (conditional_expression)
            backpatch(l1, nextinstr());
        }
        ;

M: %empty
        {   
            // Stores next instruction value 
            // Useful for backpatching
            $$ = nextinstr();
        }
        ;

N: %empty
        {
            // Helps in control flow
            $$ = new statement();
            $$->nextlist = makelist(nextinstr());
            emit("goto", "");
        }
        ;

assignment_expression: 
        conditional_expression
        {
            $$ = $1;    // assignment
        }
        | unary_expression assignment_operator assignment_expression
        {
            if($1->array_type == "array") {        // If array_type is "array", convert and emit
                $3->pos = TypeCast($3->pos, $1->type->type);
                emit("[]=", $1->Array->name, $1->pos->name, $3->pos->name);
            }
            else if($1->array_type == "ptr") {   // If array_type is "ptr", emit 
                emit("*=", $1->Array->name, $3->pos->name);
            }
            else {
                $3->pos = TypeCast($3->pos, $1->Array->type->type);
                emit("=", $1->Array->name, $3->pos->name);
            }
            $$ = $3;
        }
        ;

assignment_operator: 
        ASSIGN
        { /* Do Nothing */ }
        | MULTIPLY_ASSIGN
        { /* Do Nothing */ }
        | DIVIDE_ASSIGN
        { /* Do Nothing */ }
        | MODULO_ASSIGN
        { /* Do Nothing */ }
        | ADD_ASSIGN
        { /* Do Nothing */ }
        | SUBTRACT_ASSIGN
        { /* Do Nothing */ }
        | LEFT_SHIFT_ASSIGN
        { /* Do Nothing */ }
        | RIGHT_SHIFT_ASSIGN
        { /* Do Nothing */ }
        | AND_ASSIGN
        { /* Do Nothing */ }
        | XOR_ASSIGN
        { /* Do Nothing */ }
        | OR_ASSIGN
        { /* Do Nothing */ }
        ;

expression: 
        assignment_expression
        {
            $$ = $1;
        }
        | expression COMMA assignment_expression
        { /* Do Nothing */ }
        ;

constant_expression: 
        conditional_expression
        { /* Do Nothing */ }
        ;

declaration: 
        declaration_specifiers init_declarator_list SEMICOLON
        { /* Do Nothing */ }
        | declaration_specifiers SEMICOLON
        { /* Do Nothing */ }
        ;

declaration_specifiers: 
        storage_class_specifier declaration_specifiers
        { /* Do Nothing */ }
        |storage_class_specifier
        { /* Do Nothing */ }
        | type_specifier declaration_specifiers
        { /* Do Nothing */ }
        | type_specifier
        { /* Do Nothing */ }
        | type_qualifier declaration_specifiers
        { /* Do Nothing */ }
        | type_qualifier
        { /* Do Nothing */ }
        | function_specifier declaration_specifiers
        { /* Do Nothing */ }
        | function_specifier
        { /* Do Nothing */ }
        ;

init_declarator_list: 
        init_declarator
        { /* Do Nothing */ }
        | init_declarator_list COMMA init_declarator
        { /* Do Nothing */ }
        ;

init_declarator: 
        declarator
        {
            $$ = $1;
        }
        | declarator ASSIGN initializer
        {   
            // Find the initial value and emit it
            if($3->value != "") {
                $1->value = $3->value;
            }
            emit("=", $1->name, $3->name);
        }
        ;

storage_class_specifier: 
        EXTERN
        { /* Do Nothing */ }
        | STATIC
        { /* Do Nothing */ }
        | AUTO
        { /* Do Nothing */ }
        | REGISTER
        { /* Do Nothing */ }
        ;

type_specifier: 
        VOID
        {
            curr_var_type = "void";   // curr_var_type holds previous variable's data type
        }
        | CHAR
        {
            curr_var_type = "char";   // curr_var_type holds previous variable's data type
        }
        | SHORT
        { /* Do Nothing */ }
        | INT
        {
            curr_var_type = "int";    // curr_var_type holds previous variable's data type
        }
        | LONG
        { /* Do Nothing */ }
        | FLOAT
        {
            curr_var_type = "float";  // curr_var_type holds previous variable's data type
        }
        | DOUBLE
        { /* Do Nothing */ }
        | SIGNED
        { /* Do Nothing */ }
        | UNSIGNED
        { /* Do Nothing */ }
        | BOOL
        { /* Do Nothing */ }
        | COMPLEX
        { /* Do Nothing */ }
        | IMAGINARY
        { /* Do Nothing */ }
        | enum_specifier
        { /* Do Nothing */ }
        ;

specifier_qualifier_list: 
        type_specifier specifier_qualifier_list_opt
        { /* Do Nothing */ }
        | type_qualifier specifier_qualifier_list_opt
        { /* Do Nothing */ }
        ;

specifier_qualifier_list_opt: 
        specifier_qualifier_list
        { /* Do Nothing */ }
        | %empty
        { /* Do Nothing */ }
        ;

enum_specifier: 
        ENUM identifier_opt CURLY_BRACKET_OPEN enumerator_list CURLY_BRACKET_CLOSE
        { /* Do Nothing */ }
        | ENUM identifier_opt CURLY_BRACKET_OPEN enumerator_list COMMA CURLY_BRACKET_CLOSE
        { /* Do Nothing */ }
        | ENUM IDENTIFIER
        { /* Do Nothing */ }
        ;

identifier_opt: 
        IDENTIFIER
        { /* Do Nothing */ }
        | %empty
        { /* Do Nothing */ }
        ;

enumerator_list: 
        enumerator
        { /* Do Nothing */ }
        | enumerator_list COMMA enumerator
        { /* Do Nothing */ }
        ;

enumerator: 
        IDENTIFIER
        { /* Do Nothing */ }
        | IDENTIFIER ASSIGN constant_expression
        { /* Do Nothing */ }
        ;

type_qualifier: 
        CONST
        { /* Do Nothing */ }
        | RESTRICT
        { /* Do Nothing */ }
        | VOLATILE
        { /* Do Nothing */ }
        ;

function_specifier: 
        INLINE
        { /* Do Nothing */ }
        ;

declarator: 
        pointer direct_declarator
        {
            Type* t = $1;
            // For multi-dimensional arrays, the base type is the last type in the list
            // So, we need to traverse the list (by using type_of_array pointer) to find the base type
            while(t->type_of_array != NULL) {
                t = t->type_of_array;
            }
            t->type_of_array = $2->type;  // Store base type
            $$ = $2->update($1);    // Update
        }
        | direct_declarator
        { /* Do Nothing */ }
        ;

direct_declarator: 
        IDENTIFIER
        {
            $$ = $1->update(new Type(curr_var_type));   // For an identifier, update the type to curr_var_type
            curr_Symbol = $$;                         // Update pointer to curr_Symbol
        }
        | ROUND_BRACKET_OPEN declarator ROUND_BRACKET_CLOSE
        {
            $$ = $2;    // assignment
        }
        | direct_declarator SQUARE_BRACKET_OPEN type_qualifier_list assignment_expression SQUARE_BRACKET_CLOSE
        { /* Do Nothing */ }
        | direct_declarator SQUARE_BRACKET_OPEN type_qualifier_list SQUARE_BRACKET_CLOSE
        { /* Do Nothing */ }
        | direct_declarator SQUARE_BRACKET_OPEN assignment_expression SQUARE_BRACKET_CLOSE
        {
            Type* t = $1->type;
            Type* prev = NULL;
            // Find the base type in a recursive fashion
            while(t->type == "array") {
                prev = t;
                t = t->type_of_array;
            }
            if(prev == NULL) {
                int temp = atoi($3->pos->value.c_str());                // Get initial value
                Type* tp = new Type("array", $1->type, temp);           // Create that type
                $$ = $1->update(tp);                                    // Update the Symbol table for that Symbol
            }
            else {
                int temp = atoi($3->pos->value.c_str());                // Get initial value
                prev->type_of_array = new Type("array", t, temp);       // Create that type
                $$ = $1->update($1->type);                              // Update the Symbol table for that Symbol
            }
        }
        | direct_declarator SQUARE_BRACKET_OPEN SQUARE_BRACKET_CLOSE
        {
            Type* t = $1->type;
            Type* prev = NULL;
            // Find the base type in a recursive fashion
            while(t->type == "array") {
                prev = t;
                t = t->type_of_array;
            }
            if(prev == NULL) {
                Type* tp = new Type("array", $1->type, 0);
                $$ = $1->update(tp);
            }
            else {
                prev->type_of_array = new Type("array", t, 0);
                $$ = $1->update($1->type);
            }
        }
        | direct_declarator SQUARE_BRACKET_OPEN STATIC type_qualifier_list assignment_expression SQUARE_BRACKET_CLOSE
        { /* Do Nothing */ }
        | direct_declarator SQUARE_BRACKET_OPEN STATIC assignment_expression SQUARE_BRACKET_CLOSE
        { /* Do Nothing */ }
        | direct_declarator SQUARE_BRACKET_OPEN type_qualifier_list STATIC assignment_expression SQUARE_BRACKET_CLOSE
        { /* Do Nothing */ }
        | direct_declarator SQUARE_BRACKET_OPEN type_qualifier_list MULTIPLY SQUARE_BRACKET_CLOSE
        { /* Do Nothing */ }
        | direct_declarator SQUARE_BRACKET_OPEN MULTIPLY SQUARE_BRACKET_CLOSE
        { /* Do Nothing */ }
        | direct_declarator ROUND_BRACKET_OPEN change_table parameter_type_list ROUND_BRACKET_CLOSE
        {
            curr_ST->name = $1->name;
            if($1->type->type != "void") {
                Symbol* s = curr_ST->lookup("return");    // Lookup for return value
                s->update($1->type);
            }
            $1->nestedTable = curr_ST;
            curr_ST->parent = globalST;    // Update parent Symbol table
            changeTable(globalST);         // Switch curr_table to point to the global Symbol table
            curr_Symbol = $$;             // Update curr_Symbol
        }
        | direct_declarator ROUND_BRACKET_OPEN identifier_list ROUND_BRACKET_CLOSE
        { /* Do Nothing */ }
        | direct_declarator ROUND_BRACKET_OPEN change_table ROUND_BRACKET_CLOSE
        {
            curr_ST->name = $1->name;
            if($1->type->type != "void") {
                Symbol* s = curr_ST->lookup("return");    // Lookup for return value
                s->update($1->type);
            }
            $1->nestedTable = curr_ST;
            curr_ST->parent = globalST;   // Update parent Symbol table
            changeTable(globalST);          // Switch curr_ table to point to the global Symbol table
            curr_Symbol = $$;             // Update curr_ Symbol
        }
        ;

type_qualifier_list_opt: 
        type_qualifier_list
        { /* Do Nothing */ }
        | %empty
        { /* Do Nothing */ }
        ;

pointer: 
        MULTIPLY type_qualifier_list_opt
        {
            $$ = new Type("ptr");     //  Create new type "ptr"
        }
        | MULTIPLY type_qualifier_list_opt pointer
        {
            $$ = new Type("ptr", $3); //  Create new type "ptr"
        }
        ;

type_qualifier_list: 
        type_qualifier
        { /* Do Nothing */ }
        | type_qualifier_list type_qualifier
        { /* Do Nothing */ }
        ;

parameter_type_list: 
        parameter_list
        { /* Do Nothing */ }
        | parameter_list COMMA ELLIPSIS
        { /* Do Nothing */ }
        ;

parameter_list: 
        parameter_declaration
        { /* Do Nothing */ }
        | parameter_list COMMA parameter_declaration
        { /* Do Nothing */ }
        ;

parameter_declaration: 
        declaration_specifiers declarator
        { /* Do Nothing */ }
        | declaration_specifiers
        { /* Do Nothing */ }
        ;

identifier_list: 
        IDENTIFIER
        { /* Do Nothing */ }
        | identifier_list COMMA IDENTIFIER
        { /* Do Nothing */ }
        ;

type_name: 
        specifier_qualifier_list
        { /* Do Nothing */ }
        ;

initializer: 
        assignment_expression
        {
            $$ = $1->pos;   // assignment
        }
        | CURLY_BRACKET_OPEN initializer_list CURLY_BRACKET_CLOSE
        { /* Do Nothing */ }
        | CURLY_BRACKET_OPEN initializer_list COMMA CURLY_BRACKET_CLOSE
        { /* Do Nothing */ }
        ;

initializer_list: 
        designation_opt initializer
        { /* Do Nothing */ }
        | initializer_list COMMA designation_opt initializer
        { /* Do Nothing */ }
        ;

designation_opt: 
        designation
        { /* Do Nothing */ }
        | %empty
        { /* Do Nothing */ }
        ;

designation: 
        designator_list ASSIGN
        { /* Do Nothing */ }
        ;

designator_list: 
        designator
        { /* Do Nothing */ }
        | designator_list designator
        { /* Do Nothing */ }
        ;

designator: 
        SQUARE_BRACKET_OPEN constant_expression SQUARE_BRACKET_CLOSE
        { /* Do Nothing */ }
        | DOT IDENTIFIER
        { /* Do Nothing */ }
        ;

statement: 
        labeled_statement
        { /* Do Nothing */ }
        | compound_statement
        {
            $$ = $1;    // assignment
        }
        | expression_statement
        {
            $$ = new statement();           // Create new statement
            $$->nextlist = $1->nextlist;    // Assign same nextlist
        }
        | selection_statement
        {
            $$ = $1;    // assignment
        }
        | iteration_statement
        {
            $$ = $1;    // assignment
        }
        | jump_statement
        {
            $$ = $1;    // assignment
        }
        ;

// Used to create loop statements
loop_statement:
        labeled_statement
        { /* Do Nothing */ }
        | expression_statement
        {
            $$ = new statement();           // Create new statement
            $$->nextlist = $1->nextlist;    // Assign same nextlist
        }
        | selection_statement
        {
            $$ = $1;    // assignment
        }
        | iteration_statement
        {
            $$ = $1;    // assignment
        }
        | jump_statement
        {
            $$ = $1;    // assignment
        }
        ;

labeled_statement: 
        IDENTIFIER COLON statement
        { /* Do Nothing */ }
        | CASE constant_expression COLON statement
        { /* Do Nothing */ }
        | DEFAULT COLON statement
        { /* Do Nothing */ }
        ;

compound_statement: 
        CURLY_BRACKET_OPEN X change_table block_item_list_opt CURLY_BRACKET_CLOSE
        {
            // Nested Symbols are made using X and change_table
            $$ = $4;
            changeTable(curr_ST->parent);     // Update curr_ Symbol table
        }
        ;

block_item_list_opt: 
        block_item_list
        {
            $$ = $1;    // assignment
        }
        | %empty
        {
            $$ = new statement();   // Create new statement
        }
        ;

block_item_list: 
        block_item
        {
            $$ = $1;    // assignment
        }
        | block_item_list M block_item
        {   
            // M has been used to facilitate backpatching
            $$ = $3;
            backpatch($1->nextlist, $2);    // After $1, move to block_item via $2
        }
        ;

block_item: 
        declaration
        {
            $$ = new statement();   // Create new statement
        }
        | statement
        {
            $$ = $1;    // assignment
        }
        ;

expression_statement: 
        expression SEMICOLON
        {
            $$ = $1;    // assignment
        }
        | SEMICOLON
        {
            $$ = new expression();  // Create new expression
        }
        ;

selection_statement: 
        IF ROUND_BRACKET_OPEN expression N ROUND_BRACKET_CLOSE M statement N %prec THEN
        {
            // Augmented to facilitate control flow
            backpatch($4->nextlist, nextinstr());                   // nextlist of N now has nextinstr
            convertInt2Bool($3);                                    // Convert expression to bool
            $$ = new statement();                                   // Create new statement
            backpatch($3->truelist, $6);                            // Backpatching - if expression is true, go to M
            list<int> temp = merge($3->falselist, $7->nextlist);    // Merge falselist of expression, nextlist of statement and nextlist of last N
            $$->nextlist = merge($8->nextlist, temp);
        }
        | IF ROUND_BRACKET_OPEN expression N ROUND_BRACKET_CLOSE M statement N ELSE M statement
        {
            // Augmented to facilitate control flow
            backpatch($4->nextlist, nextinstr());                   // nextlist of N now has nextinstr
            convertInt2Bool($3);                                    // Convert expression to bool
            $$ = new statement();                                   // Create new statement
            backpatch($3->truelist, $6);                            // Backpatching - if expression is true, GOTO first M, else GOTO second M
            backpatch($3->falselist, $10);
            list<int> temp = merge($7->nextlist, $8->nextlist);     // Merge nextlist of statement, nextlist of N and nextlist of last statement
            $$->nextlist = merge($11->nextlist, temp);
        }
        | SWITCH ROUND_BRACKET_OPEN expression ROUND_BRACKET_CLOSE statement
        { /* Do Nothing */ }
        ;

iteration_statement: 
        WHILE W ROUND_BRACKET_OPEN X change_table M expression ROUND_BRACKET_CLOSE M loop_statement
        {   
            // Augmented to handle control flow, do backpatching, find loop type and generating a new Symbol Table for the loop block
            // Non-Terminals W,X,change_table and M have been used to augment it.
            $$ = new statement();                   // make new statement
            convertInt2Bool($7);                   // Convert expression to bool
            backpatch($10->nextlist, $6);           // Go back to M1 and expression after one iteration of loop_statement
            backpatch($7->truelist, $9);            // Go to M2 and loop_statement if expression is true
            $$->nextlist = $7->falselist;           // Exit loop if expression is false
            emit("goto", convertInt2String($6));   // Emit to Use GOTO to avoid fall through
            loop_type = "";
            changeTable(curr_ST->parent);
        }
        | WHILE W ROUND_BRACKET_OPEN X change_table M expression ROUND_BRACKET_CLOSE CURLY_BRACKET_OPEN M block_item_list_opt CURLY_BRACKET_CLOSE
        {
            // Augmented to handle control flow, do backpatching, find loop type and generating a new Symbol Table for the loop block
            // Non-Terminals W,X,change_table and M have been used to augment it.
            $$ = new statement();                   // make new statement
            convertInt2Bool($7);                   // Convert expression to bool
            backpatch($11->nextlist, $6);           // Go back to M1 and expression after one iteration
            backpatch($7->truelist, $10);           // Go to M2 and block_item_list_opt if expression is true
            $$->nextlist = $7->falselist;           // Exit loop if expression is false
            emit("goto", convertInt2String($6));   // Emit to Use GOTO to avoid fall through
            loop_type = "";
            changeTable(curr_ST->parent);
        }
        | DO D M loop_statement M WHILE ROUND_BRACKET_OPEN expression ROUND_BRACKET_CLOSE SEMICOLON
        {
            // Augmented to handle control flow, do backpatching, find loop type
            // Non-Terminals D and M have been used to augment it.
            $$ = new statement();                   // make new statement     
            convertInt2Bool($8);                    // Convert expression to bool
            backpatch($8->truelist, $3);            // Go back to M1 and loop_statement if expression is true
            backpatch($4->nextlist, $5);            // Go to M2 to check expression after statement is complete
            $$->nextlist = $8->falselist;           // Exit loop if expression is false  
            loop_type = "";
        }
        | DO D CURLY_BRACKET_OPEN M block_item_list_opt CURLY_BRACKET_CLOSE M WHILE ROUND_BRACKET_OPEN expression ROUND_BRACKET_CLOSE SEMICOLON
        {
            // Augmented to handle control flow, do backpatching, find loop type
            // Non-Terminals D and M have been used to augment it.
            $$ = new statement();                   // make new statement  
            convertInt2Bool($10);                   // Convert expression to bool
            backpatch($10->truelist, $4);           // Go back to M1 and block_item_list_opt if expression is true
            backpatch($5->nextlist, $7);            // Go to M2 to check expression after block_item_list_opt is complete
            $$->nextlist = $10->falselist;          // Exit loop if expression is false  
            loop_type = "";
        }
        | FOR F ROUND_BRACKET_OPEN X change_table declaration M expression_statement M expression N ROUND_BRACKET_CLOSE M loop_statement
        {
            /*
                This production rule has been augmented with non-terminals like F, X, change_table and M to handle the control flow, 
                backpatching, detect the kind of loop, make separate Symbol table for loop block and give it an suitable name
            */
            $$ = new statement();                   // make new statement
            convertInt2Bool($8);                   // Convert expression to bool
            backpatch($8->truelist, $13);           // Go to M3 if expression is true
            backpatch($11->nextlist, $7);           // Go back to M1 after N
            backpatch($14->nextlist, $9);           // Go back to expression after loop_statement
            emit("goto", convertInt2String($9));   // Emit to Use GOTO to avoid fall through
            $$->nextlist = $8->falselist;           // Exit loop if expression_statement is false
            loop_type = "";
            changeTable(curr_ST->parent);
        }
        | FOR F ROUND_BRACKET_OPEN X change_table expression_statement M expression_statement M expression N ROUND_BRACKET_CLOSE M loop_statement
        {
            /*
                This production rule has been augmented with non-terminals like F, X, change_table and M to handle the control flow, 
                backpatching, detect the kind of loop, make separate Symbol table for loop block and give it an suitable name
            */
            $$ = new statement();                   // make new statement
            convertInt2Bool($8);                   // Convert expression to bool
            backpatch($8->truelist, $13);           // Go to M3 if expression is true
            backpatch($11->nextlist, $7);           // Go back to M1 after N
            backpatch($14->nextlist, $9);           // Go back to expression after loop_statement
            emit("goto", convertInt2String($9));   // Emit to Use GOTO to avoid fall through
            $$->nextlist = $8->falselist;           // Exit loop if expression_statement is false
            loop_type = "";
            changeTable(curr_ST->parent);
        }
        | FOR F ROUND_BRACKET_OPEN X change_table declaration M expression_statement M expression N ROUND_BRACKET_CLOSE M CURLY_BRACKET_OPEN block_item_list_opt CURLY_BRACKET_CLOSE
        {
            /*
                This production rule has been augmented with non-terminals like F, X, change_table and M to handle the control flow, 
                backpatching, detect the kind of loop, make separate Symbol table for loop block and give it an suitable name
            */
            $$ = new statement();                   // make new statement
            convertInt2Bool($8);                    // Convert expression to bool
            backpatch($8->truelist, $13);           // Go to M3 if expression is true
            backpatch($11->nextlist, $7);           // Go back to M1 after N
            backpatch($15->nextlist, $9);           // Go back to expression after loop_statement
            emit("goto", convertInt2String($9));    // Emit to Use GOTO to avoid fall through
            $$->nextlist = $8->falselist;           // Exit loop if expression_statement is false
            loop_type = "";
            changeTable(curr_ST->parent);
        }
        | FOR F ROUND_BRACKET_OPEN X change_table expression_statement M expression_statement M expression N ROUND_BRACKET_CLOSE M CURLY_BRACKET_OPEN block_item_list_opt CURLY_BRACKET_CLOSE
        {
            // Augmented to handle control flow, do backpatching, find loop type and make new Symbol Table for the loop block
            // Non-Terminals F, X, change_table and M have been used to augment it.

            $$ = new statement();                   // make new statement
            convertInt2Bool($8);                   // Convert expression to bool
            backpatch($8->truelist, $13);           // Go to M3 if expression is true
            backpatch($11->nextlist, $7);           // Go back to M1 after N
            backpatch($15->nextlist, $9);           // Go back to expression after loop_statement
            emit("goto", convertInt2String($9));   // Emit to Use GOTO to avoid fall through
            $$->nextlist = $8->falselist;           // Exit loop if expression_statement is false
            loop_type = "";
            changeTable(curr_ST->parent);
        }
        ;

F: %empty
        {   
            // Start of a FOR loop
            loop_type = "FOR";
        }
        ;

W: %empty
        {
            // Start of WHILE loop
            loop_type = "WHILE";
        }
        ;

D: %empty
        {
            // Start of DO WHILE loop
            loop_type = "DO_WHILE";
        }
        ;

X: %empty
        {   
            // Create a new Symbol Table which is to be used for the loop block
            // This new Symbol Table will be nested inside the current Symbol Table 
            string newST = curr_ST->name + "." + loop_type + "$" + to_string(num_tables++);  // Generate name for new Symbol table
            Symbol* sym = curr_ST->lookup(newST);
            sym->nestedTable = new SymbolTable(newST);  // Create new Symbol table
            sym->name = newST;
            sym->nestedTable->parent = curr_ST;
            sym->type = new Type("block");    // type is "block"
            curr_Symbol = sym;    // Change the curr_Symbol pointer
        }
        ;

change_table: %empty
        {   
            // Used for changing the Symbol table on encountering functions
            // Change Symbol Table to a new one when entering a new function
            if(curr_Symbol->nestedTable != NULL) {
                // If the Symbol table already exists, switch to that table
                changeTable(curr_Symbol->nestedTable);
                emit("label", curr_ST->name);
            }
            else {
                // If the Symbol table does not exist already, create a new one and switch to it
                changeTable(new SymbolTable(""));
            }
        }
        ;

jump_statement: 
        GOTO IDENTIFIER SEMICOLON
        { /* Do Nothing */ }
        | CONTINUE SEMICOLON
        {
            $$ = new statement();
        }
        | BREAK SEMICOLON
        {
            $$ = new statement();
        }
        | RETURN expression SEMICOLON
        {
            $$ = new statement();
            emit("return", $2->pos->name);  // Emit return with a return value
        }
        | RETURN SEMICOLON
        {
            $$ = new statement();
            emit("return", "");             // Emit return without a return value
        }
        ;

translation_unit: 
        external_declaration
        { /* Do Nothing */ }
        | translation_unit external_declaration
        { /* Do Nothing */ }
        ;

external_declaration: 
        function_definition
        { /* Do Nothing */ }
        | declaration
        { /* Do Nothing */ }
        ;

function_definition: 
        declaration_specifiers declarator declaration_list_opt change_table CURLY_BRACKET_OPEN block_item_list_opt CURLY_BRACKET_CLOSE
        {   
            curr_ST->parent = globalST;
            num_tables = 0;
            changeTable(globalST);          // When function scope ends, change Symbol Table to Global Symbol Table          
        }
        ;

declaration_list_opt: 
        declaration_list
        { /* Do Nothing */ }
        | %empty
        { /* Do Nothing */ }
        ;

declaration_list: 
        declaration
        { /* Do Nothing */ }
        | declaration_list declaration
        { /* Do Nothing */ }
        ;

%%

// Display errors during parsing
void yyerror(string err) {

    cout << "Error!!!!" << endl;
    cout << err << " occured at line no. " << yylineno << endl;
    cout << yytext << " could not be parsed." << endl;

}