#include "spatialHash.h"

// Methods
void SpatialHash::copy(const SpatialHash& obj)
{
	cellSize = obj.cellSize;
	sceneWidth = obj.sceneWidth;
	sceneHeight = obj.sceneHeight;
	numCells = obj.numCells;
	hashmap = obj.hashmap;
}

// Constructors
SpatialHash::SpatialHash()
{
	cellSize = 0;
	sceneWidth = 0;
	sceneHeight = 0;
	numCells = 0;
}

SpatialHash::SpatialHash(const SpatialHash& obj)
{
	if (&obj != nullptr)
		copy(obj);
}

SpatialHash::SpatialHash(int c, int w, int h)
{
	cellSize = c;
	sceneWidth = w;
	sceneHeight = h;
	rows = sceneHeight / cellSize;
	cols = sceneWidth / cellSize;
	numCells = rows * cols;
}

// Getters and Setters
int SpatialHash::getCellSize()
{
	return cellSize;
}

int SpatialHash::getSceneWidth()
{
	return sceneWidth;
}

int SpatialHash::getSceneHeight()
{
	return sceneHeight;
}

int SpatialHash::getNumCells()
{
	return numCells;
}

int SpatialHash::getColumns()
{
	return cols;
}

int SpatialHash::getRows()
{
	return rows;
}

// Utility functions
void SpatialHash::print()
{

}

// Hashing Functions
int SpatialHash::computeHash(int x, int y)
{
	return (x / cellSize) + ((y / cellSize) * cols);
}

std::vector<int>& SpatialHash::computeBuckets(int x1, int y1, int x2, int y2)
{
	std::vector<int> buckets;
	int minHash = computeHash(x1, y1);
	int maxHash = computeHash(x2, y2);
	for (int i = minHash; i <= maxHash; ++i)
	{
		buckets.push_back(i);
	}
	return buckets;
}

void SpatialHash::registerObject(int obj_id, int x1, int y1, int x2, int y2)
{
	std::vector<int> buckets = computeBuckets(x1, y1, x2, y2);
	std::vector<int>::iterator it;
	for (it = buckets.begin(); it != buckets.end(); ++it)
	{
		hashmap[*it].push_back(obj_id);
	}
}

std::vector<int> SpatialHash::getNearby(int obj_id, int x, int y)
{
	int hash = computeHash(x, y);
	return hashmap[hash];
}

std::vector<int> SpatialHash::getNearby(int obj_id, int x1, int y1, int x2, int y2)
{
	std::vector<int> buckets;
	int minHash = computeHash(x1, y1);
	int maxHash = computeHash(x2, y2);
	for (int i = minHash; i <= maxHash; ++i)
	{
		buckets.insert(buckets.end(), hashmap[i].begin(), hashmap[i].end());
	}
	return buckets;
}