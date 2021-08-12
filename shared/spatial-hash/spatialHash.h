#pragma once
#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <unordered_map>
#include <vector>

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
		std::vector<int>& computeBuckets(int x1, int y1, int x2, int y2);
		void registerObject(int obj_id, int x1, int y1, int x2, int y2);
		std::vector<int> getNearby(int obj_id, int x, int y);
		std::vector<int> getNearby(int obj_id, int x1, int y1, int x2, int y2);
};

#endif // !SPATIAL_HASH_H
