/* C Declarations and Definitions */
%{
    #include "y.tab.h"
    int lines = 1;

    #define line_completed printf("==================== Parsed  Line No. %d ====================\n\n\n",lines); lines++; printf("\n==================== Parsing Line No. %d ====================\n",lines);
%}

%x  MULTI_LINE_COMMENT
%x  SINGLE_LINE_COMMENT

/* Regular Expression Definitions */

ESCAPE_SEQUENCE                 "\\\'"|"\\\""|"\?"|"\\"|"\a"|"\b"|"\f"|"\r"|"\t"|"\v"|"\n"

DIGIT                           [0-9]
IDENTIFIER                      [a-zA-Z_][a-zA-Z0-9_]*

INTEGER_CONSTANT                [1-9]{DIGIT}*|[0]+

DIGIT_SEQUENCE                  ({DIGIT}+)
EXPONENT_PART                   [eE]([+-]?){DIGIT_SEQUENCE}
FRACTIONAL_CONSTANT             ({DIGIT_SEQUENCE}?\.{DIGIT_SEQUENCE})|({DIGIT_SEQUENCE}\.)
FLOATING_CONSTANT               ({FRACTIONAL_CONSTANT}{EXPONENT_PART}?)|({DIGIT_SEQUENCE}{EXPONENT_PART})

ENUM_CONSTANT                   {IDENTIFIER}

C_CHAR                          [^\'\\\n]|{ESCAPE_SEQUENCE}
C_CHAR_SEQUENCE                 {C_CHAR}+
CHAR_CONSTANT                   \'({C_CHAR_SEQUENCE})\'

CONSTANT                        {INTEGER_CONSTANT}|{FLOATING_CONSTANT}|{ENUM_CONSTANT}|{CHAR_CONSTANT}

S_CHAR                          [^\"\\\n]|{ESCAPE_SEQUENCE}
S_CHAR_SEQUENCE                 {S_CHAR}+
STRING_LITERAL                  \"({S_CHAR_SEQUENCE}?)\"

WHITESPACE                      [ \t]
NEWLINE                         \n


/* Definitions of Rules and Actions */
%%

"auto"                          { return AUTO; }
"break"                         { return BREAK; }
"case"                          { return CASE; }
"char"                          { return CHAR; }
"const"                         { return CONST; }
"continue"                      { return CONTINUE; }
"default"                       { return DEFAULT; }
"do"                            { return DO; }
"double"                        { return DOUBLE; }
"else"                          { return ELSE; }
"enum"                          { return ENUM; }             
"extern"                        { return EXTERN; }
"float"                         { return FLOAT; }
"for"                           { return FOR; }
"goto"                          { return GOTO; }
"if"                            { return IF; }
"inline"                        { return INLINE; }
"int"                           { return INT; }
"long"                          { return LONG; }
"register"                      { return REGISTER; }
"restrict"                      { return RESTRICT; }
"return"                        { return RETURN; }
"short"                         { return SHORT; }
"signed"                        { return SIGNED; }
"sizeof"                        { return SIZEOF; }
"static"                        { return STATIC; }
"struct"                        { return STRUCT; }
"switch"                        { return SWITCH; }
"typedef"                       { return TYPEDEF; }
"union"                         { return UNION; }
"unsigned"                      { return UNSIGNED; }
"void"                          { return VOID; }
"volatile"                      { return VOLATILE; }
"while"                         { return WHILE; }
"_Bool"                         { return _BOOL; }
"_Complex"                      { return _COMPLEX; }
"_Imaginary"                    { return _IMAGINARY; }

{IDENTIFIER}                    { return IDENTIFIER; }
{INTEGER_CONSTANT}              { return INTEGER_CONSTANT; }
{FLOATING_CONSTANT}             { return FLOATING_CONSTANT; }
{CHAR_CONSTANT}                 { return CHAR_CONSTANT; }
{STRING_LITERAL}                { return STRING_LITERAL; }
{WHITESPACE}                    { /* ignore whitespaces */ }
{NEWLINE}                       { line_completed; }

"//"                            { BEGIN(SINGLE_LINE_COMMENT); }
<SINGLE_LINE_COMMENT>.          { /* do nothing for comment */ }
<SINGLE_LINE_COMMENT>\n         { BEGIN(INITIAL); line_completed; }
<SINGLE_LINE_COMMENT><<EOF>>    { BEGIN(INITIAL); }

"/*"                            { BEGIN(MULTI_LINE_COMMENT); }
<MULTI_LINE_COMMENT>"*/"        { BEGIN(INITIAL); }
<MULTI_LINE_COMMENT>.           { /* do nothing for comment */ }
<MULTI_LINE_COMMENT>\n          {line_completed; }

"["                             { return SQUARE_BRACKET_OPEN; }
"]"                             { return SQUARE_BRACKET_CLOSE; }
"("                             { return ROUND_BRACKET_OPEN; }
")"                             { return ROUND_BRACKET_CLOSE; }
"{"                             { return CURLY_BRACKET_OPEN; }
"}"                             { return CURLY_BRACKET_CLOSE; }
"."                             { return DOT; }
"->"                            { return ARROW; }
"++"                            { return INCREMENT; }
"--"                            { return DECREMENT; }
"&"                             { return BITWISE_AND; }
"*"                             { return MULTIPLY; }
"+"                             { return ADD; }
"-"                             { return SUBTRACT; }
"~"                             { return BITWISE_NOR; }
"!"                             { return NOT; }
"/"                             { return DIVIDE; }
"%"                             { return MODULO; }
"<<"                            { return LEFT_SHIFT; }
">>"                            { return RIGHT_SHIFT; }
"<"                             { return LESS_THAN; }
">"                             { return GREATER_THAN; }
"<="                            { return LESS_THAN_EQUAL; }
">="                            { return GREATER_THAN_EQUAL; }
"=="                            { return EQUAL; }
"!="                            { return NOT_EQUAL; }
"^"                             { return BITWISE_XOR; }
"|"                             { return BITWISE_OR; }
"&&"                            { return LOGICAL_AND; }
"||"                            { return LOGICAL_OR; }
"?"                             { return TERNARY; }
":"                             { return COLON; }
";"                             { return SEMICOLON; }
"..."                           { return ELLIPSES; }
"="                             { return ASSIGNMENT; }
"*="                            { return MULTIPLY_ASSIGNMENT; }
"/="                            { return DIVIDE_ASSIGNMENT; }
"%="                            { return MODULO_ASSIGNMENT; }
"+="                            { return ADD_ASSIGNMENT; }
"-="                            { return SUBTRACT_ASSIGNMENT; }
"<<="                           { return LEFT_SHIFT_ASSIGNMENT; }
">>="                           { return RIGHT_SHIFT_ASSIGNMENT; }
"&="                            { return AND_ASSIGNMENT; }
"^="                            { return XOR_ASSIGNMENT; }
"|="                            { return OR_ASSIGNMENT; }
","                             { return COMMA; }
"#"                             { return HASH; }

%%