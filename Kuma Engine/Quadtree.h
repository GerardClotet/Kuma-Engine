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
	void AddToQuadtree(GameObject* gameObject);
	void Draw();
	bool RemoveGameobjectTree(GameObject* obj);

	template<typename PRIMITIVE>
	void GetCandidates(std::vector<GameObject*>& gameObjects, PRIMITIVE& primitive)
	{
		root->GetCandidates(gameObjects, primitive);
	}

private:
	QuadtreeNode * root = nullptr;
	std::vector<GameObject*> out_of_tree;
};



class QuadtreeNode
{
	friend class Quadtree;

public:
	QuadtreeNode(const AABB& box);
	QuadtreeNode(Quadtree* quadtree, QuadtreeNode* parent, uint index);
	~QuadtreeNode();

	bool AddToQuadTreeNode(GameObject* gameObject);
	void Draw();
	bool RemoveGameobjectTreeNode(GameObject* obj);

	template<typename PRIMITIVE>
	void GetCandidates(std::vector<GameObject*>& gameObjects, PRIMITIVE& primitive);

private:
	void SubdivideQuadTree();
	void RedistributeQuadTree();
	bool PutGameObjectToChilds(GameObject* obj);
	void RemoveChilds();
	void GetBucketChilds(std::vector<GameObject*>& vector, bool wantSelf) const;

private:
	AABB node_box;
	std::vector<QuadtreeNode> childs;
	Quadtree* tree = nullptr;
	std::vector<GameObject*> bucket;
};


template<typename PRIMITIVE>
void QuadtreeNode::GetCandidates(std::vector<GameObject*>& gameObjects, PRIMITIVE& primitive)
{
	if (primitive.Intersects(node_box)) //change this for our epic plane function
	{
		for (uint i = 0; i < bucket.size(); i++)
		{
			gameObjects.push_back(bucket[i]);
		}

		for (uint i = 0; i < childs.size(); i++)
		{
			childs[i].GetCandidates(gameObjects, primitive);
		}
	}
}

#endif