#ifndef TESTS_HPP
#define TESTS_HPP

#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <stdexcept>
#include "SegmentDeque.hpp"

class Tests {
public:
    static void RunAllTests() {
        std::cout << "Running SegmentDeque Tests\n";
        TestConstructor();
        TestAppendAndGet();
        TestPrependAndGet();
        TestPopBack();
        TestPopFront();
        TestMixedOperations();
        TestBoundaryConditions();
        TestIterator();
        TestConstIterator();
        TestFunctionalMethods();
        TestLargeOperations();
        TestExceptions();
        TestWithStrings();
        std::cout << "All tests passed\n";
    }

private:
    static void TestConstructor() {
        std::cout << "Testing Constructor\n";

        SegmentDeque<int> deque1;
        assert(deque1.IsEmpty() && deque1.GetSize() == 0);

        SegmentDeque<int> deque2(32);
        assert(deque2.IsEmpty() && deque2.GetSize() == 0);

        try {
            SegmentDeque<int> deque3(0);
            assert(false);
        } catch (const std::invalid_argument&) {}

        std::cout << "Constructor tests passed\n";
    }

    static void TestAppendAndGet() {
        std::cout << "Testing Append and Get\n";

        SegmentDeque<int> deque;
        deque.Append(42);
        assert(deque.GetSize() == 1 && !deque.IsEmpty() && deque.Get(0) == 42);

        for (int i = 1; i <= 10; i++) {
            deque.Append(i);
        }

        assert(deque.GetSize() == 11 && deque.Get(0) == 42);
        for (int i = 1; i <= 10; i++) {
            assert(deque.Get(i) == i);
        }

        std::cout << "Append and Get tests passed\n";
    }

    static void TestPrependAndGet() {
        std::cout << "Testing Prepend and Get\n";

        SegmentDeque<int> deque;
        deque.Prepend(42);
        assert(deque.GetSize() == 1 && deque.Get(0) == 42);

        for (int i = 1; i <= 10; i++) {
            deque.Prepend(i);
        }

        assert(deque.GetSize() == 11 && deque.Get(0) == 10 && deque.Get(10) == 42);
        for (int i = 0; i < 10; i++) {
            assert(deque.Get(i) == 10 - i);
        }

        std::cout << "Prepend and Get tests passed\n";
    }

    static void TestPopBack() {
        std::cout << "Testing PopBack\n";

        SegmentDeque<int> deque;
        for (int i = 1; i <= 5; i++) {
            deque.Append(i);
        }

        assert(deque.GetSize() == 5);
        deque.PopBack();
        assert(deque.GetSize() == 4 && deque.Get(3) == 4);

        while (!deque.IsEmpty()) {
            deque.PopBack();
        }
        assert(deque.IsEmpty() && deque.GetSize() == 0);

        std::cout << "PopBack tests passed\n";
    }

    static void TestPopFront() {
        std::cout << "Testing PopFront\n";

        SegmentDeque<int> deque;
        for (int i = 1; i <= 5; i++) {
            deque.Append(i);
        }

        assert(deque.GetSize() == 5);
        deque.PopFront();
        assert(deque.GetSize() == 4 && deque.Get(0) == 2);

        while (!deque.IsEmpty()) {
            deque.PopFront();
        }
        assert(deque.IsEmpty() && deque.GetSize() == 0);

        std::cout << "PopFront tests passed\n";
    }

    static void TestMixedOperations() {
        std::cout << "Testing Mixed Operations\n";

        SegmentDeque<int> deque;
        deque.Append(1);
        deque.Prepend(0);
        deque.Append(2);
        deque.Prepend(-1);

        assert(deque.GetSize() == 4);
        assert(deque.Get(0) == -1 && deque.Get(1) == 0 && deque.Get(2) == 1 && deque.Get(3) == 2);

        deque.PopFront();
        deque.PopBack();
        assert(deque.GetSize() == 2 && deque.Get(0) == 0 && deque.Get(1) == 1);

        std::cout << "Mixed Operations tests passed\n";
    }

    static void TestBoundaryConditions() {
        std::cout << "Testing Boundary Conditions\n";

        SegmentDeque<int> deque(2);
        for (int i = 0; i < 10; i++) {
            deque.Append(i);
        }
        for (int i = 0; i < 10; i++) {
            assert(deque.Get(i) == i);
        }

        for (int i = 10; i < 20; i++) {
            deque.Prepend(i);
        }
        assert(deque.GetSize() == 20);

        for (int i = 0; i < 10; i++) {
            assert(deque.Get(i) == 19 - i);
        }
        for (int i = 10; i < 20; i++) {
            assert(deque.Get(i) == i - 10);
        }

        std::cout << "Boundary Conditions tests passed\n";
    }

