#pragma once
#include <vector>
#include <stdexcept>

template <class Tname, class Tvalue>
class UnorderedTable {
public:
    struct Iterator {
        Iterator(std::pair<Tname, Tvalue>* pointer) : ptr(pointer) {}

        std::pair<Tname, Tvalue>& operator*() const { return *ptr; }
        std::pair<Tname, Tvalue>* operator->() { return ptr; }

        Iterator& operator++() {
            ++ptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Tname key() const { return ptr->first; }
        Tvalue& value() { return ptr->second; }

        friend bool operator==(const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; }
        friend bool operator!=(const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; }
    private:
        std::pair<Tname, Tvalue>* ptr;
    };

    Iterator begin() { return data.empty() ? Iterator(nullptr) : Iterator(&data[0]); }
    Iterator end() { return data.empty() ? Iterator(nullptr) : Iterator(&data[0] + data.size()); }

    Iterator insert(const Tname& key, const Tvalue& value) {
        for (auto& elem : data) {
            if (elem.first == key) {
                throw std::runtime_error("Key already exists");
            }
        }
        data.emplace_back(key, value);
        return Iterator(&data.back());
    }

    Iterator erase(const Tname& key) {
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i].first == key) {
                if (i != data.size() - 1) {
                    std::swap(data[i], data.back());
                }
                data.pop_back();
                return (i < data.size()) ? Iterator(&data[i]) : end();
            }
        }
        return end();
    }

    Iterator find(const Tname& key) {
        for (auto it = begin(); it != end(); ++it) {
            if (it.key() == key) {
                return it;
            }
        }
        return end();
    }

    size_t size() const { return data.size(); }
    bool empty() const { return data.empty(); }

    Tvalue& operator[](const Tname& key) {
        auto it = find(key);
        if (it == end()) {
            throw std::runtime_error("Key not found");
        }
        return it.value();
    }

private:
    std::vector<std::pair<Tname, Tvalue>> data;
};