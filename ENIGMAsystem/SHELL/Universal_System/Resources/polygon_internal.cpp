/** Copyright (C) 2021 Nabeel Danish
*** Nabeel Danish
**/

#include "polygon_internal.h"
#include "earcut.hpp"
#include <math.h>
#include <cfloat>

namespace enigma {
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
    Polygon::Polygon(const glm::vec2* points, int size) {
        copy(points, size);
    }
    Polygon::Polygon(const Polygon& obj) {
        copy(obj);
    }

    int Polygon::getNumPoints() {
        return points.size();
    }
    
    std::vector<glm::vec2> Polygon::getPoints() {
        return points;
    }

    std::vector<glm::vec2> Polygon::getTransformedPoints() {
        return transformedPoints;
    }

    std::vector<glm::vec2> Polygon::getNormals() {
        return normals;
    }

    void Polygon::computeTransformedPoints() {
        // Clearing points
        transformedPoints.clear();

        // Setting variables for finding
        // Min Max Points
        double max_x = DBL_MIN;
        double min_x = DBL_MAX;
        double max_y = DBL_MIN;
        double min_y = DBL_MAX;

        // Getting origin for rotation
        glm::vec2 origin( width / 2.0, height / 2.0 );

        for (int i = 0; i < points.size(); ++i) {
            // Getting point
            glm::vec2 point = points[i];

            // Applying Scale
            double x = (point.x * scale);
            double y = (point.y * scale);

            // Applying Rotation
            glm::vec2 temp_point(x, y);
            temp_point = rotateVector(temp_point, this->angle, origin);

            // For height and width recalculation
            x = temp_point.x;
            y = temp_point.y;

            max_x = x > max_x? x : max_x;
            min_x = x < min_x? x : min_x;
            max_y = y > max_y? y : max_y;
            min_y = y < min_y? y : min_y;
            
            // Adding to the points
            transformedPoints.push_back(temp_point);
        }

        // Recalculating height and width
        scaledWidth = abs(max_x - min_x);
        scaledHeight = abs(max_y - min_y);

        // Setting the BBOX
        bbox.x = min_x - 0.5;
        bbox.y = min_y - 0.5;
        bbox.w = scaledWidth + 0.5;
        bbox.h = scaledHeight + 0.5;
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

    BoundingBox Polygon::getBBOX() {
        return bbox;
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
        }
    }

    void Polygon::setScaledWidth(int w) {
        if (w > 0) {
            scaledWidth = w;
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
        this->scale = s;
        computeTransformedPoints();
        computeNormals();
    }

    void Polygon::setAngle(double a) {
        this->angle = a;
        computeTransformedPoints();
        computeNormals();
    }

    void Polygon::addPoint(const glm::vec2& point) {
        this->points.push_back(point);
        computeTransformedPoints();
        computeNormals();
    }

    void Polygon::addPoint(int x, int y) {
        glm::vec2 point(x, y);
        this->points.push_back(point);
        computeTransformedPoints();
        computeNormals();
    }

    void Polygon::removePoint(int x, int y) {
        // TODO: Implement me
    }
    void Polygon::removePoint(const glm::vec2& point) {
        // TODO: Implement me
    }

    void Polygon::copy(const Polygon& obj) {
        this->points = obj.points;
        this->transformedPoints = obj.transformedPoints;
        this->normals = obj.normals;
        this->height = obj.height;
        this->width = obj.width;
        this->scale = obj.scale;
        this->angle = obj.angle;
        this->scaledHeight = obj.scaledHeight;
        this->scaledWidth = obj.scaledWidth;
        this->bbox = obj.bbox;
    }

    void Polygon::copy(const glm::vec2* points, int size) {
        if (size > 0) {
            this->points.clear();
            for (int i = 0; i < size; ++i) {
                this->points.push_back(points[i]);
            }
            computeTransformedPoints();
            computeNormals();
        }
    }
    
    void Polygon::computeNormals() {
        normals.clear();

        // Only compute normals if we have
        // more than 1 points
        if (transformedPoints.size() >= 3) 
        {
            // Calculating normals
            for (int i = 0; i < transformedPoints.size() - 1; ++i) {
                glm::vec2 currentNormal(
                    (transformedPoints[i + 1].x) - (transformedPoints[i].x),
                    (transformedPoints[i + 1].y) - (transformedPoints[i].y)
                );
                normals.push_back(computeLeftNormal(currentNormal));
            }

            // Adding the last normal
            glm::vec2 lastNormal(
                (transformedPoints[0].x) - (transformedPoints[transformedPoints.size() - 1].x),
                (transformedPoints[0].y) - (transformedPoints[transformedPoints.size() - 1].y)
            );
            normals.push_back(computeLeftNormal(lastNormal));
        }
    }

    void Polygon::print() {
        printf("Points = ");
        std::vector<glm::vec2>::iterator it = points.begin();
        while (it != points.end()) {
            printf("(%f, %f), ", it->x, it->y);
            ++it;
        }
        printf("\nheight = %d width = %d\n", height, width);
    }

    void Polygon::decomposeConcave() {
        std::vector<std::vector<std::array<double, 2>>> polygonToSend;
        std::vector<glm::vec2>::iterator it = points.begin();

        std::vector<std::array<double, 2>> temp_points;
        while (it != points.end()) {
            std::array<double, 2> temp_point;
            temp_point[0] = it->x;
            temp_point[1] = it->y;

            temp_points.push_back(temp_point);
            ++it;
        }
        polygonToSend.push_back(temp_points);
        std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygonToSend);

        std::vector<uint32_t>::iterator jt = indices.begin();

        numSubPolygons = 0;
        int i = 0, j = 1, k = 2;
        while (k < indices.size()) {
            std::vector<glm::vec2> subpolygon;
            
            for (int n = i; n <= k; ++n) {
                glm::vec2 point(polygonToSend[0][indices[n]][0], polygonToSend[0][indices[n]][1]);
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

    // External Function
    void addOffsets(std::vector<glm::vec2>& points, double x, double y) 
    {
        std::vector<glm::vec2>::iterator it = points.begin();
        while (it != points.end()) {
            it->x += x;
            it->y += y;
            ++it;
        }
    }

    glm::vec2 rotateVector(glm::vec2 vector, double angle, glm::vec2 pivot)
    {
        glm::vec2 rotated(vector.x, vector.y);
        double x = rotated.x - pivot.x;
        double y = rotated.y - pivot.y;
        rotated.x = ((x - pivot.x) * cos(angle) - (y - pivot.y) * sin(angle)) + pivot.x;
        rotated.y = ((x - pivot.x) * sin(angle) + (y - pivot.y) * cos(angle)) + pivot.y;
        return rotated;
    }

    glm::vec2 computeLeftNormal(glm::vec2 vector)
    {
        return glm::vec2(vector.y, -1 * vector.x);
    }

    double angleBetweenVectors(glm::vec2 vector1, glm::vec2 vector2)
    {
        glm::vec2 da = glm::normalize(vector1);
        glm::vec2 db = glm::normalize(vector2);
        return glm::acos(glm::dot(da, db));
    }
}
