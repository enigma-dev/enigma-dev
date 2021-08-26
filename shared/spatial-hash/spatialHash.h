#pragma once
#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <unordered_map>
#include <vector>

struct BBOX
{
	int x1, y1, x2, y2;
};

class SpatialHash
{
	private:
		// Attributes
		int cellSize;
		int sceneWidth;
		int sceneHeight;
		int rows;
		int cols;
		int numCells;
		std::unordered_map<int, std::vector<int>> hashmap;

		// Methods
		void copy(const SpatialHash& obj);

	public:
		// Constructors
		SpatialHash();
		SpatialHash(const SpatialHash& obj);
		SpatialHash(int c, int w, int h);

		// Getters and Setters
		int getCellSize();
		int getSceneWidth();
		int getSceneHeight();
		int getNumCells();
		int getColumns();
		int getRows();

		// Utility functions
		void print();

		// Hashing functions
		int computeHash(int x, int y);
		std::vector<int>& computeBuckets(BBOX bbox);

		void registerObject(int obj_id, int x, int y);
		void registerObject(int obj_id, BBOX bbox);

		std::vector<int> getNearby(int obj_id, int x, int y);
		std::vector<int> getNearby(int obj_id, BBOX bbox);

		void removeObject(int obj_id, int x, int y);
		void removeObject(int obj_id, BBOX bbox);

		void updateHash(int obj_id, int x_prev, int y_prev, int x_new, int y_new);
		void updateHash(int obj_id, BBOX bbox_prev, BBOX bbox_new);
};

#endif // !SPATIAL_HASH_H
