#ifndef SEGMENTDEQUE_HPP
#define SEGMENTDEQUE_HPP

#include <stdexcept>
#include "lib/Sequence.hpp"
#include "lib/ArraySequence.hpp"
#include "lib/ListSequence.hpp"
#include "Iterable.hpp"

template <typename T>
struct Segment {
    MutableArraySequence<T>* data;
    size_t front_offset;
    size_t back_size;

    Segment(MutableArraySequence<T>* arr) : data(arr), front_offset(0), back_size(0) {}

    ~Segment() {
        delete data;
    }

    size_t GetEffectiveSize() const {
        return back_size - front_offset;
    }

    bool IsEmpty() const {
        return front_offset >= back_size;
    }

    T& Get(size_t index) {
        return data->Get(front_offset + index);
    }

    const T& Get(size_t index) const {
        return data->Get(front_offset + index);
    }
};

template <typename T>
class SegmentDeque {
public:
    explicit SegmentDeque(size_t segment_capacity = 16);
    ~SegmentDeque();

    void Append(const T& value);
    void Prepend(const T& value);
    void PopBack();
    void PopFront();

    T& Get(size_t index);
    const T& Get(size_t index) const;

    size_t GetSize() const;
    bool IsEmpty() const;

    size_t GetSegmentCount() const;
    const Segment<T>* GetSegment(size_t index) const;
    Segment<T>* GetSegment(size_t index);

    template <typename Func>
    auto Map(Func func) const -> SegmentDeque<decltype(func(std::declval<T>()))>;

    template <typename Func>
    auto FlatMap(Func func) const -> SegmentDeque<typename decltype(func(std::declval<T>()))::value_type>;

    template <typename Func>
    T Reduce(Func func, T init) const;

    template <typename Func>
    SegmentDeque<T> Where(Func predicate) const;

    Iterator<T>* GetIterator() const;
    Iterator<T>* GetMutableIterator();

private:
    size_t segment_capacity;
    size_t total_size;
    ListSequence<Segment<T>*>* segments;

    void CheckBackCapacity();
    void CheckFrontCapacity();
    void CleanupEmptySegments();
};

template <typename T>
SegmentDeque<T>::SegmentDeque(size_t segment_capacity)
    : segment_capacity(segment_capacity),
      total_size(0),
      segments(new ListSequence<Segment<T>*>()) {
    if (segment_capacity == 0) {
        throw std::invalid_argument("segment_capacity == 0");
    }

    Segment<T>* initial_segment = new Segment<T>(new MutableArraySequence<T>(segment_capacity));
    segments->Append(initial_segment);
}

template <typename T>
SegmentDeque<T>::~SegmentDeque() {
    for (size_t i = 0; i < segments->GetSize(); i++) {
        delete segments->Get(i);
    }

    delete segments;
}

template <typename T>
size_t SegmentDeque<T>::GetSegmentCount() const {
    return segments->GetSize();
}

template <typename T>
const Segment<T>* SegmentDeque<T>::GetSegment(size_t index) const {
    if (index >= segments->GetSize()) {
        throw std::out_of_range("Segment index out of range");
    }
    return segments->Get(index);
}

template <typename T>
Segment<T>* SegmentDeque<T>::GetSegment(size_t index) {
    if (index >= segments->GetSize()) {
        throw std::out_of_range("Segment index out of range");
    }
    return segments->Get(index);
}

template <typename T>
void SegmentDeque<T>::CheckBackCapacity() {
    Segment<T>* last = segments->Get(segments->GetSize() - 1);

    if (last->back_size >= segment_capacity) {
        Segment<T>* new_segment = new Segment<T>(new MutableArraySequence<T>(segment_capacity));
        segments->Append(new_segment);
    }
}

template <typename T>
void SegmentDeque<T>::CheckFrontCapacity() {
    Segment<T>* first = segments->Get(0);

    if (first->front_offset == 0) {
        Segment<T>* new_segment = new Segment<T>(new MutableArraySequence<T>(segment_capacity));
        new_segment->back_size = segment_capacity;
        new_segment->front_offset = segment_capacity;
        segments->Prepend(new_segment);
    }
}

