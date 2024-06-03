#pragma once

//class Shape {
// public:
//    virtual float area() const = 0;
//    virtual float inertia() const = 0;
//};
//
//namespace ShapeType {
//class Circle : public Shape {
// public:
//    float radius;
//
//    Circle(float radius): radius{radius} {}
//
//    float area() const override {
//        return 3.14159265358979323846f * radius * radius;
//    }
//
//    float inertia() const override {
//        return 0.5f * radius * radius;
//    }
//};
//
//class Rectangle : public Shape {
// public:
//    float width, height;
//
//    Rectangle(float width, float height): width{width}, height{height} {}
//
//    float area() const override {
//        return width * height;
//    }
//
//    float inertia() const override {
//        return 1.f / 12.f * (width * width + height * height);
//    }
//};
//}

enum ShapeType {
    CIRCLE,
    POLYGON
};
