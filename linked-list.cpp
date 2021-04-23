#include <iostream>

using namespace std;

template <typename T>
class Node
{
public:
    T data;
    Node *next;

    Node() {}
    Node(const T& val) : next(nullptr), data(val) {}
    Node(T&& val) : next(nullptr), data(forward<T>(val)) {}

    void print()
    {
        Node *node = this;
        while (node != NULL)
        {
            cout << node->data << " ";
            node = node->next;
        }
        cout << endl;
    }
};

template <typename T>
class SinglyLinkedList
{
public:
    Node<T> *head = NULL;

    SinglyLinkedList() {}

    template <class ... Args>
    SinglyLinkedList(Args ... args)
    {
        const int size = sizeof...(args);
        T a[size] = {args...};
        for (int i = size - 1; i >= 0; i--)
        {
            this->push(a[i]);
        }
    }

    void push(T data)
    {
        Node<T> *new_node = new Node<T>();
        new_node->data = data;
        new_node->next = head;
        head = new_node;
    }

    void insert(Node<T> *prev, T data)
    {
        Node<T> *new_node = new Node<T>();
        new_node->data = data;
        new_node->next = prev->next;
        prev->next = new_node;
    }

    void append(T data)
    {
        Node<T> *new_node = new Node<T>();
        new_node->data = data;
        new_node->next = NULL;
        Node<T> *last = head;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = new_node;
    }

    void remove(T data)
    {
        Node<T> *temp = head;
        if (temp->data == data)
        {
            head = temp->next;
            delete temp;
            return;
        }
        Node<T> *prev = NULL;
        while (temp != NULL && temp->data != data)
        {
            prev = temp;
            temp = temp->next;
        }
        if (temp == NULL)
        {
            return;
        }
        prev->next = temp->next;
        delete temp;
    }
};

int main()
{
    SinglyLinkedList<int> int_list = SinglyLinkedList<int>(69, 420, 1337);
    int_list.remove(420);
    int_list.remove(1337);
    int_list.head->print();

    SinglyLinkedList<string> str_list = SinglyLinkedList<string>("master", "dungeon");
    str_list.remove("master");
    str_list.append("master");
    str_list.head->print();

    return 0;
}
