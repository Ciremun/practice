#include <stdio.h>

class Node
{
public:
    const char *data;
    Node *next;

    void print()
    {
        Node *node = this;
        while (node != NULL)
        {
            printf("%s ", node->data);
            node = node->next;
        }
        printf("\n");
    }
};

int main()
{
    Node *head = new Node();
    Node *second = new Node();
    Node *third = new Node();

    head->data = "head";
    head->next = second;

    second->data = "second";
    second->next = third;

    third->data = "third";
    third->next = NULL;

    head->print();

    return 0;
}
