#include <gtest/gtest.h>
#include "FigureArray.h"
#include "Point.h"
#include "Square.h"
#include "Octagon.h"
#include "Triangle.h"
#include <vector>
#include <memory>
#include <cmath>

using namespace std;

// ----- Point -----

TEST(PointTest, ConceptValidation) {
    // Должно компилироваться для скалярных типов
    EXPECT_TRUE(is_scalar_v<int>);
    EXPECT_TRUE(is_scalar_v<double>);
    
    // Не должно компилироваться для нескалярных типов
    EXPECT_FALSE(is_scalar_v<string>);
}

TEST(PointTest, DefaultConstructor) {
    Point<int> p;
    EXPECT_EQ(p.get_x(), 0);
    EXPECT_EQ(p.get_y(), 0);
    
    Point<double> p_double;
    EXPECT_DOUBLE_EQ(p_double.get_x(), 0.0);
    EXPECT_DOUBLE_EQ(p_double.get_y(), 0.0);
}

TEST(PointTest, ParameterizedConstructor) {
    Point<int> p(5, 10);
    EXPECT_EQ(p.get_x(), 5);
    EXPECT_EQ(p.get_y(), 10);
    
    Point<double> p_double(3.14, 2.71);
    EXPECT_DOUBLE_EQ(p_double.get_x(), 3.14);
    EXPECT_DOUBLE_EQ(p_double.get_y(), 2.71);
}

TEST(PointTest, GetCoordsMethod) {
    Point<int> p(7, 3);
    auto coords = p.get_coords();
    
    EXPECT_EQ(coords.first, 7);
    EXPECT_EQ(coords.second, 3);
    
    EXPECT_EQ(coords.first, p.get_x());
    EXPECT_EQ(coords.second, p.get_y());
}

TEST(PointTest, CopyAndMove) {
    Point<int> p1(15, 25);
    
    // Тест копирования
    Point<int> p2 = p1;
    EXPECT_EQ(p2.get_x(), 15);
    EXPECT_EQ(p2.get_y(), 25);
    
    // Тест присваивания копированием
    Point<int> p3;
    p3 = p1;
    EXPECT_EQ(p3.get_x(), 15);
    EXPECT_EQ(p3.get_y(), 25);
    
    // Тест перемещения
    Point<int> p4 = move(p1);
    EXPECT_EQ(p4.get_x(), 15);
    EXPECT_EQ(p4.get_y(), 25);
}
// ----- Triangle, Square, Octagon -----

TEST(ShapeTest, TriangleIntArea) {
    vector<Point<int>> points = {
        Point<int>(0, 0), 
        Point<int>(4, 0), 
        Point<int>(0, 3)
    };

    Triangle<int> triangle(points);
    EXPECT_DOUBLE_EQ(double(triangle), 6.0);
}

TEST(ShapeTest, SquareDoubleArea) {
    vector<Point<double>> points = {
        Point<double>(0, 0),
        Point<double>(5, 0),
        Point<double>(5, 2),
        Point<double>(0, 2)
    };
    Figure<double> square(points, 4);
    EXPECT_NEAR(double(square), 10.0, 1e-9);
}

TEST(ShapeTest, TriangleCopy) {
    vector<Point<float>> points = {Point<float>(1, 1), Point<float>(2, 3), Point<float>(4, 0)};
    Triangle<float> triangle1(points);
    Triangle<float> triangle2 = triangle1;
    EXPECT_TRUE(triangle1 == triangle2);
}

TEST(ShapeTest, CompareTriangleAndFigure) {
    vector<Point<int>> tri_points = {Point<int>(0,0), Point<int>(2,0), Point<int>(0,2)};
    Triangle<int> triangle(tri_points); 

    vector<Point<int>> square_points = {
        Point<int>(0,0), Point<int>(1,0), Point<int>(1,1), Point<int>(0,1)
    };
    Figure<int> square(square_points, 4);

    EXPECT_TRUE(triangle > square);
    EXPECT_TRUE(square < triangle);
}

TEST(ShapeTest, TriangleCentroid) {
    vector<Point<double>> points = {
        Point<double>(0, 0),
        Point<double>(6, 0),
        Point<double>(0, 6)
    };
    Triangle<double> triangle(points);
    auto [cx, cy] = triangle.get_center_coords();
    EXPECT_NEAR(cx, 2.0, 1e-6);
    EXPECT_NEAR(cy, 2.0, 1e-6);
}

