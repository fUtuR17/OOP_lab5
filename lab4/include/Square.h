#ifndef Square_H
#define Square_H

#include "Figure.h"

template<Scalar T>
class Square : public Figure<T> {
public:
    Square() : Figure<T>() {}

    Square(const std::vector<Point<T>>& points)
        : Figure<T>(points, 4) {
        if (points.size() != 4) {
            throw std::invalid_argument("Братик, у квадрата должно быть четыре вершинки");
        }
    }

    void read(std::istream& is) override {
        this->vertices.clear();
        this->sides = 4;

        for (int i = 0; i < 4; i++) {
            T x, y;
            is >> x >> y;
            this->vertices.push_back(std::make_unique<Point<T>>(x, y));
        }
    }

    // std::shared_ptr<Figure<T>> clone() const override {
    //     return std::make_shared<Square<T>>(*this);
    // }
};

#endif // Square_H