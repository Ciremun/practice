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
    ~SinglyLinkedList()
    {
        Node<T> *next = NULL;
        while (head != NULL)
        {
            next = head->next;
            delete head;
            head = next;
        }
    }

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
        if (prev == NULL)
        {
            return;
        }
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
        if (head == NULL)
        {
            head = new_node;
            return;
        }
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
        if (temp != NULL && temp->data == data)
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

    void remove_at(size_t pos)
    {
        if (head == NULL)
        {
            return;
        }
        Node<T> *temp = head;
        if (pos == 0)
        {
            head = temp->next;
            delete temp;
            return;
        }
        for (size_t i = 0; temp != NULL && i < pos - 1; i++)
        {
            temp = temp->next;
        }
        if (temp == NULL || temp->next == NULL)
        {
            return;
        }
        Node<T> *new_next = temp->next->next;
        delete temp->next;
        temp->next = new_next;
    }

    size_t size()
    {
        size_t count = 0;
        Node<T> *current = head;
        while (current != NULL)
        {
            current = current->next;
            count++;
        }
        return count;
    }
};

int main()
{
    SinglyLinkedList<int> *int_list = new SinglyLinkedList<int>(69, 420, 1337);
    int_list->remove(420);
    int_list->remove(1337);
    int_list->head->print();
    cout << "int_list.size() = " << int_list->size() << endl;

    SinglyLinkedList<string> *str_list = new SinglyLinkedList<string>("boss", "dungeon");
    str_list->insert(str_list->head, "slave");
    str_list->remove_at(1);
    str_list->remove("boss");
    str_list->append("master");
    str_list->head->print();
    cout << "str_list.size() = " << str_list->size() << endl;

    delete int_list;
    delete str_list;

    return 0;
}
