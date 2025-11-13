#ifndef ARRAY_H
#define ARRAY_H

#include <map>
#include <iostream>
#include <memory_resource>
#include <concepts>
#include <vector>

class map_allocator : public std::pmr::memory_resource{
    private:
        std::map<void*, std::size_t> active_allocations;
        std::map<std::size_t, std::vector<void*>> recently_used;
        
        void* do_allocate(std::size_t bytes, std::size_t alignment) override;
        void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
        bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
    public:
        map_allocator() = default;
        map_allocator(const map_allocator&) = default;
        ~map_allocator() noexcept;
};
#endif