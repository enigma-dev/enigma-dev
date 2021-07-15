/** Copyright (C) 2021 Nabeel Danish
*** Nabeel Danish
**/

#ifndef ENIGMA_POLYGON_INTERNAL_H
#define ENIGMA_POLYGON_INTERNAL_H

#include <vector>
#include "AssetArray.h"
#include "Universal_System/scalar.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace enigma 
{
	#define NULL_POINT -1;
	using BoundingBox = Rect<int>;

	// The Polygon class represents the polygon that will be used
	// for detecting collisions in 2D space by the collision detection 
	// system
	class Polygon {
		// Attributes
		private:
			std::vector<glm::vec2> points;
			std::vector<glm::vec2> transformedPoints;
			std::vector<glm::vec2> normals;
			std::vector<std::vector<glm::vec2>> subpolygons;
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
			Polygon(const glm::vec2* points, int size);
			Polygon(const Polygon& obj);
			
			// Asset Array mandatory functions
			static const char* getAssetTypeName() { return "Polygon"; }
 			bool isDestroyed() const { return _destroyed; }
  			void destroy() { _destroyed = true; }

			// Getters
			int getNumPoints();
			std::vector<glm::vec2> getPoints();
			std::vector<glm::vec2> getTransformedPoints();
			std::vector<glm::vec2> getNormals();
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
			void addPoint(const glm::vec2& point);
			void addPoint(int x, int y);

			void removePoint(int x, int y);
			void removePoint(const glm::vec2& point);

			// Deep-copying functions
			void copy(const Polygon& obj);
			void copy(const glm::vec2* points, int size);

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
	void addOffsets(std::vector<glm::vec2>& points, double x, double y);
	glm::vec2 rotateVector(glm::vec2 vector, double angle, glm::vec2 pivot);
	glm::vec2 computeLeftNormal(glm::vec2 vector);
	double angleBetweenVectors(glm::vec2 vector1, glm::vec2 vector2);
}

#endif  // !ENIGMA_POLYGON_INTERNAL_H
