#ifndef __DSA_LIST_HPP__
#define __DSA_LIST_HPP__

#include <initializer_list>
#include <iostream>
#include <memory>
#include <cassert>

namespace ptl {
    template<typename T>

        class ListIterator
        {
            public:
                using ValueType = typename T::ValueType;
                using NodePtr = typename T::NodePtr;
                using Iterator = typename T::iterator;

            private:
                NodePtr m_ptr {nullptr};

            public:
                ListIterator() = default;

                ListIterator(NodePtr ptr) : m_ptr{ptr} {}

                auto operator++() -> Iterator
                {
                    m_ptr = m_ptr->next;
                    return *this;
                }

                auto operator++(int) -> Iterator
                {
                    NodePtr current {m_ptr};
                    ++(*this);
                    return current;
                }

                auto operator--() -> Iterator
                {
                    m_ptr = m_ptr->prev;
                    return *this;
                }

                auto operator--(int) -> Iterator
                {
                    NodePtr current {m_ptr};
                    --(*this);
                    return current;
                }

                auto operator*() -> ValueType&
                {
                    return m_ptr->data;
                }

                auto operator->() -> NodePtr& {
                    return m_ptr;
                }

                explicit operator bool() const {
                    return m_ptr != nullptr;
                }

                auto operator==(ListIterator& other) -> bool
                {
                    return (this->m_ptr == other.m_ptr);
                }

                auto operator!=(const ListIterator& other) -> bool
                {
                    return !(*this == other);
                }

        };

    template<typename T, typename Alloc = std::allocator<T>>
        class List
        {
            public:
                using ValueType = T;

            private:
                struct Node
                {
                    Node* prev;
                    Node* next;
                    ValueType data;

                    template<typename... Args>
                        Node(Args&&... args) : data{std::forward<Args>(args)...}
                    {}

                    auto operator*() -> ValueType {
                        return this->data;
                    }
                };

            public:
                using NodePtr = Node*;
                using size_type = size_t;
                using NodeAlloc = std::allocator_traits<Alloc>::template rebind_alloc<Node>;
                using AllocTraits = std::allocator_traits<NodeAlloc>;
                using iterator = ListIterator<List<T>>;

            private:
                NodeAlloc m_allocator;
                NodePtr m_head {nullptr};
                NodePtr m_tail {nullptr};
                NodePtr m_end {nullptr};
                size_type m_size {};

                auto construct_end() -> void {
                    m_end = AllocTraits::allocate(m_allocator, 1);
                    AllocTraits::construct(m_allocator, m_end, T{});
                    m_end->prev = m_tail;
                    m_end->next = nullptr;
                }

                auto destroy_end() -> void {
                      AllocTraits::destroy(m_allocator, m_end);
                      AllocTraits::deallocate(m_allocator, m_end, 1);
                }

            public:
                List() {
                    construct_end();
                }

                List(std::initializer_list<T> elements)
                {
                    construct_end();

                    for (auto it {elements.begin()}; it != elements.end(); ++it)
                    {
                        push_back(*it);
                    }
                }

                auto clear() -> void
                {
                    if (m_head == nullptr && m_head == m_tail) return;

                    NodePtr current {m_head};
                    NodePtr next_node {nullptr};

                    while (current != m_end)
                    {
                        next_node = current->next;
                        AllocTraits::destroy(m_allocator, current);
                        AllocTraits::deallocate(m_allocator, current, 1);
                        current = next_node;
                    }

                    m_head = m_tail = nullptr;
                    m_end->prev = nullptr;
                    m_size = 0;
                }

                List(const List<T>& other)
                {
                    clear();
                    construct_end();

                    for (NodePtr ptr {other.m_head}; ptr != nullptr; ptr = ptr->next)
                    {
                        push_back(ptr->data);
                    }
                }

                List(List<T>&& other)
                {
                    clear();

                    this->m_head = other.m_head;
                    this->m_tail = other.m_tail;
                    this->m_end = other.m_end;
                    this->m_size = other.m_size;

                    other.m_head = nullptr;
                    other.m_tail = nullptr;
                    other.m_end = nullptr;
                    other.m_size = 0;
                }

                auto operator=(const List<T>& other) -> List<T>&
                {
                    clear();

                    for (NodePtr ptr {other.m_head}; ptr != nullptr; ptr = ptr->next)
                    {
                        push_back(ptr->data);
                    }

                    return *this;
                }

                auto operator=(List<T>&& other) -> List<T>&
                {
                    clear();

                    this->m_head = other.m_head;
                    this->m_tail = other.m_tail;
                    this->m_end = other.m_end;
                    this->m_size = other.m_size;

                    other.m_head = nullptr;
                    other.m_tail = nullptr;
                    other.m_end = nullptr;
                    other.m_size = 0;

                    return *this;
                }

                constexpr auto empty() const -> bool
                {
                    return m_size == 0;
                }

                constexpr auto size() const -> size_t
                {
                    return m_size;
                }

                auto front() const -> ValueType
                {
                    assert(this->m_head != nullptr);
                    return m_head->data;
                }