TEST(ShapeTest, TriangleInput) {
    stringstream ss("1 1\n2 3\n4 5"); // 3 точки
    Triangle<int> triangle;
    triangle.read(ss);

    stringstream out;
    out << triangle;  
    string str = out.str();
    EXPECT_NE(str.find("1"), string::npos);
    EXPECT_NE(str.find("4"), string::npos);
}

TEST(ShapeTest, FigureOutput) {
    vector<Point<float>> points = {Point<float>(1.5f, -2.0f), Point<float>(0, 0)};
    Figure<float> fig(points, 2);
    stringstream ss;
    ss << fig;
    string res = ss.str();
    EXPECT_NE(res.find("1.5"), string::npos);
    EXPECT_NE(res.find("-2"), string::npos);
}

TEST(ShapeTest, TriangleThrowsOnTwoPoints) {
    vector<Point<int>> points = {Point<int>(0,0), Point<int>(1,1)};
    EXPECT_THROW({
        Triangle<int> triangle(points);
    }, invalid_argument);
}

TEST(ShapeTest, TriangleThrowsOnFourPoints) {
    vector<Point<double>> points = {
        Point<double>(0,0),
        Point<double>(1,0),
        Point<double>(0,1),
        Point<double>(1,1)
    };
    EXPECT_THROW({
        Triangle<double> triangle(points);
    }, invalid_argument);
}

// ----- FigureArray -----


// Тест 1: Создание пустого массива
TEST(FigureArrayTest, EmptyArray) {
    FigureArray<float> array;
    EXPECT_EQ(array.size(), 0);
    EXPECT_EQ(array.total_area(), 0.0);
}

// Тест 2: Добавление Figure<T> (базового класса)
TEST(FigureArrayTest, AddBaseFigure) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> square_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(1.0f, 1.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    FigureArray<float> array;
    
    auto fig1 = make_shared<Figure<float>>(triangle_points, 3);
    auto fig2 = make_shared<Figure<float>>(square_points, 4);
    
    array.add_figure(fig1);
    array.add_figure(fig2);
    
    EXPECT_EQ(array.size(), 2);
    
    // Проверяем, что площади вычисляются корректно
    double triangle_area = 0.5; // (1*1)/2
    double square_area = 1.0;   // 1*1
    EXPECT_NEAR(array.total_area(), triangle_area + square_area, 1e-6);
}

// Тест 3: Добавление наследников (Octagon)
TEST(FigureArrayTest, AddOctagon) {
    vector<Point<float>> octagon_points = {
        Point<float>(1.0f, 0.0f),
        Point<float>(0.707f, 0.707f),
        Point<float>(0.0f, 1.0f),
        Point<float>(-0.707f, 0.707f),
        Point<float>(-1.0f, 0.0f),
        Point<float>(-0.707f, -0.707f),
        Point<float>(0.0f, -1.0f),
        Point<float>(0.707f, -0.707f)
    };
    
    FigureArray<float> array;
    // auto octagon = make_shared<Octagon<float>>(octagon_points);
    // array.add_figure(octagon);

    array.add_figure(make_shared<Octagon<float>>(octagon_points));
    EXPECT_EQ(array.size(), 1);
    
    // Площадь правильного восьмиугольника со стороной ~0.765
    double expected_area = 2.0 * (sqrt(2.0)); // ≈4.828
    EXPECT_NEAR(array.total_area(), expected_area, 0.1);
}

// Тест 4: Добавление разных типов фигур
TEST(FigureArrayTest, AddDifferentFigures) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> square_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(1.0f, 1.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> octagon_points = {
        Point<float>(1.0f, 0.0f),
        Point<float>(0.707f, 0.707f),
        Point<float>(0.0f, 1.0f),
        Point<float>(-0.707f, 0.707f),
        Point<float>(-1.0f, 0.0f),
        Point<float>(-0.707f, -0.707f),
        Point<float>(0.0f, -1.0f),
        Point<float>(0.707f, -0.707f)
    };
    
    FigureArray<float> array;
    
    auto triangle = make_shared<Triangle<float>>(triangle_points);
    auto square = make_shared<Square<float>>(square_points);
    auto octagon = make_shared<Octagon<float>>(octagon_points);
    
    array.add_figure(triangle);
    array.add_figure(square);
    array.add_figure(octagon);
    
    EXPECT_EQ(array.size(), 3);
    
    // Проверяем полиморфизм - все фигуры корректно хранятся
    EXPECT_NE(dynamic_pointer_cast<Triangle<float>>(array[0]), nullptr);
    EXPECT_NE(dynamic_pointer_cast<Square<float>>(array[1]), nullptr);
    EXPECT_NE(dynamic_pointer_cast<Octagon<float>>(array[2]), nullptr);
}