template <typename T>
void SegmentDeque<T>::Append(const T& value) {
    CheckBackCapacity();

    Segment<T>* last = segments->Get(segments->GetSize() - 1);

    if (last->back_size >= last->data->GetSize()) {
        last->data->Append(value);
    } else {
        last->data->Set(value, last->back_size);
    }

    last->back_size++;
    total_size++;
}

template <typename T>
void SegmentDeque<T>::Prepend(const T& value) {
    CheckFrontCapacity();

    Segment<T>* first = segments->Get(0);

    first->front_offset--;

    if (first->front_offset < first->data->GetSize()) {
        first->data->Set(value, first->front_offset);
    } else {
        throw std::runtime_error("Front capacity check failed");
    }
    
    total_size++;
}

template <typename T>
void SegmentDeque<T>::PopBack() {
    if (total_size == 0) {
        throw std::out_of_range("PopBack from empty deque");
    }

    for (int i = segments->GetSize() - 1; i >= 0; i--) {
        Segment<T>* segment = segments->Get(i);
        if (!segment->IsEmpty()) {
            segment->back_size--;
            total_size--;
            break;
        }
    }

    CleanupEmptySegments();
}

template <typename T>
void SegmentDeque<T>::PopFront() {
    if (total_size == 0) {
        throw std::out_of_range("PopFront from empty deque");
    }

    for (size_t i = 0; i < segments->GetSize(); i++) {
        Segment<T>* segment = segments->Get(i);
        if (!segment->IsEmpty()) {
            segment->front_offset++;
            total_size--;
            break;
        }
    }

    CleanupEmptySegments();
}

template <typename T>
void SegmentDeque<T>::CleanupEmptySegments() {
    while (segments->GetSize() > 1 && segments->Get(0)->IsEmpty()) {
        delete segments->Get(0);
        segments->RemoveNode(0);
    }

    while (segments->GetSize() > 1 && segments->Get(segments->GetSize() - 1)->IsEmpty()) {
        delete segments->Get(segments->GetSize() - 1);
        segments->RemoveNode(segments->GetSize() - 1);
    }
}

template <typename T>
T& SegmentDeque<T>::Get(size_t index) {
    if (index >= total_size) {
        throw std::out_of_range("Index out of range");
    }

    size_t current_index = 0;
    for (size_t i = 0; i < segments->GetSize(); i++) {
        Segment<T>* segment = segments->Get(i);
        size_t segment_size = segment->GetEffectiveSize();

        if (current_index + segment_size > index) {
            return segment->Get(index - current_index);
        }

        current_index += segment_size;
    }

    throw std::out_of_range("Index calculation error");
}

template <typename T>
const T& SegmentDeque<T>::Get(size_t index) const {
    if (index >= total_size) {
        throw std::out_of_range("Index out of range");
    }

    size_t current_index = 0;
    for (size_t i = 0; i < segments->GetSize(); i++) {
        Segment<T>* segment = segments->Get(i);
        size_t segment_size = segment->GetEffectiveSize();

        if (current_index + segment_size > index) {
            return segment->Get(index - current_index);
        }

        current_index += segment_size;
    }

    throw std::out_of_range("Index calculation error");
}

template <typename T>
size_t SegmentDeque<T>::GetSize() const {
    return total_size;
}

template <typename T>
bool SegmentDeque<T>::IsEmpty() const {
    return total_size == 0;
}

template <typename T>
template <typename Func>
auto SegmentDeque<T>::Map(Func func) const -> SegmentDeque<decltype(func(std::declval<T>()))> {
    using U = decltype(func(std::declval<T>()));
    SegmentDeque<U> result(segment_capacity);

    for (size_t i = 0; i < GetSize(); ++i) {
        result.Append(func(Get(i)));
    }

    return result;
}

