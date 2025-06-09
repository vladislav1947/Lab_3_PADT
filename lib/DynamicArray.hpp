#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <cstddef>
#include <stdexcept>
#include <string>

template <typename T>
class DynamicArray {
public:
    DynamicArray(T* items, size_t size);
    DynamicArray(size_t size);
    DynamicArray(const DynamicArray<T>& dynamic_array);

    DynamicArray& operator=(const DynamicArray& dynamic_array);
    DynamicArray& operator=(DynamicArray&& dynamic_array);

    ~DynamicArray();

    T& Get(size_t index);
    const T& Get(size_t index) const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    size_t GetSize() const;
    void Set(const T& value, size_t index);
    void Resize(size_t new_size);
    void InsertAt(const T& value, size_t index);
private:
    T* items;
    size_t size;

    void CheckIndex(size_t index) const;
};

template <typename T>
void DynamicArray<T>::CheckIndex(size_t index) const {
    if (index >= size || size == 0) {
        throw std::out_of_range("Index " + std::to_string(index) + " is out of range");
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(T* items, size_t size) : size(size), items(new T[size]) {
    if (items == nullptr && size != 0) {
        throw std::invalid_argument("Nullptr with non-zero size");
    }

    for (size_t i = 0; i < size; i++) {
        this->items[i] = items[i];
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(size_t size) : size(size), items(new T[size]{}) {}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& dynamic_array) : size(dynamic_array.size), items(new T[dynamic_array.size]) {
    for (size_t i = 0; i < size; i++) {
        this->items[i] = dynamic_array.items[i];
    }
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& dynamic_array) {
    if (this != &dynamic_array) {
        T* new_items = new T[dynamic_array.size];

        for (size_t i = 0; i < dynamic_array.size; i++)
            new_items[i] = dynamic_array.items[i];

        delete[] items;

        items = new_items;
        size = dynamic_array.size;
    }

    return *this;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray&& dynamic_array) {
    if (this != &dynamic_array) {
        delete []items;

        items = dynamic_array.items;
        size = dynamic_array.size;

        dynamic_array.items = nullptr;
        dynamic_array.size = 0;
    }

    return *this;
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete []items;
    size = 0;
}

template <typename T>
T& DynamicArray<T>::Get(size_t index) {
    CheckIndex(index);
    return items[index];
}

template <typename T>
const T& DynamicArray<T>::Get(size_t index) const {
    CheckIndex(index);
    return items[index];
}

template <typename T>
T& DynamicArray<T>::operator[](size_t index) {
    CheckIndex(index);
    return items[index];
}

template <typename T>
const T& DynamicArray<T>::operator[](size_t index) const {
    CheckIndex(index);
    return items[index];
}

template <typename T>
size_t DynamicArray<T>::GetSize() const {
    return size;
}

template <typename T>
void DynamicArray<T>::Set(const T& value, size_t index) {
    CheckIndex(index);
    items[index] = value;
}

template <typename T>
void DynamicArray<T>::Resize(size_t new_size) {
    T* new_items = new T[new_size]{};

    size_t min_size = (size < new_size) ? size : new_size;

    size_t i = 0;
    for (i = 0; i < min_size; i++) {
        new_items[i] = items[i];
    }

    delete []items;
    items = new_items;
    size = new_size;
}

template <typename T>
void DynamicArray<T>::InsertAt(const T& value, size_t index) {
    if (index != size) {
        CheckIndex(index);
    }

    size++;

    T* new_items = new T[size];

    size_t i;
    for (i = 0; i < index; i++) {
        new_items[i] = items[i];
    }

    new_items[index] = value;

    index++;
    for (i = index; i < size; i++) {
        new_items[i] = items[i - 1];
    }

    delete []items;
    items = new_items;
}

#endif