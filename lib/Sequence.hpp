#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

template <typename T>
class Sequence {
public:
    virtual ~Sequence() = default;

    virtual T& GetFirst() = 0;
    virtual const T& GetFirst() const = 0;
    virtual T& GetLast() = 0;
    virtual const T& GetLast() const = 0;
    virtual T& Get(size_t index) = 0;
    virtual const T& Get(size_t index) const = 0;
    virtual Sequence<T>* GetSubsequence(size_t start_index, size_t end_index) const = 0;
    virtual size_t GetSize() const = 0;

    virtual void Append(const T& value) = 0;
    virtual void Prepend(const T& value) = 0;
    virtual void InsertAt(const T& value, size_t index) = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* sequence) const = 0;
};

#endif