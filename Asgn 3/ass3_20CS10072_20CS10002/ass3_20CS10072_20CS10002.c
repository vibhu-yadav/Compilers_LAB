#include <stdio.h>
#define KEYWORD 101
#define IDENTIFIER 102
#define PUNCTUATORS 103
#define STRING 104
#define INT 105
#define FLOAT 106
#define CHAR 107
#define MULTI_COMMENT 108
#define MULTI_COMMENT_START 109
#define MULTI_COMMENT_END 110
#define SINGLE_COMMENT 111
#define SINGLE_COMMENT_START 112
#define SINGLE_COMMENT_END 113

extern char* yytext;
extern int yylex();

int main()
{
    int token;
    while(token = yylex())
    {
        switch(token) 
        {
            case KEYWORD: printf("< KEYWORD, %s >\n", yytext); break;
            case IDENTIFIER: printf("< IDENTIFIER, %s >\n", yytext); break;
            case PUNCTUATORS: printf("< PUNCTUATOR, %s >\n", yytext); break;
            case STRING: printf("< STRING LITERAL, %s >\n", yytext); break;
            case INT: printf("< INTEGER CONSTANT, %s >\n", yytext); break;
            case FLOAT: printf("< FLOAT CONSTANT, %s >\n", yytext); break;
            case CHAR: printf("< CHARACTER CONSTANT, %s >\n", yytext); break;
            case MULTI_COMMENT_START: printf("< MULTI_LINE_COMMENT_BEGINS, %s >\n", yytext); break;
            case MULTI_COMMENT_END: printf("\n< MULTI_LINE_COMMENT_ENDS, %s >\n", yytext); break;
            case MULTI_COMMENT: printf("%s", yytext); break;
            case SINGLE_COMMENT_START: printf("< SINGLE_LINE_COMMENT_BEGINS, %s >\n", yytext); break;
            case SINGLE_COMMENT_END: printf("\n< SINGLE_LINE_COMMENT_ENDS, %s >\n", yytext); break;
            case SINGLE_COMMENT: printf("%s", yytext); break;

            default: break;
        }
    }
    return 0;
}