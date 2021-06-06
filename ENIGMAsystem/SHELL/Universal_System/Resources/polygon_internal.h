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
	class Vector2D 
	{
		// Attributes
		private: 
			double x;
        	double y;

		// Methods
		public:
			Vector2D();
			Vector2D(double x, double y);
			Vector2D(const Vector2D& obj);
			void copy(const Vector2D& obj);

			double getX();
			double getY();
			double getMagnitude();
			double getAngle();
			Vector2D getNormR();
			Vector2D getNormL();
			Vector2D getUnitVector();

			void setX(double x);
			void setY(double y);
			void setMagnitude(double number);
			void setAngle(double angle);
			void scale(double number);
			void invert(bool x, bool y);

			Vector2D add(const Vector2D& B);
			Vector2D minus(const Vector2D& B);
			Vector2D rotate(double angle);
			Vector2D interpolate(double number);

			double angleBetween(Vector2D& B);
			double dotProduct(const Vector2D& B);
			double perpProduct(const Vector2D& B);
			double crossProduct(const Vector2D& B);

			bool equal(const Vector2D& B);
	};

	// The Polygon class represents the polygon that will be used
	// for detecting collisions in 2D space by the collision detection 
	// system
	class Polygon {
		// Attributes
		private:
			std::vector<Vector2D> points;
			int height;
			int width;
			int xoffset;
			int yoffset;

		// Methods
		public:
			Polygon();
			Polygon(int size);
			Polygon(int h, int w, int x, int y);
			Polygon(const Vector2D* points, int size);
			Polygon(const Polygon& obj);

			int getNumPoints();
			std::vector<Vector2D> getPoints();
			std::vector<Vector2D> getPoints(double offset_x, double offset_y);

			int getHeight();
			int getWidth();
			int getXOffset();
			int getYOffset();

			void setHeight(int h);
			void setWidth(int w);
			void setXOffset(int x);
			void setYOffset(int y);

			void addPoint(const Vector2D& point);
			void addPoint(int x, int y);

			void removePoint(int x, int y);
			void removePoint(const Vector2D& point);

			void copy(const Polygon& obj);
			void copy(const Vector2D* points, int size);

			std::vector<Vector2D> getNorms(double offset_x, double offset_y);
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