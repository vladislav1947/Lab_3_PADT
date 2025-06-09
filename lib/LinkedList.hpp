#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <cstddef>
#include <stdexcept>
#include <string>

template <typename T>
class LinkedList {
public:
    LinkedList() = default;
    LinkedList(T* items, size_t size);
    LinkedList(const LinkedList<T>& linked_list);

    LinkedList& operator=(const LinkedList& linked_list);
    LinkedList& operator=(LinkedList&& linked_list);

    ~LinkedList();

    const T& GetFirst() const;
    const T& GetLast() const;
    T& Get(size_t index);
    const T& Get(size_t index) const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    LinkedList<T>* GetSubList(size_t start_index, size_t end_index) const;
    size_t GetSize() const;

    void Append(const T& value);
    void Prepend(const T& value);
    void InsertAt(const T& value, size_t index);
    LinkedList<T>* Concat(const LinkedList<T>* linked_list) const;

    void RemoveNode(size_t index);
private:
    struct Node{
        T value;
        Node* next = nullptr;
        Node* prev = nullptr;

        Node(const T& val) : value(val) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t size = 0;

    void CheckIndex(size_t index) const;
};

template <typename T>
void LinkedList<T>::CheckIndex(size_t index) const {
    if (index >= size || size == 0) {
        throw std::out_of_range("Index " + std::to_string(index) + " is out of range");
    }
}

template <typename T>
LinkedList<T>::LinkedList(T* items, size_t size) {
    if (items == nullptr && size != 0) {
        throw std::invalid_argument("Nullptr with non-zero size");
    }

    for (size_t i = 0; i < size; i++) {
        this->Append(items[i]);
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& linked_list) {
    Node* node = linked_list.head;

    while (node) {
        this->Append(node->value);
        
        node = node->next;
    }
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& linked_list) {
    if (this != &linked_list) {
        Node* current_node = head;
        while (current_node) {
            Node* next_node = current_node->next;

            delete current_node;

            current_node = next_node;
        }

        head = tail = nullptr;
        size = 0;

        Node* node = linked_list.head;
        while (node) {
            this->Append(node->value);

            node = node->next;
        }
    }

    return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& linked_list) {
    if (this != &linked_list) {
        Node* current_node = head;
        while (current_node) {
            Node* next_node = current_node->next;

            delete current_node;

            current_node = next_node;
        }

        head = linked_list.head;
        tail = linked_list.tail;
        size = linked_list.size;

        linked_list.head = linked_list.tail = nullptr;
        linked_list.size = 0;
    }

    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    Node* current_node = head;

    while (current_node) {
        Node* next_node = current_node->next;

        delete current_node;

        current_node = next_node;
    }
}

template <typename T>
const T& LinkedList<T>::GetFirst() const {
    if (!head) {
        throw std::out_of_range("No head of list");
    }

    return head->value;
}

template <typename T>
const T& LinkedList<T>::GetLast() const {
    if (!tail) {
        throw std::out_of_range("No tail of list");
    }

    return tail->value;
}

template <typename T>
T& LinkedList<T>::Get(size_t index) {
    CheckIndex(index);

    Node* current_node = head;

    for (size_t i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    return current_node->value;
}

template <typename T>
const T& LinkedList<T>::Get(size_t index) const {
    CheckIndex(index);

    Node* current_node = head;

    for (size_t i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    return current_node->value;
}

template <typename T>
T& LinkedList<T>::operator[](size_t index) {
    CheckIndex(index);

    return this->Get(index);
}

template <typename T>
const T& LinkedList<T>::operator[](size_t index) const {
    CheckIndex(index);

    return this->Get(index);
}

template <typename T>
LinkedList<T>* LinkedList<T>::GetSubList(size_t start_index, size_t end_index) const {
    if (start_index > end_index || end_index >= size) {
        throw std::out_of_range("Indexes from " + std::to_string(start_index) + " to " + std::to_string(end_index) + " are out of range");
    }

    LinkedList<T>* sub_linked_list = new LinkedList<T>();

    for (size_t i = start_index; i <= end_index; ++i) {
        sub_linked_list->Append(this->Get(i));
    }
    
    return sub_linked_list;
}

template <typename T>
size_t LinkedList<T>::GetSize() const {
    return size;
}

template <typename T>
void LinkedList<T>::Append(const T& value) {
    Node* node = new Node(value);

    if (!tail) {
        head = tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }

    size++;
}

template <typename T>
void LinkedList<T>::Prepend(const T& value) {
    Node* node = new Node(value);

    if (!tail) {
        head = tail = node;
    } else {
        head->prev = node;
        node->next = head;
        head = node;
    }

    size++;
}

template <typename T>
void LinkedList<T>::InsertAt(const T& value, size_t index) {
    if (index != size) {
        CheckIndex(index);
    }

    if (index == 0) {
        Prepend(value);
    } else if (index == size) {
        Append(value);
    } else {
        Node* node = new Node(value);
        Node* current_node;

        if (index < size - index) {
            current_node = head;

            for (size_t i = 0; i < index; i++) {
                current_node = current_node->next;
            }
        } else {
            current_node = tail;

            for (size_t i = 0; i < index; i++) {
                current_node = current_node->prev;
            }
        }

    node->next = current_node;
    node->prev = current_node->prev;

    if (current_node->prev) {
        current_node->prev->next = node;
    }

    current_node->prev = node;

    size++;
    }
}

template <typename T>
LinkedList<T>* LinkedList<T>::Concat(const LinkedList<T>* linked_list) const {
    if (!linked_list || linked_list->GetSize() == 0) {
        throw std::invalid_argument("Argument is nullptr or empty");
    }
    
    LinkedList<T>* concat_linked_list = new LinkedList<T>(*this);

    Node* node = linked_list->head;

    while (node) {
        concat_linked_list->Append(node->value);
        node = node->next;
    }

    return concat_linked_list;
}

template <typename T>
void LinkedList<T>::RemoveNode(size_t index) {
    CheckIndex(index);

    Node* current_node = nullptr;

    if (index < size - index) {
        current_node = head;
        for (size_t i = 0; i < index; i++) {
            current_node = current_node->next;
        }
    } else {
        current_node = tail;
        for (size_t i = size - 1; i > index; --i) {
            current_node = current_node->prev;
        }
    }

    if (current_node->prev) {
        current_node->prev->next = current_node->next;
    } else {
        head = current_node->next;
    }

    if (current_node->next) {
        current_node->next->prev = current_node->prev;
    } else {
        tail = current_node->prev;
    }

    delete current_node;
    size--;
}

#endif