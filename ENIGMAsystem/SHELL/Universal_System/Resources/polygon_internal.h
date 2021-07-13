/** Copyright (C) 2021 Nabeel Danish
*** Nabeel Danish
**/

#ifndef ENIGMA_POLYGON_INTERNAL_H
#define ENIGMA_POLYGON_INTERNAL_H

#include <vector>
#include "AssetArray.h"
#include "Universal_System/scalar.h"

namespace enigma 
{
	#define NULL_POINT -1;
	using BoundingBox = Rect<int>;
	// #define precision pow(4, -10);

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
			Vector2D rotate(double angle, Vector2D origin);
			Vector2D interpolate(double number);

			double angleBetween(Vector2D& B);
			double dotProduct(const Vector2D& B);
			double perpProduct(const Vector2D& B);
			double crossProduct(const Vector2D& B);

			bool equal(const Vector2D& B);

			void print();
	};

	// The Polygon class represents the polygon that will be used
	// for detecting collisions in 2D space by the collision detection 
	// system
	class Polygon {
		// Attributes
		private:
			std::vector<Vector2D> points;
			std::vector<Vector2D> transformedPoints;
			std::vector<Vector2D> normals;
			std::vector<std::vector<Vector2D>> subpolygons;
			int height;
			int width;
			int scaledHeight;
			int scaledWidth;
			int xoffset;
			int yoffset;
			int numSubPolygons;
			double scale;
			double angle;
			BoundingBox bbox;

			// Asset Array mandatory attributes
			bool _destroyed = false;

		// Methods
		public:
			Polygon();
			Polygon(int size);
			Polygon(int h, int w, int x, int y);
			Polygon(const Vector2D* points, int size);
			Polygon(const Polygon& obj);
			
			// Asset Array mandatory functions
			static const char* getAssetTypeName() { return "Polygon"; }
 			bool isDestroyed() const { return _destroyed; }
  			void destroy() { _destroyed = true; }

			// Getters
			int getNumPoints();
			std::vector<Vector2D> getPoints();
			std::vector<Vector2D> getTransformedPoints();
			std::vector<Vector2D> getNormals();
			int getHeight();
			int getWidth();
			int getScaledHeight();
			int getScaledWidth();
			int getXOffset();
			int getYOffset();
			double getScale();
			double getAngle();
			BoundingBox getBBOX();

			// Setters
			void setHeight(int h);
			void setWidth(int w);
			void setScaledHeight(int h);
			void setScaledWidth(int w);
			void setXOffset(int x);
			void setYOffset(int y);
			void setScale(double s);
			void setAngle(double a);

			// Insertion and Deletion
			void addPoint(const Vector2D& point);
			void addPoint(int x, int y);

			void removePoint(int x, int y);
			void removePoint(const Vector2D& point);

			// Deep-copying functions
			void copy(const Polygon& obj);
			void copy(const Vector2D* points, int size);

			// Displays all points
			void print();
			
			// Mathematical computations for caching
		private:	
			void decomposeConcave();
			void computeTransformedPoints();
			void computeNormals();
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

	// Function to mutate a vector of Vector2D
	// to add offsets
	void addOffsets(std::vector<Vector2D>& points, double x, double y);
}

#endif  // !ENIGMA_POLYGON_INTERNAL_H
