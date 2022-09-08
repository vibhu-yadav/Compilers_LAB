%{
    #include <stdio.h>
    #define KEYWORD 101
    #define IDENTIFIER 102
    #define PUNCTUATORS 103
    #define STRING 104
    #define INT 105
    #define FLOAT 106
    #define CHAR 107
    #define MULTI_COMMENT 108
    #define SINGLE_COMMENT 109
    #define WS 110
    #define MULTI_COMMENT_START 111
    #define MULTI_COMMENT_END 112
    #define SINGLE_COMMENT_START 113
    #define SINGLE_COMMENT_END 114
    #define NL 115
    
%}

%x  MULTI_LINE_COMMENT
%x  SINGLE_LINE_COMMENT

KEYWORD                     "break"|"float"|"static"|"case"|"for"|"struct"|"char"|"goto"|"switch"|"continue"|"if"|"typedef"|"default"|"int"|"union"|"do"|"long"|"void"|"double"|"return"|"while"|"else"|"short"|"extern"|"sizeof"
PUNCTUATORS                 "("|")"|"["|"]"|"{"|"}"|"."|"->"|"++"|"--"|"&"|"*"|"+"|"-"|"~"|"!"|"/"|"%"|"<<"|">>"|"<"|">"|"<="|">="|"=="|"!="|"^"|"|"|"&&"|"||"|"?"|":"|";"|"..."|"="|"*="|"/="|"%="|"+="|"-="|"<<="|">>="|"&="|"^="|"|="|","|"#"
ESCAPE_SEQUENCE             "\\\'"|"\\\""|"\?"|"\\"|"\a"|"\b"|"\f"|"\r"|"\t"|"\v"
DIGIT                       [0-9]
IDENTIFIER                  [a-zA-Z_][a-zA-Z0-9_]*
S_CHAR                      [^\"\n\\]|{ESCAPE_SEQUENCE}
S_CHAR_SEQUENCE             {S_CHAR}+
STRING              \"({S_CHAR_SEQUENCE})?\"
INT                         [1-9]{DIGIT}*|[0]+
C_CHAR                      [^\'\n\\]|{ESCAPE_SEQUENCE}
C_CHAR_SEQUENCE             {C_CHAR}+
CHAR                        \'({C_CHAR_SEQUENCE})\'        
DIGIT_SEQUENCE              [0-9]+
EXPONENT_PART               ([Ee]([+-]?)({DIGIT_SEQUENCE}))
FRAC_CONSTANT               {DIGIT_SEQUENCE}\.|{DIGIT_SEQUENCE}?\.{DIGIT_SEQUENCE}
FLOAT                       ({DIGIT_SEQUENCE}{EXPONENT_PART})|({FRAC_CONSTANT}{EXPONENT_PART}?)
CONST                       {INT}|{CHAR}|{FLOAT}
WS                          [ \t]
NL                          "\n"

%%

{KEYWORD}                   { return KEYWORD;                                                   }
{IDENTIFIER}                { return IDENTIFIER;                                                }
{PUNCTUATORS}               { return PUNCTUATORS;                                               }
{STRING}            { return STRING;                                            }
{INT}                       { return INT;                                                 }
{FLOAT}                     { return FLOAT;                                               }
{CHAR}                      { return CHAR;                                             }
{NL}                       { return NL;                                                     }   
{ESCAPE_SEQUENCE}|" "                     

"/*"                        { BEGIN(MULTI_LINE_COMMENT); return MULTI_COMMENT_START;                       }
<MULTI_LINE_COMMENT>"*/"               { BEGIN(INITIAL); return MULTI_COMMENT_END;                         }
<MULTI_LINE_COMMENT>.                  { return MULTI_COMMENT;                                             }
<MULTI_LINE_COMMENT>"\n"               { return MULTI_COMMENT;                                             }

"//"                        { BEGIN(SINGLE_LINE_COMMENT); return SINGLE_COMMENT_START;          }
<SINGLE_LINE_COMMENT>"\n"   { BEGIN(INITIAL); return SINGLE_COMMENT_END;                        }
<SINGLE_LINE_COMMENT>.      { return SINGLE_COMMENT;                                            }    

%%
