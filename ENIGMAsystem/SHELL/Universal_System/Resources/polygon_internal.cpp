/** Copyright (C) 2021 Nabeel Danish
*** Nabeel Danish
**/

#include "polygon_internal.h"
#include "earcut.hpp"
#include <math.h>
#include <cfloat>

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
    Vector2D Vector2D::rotate(double angle, Vector2D origin) {
        Vector2D B(*this);
        double x = B.x - origin.getX();
        double y = B.y - origin.getY();
        B.x = ((x - origin.getX()) * cos(angle) - (y - origin.getY()) * sin(angle)) + origin.getX();
        B.y = ((x - origin.getX()) * sin(angle) + (y - origin.getY()) * cos(angle)) + origin.getY();
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
        double precision = pow(4, -10);
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
        scale = 1.0;
        angle = 0.0;
        scaledHeight = height;
        scaledWidth = width;
    }
    Polygon::Polygon(int size) {
        // pass
    }
    Polygon::Polygon(int h, int w, int x, int y) {
        this->height = h;
        this->width = w;
        this->xoffset = x;
        this->yoffset = y;
        scale = 1.0;
        angle = 0.0;
        scaledHeight = height;
        scaledWidth = width;
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

        double max_x = DBL_MIN;
        double min_x = DBL_MAX;
        double max_y = DBL_MIN;
        double min_y = DBL_MAX;

        // Getting origin for rotation
        Vector2D origin( width / 2.0, height / 2.0 );

        for (int i = 0; i < points.size(); ++i) {
            // Getting rotated point
            // TODO (NABEEL) : FIX ROTATION POINTS BUG
            Vector2D point = points[i]; //.rotate(this->angle, origin);
            
            // Debugging Part Starts
            // if (this->angle != 0) {
            //     printf("point before rotation : ");
            //     points[i].print();
            //     printf("\tpoint after rotation : ");
            //     point.print();
            //     printf("\torigin : ");
            //     origin.print();
            //     printf("\n");
            // }
            // Debugging Part Ends

            // Applying Scale
            double x = (point.getX() * scale); //  + offset_x;
            double y = (point.getY() * scale); //  + offset_y;

            // Applying Rotation
            Vector2D temp_point(x, y);
            temp_point = temp_point.rotate(this->angle, origin);

            // Applying Offset
            temp_point.setX(temp_point.getX() + offset_x);
            temp_point.setY(temp_point.getY() + offset_y);
            
            // For height and width recalculation
            max_x = x > max_x? x : max_x;
            min_x = x < min_x? x : min_x;
            max_y = y > max_y? y : max_y;
            min_y = y < min_y? y : min_y;
            
            // Adding to the points
            temp_points.push_back(temp_point);
        }

        // Recalculating height and width
        // TODO (NABEEL) : FIX BUG FOR WRONG HEIGHT AND WIDTH CALCULATION
        this->setScaledWidth(abs(max_x - min_x));
        this->setScaledHeight(abs(max_y - min_y));

        // Debugging Part Starts
        // printf("Points when sending from the getPoints(double, double) function:\n");
        // for (int i = 0; i < temp_points.size(); ++i) {
        //     printf("( %f, %f ) ", temp_points[i].getX(), temp_points[i].getY());
        // }
        // printf("\n");
        // Debugging Part Ends

        return temp_points;
    }

    int Polygon::getHeight() {
        return height;
    }
    int Polygon::getWidth() {
        return width;
    }
    int Polygon::getScaledHeight() {
        return scaledHeight;
    }
    int Polygon::getScaledWidth() {
        return scaledWidth;
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
    void Polygon::setScaledHeight(int h) {
        if (h > 0) {
            scaledHeight = h;
            // setScale(scaledHeight / height);
        }
    }
    void Polygon::setScaledWidth(int w) {
        if (w > 0) {
            scaledWidth = w;
            // setScale(scaledWidth / width);
        }
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
    void Polygon::setAngle(double a) {
        this->angle = a;
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
        this->height = obj.height;
        this->width = obj.width;
        this->scale = obj.scale;
        this->angle = obj.angle;
        scaledHeight = height;
        scaledWidth = width;
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
        std::vector<Vector2D> temp_points = getPoints(offset_x, offset_y);

        // Calculating normals
        for (int i = 0; i < temp_points.size(); ++i) {
            Vector2D currentNormal(
                (temp_points[i + 1].getX()) - (temp_points[i].getX()),
                (temp_points[i + 1].getY()) - (temp_points[i].getY())
            );
            normals.push_back(currentNormal.getNormL());
        }
        Vector2D lastNormal(
            (temp_points[0].getX()) - (temp_points[temp_points.size() - 1].getX()),
            (temp_points[0].getY()) - (temp_points[temp_points.size() - 1].getY())
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
