#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char ch;
    int count;
    struct Node *next;
} Node;

void freeList(Node *head) {
    while (head) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    FILE *fp = fopen(__FILE__, "r");
    if (!fp) return 1;

    Node *head = NULL;
    int c;

    while ((c = fgetc(fp)) != EOF) {
        Node **curr = &head;
        
        while (*curr && (*curr)->ch < (char)c) {
            curr = &((*curr)->next);
        }

        if (*curr && (*curr)->ch == (char)c) {
            (*curr)->count++;
        } else {
            Node *newNode = (Node*)malloc(sizeof(Node));
            if (!newNode) {
                fclose(fp);
                return 1;
            }
            newNode->ch = (char)c;
            newNode->count = 1;
            newNode->next = *curr;
            *curr = newNode;
        }
    }
    fclose(fp);

    Node *p = head;
    while (p) {
        if (p->ch == '\n') printf("\\n:%d\n", p->count);
        else if (p->ch == ' ') printf("space:%d\n", p->count);
        else if (p->ch == '\t') printf("\\t:%d\n", p->count);
        else if (p->ch == '\r') printf("\\r:%d\n", p->count);
        else printf("%c:%d\n", p->ch, p->count);
        p = p->next;
    }

    freeList(head);
    return 0;

}