// Тест 5: Удаление фигур
TEST(FigureArrayTest, RemoveFigures) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> square_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(1.0f, 1.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    FigureArray<float> array;
    
    auto fig1 = make_shared<Figure<float>>(triangle_points, 3);
    auto fig2 = make_shared<Figure<float>>(square_points, 4);
    auto fig3 = make_shared<Figure<float>>(triangle_points, 3);
    
    array.add_figure(fig1);
    array.add_figure(fig2);
    array.add_figure(fig3);
    
    EXPECT_EQ(array.size(), 3);
    
    // Удаление по индексу
    array.remove_figure(1);
    EXPECT_EQ(array.size(), 2);
    
    // Проверяем, что остались правильные фигуры
    EXPECT_EQ(array[0], fig1);
    EXPECT_EQ(array[1], fig3);
}

// Тест 6: Копирование FigureArray
TEST(FigureArrayTest, CopyArray) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> square_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(1.0f, 1.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    FigureArray<float> original;
    
    auto triangle = make_shared<Triangle<float>>(triangle_points);
    auto square = make_shared<Square<float>>(square_points);
    
    original.add_figure(triangle);
    original.add_figure(square);
    
    // Копирование
    FigureArray<float> copy = original;
    
    EXPECT_EQ(original.size(), 2);
    EXPECT_EQ(copy.size(), 2);
    EXPECT_NEAR(original.total_area(), copy.total_area(), 1e-9);
    
    // Проверяем, что это разные объекты
    EXPECT_NE(&original, &copy);
}

// Тест 7: Перемещение FigureArray
TEST(FigureArrayTest, MoveArray) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> square_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(1.0f, 1.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    FigureArray<float> original;
    
    auto triangle = make_shared<Triangle<float>>(triangle_points);
    auto square = make_shared<Square<float>>(square_points);
    
    original.add_figure(triangle);
    original.add_figure(square);
    
    size_t original_size = original.size();
    double original_area = original.total_area();
    
    // Перемещение
    FigureArray<float> moved = move(original);
    
    EXPECT_EQ(moved.size(), original_size);
    EXPECT_NEAR(moved.total_area(), original_area, 1e-9);
}

// Тест 8: Конструктор из vector<shared_ptr>
TEST(FigureArrayTest, ConstructorFromVector) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> square_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(1.0f, 1.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> octagon_points = {
        Point<float>(1.0f, 0.0f),
        Point<float>(0.707f, 0.707f),
        Point<float>(0.0f, 1.0f),
        Point<float>(-0.707f, 0.707f),
        Point<float>(-1.0f, 0.0f),
        Point<float>(-0.707f, -0.707f),
        Point<float>(0.0f, -1.0f),
        Point<float>(0.707f, -0.707f)
    };
    
    vector<shared_ptr<Figure<float>>> figures;
    
    figures.push_back(make_shared<Triangle<float>>(triangle_points));
    figures.push_back(make_shared<Square<float>>(square_points));
    figures.push_back(make_shared<Octagon<float>>(octagon_points));
    
    FigureArray<float> array(figures);
    
    EXPECT_EQ(array.size(), 3);
    // EXPECT_NEAR(array.total_area(), 0.5 + 1.0 + 4.828, 0.1);
}

// Тест 9: Доступ к элементам через operator[]
TEST(FigureArrayTest, ElementAccess) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> square_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(1.0f, 1.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    FigureArray<float> array;
    
    auto triangle = make_shared<Triangle<float>>(triangle_points);
    auto square = make_shared<Square<float>>(square_points);
    
    array.add_figure(triangle);
    array.add_figure(square);
    
    // Не-const доступ
    EXPECT_EQ(array[0], triangle);
    EXPECT_EQ(array[1], square);
    
    // Const доступ
    const FigureArray<float>& const_array = array;
    EXPECT_EQ(const_array[0], triangle);
    EXPECT_EQ(const_array[1], square);
    
    // Проверка на выход за границы
    EXPECT_THROW(array[5], out_of_range);
    EXPECT_THROW(const_array[5], out_of_range);
}

