#ifndef FIGURE_H
#define FIGURE_H

#include "Point.h"
#include <cmath>
#include <vector>
#include <memory>
#include <iostream>

template<Scalar T>
class Figure {

protected:
    std::vector<std::unique_ptr<Point<T>>> vertices;
    int sides;

    // Будет всегда возвращаться double, тк если T = int, то может произойти округление до 0 
    double signed_area() const {
        if (sides == 0) return 0;
        double area = 0;
        for (int i = 0; i < sides; i++) {
            // unique_ptr перегружает операторы * и ->
            area += vertices[(i) % sides]->get_x() * vertices[(i + 1) % sides]->get_y() - vertices[(i + 1) % sides]->get_x() * vertices[(i) % sides]->get_y();
        }
        return area / 2;
    }
    
public:
    // ----- Конструкторы ----- 
    Figure(): vertices{}, sides(0) {};
    
    Figure(const std::vector<Point<T>> points, int sides_amount): sides(sides_amount) {
        // В вектор пушатся новые копии точек, созданные через конструктор на лету
        for (const auto& point : points) {
            vertices.push_back(std::make_unique<Point<T>>(point));
        }
    }
    
    // Копирование
    Figure(const Figure& other) : sides(other.sides) {
        // Тут уже идет итерация по вектору, где хранятся std::unique_ptr<Point<T>>, поэтому 
        // нужно разыменовывать указатель, чтобы создать новый объект Point<T>
        for (const auto& ptr : other.vertices) {
            vertices.push_back(std::make_unique<Point<T>>(*ptr));
        }
    }

    // Переопределенное = для копирования
    Figure& operator=(const Figure& other) {
        if (this != &other) {
            vertices.clear();
            sides = other.sides;
            for (const auto& ptr : other.vertices) {
                vertices.push_back(std::make_unique<Point<T>>(*ptr));
            }
        }
        return *this;
    }

    // Перемещение - сможет работать по дефолту, тк под капотом лежит вектор
    Figure(Figure&&) = default;
    Figure& operator=(Figure&&) = default;

    // ----- Деструктор ----- 
    virtual ~Figure() = default;
    
    // ----- Интерфейс класса -----

    // Виртуальные методы
    virtual void print(std::ostream& os) const {
        // os << "Фигура с  " << sides << " сторонами." << endl;
        os << "Координаты: "<< std::endl;
        for (size_t i = 0; i < sides; i++) {
            os << "(" << vertices[i]->get_x() << ", " << vertices[i]->get_y() << ")";
            if (i != sides - 1) os << ", ";
        }
    }

    // Обязательно перегрузить функцию в наследниках (сделать заданное число вершин)
    virtual void read(std::istream& is) {
        vertices.clear();
        is >> sides;
        for (int i = 0; i < sides; i++) {
            T x, y;
            is >> x >> y;
            vertices.push_back(std::make_unique<Point<T>>(x, y));
        }
    }

    // Перегрузка операторов сравнения (Учитывают только площадь по модулю с заданной точностью)
    virtual bool operator>(const Figure& other) const {
        return double(*this) > double(other);
    }

    virtual bool operator<(const Figure& other) const {
        return double(*this) < double(other);
    }

    virtual bool operator>=(const Figure& other) const {
        return double(*this) >= double(other);
    }

    virtual bool operator<=(const Figure& other) const {
        return double(*this) <= double(other);
    }

    virtual bool operator==(const Figure& other) const {
        if (sides != other.sides){
            return false;
        }
        constexpr double eps = 1e-9;
        return std::abs(double(*this) - double(other)) < eps;
    }

    // Приведение к double (Вычисление площади)
    operator double() const {
        return std::abs(signed_area());
    }

    // Вычисление координат центра
    std::pair<double, double> get_center_coords() const {
        double area = this->signed_area();
        if (area == 0) return {0, 0};
        double x = 0;
        double y = 0;
        for (int i = 0; i < sides; i++) {
            x += ((vertices[(i) % sides]->get_x() + vertices[(i + 1) % sides]->get_x()) * (vertices[(i) % sides]->get_x() * vertices[(i + 1) % sides]->get_y() - vertices[(i + 1) % sides]->get_x() * vertices[i]->get_y()));
            y += ((vertices[(i) % sides]->get_y() + vertices[(i + 1) % sides]->get_y()) * (vertices[(i) % sides]->get_x() * vertices[(i + 1) % sides]->get_y() - vertices[(i + 1) % sides]->get_x() * vertices[i]->get_y()));
        }
        x = x / (6 * area);
        y = y / (6 * area);

        return {x, y};
    }
    
    // Дружественные функции ввода/вывода
    friend std::ostream& operator<<(std::ostream& os, const Figure<T>& fig) {
        fig.print(os); 
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Figure<T>& fig) {
        fig.read(is);
        return is;
    }

    // Сделано для корректного копирования FigureArray
    // virtual std::shared_ptr<Figure<T>> clone() const = 0;
};

#endif // FIGURE_H