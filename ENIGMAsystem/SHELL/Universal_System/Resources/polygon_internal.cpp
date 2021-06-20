/** Copyright (C) 2021 Nabeel Danish
*** Nabeel Danish
**/

#include "polygon_internal.h"
#include "earcut.hpp"
#include <math.h>

namespace enigma {

    // Vector2D Class Implementation
    Vector2D::Vector2D() {
        x = NULL_POINT;
        y = NULL_POINT;
    }
    Vector2D::Vector2D(const Vector2D& obj) {
        copy(obj);
    }
    Vector2D::Vector2D(double x, double y) {
        this->x = x;
        this->y = y;
    }
    void Vector2D::copy(const Vector2D& obj) {
        if (&obj != nullptr) {
            this->x = obj.x; 
            this->y = obj.y;
        }
    }
    double Vector2D::getX() {
        return this->x;
    }
    double Vector2D::getY() {
        return this->y;
    }
    void Vector2D::setX(double x) {
        this->x = x;
    }
    void Vector2D::setY(double y) {
        this->y = y;
    }
    double Vector2D::getMagnitude() {
        return sqrt(pow(x, 2) + pow(y, 2));
    }
    double Vector2D::getAngle() {
        return atan2(y, x);
    }
    Vector2D Vector2D::getNormR() {
        return Vector2D(-1 * y, x);
    }
    Vector2D Vector2D::getNormL() {
        return Vector2D(y, -1 * x);
    }
    Vector2D Vector2D::getUnitVector() {
        return Vector2D(x / getMagnitude(), y / getMagnitude());
    }

    void Vector2D::setMagnitude(double number) {
        double current_angle = getAngle();
        x = number * cos(current_angle);
        y = number * sin(current_angle);
    }
    void Vector2D::setAngle(double angle) {
        double current_magnitude = getMagnitude();
        x = current_magnitude * cos(angle);
        y = current_magnitude * sin(angle);
    }
    void Vector2D::scale(double number) {
        x *= number;
        y *= number;
    }
    void Vector2D::invert(bool invert_x, bool invert_y) {
        if (invert_x) {
            this->x *= -1;
        } 
        if (invert_y) {
            this->y *= -1;
        }
    }

    Vector2D Vector2D::add(const Vector2D& B) {
        return Vector2D(x + B.x, y + B.y);
    }
    Vector2D Vector2D::minus(const Vector2D& B) {
        return Vector2D(x - B.y, y - B.y);
    }
    Vector2D Vector2D::rotate(double angle) {
        Vector2D B(*this);
        B.x = B.x * cos(angle) - B.y * sin(angle);
        B.y = B.x * sin(angle) + B.y * cos(angle);
        return B;
    }
    Vector2D Vector2D::interpolate(double number) {
        return Vector2D(x * number, y * number);
    }

    double Vector2D::angleBetween(Vector2D& B) {
        Vector2D a = getUnitVector();
        Vector2D b = B.getUnitVector();
        return acos(1.0 / a.dotProduct(b));
    }
    double Vector2D::dotProduct(const Vector2D& B) {
        return x * B.x + y * B.y;
    }
    double Vector2D::perpProduct(const Vector2D& B) {
        return y * B.x + x * B.y;
    }
    double Vector2D::crossProduct(const Vector2D& B) {
        return x * B.y - y * B.x;
    }

    bool Vector2D::equal(const Vector2D& B) {
        return (x - B.x < precision && y - B.y < precision);
    }

    void Vector2D::print() {
        printf("( %f, %f )", x, y);
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
    Polygon::Polygon(const Vector2D* points, int size) {
        copy(points, size);
    }
    Polygon::Polygon(const Polygon& obj) {
        copy(obj);
    }

    int Polygon::getNumPoints() {
        return points.size();
    }
    std::vector<Vector2D> Polygon::getPoints() {
        return points;
    }
    std::vector<Vector2D> Polygon::getPoints(double offset_x, double offset_y) {
        std::vector<Vector2D> temp_points;
        for (int i = 0; i < points.size(); ++i) {
            Vector2D temp_point(
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
    double Polygon::getScale() {
        return scale;
    }
    double Polygon::getAngle() {
        return angle;
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
    void Polygon::setScale(double s) {
        if (s > 0)
            this->scale = s;
    }

    void Polygon::addPoint(const Vector2D& point) {
        this->points.push_back(point);
    }
    void Polygon::addPoint(int x, int y) {
        Vector2D point(x, y);
        this->points.push_back(point);
    }

    void Polygon::removePoint(int x, int y) {
        // TODO: Implement me
    }
    void Polygon::removePoint(const Vector2D& point) {
        // TODO: Implement me
    }

    void Polygon::copy(const Polygon& obj) {
        if (&points != nullptr) {
            this->points.clear();
            for (int i = 0; i < obj.points.size(); ++i) {
                this->points.push_back(obj.points[i]);
            }
        }
    }
    void Polygon::copy(const Vector2D* points, int size) {
        if (points != nullptr && size > 0) {
            this->points.clear();
            for (int i = 0; i < size; ++i) {
                this->points.push_back(points[i]);
            }
        }
    }
    std::vector<Vector2D> Polygon::getNorms(double offset_x, double offset_y) {
        std::vector<Vector2D> normals;
        for (int i = 0; i < points.size(); ++i) {
            Vector2D currentNormal(
                (offset_x + points[i + 1].getX()) - (offset_x + points[i].getX()),
                (offset_y + points[i + 1].getY()) - (offset_y + points[i].getY())
            );
            normals.push_back(currentNormal.getNormL());
        }
        Vector2D lastNormal(
            (offset_x + points[0].getX()) - (offset_x + points[points.size() - 1].getX()),
            (offset_y + points[0].getY()) - (offset_y + points[points.size() - 1].getY())
        );
        normals.push_back(lastNormal.getNormL());
        return normals;
    }
    void Polygon::print() {
        printf("Points = ");
        std::vector<Vector2D>::iterator it = points.begin();
        while (it != points.end()) {
            printf("(%f, %f), ", (*it).getX(), (*it).getY());
            ++it;
        }
        printf("\nheight = %d width = %d\n", height, width);
    }
    void Polygon::decomposeConcave() {
        std::vector<std::vector<std::array<double, 2>>> polygonToSend;
        std::vector<Vector2D>::iterator it = points.begin();

        std::vector<std::array<double, 2>> temp_points;
        while (it != points.end()) {
            std::array<double, 2> temp_point;
            temp_point[0] = (*it).getX();
            temp_point[1] = (*it).getY();

            temp_points.push_back(temp_point);
            ++it;
        }
        polygonToSend.push_back(temp_points);
        std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygonToSend);

        std::vector<uint32_t>::iterator jt = indices.begin();

        numSubPolygons = 0;
        int i = 0, j = 1, k = 2;
        while (k < indices.size()) {
            std::vector<Vector2D> subpolygon;
            
            for (int n = i; n <= k; ++n) {
                Vector2D point(polygonToSend[0][indices[n]][0], polygonToSend[0][indices[n]][1]);
                subpolygon.push_back(point);
            }

            this->subpolygons.push_back(subpolygon);
            ++(this->numSubPolygons);

            ++i;
            ++j;
            ++k;
        }
    }

    // Asset Array Implementation
    AssetArray<Polygon> polygons;
}
