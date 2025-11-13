#include "Allocator.h"
#include <map>
#include <iostream>
#include <memory_resource>
#include <concepts>
#include <vector>


void* map_allocator::do_allocate(std::size_t bytes, std::size_t alignment) {
    auto it = recently_used.find(bytes);
    if (it != recently_used.end() && !it->second.empty()) {
        void* ptr = it->second.back();
        it->second.pop_back();
        active_allocations[ptr] = bytes;
        return ptr;
    }
    
    // Если нет свободных - выделяем новый
    void* ptr = ::operator new(bytes);
    active_allocations[ptr] = bytes;
    return ptr;
}


void map_allocator::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) {
    auto it = active_allocations.find(ptr);
    if (it == active_allocations.end()) {
        throw std::runtime_error("Попытка освободить память, которую мы не выделяли!");
    }
    
    // Проверяем, что размер совпадает
    if (it->second != bytes) {
        std::cerr << "Предупреждение: размер при освобождении не совпадает с выделенным\n";
    }
    
    // Перемещаем в свободные блоки для переиспользования
    recently_used[bytes].push_back(ptr);
    active_allocations.erase(ptr);
}


bool map_allocator::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    if (this == &other) {
        return true;
    }
    
    // Пробуем преобразовать other к нашему типу
    const map_allocator* other_ptr = dynamic_cast<const map_allocator*>(&other);
    if (other_ptr == nullptr) {
        return false;
    }
    return true;
}


map_allocator::~map_allocator() noexcept{
    for (auto& [ptr, size] : active_allocations) {
        ::operator delete(ptr);
    }
    active_allocations.clear();
    
    for (auto& [size, blocks] : recently_used) {
        for (void* ptr : blocks) {
            ::operator delete(ptr);
        }
    }
    recently_used.clear();
}