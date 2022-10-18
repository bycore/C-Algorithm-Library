#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXTOKENS 100
#define MAXTOKENLEN 64
/*标记的类型，包括标识符，限定符，类型*/
enum type_tag { IDENTIFIER,
                QUALIFIER,
                TYPE };

/*标记的数据结构，包括类型和内容*/
struct token {
    char type;
    char string[MAXTOKENS];
};

int top = -1;
struct token stack[MAXTOKENS]; /*保存第一个标识符之前的所有标记*/
struct token this;             /*保存刚读入的那个标记*/

#define pop stack[top--]         /*出栈*/
#define push(s) stack[++top] = s /*入栈*/

/*理解所有分析过程的代码段*/
void read_to_first_identifer(); /*读至第一个标识符*/

/*分析字符串的类型*/
enum type_tag classify_string(void); /*字符串分类*/

void gettoken(void);         /*取标记*/
void deal_with_declarator(); /*处理声明器*/
void deal_with_arrays();
void deal_with_function_args();
void deal_with_pointers();

int main() {
    /*将标记压入堆栈中，直到遇见标识符*/
    read_to_first_identifer();
    deal_with_declarator();
    printf("\n");
    return 0;
}

enum type_tag classify_string(void) {
    /*推断标示符的类型*/
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

void gettoken(void) /*读取下一个标记到"this"中*/
{
    char* p = this.string;

    /*略过空白符*/
    while ((*p = getchar()) == ' ')
        ;

    if (isalnum(*p)) {
        /*读入的标识符以A-Z，0-9开头*/
        while (isalnum(*++p = getchar()))
            ;
        ungetc(*p, stdin);
        *p = '\0';
        this.type = classify_string();
        return;
    }

    if (*p == '*') {
        strcpy(this.string, "pointer to ");
        this.type = '*';
        return;
    }

    this.string[1] = '\0';
    this.type = *p;
    return;
}

/*理解所有分析过程的代码段*/
void read_to_first_identifer() {
    gettoken();
    while (this.type != IDENTIFIER) {
        push(this);
        gettoken();
    }
    printf("%s is ", this.string);
    gettoken();
}

void deal_with_arrays() {
    while (this.type == '[') {
        printf("array ");
        gettoken(); /*数字或']'*/
        if (isdigit(this.string[0])) {
            printf("0..%d ", atoi(this.string) - 1);
            gettoken(); /*读取']'*/
        }
        gettoken(); /*注意：读取了']'后继续向后读取了一个标记*/
        printf("of ");
    }
}

void deal_with_function_args() {
    while (this.type != ')') {
        gettoken();
    }
    gettoken(); /*注意：读取越过了')'，即读取了')'后的下一个标记*/
    printf("function returning ");
}

void deal_with_pointers() {
    while (stack[top].type == '*') {
        printf("%s ", pop.string);
    }
}

void deal_with_declarator() {
    /*优先处理右边的可能的符号[和(*/
    /*处理标识符之后可能存在的数组，函数*/
    switch (this.type) {
        case '[':
            deal_with_arrays();
            break;
        case '(':
            deal_with_function_args();
    }

    /*然后处理左边的符号*，也就是在堆栈中的符号*/
    deal_with_pointers();

    /*处理在读入到标识符之前压入到堆栈中的符号*/
    while (top >= 0) {
        if (stack[top].type == '(') {
            pop;
            gettoken(); /*读取')'之后的符号*/
            deal_with_declarator();
        } else {
            printf("%s", pop.string);
        }
    }
}