    static void TestIterator() {
        std::cout << "Testing Iterator\n";

        SegmentDeque<int> deque;
        Iterator<int>* empty_it = deque.GetMutableIterator();
        assert(!empty_it->Next());
        delete empty_it;

        for (int i = 1; i <= 5; i++) {
            deque.Append(i);
        }

        Iterator<int>* it = deque.GetMutableIterator();
        std::vector<int> iterated;
        while (it->Next()) {
            iterated.push_back(it->Get());
        }
        delete it;

        assert(iterated.size() == 5);
        for (int i = 0; i < 5; i++) {
            assert(iterated[i] == i + 1);
        }

        Iterator<int>* mut_it = deque.GetMutableIterator();
        while (mut_it->Next()) {
            mut_it->Get() *= 2;
        }
        delete mut_it;

        for (int i = 0; i < 5; i++) {
            assert(deque.Get(i) == (i + 1) * 2);
        }

        std::cout << "Iterator tests passed\n";
    }

    static void TestConstIterator() {
        std::cout << "Testing Const Iterator\n";

        SegmentDeque<int> deque;
        for (int i = 1; i <= 5; i++) {
            deque.Append(i);
        }

        const SegmentDeque<int>& const_deque = deque;
        Iterator<int>* const_it = const_deque.GetIterator();

        std::vector<int> iterated;
        while (const_it->Next()) {
            iterated.push_back(const_it->Get());
        }
        delete const_it;

        assert(iterated.size() == 5);
        for (int i = 0; i < 5; i++) {
            assert(iterated[i] == i + 1);
        }

        std::cout << "Const Iterator tests passed\n";
    }

    static void TestFunctionalMethods() {
        std::cout << "Testing Functional Methods\n";

        SegmentDeque<int> deque;
        for (int i = 1; i <= 5; i++) {
            deque.Append(i);
        }

        auto doubled = deque.Map([](int x) { return x * 2; });
        assert(doubled.GetSize() == 5);
        for (int i = 0; i < 5; i++) {
            assert(doubled.Get(i) == (i + 1) * 2);
        }

        auto evens = deque.Where([](int x) { return x % 2 == 0; });
        assert(evens.GetSize() == 2 && evens.Get(0) == 2 && evens.Get(1) == 4);

        int sum = deque.Reduce([](int acc, int x) { return acc + x; }, 0);
        assert(sum == 15);

        std::cout << "Functional Methods tests passed\n";
    }

    static void TestLargeOperations() {
        std::cout << "Testing Large Operations\n";

        SegmentDeque<int> deque(16);
        const int LARGE_SIZE = 1000;

        for (int i = 0; i < LARGE_SIZE; i++) {
            if (i % 2 == 0) {
                deque.Append(i);
            } else {
                deque.Prepend(i);
            }
        }
        assert(deque.GetSize() == LARGE_SIZE);

        for (size_t i = 0; i < deque.GetSize(); i++) {
            deque.Get(i);
        }

        for (int i = 0; i < LARGE_SIZE / 2; i++) {
            if (i % 2 == 0) {
                deque.PopBack();
            } else {
                deque.PopFront();
            }
        }
        assert(deque.GetSize() == LARGE_SIZE / 2);

        std::cout << "Large Operations tests passed\n";
    }

    static void TestExceptions() {
        std::cout << "Testing Exceptions\n";

        SegmentDeque<int> deque;

        try {
            deque.PopBack();
            assert(false);
        } catch (const std::out_of_range&) {}

        try {
            deque.PopFront();
            assert(false);
        } catch (const std::out_of_range&) {}

        deque.Append(1);
        try {
            deque.Get(1);
            assert(false);
        } catch (const std::out_of_range&) {}

        try {
            deque.Get(100);
            assert(false);
        } catch (const std::out_of_range&) {}

        Iterator<int>* it = deque.GetMutableIterator();
        try {
            it->Get();
            assert(false);
        } catch (const std::out_of_range&) {}
        delete it;

        std::cout << "Exception tests passed\n";
    }

    static void TestWithStrings() {
        std::cout << "Testing with String type\n";

        SegmentDeque<std::string> deque;
        deque.Append("Hello");
        deque.Append("World");
        deque.Prepend("Hi");

        assert(deque.GetSize() == 3);
        assert(deque.Get(0) == "Hi" && deque.Get(1) == "Hello" && deque.Get(2) == "World");

        auto uppercased = deque.Map([](const std::string& s) {
            std::string result = s;
            for (char& c : result) {
                c = std::toupper(c);
            }
            return result;
        });

        assert(uppercased.Get(0) == "HI" && uppercased.Get(1) == "HELLO" && uppercased.Get(2) == "WORLD");

        std::cout << "String type tests passed\n";
    }
};

void RunDequeTests() {
    Tests::RunAllTests();
}

#endif