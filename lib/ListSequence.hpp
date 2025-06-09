#ifndef LISTSEQUENCE_HPP
#define LISTSEQUENCE_HPP

#include <cstddef>
#include "Sequence.hpp"
#include "LinkedList.hpp"

template <typename T>
class ListSequence : public Sequence<T> {
public:
    ListSequence();
    ListSequence(T* items, size_t size);
    ListSequence(const ListSequence<T>& list_sequence);
    ListSequence(LinkedList<T>* list_sequence);

    ListSequence& operator=(const ListSequence& list_sequence);
    ListSequence& operator=(ListSequence&& list_sequence);

    ~ListSequence();

    T& GetFirst() override;
    const T& GetFirst() const override;
    T& GetLast() override;
    const T& GetLast() const override;
    T& Get(size_t index) override;
    const T& Get(size_t index) const override;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    ListSequence<T>* GetSubsequence(size_t start_index, size_t end_index) const override;
    size_t GetSize() const override;

    void Append(const T& value) override;
    void Prepend(const T& value) override;
    void InsertAt(const T& value, size_t index);
    ListSequence<T>* Concat(const Sequence<T>* list_sequence) const override;

    void RemoveNode(size_t index);
private:
    LinkedList<T>* items;
};

template <typename T>
ListSequence<T>::ListSequence() : items(new LinkedList<T>()) {}

template <typename T>
ListSequence<T>::ListSequence(T* items, size_t size) {
    if (!items && size != 0) {
        throw std::invalid_argument("Nullptr with non-zero size");
    }

    this->items = new LinkedList<T>(items, size);
}

template <typename T>
ListSequence<T>::ListSequence(const ListSequence<T>& list_sequence) {
    this->items = new LinkedList<T>(*list_sequence.items);
}

template <typename T>
ListSequence<T>::ListSequence(LinkedList<T>* list_sequence) {
    if (!list_sequence)
        throw std::invalid_argument("Null linked list");
    this->items = list_sequence;
}

template <typename T>
ListSequence<T>& ListSequence<T>::operator=(const ListSequence& list_sequence) {
    if (this != &list_sequence) {
        delete items;
        items = new LinkedList<T>(*list_sequence.items);
    }

    return *this;
}

template <typename T>
ListSequence<T>& ListSequence<T>::operator=(ListSequence&& list_sequence) {
    if (this != &list_sequence) {
        delete items;
        items = list_sequence.items;
        list_sequence.items = nullptr;
    }

    return *this;
}

template <typename T>
ListSequence<T>::~ListSequence() {
    delete items;
}

template <typename T>
T& ListSequence<T>::GetFirst() {
    return const_cast<T&>(static_cast<const ListSequence<T>*>(this)->GetFirst());
}

template <typename T>
const T& ListSequence<T>::GetFirst() const {
    return items->GetFirst();
}

template <typename T>
T& ListSequence<T>::GetLast() {
    return const_cast<T&>(static_cast<const ListSequence<T>*>(this)->GetLast());
}

template <typename T>
const T& ListSequence<T>::GetLast() const {
    return items->GetLast();
}

template <typename T>
T& ListSequence<T>::Get(size_t index) {
    return items->Get(index);
}

template <typename T>
const T& ListSequence<T>::Get(size_t index) const {
    return items->Get(index);
}

template <typename T>
T& ListSequence<T>::operator[](size_t index) {
    return (*items)[index];
}

template <typename T>
const T& ListSequence<T>::operator[](size_t index) const {
    return (*items)[index];
}

template <typename T>
ListSequence<T>* ListSequence<T>::GetSubsequence(size_t start_index, size_t end_index) const {
    ListSequence<T>* sub_list_sequence = new ListSequence<T>(items->GetSubList(start_index,end_index));
    return sub_list_sequence;
}

template <typename T>
size_t ListSequence<T>::GetSize() const {
    return items->GetSize();
}

template <typename T>
void ListSequence<T>::Append(const T& value) {
    items->Append(value);
}

template <typename T>
void ListSequence<T>::Prepend(const T& value) {
    items->Prepend(value);
}

template <typename T>
void ListSequence<T>::InsertAt(const T& value, size_t index) {
    if (index == 0) {
        Prepend(value);
    } else if (index == this->GetSize()) {
        Append(value);
    } else {
        items->InsertAt(value, index);
    }
}

template <typename T>
ListSequence<T>* ListSequence<T>::Concat(const Sequence<T>* list_sequence) const {
    auto* other_list = dynamic_cast<const ListSequence<T>*>(list_sequence);
    if (!other_list) {
        throw std::invalid_argument("Can only concat with ListSequence");
    }

    ListSequence<T>* new_items = new ListSequence<T>(this->items->Concat(other_list->items));

    return new_items;
}

template <typename T>
void ListSequence<T>::RemoveNode(size_t index) {
    items->RemoveNode(index);
}

#endif