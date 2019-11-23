#ifndef QUADTREE_H
#define QUADTREE_H

#define MAX_BUCKET_SIZE 2
#define CHILD_SIZE 4


#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Math/float3.h"
#include "Globals.h"
#include <vector>


class GameObject;
class QuadtreeNode;


class Quadtree
{
	friend class QuadtreeNode;
public:
	Quadtree(const AABB& box);
	~Quadtree();

	void Clear();
	void AddToQuadtree(const GameObject* gameObject);
	void Draw();
	bool RemoveGameobjectTree(const GameObject* obj);
private:
	QuadtreeNode * root = nullptr;
	std::vector<const GameObject*> out_of_tree;
};



class QuadtreeNode
{
	friend class Quadtree;

public:
	QuadtreeNode(const AABB& box);
	QuadtreeNode(Quadtree* quadtree, QuadtreeNode* parent, uint index);
	~QuadtreeNode();

	bool AddToQuadTreeNode(const GameObject* gameObject);
	void Draw();
	bool RemoveGameobjectTreeNode(const GameObject* obj);

private:
	void SubdivideQuadTree();
	void RedistributeQuadTree();
	bool PutGameObjectToChilds(const GameObject* obj);
	void RemoveChilds();
	void GetBucketChilds(std::vector<const GameObject*>& vector, bool wantSelf) const;

private:
	AABB node_box;
	std::vector<QuadtreeNode> childs;
	Quadtree* tree = nullptr;
	std::vector<const GameObject*> bucket;
};

#endif