/** Copyright (C) 2021 Nabeel Danish
*** Nabeel Danish
**/

#include <math.h>
#include <cfloat>
#include <algorithm>
#include <array>
#include <vector>

#include "polygon_internal.h"
#include "earcut.hpp"

// Polygon class implementation
namespace enigma 
{
    // Constructor
    Polygon::Polygon() 
    {
        points.clear();
        xoffset = 0;
        width = 0;
        height = 0;
        yoffset = 0;
    }

    // Constructor
    Polygon::Polygon(int size) 
    {
        // pass
    }

    // Parameterized Constructor 
    Polygon::Polygon(int h, int w, int x, int y) 
    {
        this->height = h;
        this->width = w;
        this->xoffset = x;
        this->yoffset = y;
    }

    // Constructor to initialize with points
    Polygon::Polygon(const glm::vec2* points, int size) 
    {
        copy(points, size);
    }

    // Copy Constructor
    Polygon::Polygon(const Polygon& obj) 
    {
        copy(obj);
    }

    // ---------------
    // Getters
    // ---------------
    int Polygon::getNumPoints() 
    {
        return points.size();
    }

    glm::vec2 Polygon::computeCenter() 
    {
        return glm::vec2(width / 2.0, height / 2.0);
    }
     
    std::vector<glm::vec2> Polygon::getPoints() 
    {
        return points;
    }

    int Polygon::getHeight() 
    {
        return height;
    }

    int Polygon::getWidth() 
    {
        return width;
    }

    int Polygon::getXOffset() 
    {
        return xoffset;
    }

    int Polygon::getYOffset() 
    {
        return yoffset;
    }

    void Polygon::setHeight(int h) 
    {
        if (h > 0)
            this->height = h;
    }

    void Polygon::setWidth(int w) 
    {
        if (w > 0)
            this->width = w;
    }

    void Polygon::setXOffset(int x) 
    {
        if (x > 0)
            this->xoffset = x;
    }

    void Polygon::setYOffset(int y) 
    {
        if (y > 0)
            this->yoffset = y;
    }

    void Polygon::addPoint(const glm::vec2& point) 
    {
        this->points.push_back(point);
    }

    void Polygon::addPoint(int x, int y) 
    {
        glm::vec2 point(x, y);
        this->points.push_back(point);
    }

    void Polygon::removePoint(int x, int y) 
    {
        glm::vec2 point(x, y);
        points.erase(std::remove(points.begin(), points.end(), point), points.end());
    }
    void Polygon::removePoint(const glm::vec2& point) 
    {
        points.erase(std::remove(points.begin(), points.end(), point), points.end());
    }

    void Polygon::copy(const Polygon& obj) 
    {
        this->points = obj.points;
        this->height = obj.height;
        this->width = obj.width;
    }

    void Polygon::copy(const glm::vec2* points, int size) 
    {
        if (size > 0) {
            this->points.clear();
            for (int i = 0; i < size; ++i) {
                this->points.push_back(points[i]);
            }
        }
    }
    
    void Polygon::print() 
    {
        printf("Points = ");
        std::vector<glm::vec2>::iterator it = points.begin();
        while (it != points.end()) {
            printf("(%f, %f), ", it->x, it->y);
            ++it;
        }
        printf("\nheight = %d width = %d\n", height, width);
    }

