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
	using BoundingBox = Rect<int>;

	// The Polygon class represents the polygon that will be used
	// for detecting collisions in 2D space by the collision detection 
	// system
	class Polygon 
	{
		// Attributes
		private:
			std::vector<glm::vec2> points;
			std::vector<std::vector<glm::vec2>> subpolygons;
			int height;
			int width;
			int xoffset;
			int yoffset;
			int numSubPolygons;

			// Asset Array mandatory attributes
			bool _destroyed = false;

		// Methods
		public:
			// Connstructors
			Polygon();
			Polygon(int size);
			Polygon(int h, int w, int x, int y);
			Polygon(const glm::vec2* points, int size);
			Polygon(const Polygon& obj);
			
			// Asset Array override functions
			static const char* getAssetTypeName() { return "Polygon"; }
 			bool isDestroyed() const { return _destroyed; }
  			void destroy() { _destroyed = true; }

			// Getters
			std::vector<glm::vec2> getPoints();
			int getHeight();
			int getWidth();
			int getXOffset();
			int getYOffset();

			// Computational Getters
			int getNumPoints();
			glm::vec2 computeCenter();

			// Setters
			void setHeight(int h);
			void setWidth(int w);
			void setXOffset(int x);
			void setYOffset(int y);

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
	};

	// MinMax Projection class; to determine collision
	// The class holds the minimum and the maximum projection on the axis
	// on which it is used, and stores the indices of those points from the 
	// list of points specified. Mainly used to return from the getMinMaxProjection()
	// function
	struct MinMaxProjection 
	{
		double min_projection, max_projection;
		int min_index, max_index;
	};

	// Mathematical functions for glm::vec2 calculations
	glm::vec2 rotateVector(glm::vec2 vector, double angle, glm::vec2 pivot);
	glm::vec2 computeLeftNormal(glm::vec2 vector);
	double angleBetweenVectors(glm::vec2 vector1, glm::vec2 vector2);

	// Main Asset Array; is used for storing Polygon 
	// Assets in the game amd is used throughout in the enigma codebase
	extern AssetArray<Polygon> polygons;

	// Functions for computing tranformations on polygon points
	void offsetPoints(std::vector<glm::vec2>& points, double x, double y);
	void rotatePoints(std::vector<glm::vec2>& points, double angle, glm::vec2 pivot);
	void scalePoints(std::vector<glm::vec2>& points, double xscale, double yscale);

	// Combined function for computing the SCALE->ROT->OFFSET transformation 
	// on the polygon points provided
	void transformPoints(std::vector<glm::vec2>& points, 
							double offset_x, double offset_y, 
							double angle, glm::vec2 pivot, 
							double xscale, double yscale);

	// Computes the normals along the edges of the polygon points
	// that are used in the collision detection
	std::vector<glm::vec2> computeNormals(std::vector<glm::vec2>& points); 

	// Function to compute the bbox from the points
	BoundingBox computeBBOXFromPoints(std::vector<glm::vec2>& points);
}

#endif  // !ENIGMA_POLYGON_INTERNAL_H
