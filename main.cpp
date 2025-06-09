#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "SegmentDeque.hpp"
#include "Tests.hpp"

class InteractiveDeque {
private:
    SegmentDeque<int> deque;

    void ShowHelp() {
        std::cout << "\nAvailable commands:\n";
        std::cout << "  help                    - Show this help menu\n";
        std::cout << "  deque <values...>       - Create new deque with given values\n";
        std::cout << "  append <value>          - Add value to the end\n";
        std::cout << "  prepend <value>         - Add value to the beginning\n";
        std::cout << "  pop_back                - Remove last element\n";
        std::cout << "  pop_front               - Remove first element\n";
        std::cout << "  get <index>             - Get element at index\n";
        std::cout << "  set <index> <value>     - Set element at index to value\n";
        std::cout << "  print                   - Print current deque\n";
        std::cout << "  size                    - Show deque size\n";
        std::cout << "  empty                   - Check if deque is empty\n";
        std::cout << "  clear                   - Clear the deque\n";
        std::cout << "  map <operation>         - Apply operation to all elements (double, square, abs)\n";
        std::cout << "  filter <condition>      - Filter elements (even, odd, positive, negative)\n";
        std::cout << "  reduce <operation>      - Reduce deque to single value (sum, product, max, min)\n";
        std::cout << "  iterate                 - Show elements using iterator\n";
        std::cout << "  segments                - Show segment information\n";
        std::cout << "  exit                    - Exit the program\n\n";
    }

    void PrintDeque() {
        if (deque.IsEmpty()) {
            std::cout << "Deque is empty\n";
            return;
        }

        std::cout << "Deque: [";

        for (size_t i = 0; i < deque.GetSize(); i++) {
            std::cout << deque.Get(i);
            if (i < deque.GetSize() - 1) std::cout << ", ";
        }

        std::cout << "]\n";
    }

    void CreateDeque(const std::vector<std::string>& tokens) {
        deque = SegmentDeque<int>();

        for (size_t i = 1; i < tokens.size(); i++) {
            try {
                int value = std::stoi(tokens[i]);
                deque.Append(value);
            } catch (const std::exception&) {
                std::cout << "Invalid number: " << tokens[i] << "\n";
                return;
            }
        }

        std::cout << "Created deque with " << deque.GetSize() << " elements\n";
        PrintDeque();
    }

    void HandleAppend(const std::vector<std::string>& tokens) {
        if (tokens.size() != 2) {
            std::cout << "Usage: append <value>\n";
            return;
        }

        try {
            int value = std::stoi(tokens[1]);
            deque.Append(value);
            std::cout << "Appended " << value << "\n";
        } catch (const std::exception&) {
            std::cout << "Invalid number: " << tokens[1] << "\n";
        }
    }

    void HandlePrepend(const std::vector<std::string>& tokens) {
        if (tokens.size() != 2) {
            std::cout << "Usage: prepend <value>\n";
            return;
        }

        try {
            int value = std::stoi(tokens[1]);
            deque.Prepend(value);
            std::cout << "Prepended " << value << "\n";
        } catch (const std::exception&) {
            std::cout << "Invalid number: " << tokens[1] << "\n";
        }
    }

    void HandleGet(const std::vector<std::string>& tokens) {
        if (tokens.size() != 2) {
            std::cout << "Usage: get <index>\n";
            return;
        }

        try {
            size_t index = std::stoull(tokens[1]);
            int value = deque.Get(index);
            std::cout << "Element at index " << index << ": " << value << "\n";
        } catch (const std::out_of_range&) {
            std::cout << "Index out of range\n";
        } catch (const std::exception&) {
            std::cout << "Invalid index: " << tokens[1] << "\n";
        }
    }

    void HandleSet(const std::vector<std::string>& tokens) {
        if (tokens.size() != 3) {
            std::cout << "Usage: set <index> <value>\n";
            return;
        }

        try {
            size_t index = std::stoull(tokens[1]);
            int value = std::stoi(tokens[2]);
            deque.Get(index) = value;
            std::cout << "Set element at index " << index << " to " << value << "\n";
        } catch (const std::out_of_range&) {
            std::cout << "Index out of range\n";
        } catch (const std::exception&) {
            std::cout << "Invalid parameters\n";
        }
    }

    void HandleMap(const std::vector<std::string>& tokens) {
        if (tokens.size() != 2) {
            std::cout << "Usage: map <operation> (double, square, abs)\n";
            return;
        }

        if (tokens[1] == "double") {
            deque = deque.Map([](int x) { return x * 2; });
            std::cout << "Applied double operation\n";
        } else if (tokens[1] == "square") {
            deque = deque.Map([](int x) { return x * x; });
            std::cout << "Applied square operation\n";
        } else if (tokens[1] == "abs") {
            deque = deque.Map([](int x) { return x < 0 ? -x : x; });
            std::cout << "Applied absolute value operation\n";
        } else {
            std::cout << "Unknown operation. Available: double, square, abs\n";
        }
    }