    void Polygon::decomposeConcave() 
    {
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
    // Stores the polygon resources, that are accessed throughout 
    // enigma namespace like in graphics, and in enigma_user namespace
    enigma::AssetArray<enigma::Polygon> polygons;

    // GLM function to rotate a vec2 around a pivot with angle
    glm::vec2 rotateVector(glm::vec2 vector, double angle, glm::vec2 pivot)
    {
        glm::vec2 rotated(vector.x, vector.y);
        double x = rotated.x - pivot.x;
        double y = rotated.y - pivot.y;
        rotated.x = ((x - pivot.x) * cos(angle) - (y - pivot.y) * sin(angle)) + pivot.x;
        rotated.y = ((x - pivot.x) * sin(angle) + (y - pivot.y) * cos(angle)) + pivot.y;
        return rotated;
    }

    // Computing left normal of a glm::vec2 vector
    glm::vec2 computeLeftNormal(glm::vec2 vector)
    {
        return glm::normalize(glm::vec2(vector.y, -1 * vector.x));
    }

    // Calculating the angle between two glm::vec2 vectors
    double angleBetweenVectors(glm::vec2 vector1, glm::vec2 vector2)
    {
        glm::vec2 point(vector2.x - vector1.x, vector1.y - vector2.y);
        double angle = glm::atan(point.y / point.x);
        double quad = 1.5708;
        if (point.x < 0 && point.y > 0) {
            angle = 2 * quad + angle;
        } else if (point.x < 0 && point.y < 0) {
            angle += 2 * quad;
        } else if (point.x > 0 && point.y < 0) {
            angle = 4 * quad + angle;
        }
        return angle;
    }

    // Functions for computing tranformations on polygon points
	void offsetPoints(std::vector<glm::vec2>& points, double x, double y)
    {
        std::vector<glm::vec2>::iterator it = points.begin();
        while (it != points.end()) {
            it->x += x;
            it->y += y;
            ++it;
        }
    }

	void rotatePoints(std::vector<glm::vec2>& points, double angle, glm::vec2 pivot)
    {
        std::vector<glm::vec2>::iterator it = points.begin();
        while (it != points.end())
        {
            *it = rotateVector(*it, angle, pivot);
            ++it;
        }
    }

	void scalePoints(std::vector<glm::vec2>& points, double xscale, double yscale)
    {
        std::vector<glm::vec2>::iterator it = points.begin();
        while (it != points.end())
        {
            it->x *= xscale;
            it->y *= yscale;
            ++it;
        }
    }

    void transformPoints(std::vector<glm::vec2>& points, double offset_x, double offset_y, double angle, glm::vec2 pivot, double xscale, double yscale)
    {
        std::vector<glm::vec2>::iterator it = points.begin();
        while (it != points.end()) 
        {
            // Applying scale
            it->x *= xscale;
            it->y *= yscale;

            // Applying rotation
            *it = rotateVector(*it, angle, pivot);

            // Applying Offset
            it->x += offset_x;
            it->y += offset_y;

            ++it;
        }
    }

    std::vector<glm::vec2> computeNormals(std::vector<glm::vec2>& points) 
    {
        std::vector<glm::vec2> normals;

        // Only compute normals if we have
        // more than 1 points
        if (points.size() >= 3) 
        {
            // Calculating normals
            for (int i = 0; i < points.size() - 1; ++i) 
            {
                glm::vec2 currentNormal(
                    (points[i + 1].x) - (points[i].x),
                    (points[i + 1].y) - (points[i].y)
                );
                normals.push_back(computeLeftNormal(currentNormal));
            }

            // Adding the last normal
            glm::vec2 lastNormal(
                (points[0].x) - (points[points.size() - 1].x),
                (points[0].y) - (points[points.size() - 1].y)
            );
            normals.push_back(computeLeftNormal(lastNormal));
        }

        return normals;
    }

    // Function to compute the bbox from the points
	enigma::BoundingBox computeBBOXFromPoints(std::vector<glm::vec2>& points)
    {
        std::vector<glm::vec2>::iterator it = points.begin();
        double min_x = DBL_MAX, 
                min_y = DBL_MAX, 
                max_x = DBL_MIN, 
                max_y = DBL_MIN;
        
        while (it != points.end())
        {
            min_x = it->x < min_x? it->x : min_x;
            min_y = it->y < min_y? it->y : min_y;
            max_x = it->x > max_x? it->x : max_x;
            max_y = it->y > max_y? it->y : max_y;
            ++it;
        }
        
        enigma::BoundingBox bbox;
        bbox.x = min_x;
        bbox.y = min_y;
        bbox.w = max_x - min_x;
        bbox.h = max_y - min_y;

        return bbox;
    }
}
