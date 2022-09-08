/* C Declarations and Definitions */
%{
    #define KEYWORD                 10
    #define IDENTIFIER              11
    #define INTEGER_CONSTANT        12
    #define FLOATING_CONSTANT       13
    #define ENUMERATION_CONSTANT    14
    #define CHARACTER_CONSTANT      15
    #define STRING_LITERAL          16
    #define PUNCTUATOR              17
    #define MULTI_LINE_COMMENT      18
    #define SINGLE_LINE_COMMENT     19
    #define INVALID_TOKEN           20
%}

%x MULTI_LINE_COMMENT_CONTENT
%x SINGLE_LINE_COMMENT_CONTENT

/* Regular Expression Definitions*/

KEYWORD             "auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|"inline"|"int"|"long"|"register"|"restrict"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while"|"_Bool"|"_Complex"|"_Imaginary"

ESCAPE_SEQUENCE     "\\\'"|"\\\""|"\\?"|"\\\\"|"\\a"|"\\b"|"\\f"|"\\n"|"\\r"|"\\t"|"\\v"

PUNCTUATOR


IDENTIFIER


DIGIT

DIGIT_SEQUENCE

NONZERO_DIGIT

INTEGER_CONSTANT


EXPONENT_PART

FRACTIONAL_CONSTANT

FLOATING_CONSTANT


ENUMERATION_CONSTANT


C_CHAR

C_CHAR_SEQUENCE

CHARACTER_CONSTANT


S_CHAR

S_CHAR_SEQUENCE

STRING_LITERAL


WHITESPACE