#include <stdio.h>
#include <string.h>


int is_space(int c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r');
}

int is_alpha(int c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_digit(int c) {
    return (c >= '0' && c <= '9');
}

int is_alnum(int c) {
    return (is_alpha(c) || is_digit(c));
}


#define TOKEN_NUM 256
#define TOKEN_ERR -2

FILE *fp;
int token;
int has_error = 0; 


char token_str[256];
int indent_level = 0;


void print_indent() {
    for (int i = 0; i < indent_level; i++) {
        printf("\t"); 
    }
}


int get_token() {
    int c;
    int i;

    while ((c = fgetc(fp)) != EOF) {
        if (is_space(c)) continue;

        if (is_digit(c)) {
            i = 0;
            token_str[i++] = (char)c;
            while ((c = fgetc(fp)) != EOF && is_digit(c)) {
                token_str[i++] = (char)c;
            }
            token_str[i] = '\0'; // 確保字串結尾
            if (c != EOF) ungetc(c, fp);
            return TOKEN_NUM;
        }
   
        else if (c == '+') return '+';
        else if (c == '(') return '(';
        else if (c == ')') return ')';
        
        else {
            return TOKEN_ERR;
        }
    }
    return EOF;
}


void parse_S();
void parse_S_prime();
void parse_E();



void parse_S() {
    if (has_error) return;
    
    print_indent();
    printf("S -> E S'\n");
    
    indent_level++; 
    switch (token) {
        case TOKEN_NUM:
        case '(':
            parse_E();
            parse_S_prime();
            break;
        default:
            has_error = 1;
            break;
    }
    indent_level--; 
}

void parse_S_prime() {
    if (has_error) return;
    
    print_indent();
    switch (token) {
        case '+':
            printf("S' -> + S\n");
            token = get_token(); 
            
            indent_level++;
            parse_S();
            indent_level--; 
            return;
        case ')':
        case EOF:
            printf("S' -> epsilon\n");
            return;
        default:
            has_error = 1;
            return;
    }
}

void parse_E() {
    if (has_error) return;
    
    print_indent();
    switch (token) {
        case TOKEN_NUM:
            printf("%s\n", token_str); 
            token = get_token();
            return;
        case '(':
            printf("E -> ( S )\n");
            token = get_token(); 
            
           
            parse_S();
            indent_level--;
            
            if (token != ')') {
                has_error = 1;
                return;
            }
            token = get_token(); 
            return;
        default:
            has_error = 1;
            return;
    }
}

int main() {
    fp = fopen("input.txt", "r");
    if (!fp) {
        printf("Error: Cannot open input.txt\n");
        return 1;
    }

    token = get_token();
    parse_S();

    if (!has_error && token == EOF) {
        printf("\nParse Success!\n");
    } else {
        printf("\nParseError\n");
    }

    fclose(fp);
    return 0;
}
