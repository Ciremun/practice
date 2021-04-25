#include <iostream>
#include <variant>

using namespace std;

template <typename T>
using base_type = typename remove_cv<typename remove_reference<T>::type>::type;

template <typename... Types>
class Node
{
public:
    using Data = variant<Types...>;
    Data data;
    Node *next;

    Node() {}

    template <typename T>
    Node(const T &val, Node *nxt) : next(nxt), data(Data(val)) {}

    template <typename T>
    Node(T &&val, Node *nxt) : next(nxt), data(forward<T>(val)) {}

    void print()
    {
        Node *node = this;
        while (node != NULL)
        {
            visit([](auto &&val) { cout << val << " "; }, node->data);
            node = node->next;
        }
        cout << endl;
    }

    void print_self() const
    {
        visit([](auto &&val) { cout << val << endl; }, data);
    }
};

template <typename... Types>
class SinglyLinkedList
{
public:
    Node<Types...> *head = NULL;

    SinglyLinkedList() {}
    ~SinglyLinkedList()
    {
        Node<Types...> *next = NULL;
        while (head != NULL)
        {
            next = head->next;
            delete head;
            head = next;
        }
    }

    template <typename... Args>
    SinglyLinkedList(Args... args)
    {
        ([&](auto &&input) { this->append(input); }(args), ...);
    }

    template <typename T>
    void push(T data)
    {
        Node<Types...> *new_node = new Node<Types...>(move(data), head);
        head = new_node;
    }

    template <typename T>
    void insert(Node<Types...> *prev, T data)
    {
        if (prev == NULL)
        {
            return;
        }
        Node<Types...> *new_node = new Node<Types...>(move(data), prev->next);
        prev->next = new_node;
    }

    template <typename T>
    void append(T data)
    {
        Node<Types...> *new_node = new Node<Types...>(move(data), NULL);
        if (head == NULL)
        {
            head = new_node;
            return;
        }
        Node<Types...> *last = head;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = new_node;
    }

    template <typename T>
    void remove(T data)
    {
        Node<Types...> *temp = head;
        if (temp != NULL && visit([&data](auto &&val) -> bool {
                if constexpr (is_same_v<T, base_type<decltype(val)>>)
                {
                    return val == data;
                }
                else
                {
                    return false;
                }
            },
                                  temp->data))
        {
            head = temp->next;
            delete temp;
            return;
        }
        Node<Types...> *prev = NULL;
        while (temp != NULL && visit([&data](auto &&val) -> bool {
                   if constexpr (is_same_v<T, base_type<decltype(val)>>)
                   {
                       return val != data;
                   }
                   else
                   {
                       return true;
                   }
               },
                                     temp->data))
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
        Node<Types...> *temp = head;
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
        Node<Types...> *new_next = temp->next->next;
        delete temp->next;
        temp->next = new_next;
    }

    void reverse()
    {
        Node<Types...> *prev = NULL;
        Node<Types...> *current = head;
        Node<Types...> *next = NULL;
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
        Node<Types...> *current = head;
        while (current != NULL)
        {
            current = current->next;
            count++;
        }
        return count;
    }

    const Node<Types...> *at(size_t idx)
    {
        return operator[](idx);
    }

    template <typename T>
    bool contains(T data)
    {
        Node<Types...> *current = head;
        while (current != NULL)
        {
            if (visit([&data](auto &&val) -> bool {
                    if constexpr (is_same_v<T, base_type<decltype(val)>>)
                    {
                        return val == data;
                    }
                    else
                    {
                        return false;
                    }
                },
                      current->data))
            {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    const Node<Types...> *operator[](size_t idx) const
    {
        if (idx == 0)
        {
            return head;
        }
        Node<Types...> *temp = head;
        for (size_t i = 1; temp != NULL && i <= idx; i++)
        {
            temp = temp->next;
        }
        return temp;
    }

    bool operator==(const SinglyLinkedList &other)
    {
        Node<Types...> *left = head;
        Node<Types...> *right = other.head;
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
    using SLL = SinglyLinkedList<int, float, string>;
    auto *list_1 = new SLL(69, 420.5f, string("dungeon master"));
    list_1->print();

    cout << "(*list_1)[2] == `dungeon master`: " << (get<string>(list_1->at(2)->data) == string("dungeon master")) << endl;
    cout << "size: " << list_1->size() << endl;
    cout << "contains `420.5f`: " << list_1->contains(420.5f) << endl;
    cout << "list_1[0]: ";
    (*list_1)[0]->print_self();
    cout << endl;

    auto *list_2 = new SinglyLinkedList<string>(string("boss"), string("dungeon"));
    list_2->insert(list_2->head, string("slave"));
    list_2->remove_at(1);
    list_2->remove(string("boss"));
    list_2->append(string("master"));
    list_2->print();
    cout << "size: " << list_2->size() << endl;
    cout << "contains `slave`: " << list_2->contains(string("slave")) << endl;
    cout << "list_2[1]: ";
    (*list_2)[1]->print_self();
    cout << endl;

    auto *list_3 = new SinglyLinkedList<bool, string, int, float>(false, string("second"), 3, 3.5f);
    cout << "list_3: \t";
    list_3->print();
    list_3->reverse();
    cout << "reversed: \t";
    list_3->print();
    cout << endl;

    auto *bool_list = new SinglyLinkedList<bool>(true, true, false);
    auto *bool_list_2 = new SinglyLinkedList<bool>(true, false, false);

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
