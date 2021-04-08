// Nabeel Danish

#ifndef ENIGMA_POLYGON_INTERNAL_H
#define ENIGMA_POLYGON_INTERNAL_H

#include <vector>
#include "AssetArray.h"

namespace enigma 
{
	#define NULL_POINT -1;
	#define precision pow(4, -10)

	// The Point class represents a point in a polygon
	// Resource
	class Point 
	{
		// Attributes
		private: 
			double x;
        	double y;

		// Methods
		public:
			Point();
			Point(double x, double y);
			Point(const Point& obj);
			void copy(const Point& obj);

			double getX();
			double getY();
			double getMagnitude();
			double getAngle();
			Point getNormR();
			Point getNormL();
			Point getUnitVector();

			void setX(double x);
			void setY(double y);
			void setMagnitude(double number);
			void setAngle(double angle);
			void scale(double number);
			void invert(bool x, bool y);

			Point add(const Point& B);
			Point minus(const Point& B);
			Point rotate(double angle);
			Point interpolate(double number);

			double angleBetween(Point& B);
			double dotProduct(const Point& B);
			double perpProduct(const Point& B);
			double crossProduct(const Point& B);

			bool equal(const Point& B);
	};

	// The Polygon class represents the polygon that will be used
	// for detecting collisions in 2D space by the collision detection 
	// system
	class Polygon {
		// Attributes
		private:
			std::vector<Point> points;
			int height;
			int width;
			int xoffset;
			int yoffset;

		// Methods
		public:
			Polygon();
			Polygon(int size);
			Polygon(int h, int w, int x, int y);
			Polygon(const Point* points, int size);
			Polygon(const Polygon& obj);

			int getNumPoints();
			std::vector<Point> getPoints();
			std::vector<Point> getPoints(double offset_x, double offset_y);

			int getHeight();
			int getWidth();
			int getXOffset();
			int getYOffset();

			void setHeight(int h);
			void setWidth(int w);
			void setXOffset(int x);
			void setYOffset(int y);

			void addPoint(const Point& point);
			void addPoint(int x, int y);

			void removePoint(int x, int y);
			void removePoint(const Point& point);

			void copy(const Polygon& obj);
			void copy(const Point* points, int size);

			std::vector<Point> getNorms(double offset_x, double offset_y);
	};

	// MinMax Projection class to determine 
	// collision
	struct MinMaxProjection {
		public:
			double min_projection, max_projection;
			int min_index, max_index;
	};

	// Main Asset Array is used for storing Polygon 
	// Assets in the game 
	extern AssetArray<Polygon> polygons;
}

#endif  // !ENIGMA_POLYGON_INTERNAL_H