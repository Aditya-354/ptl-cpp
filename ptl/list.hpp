#ifndef __LIST_PTL_HPP__
#define __LIST_PTL_HPP__

#include <initializer_list>
#include <iostream>

namespace ptl {
    template<typename NodeType>
        struct Node {
            NodeType data;
            Node* next;
        };

    template<typename T>
        class ListIterator
        {
            public:
                using ValueType = typename T::ValueType;
                using NodePtr = typename T::NodePtr;

                ListIterator() = default;

                ListIterator(NodePtr ptr)
                    : list_ptr{ptr}
                {}

                auto operator++() -> NodePtr {
                    list_ptr = list_ptr->next;
                    return this->list_ptr;
                }

                auto operator++(int) -> NodePtr {
                    NodePtr current = list_ptr;
                    list_ptr++;
                    return current;
                }

                auto operator--() -> NodePtr {
                    list_ptr = list_ptr->next;
                    return *this;
                }

                auto operator--(int) -> NodePtr {
                    NodePtr current = list_ptr;
                    list_ptr--;
                    return current;
                }

                auto operator*() -> ValueType {
                    return list_ptr->data;
                }

                auto operator->() -> NodePtr {
                    return list_ptr;
                }

                auto operator==(const ListIterator& other) -> bool {
                    return this->list_ptr == other.list_ptr;
                }

                auto operator!=(const ListIterator& other) -> bool {
                    return !(*this == other);
                }
                

            private:
                NodePtr list_ptr;
        };

    template<typename T>
        class LinkedList {
            public:
                using ValueType = T;
                using iterator = ListIterator<LinkedList<T>>;
                using NodePtr = Node<T>*;
            private:
                NodePtr m_ListHead {nullptr};
                NodePtr m_ListTail {nullptr};
                size_t m_Size {0};

            public:
                LinkedList() = default;

                LinkedList(std::initializer_list<T> elements)
                    : m_ListHead(nullptr), m_ListTail(nullptr), m_Size(0) {
                        for (auto it { elements.begin() }; it != elements.end(); ++ it) {
                            push_back(*it);
                        }
                    }

                LinkedList(const LinkedList<T>& other) {
                    this->m_ListHead = nullptr;
                    this->m_ListTail = nullptr;
                    this->m_Size = 0;
                    NodePtr ptr { other.m_ListHead };
                    while (ptr != nullptr) {
                        push_back(ptr->data);
                        ptr = ptr->next;
                    }
                }

                LinkedList(LinkedList<T>&& other) {
                    if (this != &other)
                    {
                        clear();
                        this->m_Size = other.m_Size;
                        this->m_ListHead = other.m_ListHead;
                        this->m_ListTail = other.m_ListTail;

                        other.m_Size = 0;
                        other.m_ListHead = nullptr;
                        other.m_ListTail = nullptr;
                    }
                }

                auto operator=(LinkedList<T>&& other) -> LinkedList<T>& {
                    if (this != &other) {
                        clear();
                        this->m_Size = other.m_Size;
                        this->m_ListHead = other.m_ListHead;
                        this->m_ListTail = other.m_ListTail;

                        other.m_Size = 0;
                        other.m_ListHead = nullptr;
                        other.m_ListTail = nullptr;
                    }
                    return *this;
                }

                auto operator=(const LinkedList<T>& other) -> LinkedList<T>& {
                    NodePtr ptr { other.m_ListHead };
                    while (ptr != nullptr) {
                        push_back(ptr->data);
                        ptr = ptr->next;
                    }
                    return *this;
                }

               auto push_back(T new_data) -> void {
                    if (m_Size == 0) {
                        push_front(new_data);
                        return;
                    }
                    NodePtr temp { new Node<T>() };
                    temp->data = new_data;
                    temp->next = nullptr;
                    m_ListTail->next = temp;
                    m_ListTail = temp;
                    m_Size ++;
                }

                auto push_front(T new_data) -> void {
                    NodePtr temp { new Node<T>() };
                    temp->data = new_data;
                    temp->next = m_ListHead;
                    m_ListHead = temp;
                    if (m_Size == 0) {
                        m_ListTail = m_ListHead;
                    }
                    m_Size ++;
                }

                auto insert(iterator it, const T& data) -> void {
                    NodePtr temp { new Node<T>() };
                    temp->data = data;
                    temp->next = it->next;
                    it->next = temp;
                    m_Size++;
                }

                auto pop_front() -> void {
                    if (m_Size == 0) return;

                    if (m_Size == 1) {
                        delete m_ListHead;
                        m_ListHead = nullptr;
                        m_ListTail = m_ListHead;
                        m_Size --;
                        return;
                    }

                    NodePtr current { m_ListHead };
                    m_ListHead = m_ListHead->next;
                    delete current;
                    m_Size --;
                }

                auto pop_back() -> void {
                    if (m_Size == 0) return;

                    if (m_Size == 1) {
                        delete m_ListHead;
                        m_ListHead = nullptr;
                        m_ListTail = m_ListHead;
                        m_Size --;
                        return;
                    }

                    NodePtr ptr { m_ListHead };
                    while (ptr->next != nullptr)
                    {
                        if (ptr->next->next == nullptr) m_ListTail = ptr;
                        ptr = ptr->next;
                    }
                    delete ptr->next;
                    m_ListTail->next = nullptr;
                    m_Size --;
                }

                auto begin() -> iterator {
                    return {m_ListHead};
                }

                auto end() -> iterator {
                    return {m_ListTail->next};
                }

                auto clear() -> void {
                    if (m_Size == 0) return;

                    NodePtr current {m_ListHead};
                    NodePtr next_node {nullptr};

                    while (current != nullptr)
                    {
                        next_node = current->next;
                        delete current;
                        current = next_node;
                    }
                    m_ListHead = nullptr;
                    m_ListTail = nullptr;
                    m_Size = 0;
                }

                auto size() const -> size_t {
                    return m_Size;
                }

                auto at(size_t index) const -> T& {
                    if (index >= m_Size) throw std::logic_error("Index out of bounds.");
                    size_t i {};
                    NodePtr list { m_ListHead };
                    while (i < m_Size && i != index && list != nullptr) {
                        list = list->next;
                        i ++;
                    }
                    return list->data;
                }

                auto head() const -> NodePtr {
                    return m_ListHead;
                }

                auto tail() const -> NodePtr {
                    return m_ListTail;
                }

                auto front() const -> const T& {
                    return m_ListHead->data;
                }


                auto back() const -> const T& {
                    return m_ListTail->data;
                }

                ~LinkedList() {
                    NodePtr current { m_ListHead };
                    NodePtr next_node {};

                    while (current != nullptr) {
                        next_node = current->next;
                        delete current;
                        current = next_node;
                    }
                    std::cout << "Linked list destroyed safely.\n";
                }
        };
}
#endif