// Тест 10: Вывод информации
TEST(FigureArrayTest, PrintMethods) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    FigureArray<float> array;
    auto triangle = make_shared<Triangle<float>>(triangle_points);
    array.add_figure(triangle);
    
    // Проверяем, что методы не падают
    testing::internal::CaptureStdout();
    array.print_areas();
    string areas_output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(areas_output.empty());
    
    testing::internal::CaptureStdout();
    array.print_centers();
    string centers_output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(centers_output.empty());
    
    testing::internal::CaptureStdout();
    array.print_coords();
    string all_info_output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(all_info_output.empty());
}

// Тест 12: Сравнение фигур через FigureArray
TEST(FigureArrayTest, FigureComparison) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> large_square_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(2.0f, 0.0f),
        Point<float>(2.0f, 2.0f),
        Point<float>(0.0f, 2.0f)
    };
    
    FigureArray<float> array;
    
    // Треугольник с площадью 0.5
    auto small_triangle = make_shared<Triangle<float>>(triangle_points);
    
    // Квадрат с площадью 4.0
    auto large_square = make_shared<Square<float>>(large_square_points);
    
    array.add_figure(small_triangle);
    array.add_figure(large_square);
    
    // Проверяем, что квадрат больше треугольника
    EXPECT_TRUE(*large_square > *small_triangle);
    EXPECT_TRUE(*small_triangle < *large_square);
}

// Тест 13: Работа с разными типами T (int, double)
TEST(FigureArrayTest, DifferentTypes) {
    // Тест с int
    FigureArray<int> int_array;
    vector<Point<int>> int_points = {Point<int>(0,0), Point<int>(1,0), Point<int>(0,1)};
    auto int_triangle = make_shared<Triangle<int>>(int_points);
    int_array.add_figure(int_triangle);
    EXPECT_NEAR(int_array.total_area(), 0.5, 1e-9);
    
    // Тест с double
    FigureArray<double> double_array;
    vector<Point<double>> double_points = {Point<double>(0,0), Point<double>(1,0), Point<double>(0,1)};
    auto double_triangle = make_shared<Triangle<double>>(double_points);
    double_array.add_figure(double_triangle);
    EXPECT_NEAR(double_array.total_area(), 0.5, 1e-9);
}

// Тест 14: Очистка массива
TEST(FigureArrayTest, ClearArray) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    vector<Point<float>> square_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(1.0f, 0.0f),
        Point<float>(1.0f, 1.0f),
        Point<float>(0.0f, 1.0f)
    };
    
    FigureArray<float> array;
    
    auto fig1 = make_shared<Figure<float>>(triangle_points, 3);
    auto fig2 = make_shared<Figure<float>>(square_points, 4);
    
    array.add_figure(fig1);
    array.add_figure(fig2);
    
    EXPECT_EQ(array.size(), 2);
    
    // Поочередное удаление
    array.remove_figure(0);
    array.remove_figure(0);
    
    EXPECT_EQ(array.size(), 0);
    EXPECT_EQ(array.total_area(), 0.0);
}

// Тест 15: Центры масс фигур
TEST(FigureArrayTest, CentersOfMass) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(3.0f, 0.0f),
        Point<float>(0.0f, 3.0f)
    };
    
    FigureArray<float> array;
    auto triangle = make_shared<Triangle<float>>(triangle_points);
    array.add_figure(triangle);
    
    // Центр масс треугольника должен быть в среднем арифметическом вершин
    auto center = array[0]->get_center_coords();
    EXPECT_NEAR(center.first, 1.0, 1e-6);  // (0+3+0)/3 = 1
    EXPECT_NEAR(center.second, 1.0, 1e-6); // (0+0+3)/3 = 1
}

// Тест 16: Площадь квадрата
TEST(FigureArrayTest, SquareArea) {
    vector<Point<float>> square_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(2.0f, 0.0f),
        Point<float>(2.0f, 2.0f),
        Point<float>(0.0f, 2.0f)
    };
    
    FigureArray<float> array;
    auto square = make_shared<Square<float>>(square_points);
    array.add_figure(square);
    
    EXPECT_NEAR(array.total_area(), 4.0, 1e-6); // 2*2 = 4
}

// Тест 17: Площадь треугольника
TEST(FigureArrayTest, TriangleArea) {
    vector<Point<float>> triangle_points = {
        Point<float>(0.0f, 0.0f),
        Point<float>(4.0f, 0.0f),
        Point<float>(0.0f, 3.0f)
    };
    
    FigureArray<float> array;
    auto triangle = make_shared<Triangle<float>>(triangle_points);
    array.add_figure(triangle);
    
    EXPECT_NEAR(array.total_area(), 6.0, 1e-6); // (4*3)/2 = 6
}