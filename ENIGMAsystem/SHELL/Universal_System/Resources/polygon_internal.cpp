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
        diagonals.clear();
        xoffset = 0;
        width = 0;
        height = 0;
        yoffset = 0;
        concave = false;
    }

    // Constructor
    Polygon::Polygon(int size) 
    {
        // pass
    }

    // Parameterized Constructor 
    Polygon::Polygon(int h, int w, int x, int y) 
    {
        points.clear();
        diagonals.clear();
        this->height = h;
        this->width = w;
        this->xoffset = x;
        this->yoffset = y;
        concave = false;
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

    std::vector<Diagonal> Polygon::getDiagonals()
    {
        return diagonals;
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

    bool Polygon::isConcave()
    {
        return concave;
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

    void Polygon::setConcave(bool c)
    {
        this->concave = c;
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
        this->diagonals = obj.diagonals;
        this->concave = obj.concave;
        this->xoffset = obj.xoffset;
        this->yoffset = obj.yoffset;
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
        std::vector<glm::vec2> temp_points = points;
        std::vector<Diagonal> temp_diagonals;
        triangulate(temp_points, temp_diagonals);
        
        std::vector<Diagonal>::iterator it;
        for (it = temp_diagonals.begin(); it != temp_diagonals.end(); ++it)
        {
            Diagonal d;
            d.a = it->a;
            d.b = it->b;
            d.i = find(points.begin(), points.end(), it->a) - points.begin();
            d.j = find(points.begin(), points.end(), it->b) - points.begin();
            this->diagonals.push_back(d);
        }

        // Debugging Starts
        // for (it = this->diagonals.begin(); it != this->diagonals.end(); ++it)
        // {
        //     printf("%d = ( %f, %f ) -> %d = ( %f, %f )\n", it->i, it->a.x, it->a.y, it->j, it->b.x, it->b.y);
        // }
        // Debugging Ends
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

    // ==========================================================
    // Mathematical Functions for Polygon triangulation
    // ==========================================================

    // Xor function for booleans
    bool Xor(bool x, bool y)
    {
        return !x ^ !y;
    }

    // Area bounded by a triangle
	double areaOfTriangle(glm::vec2 a, glm::vec2 b, glm::vec2 c) 
    {
        return ((b.x - a.x) * (c.y - a.y)) - ((c.x - a.x) * (b.y - a.y));
    }

    // Returns true if the point is stricly to the left of the 
    // line segment made by A and B
    bool pointLeft(glm::vec2 a, glm::vec2 b, glm::vec2 point)
    {
        return areaOfTriangle(a, b, point) > 0;
    }

    // Returns true if the point is either to the left or on 
    // the line segment made by A and B
	bool pointLeftOn(glm::vec2 a, glm::vec2 b, glm::vec2 point)
    {
        return areaOfTriangle(a, b, point) >= 0;
    }

    // Returns true if the point is on the line segment made by 
    // A and B
	bool pointCollinear(glm::vec2 a, glm::vec2 b, glm::vec2 point)
    {
        return areaOfTriangle(a, b, point) == 0;
    }

    // Returns true if the points (a, b, point) are collinear and 
    // point lies on the closed segment of (a, b)
    bool pointBetween(glm::vec2 a, glm::vec2 b, glm::vec2 point)
    {
        if (!pointCollinear(a, b, point))
            return false;
        
        if (a.x != b.x) 
            return ((a.x <= point.x) && (point.x <= b.x)) || ((a.x >= point.x) && (point.x >= b.x));
        else
            return ((a.y <= point.y) && (point.y <= b.y)) || ((a.y >= point.y) && (point.y >= b.y));
    }

    // Returns true if the line segments (a, b) and (c, d) are properly 
    // intersecting each other
    bool properIntersection(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d)
    {
        // Eliminating improper cases
        if  (  
                pointCollinear(a, b, c) || 
                pointCollinear(a, b, d) || 
                pointCollinear(c, d, a) ||
                pointCollinear(c, d, b)
            )
            {
                return false;
            }
        
        return     Xor(pointLeft(a, b, c), pointLeft(a, b, d)) 
                && Xor(pointLeft(c, d, a), pointLeft(c, d, b));
    }

    // Returns true if the line segments (a, b) and (c, d) are intersecting
    // , properly or improperly
    bool intersection(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d)
    {
        if (properIntersection(a, b, c, d))
            return true;
        
        else if (
                    pointBetween(a, b, c) ||
                    pointBetween(a, b, d) ||
                    pointBetween(c, d, a) ||
                    pointBetween(c, d, b)
                )
                return true;
        else
            return false;
    }

    // Returns true if the line segment (points[i], points[j]) is a 
    // proper diagonal of polygon, whether internal or external
    bool diagonal(std::vector<glm::vec2>& points, int i, int j)
    {
        int n = points.size();

        // Iterating over the edges
        for (int k = 0; k < n; ++k)
        {
            int k1 = (k + 1) % n;
            // Skip the edge that is incident
            if (k == i || k1 == i || k == j || k1 == j)
                continue;
            else
            {
                // If an intersection is found between line segment 
                // (i, j) and (k, k1) than i, j is not a diagonal
                if (intersection(points[i], points[j], points[k], points[k1]))
                    return false;
            }
        }
        return true;
    }

    // Returns true if the diagonal formed by A and B is inside the 
    // cone that is formed by the point points[A - 1], points[A], 
    // and points[A + 1]
    bool diagonalInCone(std::vector<glm::vec2>& points, int a, int b)
    {
        int n = points.size();

        // Points before and after A
        int a0 = (a + n - 1) % n;
        int a1 = (a + 1) % n;

        // If the cone formed is convex. We check this if a1 is 
        // left or on the line (a0, a)
        if (pointLeftOn(points[a0], points[a], points[a1])) 
        {
            // In Cone if a0 is left of (a, b) and a1 is left of (b, a)
            return pointLeft(points[a], points[b], points[a0]) 
                && pointLeft(points[b], points[a], points[a1]);
        }
        else
        {
            return !(   pointLeftOn(points[a], points[b], points[a1]) 
                     && pointLeftOn(points[b], points[a], points[a0]));
        }
    }

    // Returns true if the line segment (i, j) is a proper internal diagonal
    bool internalDiagonal(std::vector<glm::vec2>& points, int i, int j)
    {
        return diagonalInCone(points, i, j) && diagonal(points, i, j);
    }

    // Function removes point i from points
    void clipEar(std::vector<glm::vec2>& points, int i)
    {
        points.erase(points.begin() + i);
    }

    // Function to triangulate a polygon. It returns a vector of diagonals
    // that are triangulasing the polygon
    void triangulate(std::vector<glm::vec2>& points, std::vector<Diagonal>& diagonals)
    {
        int n = points.size();
        if (n > 3)
        {
            for (int i = 0; i < n; ++i)
            {
                int i1 = (i + 1) % n;
                int i2 = (i + 2) % n;
                if (internalDiagonal(points, i, i2))
                {
                    Diagonal d;
                    d.a = points[i];
                    d.b = points[i2];
                    diagonals.push_back(d);

                    clipEar(points, i1);
                    triangulate(points, diagonals);
                    break;
                }
            }
        }
    }
}
