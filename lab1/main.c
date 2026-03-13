#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char data;
    struct Node *next;
} Node;

void append(Node **head, char val) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = val;
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void printList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        if (current->data == '\n') {
            printf("'\\n'");
        } else if (current->data == '\t') {
            printf("'\\t'");
        } else if (current->data == ' ') {
            printf("' '");
        } else {
            printf("'%c'", current->data);
        }
        
        if (current->next != NULL) {
            printf(" ");
        }
        current = current->next;
    }
    printf("\n");
}

int main() {
    FILE *fp = fopen("main.c", "r");
    if (fp == NULL) {
        return 1;
    }
    Node *head = NULL;
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        append(&head, (char)ch);
    }
    fclose(fp);
    printList(head);
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}