                auto back() const -> ValueType
                {
                    assert(this->m_tail != nullptr);
                    return m_tail->data;
                }


                auto insert(iterator pos, const T& val) -> void
                {
                    NodePtr temp { AllocTraits::allocate(m_allocator, 1) };
                    AllocTraits::construct(m_allocator, temp, val);

                    // insert before pos
                    if (pos && pos->prev == nullptr) {
                        temp->prev = nullptr;
                        temp->next = m_head;
                        m_head->prev = temp;
                        m_head = temp;
                    } else if (pos && pos->next == nullptr) {
                        m_tail->prev->next = temp;
                        temp->prev = m_tail->prev;
                        temp->next = m_tail;
                        m_tail->prev = temp;
                    } else if (pos) {
                        temp->next = pos->next->prev;
                        temp->prev = pos->prev;
                        pos->prev->next = temp;
                        pos->prev = temp;
                    } else {
                        temp->prev = temp->next = nullptr;
                        m_head = m_tail = temp;
                        m_tail->next = m_end;
                    }
                    m_size++;
                }

                auto erase(iterator pos) -> void
                {
                    assert(!empty());

                    if (pos == iterator{m_head} && m_head == m_tail) {
                        AllocTraits::destroy(m_allocator, m_head);
                        AllocTraits::deallocate(m_allocator, m_head, 1);
                        m_head = m_tail = nullptr;
                        m_size--;
                        return;
                    }

                    NodePtr erase_node;

                    if (pos && pos->prev == nullptr) {
                        erase_node = m_head;
                        m_head = m_head->next;
                        m_head->prev = nullptr;
                    } else if (pos && pos->next == nullptr) {
                        erase_node = m_tail;
                        m_tail = m_tail->prev;
                        m_tail->next = m_end;
                    } else if (pos) {
                        erase_node = pos->prev->next;
                        pos->prev->next = pos->next;
                        pos->next->prev = pos->prev;
                    }

                    AllocTraits::destroy(m_allocator, erase_node);
                    AllocTraits::deallocate(m_allocator, erase_node, 1);

                    m_size--;
                }

                template<typename... Args>
                    auto emplace_back(Args&&... args) -> void
                    {
                        NodePtr temp { AllocTraits::allocate(m_allocator, 1) };
                        AllocTraits::construct(m_allocator, temp, std::forward<Args>(args)...);

                        if (!m_head)
                        {
                            temp->prev = nullptr;
                            temp->next = m_end;
                            m_head = m_tail = temp;
                        } 
                        else
                        {
                            m_tail->next = temp;
                            temp->next = m_end;
                            temp->prev = m_tail;
                            m_end->prev = temp;
                            m_tail = temp;
                        }
                        m_size++;
                    }

                auto push_back(const T& obj) -> void
                {
                    NodePtr temp { AllocTraits::allocate(m_allocator, 1) };
                    AllocTraits::construct(m_allocator, temp, obj);

                    if (!m_head)
                    {
                        temp->prev = nullptr;
                        temp->next = m_end;
                        m_head = m_tail = temp;
                    } 
                    else
                    {
                        m_tail->next = temp;
                        temp->next = m_end;
                        m_end->prev = temp;
                        temp->prev = m_tail;
                        m_tail = temp;
                    }
                    m_size++;
                }

                auto push_back(T&& obj) -> void
                {
                    emplace_back(std::move(obj));
                }


                auto pop_back() -> void
                {
                    assert(!empty());

                    if (m_size == 1) {
                        AllocTraits::destroy(m_allocator, m_head);
                        AllocTraits::deallocate(m_allocator, m_head, 1);
                        m_head = m_tail = nullptr;
                        m_size--;
                        return;
                    }

                    NodePtr current {m_tail};
                    m_tail = m_tail->prev;
                    m_tail->next = m_end;
                    AllocTraits::destroy(m_allocator, current);
                    AllocTraits::deallocate(m_allocator, current, 1);
                    m_size--;
                }

                auto pop_front() -> void {
                    assert(!empty());

                    if (m_size == 1) {
                        AllocTraits::destroy(m_allocator, m_head);
                        AllocTraits::deallocate(m_allocator, m_head, 1);
                        m_head = m_tail = nullptr;
                        m_size--;
                        return;
                    }

                    NodePtr current {m_head};
                    m_head = m_head->next;
                    m_head->prev = nullptr;
                    AllocTraits::destroy(m_allocator, current);
                    AllocTraits::deallocate(m_allocator, current, 1);
                    m_size--;
                }

                auto begin() -> iterator
                {
                    return iterator{m_head};
                }

                auto cbegin() -> const iterator {
                    return iterator{m_head};
                }

                auto rbegin() -> iterator {
                    return iterator{m_tail};
                }

                auto end() -> iterator
                {
                    return iterator{m_end};
                }

                auto cend() -> const iterator
                {
                    return iterator{m_end};
                }

                auto rend() -> iterator
                {
                    return iterator{m_end};
                }

                ~List()
                {
                    clear();
                    destroy_end();
                    std::cout << "List destroyed successfully.\n";
                }
        };
}
#endif
