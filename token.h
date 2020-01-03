#ifndef TOKEN_H
#define TOKEN_H

#define KEYWORD_COUNT 2
// ()- Parenthesis, []-Bracket, {}-Braces
typedef enum{
    //Keywords
    VAR, PRINT,
    
    //Operators
    ASSIGNMENT, EQUALS, ADDITION, SUBTRACTION, ASTERISK, 

    //
    SEMI_COLON, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_BRACKET, RIGHT_BRACKET,
    LEFT_BRACE, RIGHT_BRACE,

    //Misc
    NUMBER, STRING_LITERAL, IDENTIFIER,

    //Type not init
    NONE
} TokenType;

typedef union{
    int i;
    float f;
    char c;
    char str[100];
} Data;

typedef struct{
    Data value;
    TokenType type;
} Token;

//Print token with appropriate formating according to type and with custom ending.
void printk(Token token, char *end);
void token_init(Token *token);
#endif
