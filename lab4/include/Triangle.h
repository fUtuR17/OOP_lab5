#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Figure.h"

template<Scalar T>
class Triangle : public Figure<T> {
public:
    Triangle() : Figure<T>() {}

    Triangle(const std::vector<Point<T>>& points): Figure<T>(points, 3) {
        if (points.size() != 3) {
            throw std::invalid_argument("Братик, у треугольника должно быть три вершинки");
        }
    }

    void read(std::istream& is) override {
        this->vertices.clear();
        this->sides = 3;

        for (int i = 0; i < 3; i++) {
            T x, y;
            is >> x >> y;
            this->vertices.push_back(std::make_unique<Point<T>>(x, y));
        }
    }

    // std::shared_ptr<Figure<T>> clone() const override {
    //     return std::make_shared<Triangle<T>>(*this);
    // }
};

#endif // TRIANGLE_H