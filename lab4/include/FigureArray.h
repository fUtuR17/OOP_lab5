// #ifndef FIGURE_ARRAY_H
// #define FIGURE_ARRAY_H

// #include "Figure.h"
// #include "Square.h"
// #include "Octagon.h"
// #include "Triangle.h"
// #include <vector>
// #include <memory>

// class FigureArray {
// private:
//     std::vector<std::shared_ptr<Figure>> figures;

// public:
//     FigureArray() = default;
    
//     // Добавление фигур
//     void addSquare(double x, double y, double radius);
//     void addOctagon(double x, double y, double radius);
//     void addTriangle(double x, double y, double radius);
    
//     // Удаление по индексу
//     void removeFigure(int index);
    
//     // Получение информации
//     size_t size() const;
//     double totalArea() const;
    
//     // Вывод информации
//     void printAllCenters() const;
//     void printAllAreas() const;
//     void printAllInfo() const;
    
//     // Доступ к фигурам
//     std::shared_ptr<Figure> operator[](size_t index);
//     const std::shared_ptr<Figure> operator[](size_t index) const;
// };

// #endif


#ifndef FIGURE_ARRAY_H
#define FIGURE_ARRAY_H

#include "Figure.h"
#include "Square.h"
#include "Octagon.h"
#include "Triangle.h"
#include <vector>
#include <memory>
#include <iostream>

template <Scalar T>
class FigureArray {
private:
    std::vector<std::shared_ptr<Figure<T>>> figures;
public:
    // ----- Конструкторы ----- 
    FigureArray() = default;
    FigureArray(const std::vector<std::shared_ptr<Figure<T>>>& vec) : figures(vec) {}

    FigureArray(const FigureArray& other) = default;
    FigureArray& operator=(const FigureArray& other) = default;
    FigureArray(FigureArray&&) = default;
    FigureArray& operator=(FigureArray&&) = default;
    
    // ----- Деструктор -----
    ~FigureArray() = default;
    
    // Копирование
    // FigureArray(const FigureArray& other){
    //     // for (const auto& ptr : other.figures) {
    //     //     figures.push_back(std::make_shared<Figure<T>>(*ptr));
    //     // }
    //     for (const auto& ptr : other.figures) {
    //         figures.push_back(ptr->clone());
    //     }
    // }

    // // Переопределенное = для копирования
    // FigureArray& operator=(const FigureArray& other) {
    //     if (this != &other) {
    //         figures.clear();
    //         for (const auto& ptr : other.figures) {
    //             figures.push_back(ptr->clone());
    //         }
    //     }
    //     return *this;
    // }

    // ----- Операции с фигурами -----
    // Добавление фигур
    void add_figure(std::shared_ptr<Figure<T>> fig){
        figures.push_back(fig);
    }
    
    // Удаление по индексу
    void remove_figure(int index) {
        if (index >= 0 && index < static_cast<int>(figures.size())) {
            figures.erase(figures.begin() + index);
        }
    }
    
    // Получение информации
    size_t size() const {
        return figures.size();
    }

    double total_area() const {
        double total = 0;
        for (const auto& figure : figures) {
            total += double(*figure);
        }
        return total;
    }
    
    // Вывод информации
    void print_centers() const {
        std::cout << "Геометрические центры фигур:" << std::endl;
        for (size_t i = 0; i < figures.size(); i++) {
            auto center = figures[i]->get_center_coords();
            std::cout << "Figure " << i << ": (" << center.first << ", " << center.second << ")" << std::endl;
        }
    }

    void print_areas() const {
        std::cout << "Площади фигур:" << std::endl;
        for (size_t i = 0; i < figures.size(); i++) {
            std::cout << "Фигура" << i << ": " << double(*figures[i]) << std::endl;
        }
    }

    inline void print_coords() const {
        std::cout << "All Figures Information:" << std::endl;
        for (size_t i = 0; i < figures.size(); i++) {
            std::cout << "Figure " << i << ": " << *figures[i] << std::endl;
        }
    }
    
    // Доступ к фигурам
    std::shared_ptr<Figure<T>> operator[](size_t index) {
        if (index >= figures.size()) throw std::out_of_range("Index out of range");
        return figures[index];
    }

    std::shared_ptr<const Figure<T>> operator[](size_t index) const {
        if (index >= figures.size()) throw std::out_of_range("Index out of range");
        return figures[index];
    }
};

#endif // FIGURE_ARRAY_H