    void HandleFilter(const std::vector<std::string>& tokens) {
        if (tokens.size() != 2) {
            std::cout << "Usage: filter <condition> (even, odd, positive, negative)\n";
            return;
        }

        if (tokens[1] == "even") {
            deque = deque.Where([](int x) { return x % 2 == 0; });
            std::cout << "Filtered even numbers\n";
        } else if (tokens[1] == "odd") {
            deque = deque.Where([](int x) { return x % 2 != 0; });
            std::cout << "Filtered odd numbers\n";
        } else if (tokens[1] == "positive") {
            deque = deque.Where([](int x) { return x > 0; });
            std::cout << "Filtered positive numbers\n";
        } else if (tokens[1] == "negative") {
            deque = deque.Where([](int x) { return x < 0; });
            std::cout << "Filtered negative numbers\n";
        } else {
            std::cout << "Unknown condition. Available: even, odd, positive, negative\n";
        }
    }

    void HandleReduce(const std::vector<std::string>& tokens) {
        if (tokens.size() != 2) {
            std::cout << "Usage: reduce <operation> (sum, product, max, min)\n";
            return;
        }

        if (deque.IsEmpty()) {
            std::cout << "Cannot reduce empty deque\n";
            return;
        }

        if (tokens[1] == "sum") {
            int result = deque.Reduce([](int acc, int x) { return acc + x; }, 0);
            std::cout << "Sum: " << result << "\n";
        } else if (tokens[1] == "product") {
            int result = deque.Reduce([](int acc, int x) { return acc * x; }, 1);
            std::cout << "Product: " << result << "\n";
        } else if (tokens[1] == "max") {
            int result = deque.Reduce([](int acc, int x) { return acc > x ? acc : x; }, deque.Get(0));
            std::cout << "Max: " << result << "\n";
        } else if (tokens[1] == "min") {
            int result = deque.Reduce([](int acc, int x) { return acc < x ? acc : x; }, deque.Get(0));
            std::cout << "Min: " << result << "\n";
        } else {
            std::cout << "Unknown operation. Available: sum, product, max, min\n";
        }
    }

    void HandleIterate() {
        if (deque.IsEmpty()) {
            std::cout << "Deque is empty\n";
            return;
        }

        std::cout << "Iterating through deque: ";
        Iterator<int>* it = deque.GetIterator();
        bool first = true;

        while (it->Next()) {
            if (!first) {
                std::cout << ", ";
            }

            std::cout << it->Get();
            first = false;
        }

        std::cout << "\n";
        delete it;
    }

    void ShowSegments() {
        std::cout << "Segment count: " << deque.GetSegmentCount() << "\n";
        for (size_t i = 0; i < deque.GetSegmentCount(); i++) {
            const Segment<int>* segment = deque.GetSegment(i);

            std::cout << "Segment " << i << ": front_offset=" << segment->front_offset 
                    << ", back_size=" << segment->back_size 
                    << ", effective_size=" << segment->GetEffectiveSize() << "\n";
        }
    }

    std::vector<std::string> TokenizeInput(const std::string& input) {
        std::vector<std::string> tokens;
        std::istringstream iss(input);
        std::string token;

        while (iss >> token) {
            tokens.push_back(token);
        }

        return tokens;
    }

public:
    void Run() {
        std::cout << "SegmentDeque Interactive Menu\n";

        ShowHelp();

        std::string input;
        while (true) {
            std::cout << "deque> ";
            std::getline(std::cin, input);

            if (input.empty()) continue;

            std::vector<std::string> tokens = TokenizeInput(input);
            if (tokens.empty()) continue;

            std::string command = tokens[0];

            try {
                if (command == "help") {
                    ShowHelp();
                } else if (command == "exit") {
                    std::cout << "Goodbye!\n";
                    break;
                } else if (command == "deque") {
                    CreateDeque(tokens);
                } else if (command == "append") {
                    HandleAppend(tokens);
                } else if (command == "prepend") {
                    HandlePrepend(tokens);
                } else if (command == "pop_back") {
                    if (deque.IsEmpty()) {
                        std::cout << "Deque is empty\n";
                    } else {
                        deque.PopBack();
                        std::cout << "Popped back element\n";
                    }
                } else if (command == "pop_front") {
                    if (deque.IsEmpty()) {
                        std::cout << "Deque is empty\n";
                    } else {
                        deque.PopFront();
                        std::cout << "Popped front element\n";
                    }
                } else if (command == "get") {
                    HandleGet(tokens);
                } else if (command == "set") {
                    HandleSet(tokens);
                } else if (command == "print") {
                    PrintDeque();
                } else if (command == "size") {
                    std::cout << "Size: " << deque.GetSize() << "\n";
                } else if (command == "empty") {
                    std::cout << "Empty: " << (deque.IsEmpty() ? "true" : "false") << "\n";
                } else if (command == "clear") {
                    deque = SegmentDeque<int>();
                    std::cout << "Deque cleared\n";
                } else if (command == "map") {
                    HandleMap(tokens);
                } else if (command == "filter") {
                    HandleFilter(tokens);
                } else if (command == "reduce") {
                    HandleReduce(tokens);
                } else if (command == "iterate") {
                    HandleIterate();
                } else if (command == "segments") {
                    ShowSegments();
                } else {
                    std::cout << "Unknown command: " << command << ". Type 'help' for available commands.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }
};

int main() {
    std::cout << "Running tests\n";

    try {
        RunDequeTests();
    } catch (const std::exception& e) {
        std::cout << "Test failed: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\n";

    InteractiveDeque menu;
    menu.Run();

    return 0;
}