#ifndef POINT_H
#define POINT_H

#include <concepts>
#include <type_traits>
#include <iostream>
#include <utility>

template<class T>
concept Scalar = std::is_arithmetic_v<T>;

template<Scalar T>
class Point {
private:
    T x{};
    T y{};

public:
    // ----- Конструкторы -----
    Point() = default;
    Point(T coord1, T coord2) : x(coord1), y(coord2) {}

    // Специальные члены по умолчанию (не обязательны, но можно оставить явно)
    Point(const Point&) = default;
    Point& operator=(const Point&) = default;
    Point(Point&&) = default;
    Point& operator=(Point&&) = default;

    // ----- Деструктор -----
    ~Point() = default;

    // ----- Методы -----
    T get_x() const { return x; }
    T get_y() const { return y; }
    std::pair<T, T> get_coords() const { return {x, y}; }

    bool operator==(const Point<T>& other) const {
        return x == other.x && y == other.y;
    }

    void print() const {
        std::cout << "Point(" << x << ", " << y << ")\n";
    }
};

#endif // POINT_H