template <typename T>
template <typename Func>
auto SegmentDeque<T>::FlatMap(Func func) const -> SegmentDeque<typename decltype(func(std::declval<T>()))::value_type> {
    using Container = decltype(func(std::declval<T>()));
    using U = typename Container::value_type;

    SegmentDeque<U> result(segment_capacity);

    for (size_t i = 0; i < GetSize(); ++i) {
        Container intermediate = func(Get(i));

        for (size_t j = 0; j < intermediate.GetSize(); ++j) {
            result.Append(intermediate.Get(j));
        }
    }

    return result;
}

template <typename T>
template <typename Func>
T SegmentDeque<T>::Reduce(Func func, T init) const {
    T result = init;

    for (size_t i = 0; i < GetSize(); ++i) {
        result = func(result, Get(i));
    }

    return result;
}

template <typename T>
template <typename Func>
SegmentDeque<T> SegmentDeque<T>::Where(Func predicate) const {
    SegmentDeque<T> result(segment_capacity);

    for (size_t i = 0; i < GetSize(); ++i) {
        if (predicate(Get(i))) {
            result.Append(Get(i));
        }
    }

    return result;
}

template <typename T>
class ConstDequeIterator : public Iterator<T> {
public:
    explicit ConstDequeIterator(const SegmentDeque<T>* deque) : container(deque), segment_idx(0), pos_in_segment(0), started(false) {}

    bool Next() override {
        if (!started) {
            started = true;

            while (segment_idx < container->GetSegmentCount() && container->GetSegment(segment_idx)->IsEmpty()) {
                segment_idx++;
            }

            return segment_idx < container->GetSegmentCount();
        }

        if (segment_idx >= container->GetSegmentCount()) {
            return false;
        }

        const Segment<T>* segment = container->GetSegment(segment_idx);
        pos_in_segment++;

        if (pos_in_segment >= segment->GetEffectiveSize()) {
            segment_idx++;
            pos_in_segment = 0;

            while (segment_idx < container->GetSegmentCount() && container->GetSegment(segment_idx)->IsEmpty()) {
                segment_idx++;
            }
        }

        return segment_idx < container->GetSegmentCount();
    }

    T& Get() override {
        if (!started || segment_idx >= container->GetSegmentCount()) {
            throw std::out_of_range("Iterator out of range");
        }

        const Segment<T>* segment = container->GetSegment(segment_idx);
        return const_cast<Segment<T>*>(segment)->Get(pos_in_segment);
    }

private:
    const SegmentDeque<T>* container;
    size_t segment_idx;
    size_t pos_in_segment;
    bool started;
};

template <typename T>
class MutableDequeIterator : public Iterator<T> {
public:
    explicit MutableDequeIterator(SegmentDeque<T>* deque) : container(deque), segment_idx(0), pos_in_segment(0), started(false) {}

    bool Next() override {
        if (!started) {
            started = true;

            while (segment_idx < container->GetSegmentCount() && container->GetSegment(segment_idx)->IsEmpty()) {
                segment_idx++;
            }

            return segment_idx < container->GetSegmentCount();
        }

        if (segment_idx >= container->GetSegmentCount()) return false;

        Segment<T>* segment = container->GetSegment(segment_idx);
        pos_in_segment++;

        if (pos_in_segment >= segment->GetEffectiveSize()) {
            segment_idx++;
            pos_in_segment = 0;

            while (segment_idx < container->GetSegmentCount() && container->GetSegment(segment_idx)->IsEmpty()) {
                segment_idx++;
            }
        }

        return segment_idx < container->GetSegmentCount();
    }

    T& Get() override {
        if (!started || segment_idx >= container->GetSegmentCount()) {
            throw std::out_of_range("Iterator out of range");
        }

        Segment<T>* segment = container->GetSegment(segment_idx);
        return segment->Get(pos_in_segment);
    }

private:
    SegmentDeque<T>* container;
    size_t segment_idx;
    size_t pos_in_segment;
    bool started;
};

template <typename T>
Iterator<T>* SegmentDeque<T>::GetIterator() const {
    return new ConstDequeIterator<T>(this);
}

template <typename T>
Iterator<T>* SegmentDeque<T>::GetMutableIterator() {
    return new MutableDequeIterator<T>(this);
}

#endif