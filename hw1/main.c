#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- 自己實作字元判斷函式 (取代 ctype.h) ---
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
// -------------------------------------------

// 輸出格式： value: TOKEN_TYPE
void printToken(const char* value, const char* type) {
    printf("%s: %s\n", value, type);
}

// 關鍵字檢查
int checkKeyword(const char* s) {
    if (strcmp(s, "int") == 0) return 1;
    if (strcmp(s, "main") == 0) return 2;
    if (strcmp(s, "if") == 0) return 3;
    if (strcmp(s, "else") == 0) return 4;
    if (strcmp(s, "while") == 0) return 5;
    return 0;
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        printf("Error: Cannot open input.txt\n");
        return 1;
    }

    int c;
    char buf[256];
    int i;

    while ((c = fgetc(fp)) != EOF) {
        // 跳過空白字元
        if (is_space(c)) continue;

        // 1. 識別碼 (ID) 與 關鍵字辨識 (以字母或下底線開頭)
        if (is_alpha(c) || c == '_') {
            i = 0;
            buf[i++] = (char)c;
            while ((c = fgetc(fp)) != EOF && (is_alnum(c) || c == '_')) {
                buf[i++] = (char)c;
            }
            buf[i] = '\0';
            ungetc(c, fp); // 將多讀的字元退回

            int keyType = checkKeyword(buf);
            switch(keyType) {
                case 1: printToken(buf, "TYPE_TOKEN"); break;
                case 2: printToken(buf, "MAIN_TOKEN"); break;
                case 3: printToken(buf, "IF_TOKEN"); break;
                case 4: printToken(buf, "ELSE_TOKEN"); break;
                case 5: printToken(buf, "WHILE_TOKEN"); break;
                default: printToken(buf, "ID_TOKEN"); break;
            }
        }
        // 2. 數字文字 (LITERAL_TOKEN) 辨識
        else if (is_digit(c)) {
            i = 0;
            buf[i++] = (char)c;
            while ((c = fgetc(fp)) != EOF && is_digit(c)) {
                buf[i++] = (char)c;
            }
            buf[i] = '\0';
            ungetc(c, fp); // 將多讀的字元退回
            printToken(buf, "LITERAL_TOKEN");
        }
        // 3. 運算子與符號辨識 (包含前瞻判斷 ==, >=, <=)
        else {
            if (c == '(') printToken("(", "LEFTPAREN_TOKEN");
            else if (c == ')') printToken(")", "REFTPAREN_TOKEN");
            else if (c == '{') printToken("{", "LEFTBRACE_TOKEN");
            else if (c == '}') printToken("}", "REFTBRACE_TOKEN");
            else if (c == ';') printToken(";", "SEMICOLON_TOKEN");
            else if (c == '+') printToken("+", "PLUS_TOKEN");
            else if (c == '-') printToken("-", "MINUS_TOKEN");
            else if (c == '=') {
                int next = fgetc(fp);
                if (next == '=') printToken("==", "EQUAL_TOKEN");
                else { ungetc(next, fp); printToken("=", "ASSIGN_TOKEN"); }
            }
            else if (c == '>') {
                int next = fgetc(fp);
                if (next == '=') printToken(">=", "GREATEREQUAL_TOKEN");
                else { ungetc(next, fp); printToken(">", "GREATER_TOKEN"); }
            }
            else if (c == '<') {
                int next = fgetc(fp);
                if (next == '=') printToken("<=", "LESSEQUAL_TOKEN");
                else { ungetc(next, fp); printToken("<", "LESS_TOKEN"); }
            }
        }
    }

    fclose(fp);
    return 0;
}
