#ifndef STACK_H
#define STACK_H

#include <vector>
#include <memory_resource>
#include <stdexcept>

template<typename T>
class Stack {
private:
    std::pmr::vector<T> data;

public:
    // Конструкторы с передачей memory_resource*
    explicit Stack(std::pmr::memory_resource* resource = std::pmr::get_default_resource())
        : data(resource) {}

    Stack(std::size_t initial_capacity, std::pmr::memory_resource* resource = std::pmr::get_default_resource())
        : data(resource) {
        data.reserve(initial_capacity);
    }

    // Базовые методы стека
    void push(const T& value) {
        data.push_back(value);
    }

    void pop() {
        if (empty()) {
            throw std::runtime_error("Стек пустой");
        }
        data.pop_back();
    }

    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Стек пустой");
        }
        return data.back();
    }

    bool empty() const noexcept {
        return data.empty();
    }

    std::size_t size() const noexcept {
        return data.size();
    }

    // Получить используемый memory_resource
    std::pmr::memory_resource* get_memory_resource() const {
        return data.get_allocator().resource();
    }
};

#endif