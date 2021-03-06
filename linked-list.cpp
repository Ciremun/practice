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
        if constexpr (is_same_v<T, const Node<Types...> *>)
        {
            remove_node(data);
            return;
        }
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

    void remove_node(const Node<Types...> *target)
    {
        if (target == NULL)
        {
            return;
        }
        if (target == head)
        {
            head = target->next;
            delete target;
            return;
        }
        Node<Types...> *temp = head;
        Node<Types...> *prev = NULL;
        while (temp != NULL && temp != target)
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
    using SLL = SinglyLinkedList<int, float, string, bool>;
    auto *list_1 = new SLL(69, 420.5f, string("dungeon master"));
    list_1->print();
    cout << "(*list_1)[2] == `dungeon master`: " << (get<string>(list_1->at(2)->data) == string("dungeon master")) << endl;
    cout << "size: " << list_1->size() << endl;
    cout << "contains `420.5f`: " << list_1->contains(420.5f) << endl;
    cout << "print list_1[1]: ";
    (*list_1)[1]->print_self();
    cout << "remove list_1[1] by ptr" << endl;
    list_1->remove((*list_1)[1]);
    list_1->print();
    cout << "insert `true` after head" << endl;
    list_1->insert(list_1->head, true);
    list_1->print();
    cout << "remove true by value" << endl;
    list_1->remove(true);
    list_1->print();
    cout << "remove at 1" << endl;
    list_1->remove_at(1);
    list_1->print();
    cout << "push 3.14f" << endl;
    list_1->push(3.14f);
    list_1->print();
    cout << "reverse" << endl;
    list_1->reverse();
    list_1->print();
    cout << "69 == 3.14: " << (list_1->head->data == list_1->head->next->data) << endl;
    cout << "69 != 3.14: " << (list_1->head->data != list_1->head->next->data) << endl;
    delete list_1;
    return 0;
}
