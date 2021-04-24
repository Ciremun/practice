#include <iostream>

using namespace std;

template <typename T>
class Node
{
public:
    T data;
    Node *next;

    Node() {}
    Node(const T &val, Node<T>* nxt) : next(nxt), data(val) {}
    Node(T &&val, Node<T>* nxt) : next(nxt), data(forward<T>(val)) {}

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

    template <class... Args>
    SinglyLinkedList(Args... args)
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
        Node<T> *new_node = new Node<T>(move(data), head);
        head = new_node;
    }

    void insert(Node<T> *prev, T data)
    {
        if (prev == NULL)
        {
            return;
        }
        Node<T> *new_node = new Node<T>(move(data), prev->next);
        prev->next = new_node;
    }

    void append(T data)
    {
        Node<T> *new_node = new Node<T>(move(data), NULL);
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

    void remove_at(size_t idx)
    {
        if (head == NULL)
        {
            return;
        }
        Node<T> *temp = head;
        if (idx == 0)
        {
            head = temp->next;
            delete temp;
            return;
        }
        for (size_t i = 0; temp != NULL && i < idx - 1; i++)
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

    void reverse()
    {
        Node<T> *prev = NULL;
        Node<T> *current = head;
        Node<T> *next = NULL;
        while (current != NULL)
        {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        head = prev;
    }

    void print()
    {
        head->print();
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

    bool contains(T data)
    {
        Node<T> *current = head;
        while (current != NULL)
        {
            if (current->data == data)
            {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    const Node<T> *operator[](size_t idx) const
    {
        if (idx == 0)
        {
            return head;
        }
        Node<T> *temp = head;
        for (size_t i = 1; temp != NULL && i <= idx; i++)
        {
            temp = temp->next;
        }
        return temp;
    }

    bool operator==(const SinglyLinkedList<T>& other)
    {
        Node<T> *left = head;
        Node<T> *right = other.head;
        while (left != NULL && right != NULL)
        {
            if (left->data != right->data)
            {
                return false;
            }
            left = left->next;
            right = right->next;
        }
        if (left != NULL || right != NULL)
        {
            return false;
        }
        return true;
    }

    bool operator!=(const SinglyLinkedList<T>& other)
    {
        return !(*this == other);
    }
};

int main()
{
    SinglyLinkedList<int> *int_list = new SinglyLinkedList<int>(69, 420, 1337);
    int_list->remove(420);
    int_list->remove(1337);
    int_list->print();
    cout << "size: " << int_list->size() << endl;
    cout << "contains `420`: " << int_list->contains(420) << endl;
    cout << "int_list[0]: " << (*int_list)[0]->data << endl;
    cout << endl;

    SinglyLinkedList<string> *str_list = new SinglyLinkedList<string>("boss", "dungeon");
    str_list->insert(str_list->head, "slave");
    str_list->remove_at(1);
    str_list->remove("boss");
    str_list->append("master");
    str_list->print();
    cout << "size: " << str_list->size() << endl;
    cout << "contains `master`: " << str_list->contains("master") << endl;
    cout << "str_list[1]: " << (*str_list)[1]->data << endl;
    cout << endl;

    SinglyLinkedList<float> *float_list = new SinglyLinkedList<float>(69.5f, 420.5f, 1337.5f);
    cout << "float_list: \t";
    float_list->print();
    float_list->reverse();
    cout << "reversed: \t";
    float_list->print();

    SinglyLinkedList<bool> *bool_list = new SinglyLinkedList<bool>(true, true, false);
    SinglyLinkedList<bool> *bool_list_2 = new SinglyLinkedList<bool>(true, false, false);

    cout << "bool_list != bool_list_2: " << (*bool_list != *bool_list_2) << endl;
    bool_list_2->insert(bool_list_2->head, true);
    bool_list_2->remove_at(2);
    cout << "bool_list == bool_list_2: " << (*bool_list == *bool_list_2) << endl;

    delete int_list;
    delete str_list;
    delete float_list;
    delete bool_list;
    delete bool_list_2;

    return 0;
}
