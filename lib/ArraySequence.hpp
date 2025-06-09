#ifndef ARRAYSEQUENCE_HPP
#define ARRAYSEQUENCE_HPP

#include <cstddef>
#include "Sequence.hpp"
#include "DynamicArray.hpp"

template <typename T>
class ArraySequence : public Sequence<T> {
public:
    ArraySequence();
    ArraySequence(T* items, size_t size);
    ArraySequence(size_t size);
    ArraySequence(const ArraySequence<T>& array_sequence);

    ArraySequence& operator=(const ArraySequence& array_sequence);
    ArraySequence& operator=(ArraySequence&& array_sequence);

    virtual ~ArraySequence();

    T& GetFirst() override;
    const T& GetFirst() const override;
    T& GetLast() override;
    const T& GetLast() const override;
    T& Get(size_t index) override;
    const T& Get(size_t index) const override;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    ArraySequence<T>* GetSubsequence(size_t start_index, size_t end_index) const override;
    size_t GetSize() const override;
    void Set(T value, size_t index);

    void Append(const T& value) override;
    void Prepend(const T& value) override;
    void InsertAt(const T& value, size_t index) override;
    ArraySequence<T>* Concat(const Sequence<T>* array_sequence) const override;
protected:
    DynamicArray<T>* items;

    virtual ArraySequence<T>* Instance() = 0;
    virtual ArraySequence<T>* Clone() const = 0;
};

template <typename T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence() = default;
    using ArraySequence<T>::ArraySequence;
private:
    ArraySequence<T>* Instance() override{
        return this;
    }

    ArraySequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    using ArraySequence<T>::ArraySequence;
private:
    ArraySequence<T>* Instance() override{
        return this->Clone();
    }

    ArraySequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }
};

template <typename T>
ArraySequence<T>::ArraySequence() : items(new DynamicArray<T>(0)) {}

template <typename T>
ArraySequence<T>::ArraySequence(T *items, size_t size) {
    if (items == nullptr && size != 0) {
        throw std::invalid_argument("nullptr with non-zero size");
    }

    this->items = new DynamicArray<T>(items, size);
}

template <typename T>
ArraySequence<T>::ArraySequence(size_t size) : items(new DynamicArray<T>(size)) {}

template <typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& array_sequence) {
    items = new DynamicArray<T>(*array_sequence.items);
}

template <typename T>
ArraySequence<T>& ArraySequence<T>::operator=(const ArraySequence& array_sequence) {
    if (this != &array_sequence) {
        delete items;
        items = new DynamicArray<T>(*array_sequence.items);
    }

    return *this;
}

template <typename T>
ArraySequence<T>& ArraySequence<T>::operator=(ArraySequence&& array_sequence) {
    if (this != &array_sequence) {
         std::swap(this->items, array_sequence.items);
    }

    return *this;
}

template <typename T>
ArraySequence<T>::~ArraySequence() {
    delete items;
}

template <typename T>
T& ArraySequence<T>::GetFirst() {
    return items->Get(0);
}

template <typename T>
const T& ArraySequence<T>::GetFirst() const {
    return items->Get(0);
}

template <typename T>
T& ArraySequence<T>::GetLast() {
    return items->Get(items->GetSize() - 1);
}

template <typename T>
const T& ArraySequence<T>::GetLast() const {
    return items->Get(items->GetSize() - 1);
}

template <typename T>
T& ArraySequence<T>::Get(size_t index) {
    return items->Get(index);
}

template <typename T>
const T& ArraySequence<T>::Get(size_t index) const {
    return items->Get(index);
}

template <typename T>
T& ArraySequence<T>::operator[](size_t index) {
    return (*items)[index];
}

template <typename T>
const T& ArraySequence<T>::operator[](size_t index) const {
    return (*items)[index];
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::GetSubsequence(size_t start_index, size_t end_index) const {
    if (start_index > end_index || end_index >= items->GetSize()) {
        throw std::out_of_range("Indexes from " + std::to_string(start_index) + " to " + std::to_string(end_index) + " are out of range");
    }

    size_t length = end_index - start_index + 1;

    T* temp_items = new T[length];

    for (size_t i = 0; i < length; i++) {
        temp_items[i] = items->Get(i + start_index);
    }

    ArraySequence<T>* sub_sequence = this->Clone();
    delete sub_sequence->items;
    sub_sequence->items = new DynamicArray<T>(temp_items, length);

    delete[] temp_items;
    return sub_sequence;
}

template <typename T>
size_t ArraySequence<T>::GetSize() const {
    return items->GetSize();
}

template <typename T>
void ArraySequence<T>::Set(T value, size_t index) {
    ArraySequence<T>* instance = Instance();
    instance->items->Set(value, index);

    if (instance != this) {
        *this = *instance;
        delete instance;
    }
}

template <typename T>
void ArraySequence<T>::Append(const T& value) {
    ArraySequence<T>* instance = Instance();
    instance->items->Resize(instance->items->GetSize() + 1);
    instance->items->Set(value, instance->items->GetSize() - 1);

    if (instance != this) {
        *this = *instance;
        delete instance;
    }
}

template <typename T>
void ArraySequence<T>::Prepend(const T& value) {
    ArraySequence<T>* instance = Instance();
    instance->items->Resize(instance->items->GetSize() + 1);

    for (size_t i = instance->items->GetSize() - 1; i > 0; i--) {
        instance->items->Set(instance->items->Get(i - 1), i);
    }

    instance->items->Set(value, 0);

    if (instance != this) {
        *this = *instance;
        delete instance;
    }
}

template <typename T>
void ArraySequence<T>::InsertAt(const T& value, size_t index) {
    if (index == 0) {
        Prepend(value);
    } else if (index == this->GetSize()) {
        Append(value);
    } else {
        ArraySequence<T>* instance = Instance();
        instance->items->InsertAt(value, index);

        if (instance != this) {
            *this = *instance;
            delete instance;
        }
    }
}

template <typename T>
ArraySequence<T>* ArraySequence<T>::Concat(const Sequence<T>* array_sequence) const {
    if (!array_sequence || array_sequence->GetSize() == 0) {
        throw std::invalid_argument("Argument is nullptr or empty");
    }

    ArraySequence<T>* result = this->Clone();
    size_t original_size = this->GetSize();

    result->items->Resize(original_size + array_sequence->GetSize());

    for (size_t i = 0; i < array_sequence->GetSize(); i++) {
        result->items->Set(array_sequence->Get(i), original_size + i);
    }

    return result;
}

#endif