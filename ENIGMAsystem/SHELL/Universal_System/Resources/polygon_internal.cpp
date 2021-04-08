// Nabeel Danish

#include "polygon_internal.h"
#include <math.h>

namespace enigma {

    // Point Class Implementation
    Point::Point() {
        x = NULL_POINT;
        y = NULL_POINT;
    }
    Point::Point(const Point& obj) {
        copy(obj);
    }
    Point::Point(double x, double y) {
        this->x = x;
        this->y = y;
    }
    void Point::copy(const Point& obj) {
        if (&obj != nullptr) {
            this->x = obj.x; 
            this->y = obj.y;
        }
    }
    double Point::getX() {
        return this->x;
    }
    double Point::getY() {
        return this->y;
    }
    void Point::setX(double x) {
        this->x = x;
    }
    void Point::setY(double y) {
        this->y = y;
    }
    double Point::getMagnitude() {
        return sqrt(pow(x, 2) + pow(y, 2));
    }
    double Point::getAngle() {
        return atan2(y, x);
    }
    Point Point::getNormR() {
        return Point(-1 * y, x);
    }
    Point Point::getNormL() {
        return Point(y, -1 * x);
    }
    Point Point::getUnitVector() {
        return Point(x / getMagnitude(), y / getMagnitude());
    }

    void Point::setMagnitude(double number) {
        double current_angle = getAngle();
        x = number * cos(current_angle);
        y = number * sin(current_angle);
    }
    void Point::setAngle(double angle) {
        double current_magnitude = getMagnitude();
        x = current_magnitude * cos(angle);
        y = current_magnitude * sin(angle);
    }
    void Point::scale(double number) {
        x *= number;
        y *= number;
    }
    void Point::invert(bool invert_x, bool invert_y) {
        if (invert_x) {
            this->x *= -1;
        } 
        if (invert_y) {
            this->y *= -1;
        }
    }

    Point Point::add(const Point& B) {
        return Point(x + B.x, y + B.y);
    }
    Point Point::minus(const Point& B) {
        return Point(x - B.y, y - B.y);
    }
    Point Point::rotate(double angle) {
        Point B(*this);
        B.x = B.x * cos(angle) - B.y * sin(angle);
        B.y = B.x * sin(angle) + B.y * cos(angle);
        return B;
    }
    Point Point::interpolate(double number) {
        return Point(x * number, y * number);
    }

    double Point::angleBetween(Point& B) {
        Point a = getUnitVector();
        Point b = B.getUnitVector();
        return acos(a.dotProduct(b));
    }
    double Point::dotProduct(const Point& B) {
        return x * B.x + y * B.y;
    }
    double Point::perpProduct(const Point& B) {
        return y * B.x + x * B.y;
    }
    double Point::crossProduct(const Point& B) {
        return x * B.y - y * B.x;
    }

    bool Point::equal(const Point& B) {
        return (x - B.x < precision && y - B.y < precision);
    }

    // Polygon class implementation
    Polygon::Polygon() {
        points.clear();
        xoffset = 0;
        width = 0;
        height = 0;
        yoffset = 0;
    }
    Polygon::Polygon(int size) {
        // pass
    }
    Polygon::Polygon(int h, int w, int x, int y) {
        this->height = h;
        this->width = w;
        this->xoffset = x;
        this->yoffset = y;
    }
    Polygon::Polygon(const Point* points, int size) {
        copy(points, size);
    }
    Polygon::Polygon(const Polygon& obj) {
        copy(obj);
    }

    int Polygon::getNumPoints() {
        return points.size();
    }
    std::vector<Point> Polygon::getPoints() {
        return points;
    }
    std::vector<Point> Polygon::getPoints(double offset_x, double offset_y) {
        std::vector<Point> temp_points;
        for (int i = 0; i < points.size(); ++i) {
            Point temp_point(
                points[i].getX() + offset_x,
                points[i].getY() + offset_y
            );
            temp_points.push_back(temp_point);
        }
        return temp_points;
    }

    int Polygon::getHeight() {
        return height;
    }
    int Polygon::getWidth() {
        return width;
    }
    int Polygon::getXOffset() {
        return xoffset;
    }
    int Polygon::getYOffset() {
        return yoffset;
    }

    void Polygon::setHeight(int h) {
        if (h > 0)
            this->height = h;
    }
    void Polygon::setWidth(int w) {
        if (w > 0)
            this->width = w;
    }
    void Polygon::setXOffset(int x) {
        if (x > 0)
            this->xoffset = x;
    }
    void Polygon::setYOffset(int y) {
        if (y > 0)
            this->yoffset = y;
    }

    void Polygon::addPoint(const Point& point) {
        this->points.push_back(point);
    }
    void Polygon::addPoint(int x, int y) {
        Point point(x, y);
        this->points.push_back(point);
    }

    void Polygon::removePoint(int x, int y) {

    }
    void Polygon::removePoint(const Point& point) {

    }

    void Polygon::copy(const Polygon& obj) {
        if (&points != nullptr) {
            this->points.clear();
            for (int i = 0; i < obj.points.size(); ++i) {
                this->points.push_back(obj.points[i]);
            }
        }
    }
    void Polygon::copy(const Point* points, int size) {
        if (points != nullptr && size > 0) {
            this->points.clear();
            for (int i = 0; i < size; ++i) {
                this->points.push_back(points[i]);
            }
        }
    }
    std::vector<Point> Polygon::getNorms(double offset_x, double offset_y) {
        std::vector<Point> normals;
        for (int i = 0; i < points.size(); ++i) {
            Point currentNormal(
                (offset_x + points[i + 1].getX()) - (offset_x + points[i].getX()),
                (offset_y + points[i + 1].getY()) - (offset_y + points[i].getY())
            );
            normals.push_back(currentNormal.getNormL());
        }
        Point lastNormal(
            (offset_x + points[0].getX()) - (offset_x + points[points.size() - 1].getX()),
            (offset_y + points[0].getY()) - (offset_y + points[points.size() - 1].getY())
        );
        normals.push_back(lastNormal.getNormL());
        return normals;
    }

    // Asset Array Implementation
    AssetArray<Polygon> polygons;
}