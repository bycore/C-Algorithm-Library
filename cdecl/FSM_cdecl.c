#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define MAXTOKENS 100
#define MAXTOKENLEN 64

enum type_tag { IDENTIFIER,
                QUALIFIER,
                TYPE };

struct token {
    char type;
    char string[MAXTOKENLEN];
};

int top = -1;
struct token stack[MAXTOKENS];

struct token this;

#define pop stack[top--]
#define push(s) stack[++top] = s

enum type_tag classify_string(void) {
    char* s = this.string;
    if (!strcmp(s, "const")) {
        strcpy(s, "read-only ");
        return QUALIFIER;
    }
    if (!strcmp(s, "volatile"))
        return QUALIFIER;
    if (!strcmp(s, "void"))
        return TYPE;
    if (!strcmp(s, "char"))
        return TYPE;
    if (!strcmp(s, "signed"))
        return TYPE;
    if (!strcmp(s, "unsigned"))
        return TYPE;
    if (!strcmp(s, "short"))
        return TYPE;
    if (!strcmp(s, "int"))
        return TYPE;
    if (!strcmp(s, "long"))
        return TYPE;
    if (!strcmp(s, "float"))
        return TYPE;
    if (!strcmp(s, "double"))
        return TYPE;
    if (!strcmp(s, "struct"))
        return TYPE;
    if (!strcmp(s, "union"))
        return TYPE;
    if (!strcmp(s, "enum"))
        return TYPE;
    return IDENTIFIER;
}

void gettoken(void) {
    char* p = this.string;
    while ((*p = getchar()) == ' ')
        ;
    if (isalnum(*p)) {
        while (isalnum(*++p = getchar()))
            ;
        ungetc(*p, stdin);
        *p = '\0';
        this.type = classify_string();
        return;
    }

    this.string[1] = '\0';
    this.type = *p;
    return;
}

void initialize(), get_array(), get_params(), get_lparen(), get_ptr_part(), get_type();
void (*nextstate)(void) = initialize;

int main() {
    /*用有限状态机实现的cdecl*/
    while (nextstate != NULL)
        (*nextstate)();
    return 0;
}

void initialize() {
    gettoken();
    while (this.type != IDENTIFIER) {
        push(this);
        gettoken();
    }
    printf("%s is ", this.string);
    gettoken();
    nextstate = get_array;
}

void get_array() {
    nextstate = get_params;
    while (this.type == '[') {
        printf("array ");
        gettoken(); /*一个数字或']'*/
        if (isdigit(this.string[0])) {
            printf("0..%d ", atoi(this.string) - 1);
            gettoken(); /*读取']'*/
        }
        gettoken(); /*在']'之后读取*/
        printf("of ");
        nextstate = get_lparen;
    }
}

void get_params() {
    nextstate = get_lparen;
    if (this.type == '(') {
        while (this.type != ')') {
            gettoken();
        }
        gettoken();
        printf("function returning ");
    }
}

void get_lparen() {
    nextstate = get_ptr_part;
    if (top >= 0) {
        if (stack[top].type == '(') {
            pop;
            gettoken(); /*在')'之后读取*/
            nextstate = get_array;
        }
    }
}

void get_ptr_part() {
    nextstate = get_type;
    if (stack[top].type == '*') {
        printf("pointer to ");
        pop;
        nextstate = get_lparen;
    } else if (stack[top].type == QUALIFIER) {
        printf("%s ", pop.string);
        nextstate = get_lparen;
    }
}

void get_type() {
    nextstate = NULL;
    /*处理在读入标识符之前被放在堆栈里的所有标记*/
    while (top >= 0) {
        printf("%s ", pop.string);
    }
    printf("\n");
}