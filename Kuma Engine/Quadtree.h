#ifndef QUADTREE_H
#define QUADTREE_H

#define MAX_BUCKET_SIZE 2
#define CHILD_SIZE 4


class GameObject;
#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>
#include <map>


class Quadtree
{
	friend class QuadtreeNode;
public:
	Quadtree();
	~Quadtree();

	void Create(const AABB limits);
	void Clear();
	void AddGameObject(const GameObject* gameObject);
	void Draw();
private:
	QuadtreeNode * root = nullptr;
	std::vector<const GameObject*> out_of_tree;
};



class QuadtreeNode
{
	friend class Quadtree;

public:
	QuadtreeNode();
	~QuadtreeNode();

	void Create(const AABB limits);
	void Clear();
	bool AddGameObject(const GameObject* gameObject);

public:
	AABB node_box;
	void Draw();

private:
	std::vector<QuadtreeNode> childs;
	Quadtree* tree = nullptr;
	std::vector<const GameObject*> bucket;
};

#endif