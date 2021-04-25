#include <iostream>
#include <variant>

using namespace std;

template<typename T>
using base_type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

enum class type
{
    int_type,
    float_type,
    string_type,
    bool_type
};

class Node
{
public:
    using Data = variant<int, float, string, bool>;
    Data data;
    Node *next;
    type t;

    Node() {}

    template <typename T>
    Node(const T &val, Node *nxt) : next(nxt), data(Data(val))
    {
        set_type(val);
    }

    template <typename T>
    Node(T &&val, Node *nxt) : next(nxt), data(forward<T>(val))
    {
        set_type(val);
    }

    template <typename T>
    void set_type(const T&val)
    {
        if (is_same_v<T, int>)
        {
            t = type::int_type;
        }
        else if (is_same_v<T, float>)
        {
            t = type::float_type;
        }
        else if (is_same_v<T, string>)
        {
            t = type::string_type;
        }
        else if (is_same_v<T, bool>)
        {
            t = type::bool_type;
        }
    }

    template <typename F>
    bool value(F callback) const
    {
        switch (this->t)
        {
            case type::int_type:
                return callback(get<int>(this->data));
            case type::float_type:
                return callback(get<float>(this->data));
            case type::string_type:
                return callback(get<string>(this->data));
            case type::bool_type:
                return callback(get<bool>(this->data));
            default:
                return false;
        }
    }

    void print()
    {
        Node *node = this;
        while (node != NULL)
        {
            node->value([](auto &val) -> bool { cout << val << " "; return true; });
            node = node->next;
        }
        cout << endl;
    }
};

class SinglyLinkedList
{
public:
    Node *head = NULL;

    SinglyLinkedList() {}
    ~SinglyLinkedList()
    {
        Node *next = NULL;
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
        ([&](auto &&input) { this->append(input); }(args), ...);
    }

    template <typename T>
    void push(T data)
    {
        Node *new_node = new Node(move(data), head);
        head = new_node;
    }

    template <typename T>
    void insert(Node *prev, T data)
    {
        if (prev == NULL)
        {
            return;
        }
        Node *new_node = new Node(move(data), prev->next);
        prev->next = new_node;
    }

    template <typename T>
    void append(T data)
    {
        Node *new_node = new Node(move(data), NULL);
        if (head == NULL)
        {
            head = new_node;
            return;
        }
        Node *last = head;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = new_node;
    }

    template <typename T>
    void remove(T data)
    {
        Node *temp = head;
        if (temp != NULL && temp->value([&data](auto &val) -> bool
        {
            if constexpr (is_same_v<T, base_type<decltype(val)>>)
            {
                return val == data;
            }
            else
            {
                return false;
            }
        }))
        {
            head = temp->next;
            delete temp;
            return;
        }
        Node *prev = NULL;
        while (temp != NULL && temp->value([&data](auto &val) -> bool
        {
            if constexpr (is_same_v<T, base_type<decltype(val)>>)
            {
                return val != data;
            }
            else
            {
                return true;
            }
        }))
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
        Node *temp = head;
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
        Node *new_next = temp->next->next;
        delete temp->next;
        temp->next = new_next;
    }

    void reverse()
    {
        Node *prev = NULL;
        Node *current = head;
        Node *next = NULL;
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
        Node *current = head;
        while (current != NULL)
        {
            current = current->next;
            count++;
        }
        return count;
    }

    template <typename T>
    bool contains(T data)
    {
        Node *current = head;
        while (current != NULL)
        {
            if (current->value([&data](auto &val) -> bool
            {
                if constexpr (is_same_v<T, base_type<decltype(val)>>)
                {
                    return val == data;
                }
                else
                {
                    return false;
                }
            }))
            {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    const Node *operator[](size_t idx) const
    {
        if (idx == 0)
        {
            return head;
        }
        Node *temp = head;
        for (size_t i = 1; temp != NULL && i <= idx; i++)
        {
            temp = temp->next;
        }
        return temp;
    }

    bool operator==(const SinglyLinkedList &other)
    {
        Node *left = head;
        Node *right = other.head;
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

    bool operator!=(const SinglyLinkedList &other)
    {
        return !(*this == other);
    }
};

int main()
{
    SinglyLinkedList *list_1 = new SinglyLinkedList(69, 420.5f, string("dungeon master"));
    list_1->remove(420);
    list_1->remove(1337);
    list_1->print();
    cout << "size: " << list_1->size() << endl;
    cout << "contains `420`: " << list_1->contains(420) << endl;
    (*list_1)[0]->value([](auto &val){ cout << "list_1[0]: " <<  val << endl; return true; });
    cout << endl;

    SinglyLinkedList *list_2 = new SinglyLinkedList(string("boss"), string("dungeon"));
    list_2->insert(list_2->head, string("slave"));
    list_2->remove_at(1);
    list_2->remove(string("boss"));
    list_2->append(string("master"));
    list_2->print();
    cout << "size: " << list_2->size() << endl;
    cout << "contains `master`: " << list_2->contains(string("master")) << endl;
    (*list_2)[1]->value([](auto &val){ cout << "list_2[1]: " <<  val << endl; return true; });
    cout << endl;

    SinglyLinkedList *list_3 = new SinglyLinkedList(false, string("second"), 3, 3.5f);
    cout << "list_3: \t";
    list_3->print();
    list_3->reverse();
    cout << "reversed: \t";
    list_3->print();
    cout << endl;

    SinglyLinkedList *bool_list = new SinglyLinkedList(true, true, false);
    SinglyLinkedList *bool_list_2 = new SinglyLinkedList(true, false, false);

    bool_list->print();
    bool_list_2->print();

    cout << "bool_list != bool_list_2: " << (*bool_list != *bool_list_2) << endl;
    bool_list_2->insert(bool_list_2->head, true);
    bool_list_2->remove_at(2);

    bool_list->print();
    bool_list_2->print();

    cout << "bool_list == bool_list_2: " << (*bool_list == *bool_list_2) << endl;

    delete list_1;
    delete list_2;
    delete list_3;
    delete bool_list;
    delete bool_list_2;

    return 0;
}
