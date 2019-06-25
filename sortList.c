#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *next;
} Node;

Node *createNode(int val) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->data = val;
    node->next = NULL;
    return node;
}

// insert after header node
void pushFront(Node *head, int val) {
    if (head) {
        Node *tmp = createNode(val);
        tmp->next = head->next;
        head->next = tmp;
    }
}

Node *copyList(Node *head) {
    if (!head) return NULL;
    Node *list = createNode(head->data);
    Node *tail = list;
    while (head->next) {
        head = head->next;
        tail->next = createNode(head->data);
        tail = tail->next;
    }
    return list;
}

void clearList(Node *head) {
    while (head) {
        Node *tmp = head;
        head = head->next;
        free(tmp);
    }
}

// merge two sorted lists
Node *mergeList(Node *list1, Node *list2) {
    if (!list1) return list2;
    if (!list2) return list1;
    Node *head;
    if (list1->data < list2->data) {
        head = list1;
    } else {  // let list1 < list2
        head = list2;
        list2 = list1;
        list1 = head;
    }
    while (list1->next) {
        if (list2->data < list1->next->data) {
            Node *tmp = list1->next;
            list1->next = list2;
            list2 = tmp;
        }
        list1 = list1->next;
    }
    list1->next = list2;
    return head;
}

Node *sortList(Node *head) {
    if (!head || !head->next) return head;
    // slow and fast pointers
    Node *mid = head, *right = head->next;
    while (right->next && right->next->next) {
        mid = mid->next;
        right = right->next->next;
    }
    right = mid->next;
    mid->next = NULL;
    head = sortList(head);
    right = sortList(right);
    return mergeList(head, right);
}

// bottom-up mergesort
Node *mergeSort(Node *head) {
    if (!head || !head->next) return head;
    Node *counter[64];
    int i, fill = 0;
    while (head) {
        Node *carry = head;
        head = head->next;
        carry->next = NULL;
        for (i = 0; i < fill && counter[i]; ++i) {
            carry = mergeList(counter[i], carry);
            counter[i] = NULL;
        }
        counter[i] = carry;
        if (i == fill) ++fill;
    }
    for (i = 1; i < fill; ++i)
        counter[i] = mergeList(counter[i - 1], counter[i]);
    return counter[fill - 1];
}

int main() {
    Node *list = createNode(-1);
    int n = 1024, r;
    unsigned long next = 1;
    while (n--) {
        next = next * 1103515245 + 12345;
        r = (unsigned) (next / 65536) % 32768;
        pushFront(list, r);
    }
    Node *copy = copyList(list);

    list = sortList(list);
    copy = mergeSort(copy);

    clearList(list);
    clearList(copy);

    return 0;
}
