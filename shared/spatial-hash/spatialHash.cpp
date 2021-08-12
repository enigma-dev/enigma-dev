#include <algorithm>
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

std::vector<int>& SpatialHash::computeBuckets(BBOX bbox)
{
	std::vector<int> buckets;
	int minHash = computeHash(bbox.x1, bbox.y1);
	int maxHash = computeHash(bbox.x2, bbox.y2);
	for (int i = minHash; i <= maxHash; ++i)
	{
		buckets.push_back(i);
	}
	return buckets;
}

void SpatialHash::registerObject(int obj_id, int x, int y)
{
	hashmap[computeHash(x, y)].push_back(obj_id);
}

void SpatialHash::registerObject(int obj_id, BBOX bbox)
{
	std::vector<int> buckets = computeBuckets(bbox);
	std::vector<int>::iterator it;
	for (it = buckets.begin(); it != buckets.end(); ++it)
	{
		hashmap[*it].push_back(obj_id);
	}
}

std::vector<int> SpatialHash::getNearby(int obj_id, int x, int y)
{
	return hashmap[computeHash(x, y)];
}

std::vector<int> SpatialHash::getNearby(int obj_id, BBOX bbox)
{
	std::vector<int> buckets;
	int minHash = computeHash(bbox.x1, bbox.y1);
	int maxHash = computeHash(bbox.x2, bbox.y2);
	for (int i = minHash; i <= maxHash; ++i)
	{
		buckets.insert(buckets.end(), hashmap[i].begin(), hashmap[i].end());
	}
	return buckets;
}

void SpatialHash::removeObject(int obj_id, int x, int y)
{
	int hash = computeHash(x, y);
	std::vector<int>::iterator position = std::find(hashmap[hash].begin(), hashmap[hash].end(), obj_id);
	if (position != hashmap[hash].end())
		hashmap[hash].erase(position);
}

void SpatialHash::removeObject(int obj_id, BBOX bbox)
{
	int minHash = computeHash(bbox.x1, bbox.y1);
	int maxHash = computeHash(bbox.x1, bbox.y1);
	for (int i = minHash; i <= maxHash; ++i)
	{
		std::vector<int>::iterator position = std::find(hashmap[i].begin(), hashmap[i].end(), obj_id);
		if (position != hashmap[i].end())
			hashmap[i].erase(position);
	}
}

void SpatialHash::updateHash(int obj_id, int x_prev, int y_prev, int x_new, int y_new)
{
	removeObject(obj_id, x_prev, y_prev);
	registerObject(obj_id, x_new, y_new);
}

void SpatialHash::updateHash(int obj_id, BBOX bbox_prev, BBOX bbox_new)
{
	removeObject(obj_id, bbox_prev);
	registerObject(obj_id, bbox_new);
}