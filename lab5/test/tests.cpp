
#include <gtest/gtest.h>
#include "stack.h"
#include <memory_resource>
#include <string>

TEST(StackTest, BasicIntOperations) {
    Stack<int> stack;
    
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
    
    stack.push(10);
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 10);
    
    stack.push(20);
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), 20);
    
    stack.pop();
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 10);
    
    stack.pop();
    EXPECT_TRUE(stack.empty());
}

TEST(StackTest, ExceptionHandling) {
    Stack<int> stack;
    
    EXPECT_THROW(stack.top(), std::runtime_error);
    EXPECT_THROW(stack.pop(), std::runtime_error);
    
    stack.push(1);
    EXPECT_NO_THROW(stack.top());
    EXPECT_NO_THROW(stack.pop());
}

TEST(StackTest, CustomStructOperations) {
    // Структура создается прямо в тесте
    struct Point {
        int x, y;
        Point(int x, int y) : x(x), y(y) {}
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    Stack<Point> stack;
    
    Point p1(1, 2);
    Point p2(3, 4);
    
    stack.push(p1);
    EXPECT_EQ(stack.top().x, 1);
    EXPECT_EQ(stack.top().y, 2);
    
    stack.push(p2);
    EXPECT_EQ(stack.top().x, 3);
    EXPECT_EQ(stack.top().y, 4);
    
    stack.pop();
    EXPECT_EQ(stack.top().x, 1);
    EXPECT_EQ(stack.top().y, 2);
}

TEST(StackTest, StringOperations) {
    Stack<std::string> stack;
    
    stack.push("hello");
    EXPECT_EQ(stack.top(), "hello");
    
    stack.push("world");
    EXPECT_EQ(stack.top(), "world");
    
    stack.pop();
    EXPECT_EQ(stack.top(), "hello");
}

TEST(StackTest, WithCustomAllocator) {
    // Создаем аллокатор с монотонным буфером
    std::array<std::byte, 1024> buffer;
    std::pmr::monotonic_buffer_resource pool{buffer.data(), buffer.size()};
    
    Stack<int> stack(&pool);
    
    for (int i = 0; i < 10; ++i) {
        stack.push(i);
    }
    
    EXPECT_EQ(stack.size(), 10);
    EXPECT_EQ(stack.top(), 9);
    
    // Проверяем, что используется наш аллокатор
    EXPECT_EQ(stack.get_memory_resource(), &pool);
}

TEST(StackTest, ComplexStructInTest) {
    // Еще одна структура прямо в тесте
    struct Employee {
        std::string name;
        int id;
        double salary;
        
        Employee(const std::string& n, int i, double s) : name(n), id(i), salary(s) {}
    };
    
    Stack<Employee> stack;
    
    Employee e1("John", 1001, 75000.0);
    Employee e2("Jane", 1002, 80000.0);
    
    stack.push(e1);
    EXPECT_EQ(stack.top().name, "John");
    EXPECT_EQ(stack.top().id, 1001);
    
    stack.push(e2);
    EXPECT_EQ(stack.top().name, "Jane");
    EXPECT_EQ(stack.top().id, 1002);
    
    stack.pop();
    EXPECT_EQ(stack.top().name, "John");
}

TEST(StackTest, ReserveCapacity) {
    Stack<int> stack(100);  // Резервируем 100 элементов
    
    for (int i = 0; i < 100; ++i) {
        stack.push(i);
    }
    
    EXPECT_EQ(stack.size(), 100);
    EXPECT_EQ(stack.top(), 99);
}

TEST(StackTest, MultipleTypes) {
    // Тестируем разные типы в одном тесте
    struct TempStruct {
        char c;
        int i;
        TempStruct(char ch, int val) : c(ch), i(val) {}
    };
    
    Stack<int> int_stack;
    Stack<std::string> string_stack;
    Stack<TempStruct> struct_stack;
    
    int_stack.push(42);
    string_stack.push("test");
    struct_stack.push(TempStruct('A', 100));
    
    EXPECT_EQ(int_stack.top(), 42);
    EXPECT_EQ(string_stack.top(), "test");
    EXPECT_EQ(struct_stack.top().c, 'A');
    EXPECT_EQ(struct_stack.top().i, 100);
}
