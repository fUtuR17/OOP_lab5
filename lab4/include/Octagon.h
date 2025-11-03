#ifndef Octagon_H
#define Octagon_H

#include "Figure.h"

template<Scalar T>
class Octagon : public Figure<T> {
public:
    Octagon() : Figure<T>() {}

    Octagon(const std::vector<Point<T>>& points)
        : Figure<T>(points, 8) {
        if (points.size() != 8) {
            throw std::invalid_argument("Братик, у восьмиугольника должно быть восемь вершинок");
        }
    }

    void read(std::istream& is) override {
        this->vertices.clear();
        this->sides = 8;

        for (int i = 0; i < 8; i++) {
            T x, y;
            is >> x >> y;
            this->vertices.push_back(std::make_unique<Point<T>>(x, y));
        }
    }
    
    // std::shared_ptr<Figure<T>> clone() const override {
    //     return std::make_shared<Octagon<T>>(*this);
    // }
};

#endif // Octagon_H