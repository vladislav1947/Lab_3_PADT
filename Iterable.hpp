#ifndef ITERABLE_HPP
#define ITERABLE_HPP

template <typename T>
class Iterator {
public:
    virtual ~Iterator() = default;

    virtual bool Next() = 0;

    virtual T& Get() = 0;
};

template <typename T>
class Iterable {
public:
    virtual ~Iterable() = default;

    virtual Iterator<T>* GetIterator() const = 0;
};

#endif