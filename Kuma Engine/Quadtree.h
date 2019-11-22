#ifndef QUADTREE_H
#define QUADTREE_H

#define MAX_BUCKET_SIZE 2
#define CHILD_SIZE 4


class GameObject;
#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>
#include <map>

class QuadtreeNode
{
public:
	QuadtreeNode();
	~QuadtreeNode();

	void Create(const AABB limits);
	void Clear();
	
public:
	AABB node_box;
	void Draw();

private:
	QuadtreeNode* children[4] = { nullptr,nullptr,nullptr,nullptr };
};

class Quadtree
{
public:
	Quadtree();
	~Quadtree();

	void Create(const AABB limits);
	void Clear();

	void Draw();
private:
	QuadtreeNode * root = nullptr;
};


#endif