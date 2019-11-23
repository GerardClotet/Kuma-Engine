#include "Quadtree.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"



Quadtree::Quadtree(const AABB& box)
{
	root = new QuadtreeNode(box);
	root->tree = this;
}

Quadtree::~Quadtree()
{
	RELEASE(root);
}


void Quadtree::AddToQuadtree(const GameObject * gameObject)
{
	if (root->AddToQuadTreeNode(gameObject) == false) 
		out_of_tree.push_back(gameObject); //the game object is outside the quadtree
	
}

void Quadtree::Draw()
{
	root->Draw();
}

bool Quadtree::RemoveGameobjectTree(const GameObject* obj)
{
	if (root->RemoveGameobjectTreeNode(obj) == true) //the gameobject is inside the quadtree
		return true;

	else
	{
		//the game object we want to erase is outside the quadtree
		for (std::vector<const GameObject*>::iterator iter = out_of_tree.begin(); iter != out_of_tree.end(); ++iter)
		{
			if ((*iter) == obj)
			{
				out_of_tree.erase(iter);
				return true;
			}
		}
		return false; //not found the gameobject in any list
	}

}

void Quadtree::Clear()
{
	root->childs.clear();
	root->bucket.clear();
}







//---------------------------------------------------------------------------





QuadtreeNode::QuadtreeNode(const AABB& box)
{
	node_box = box;
}

QuadtreeNode::QuadtreeNode(Quadtree* quadtree, QuadtreeNode* parent, uint index)
{
	//subdivide the box 
	float3 minPoint, maxPoint;

	minPoint.x = (index / 2) == 1 ? parent->node_box.minPoint.x : (parent->node_box.maxPoint.x + parent->node_box.minPoint.x) / 2;
	maxPoint.x = (index / 2) == 1 ? (parent->node_box.maxPoint.x + parent->node_box.minPoint.x) / 2 : parent->node_box.maxPoint.x;

	minPoint.y = parent->node_box.minPoint.y;
	maxPoint.y = parent->node_box.maxPoint.y;

	minPoint.z = index % 2 == 0 ? parent->node_box.minPoint.z : (parent->node_box.maxPoint.z + parent->node_box.minPoint.z) / 2;
	maxPoint.z = index % 2 == 0 ? (parent->node_box.maxPoint.z + parent->node_box.minPoint.z) / 2 : parent->node_box.maxPoint.z;
	
	node_box = AABB(minPoint, maxPoint);
}

QuadtreeNode::~QuadtreeNode()
{
}


bool QuadtreeNode::AddToQuadTreeNode(const GameObject * gameObject)
{
	if (node_box.Intersects(gameObject->bbox.aabb_global))
	{
		if (childs.empty()) //No childs in the box?
		{
			bucket.push_back(gameObject);
			if (bucket.size() > MAX_BUCKET_SIZE)
			{
				SubdivideQuadTree();
				RedistributeQuadTree();
			}
		}
		else
		{
			if (!PutGameObjectToChilds(gameObject))
				bucket.push_back(gameObject);
		}

		return true;
	}

	else
		return false;
}



void QuadtreeNode::Draw()
{
	float3 corners[8];
	node_box.GetCornerPoints(corners);
	//get the corners of the node and draw them
	App->renderer3D->DrawQuadtreeCube(corners, { 1.0f, 0.5f, 0.9f });

	//if the node has childs, draw their childs recursively
	for (uint i = 0; i < childs.size(); i++)
		childs[i].Draw();
}

bool QuadtreeNode::RemoveGameobjectTreeNode(const GameObject * obj)
{
	for (std::vector<const GameObject*>::iterator iter = bucket.begin(); iter != bucket.end(); ++iter)
	{
		if ((*iter) == obj) //erase from bucket list if it matches
		{
			bucket.erase(iter);
			RemoveChilds();
			return true;
		}
	}

	//recursive remove for their childs
	for (uint i = 0; i < childs.size(); i++)
	{
		if (childs[i].RemoveGameobjectTreeNode(obj) == true) 
		{
			RemoveChilds();
			return true;
		}
	}

	return false;
}


void QuadtreeNode::SubdivideQuadTree()
{
	if (!childs.empty())
		LOG("ERROR - YOU CAN'T SPLIT IF THERE ARE ALREADY CHILDS");
	else
	{
		for (int i = 0; i < CHILD_SIZE; i++)
		{
			childs.push_back(QuadtreeNode(tree, this, i)); //generate a node and subdivide it
		}
	}

}


void QuadtreeNode::RedistributeQuadTree()
{
	for (std::vector<const GameObject*>::iterator iter = bucket.begin();iter!= bucket.end();)
	{
		if (PutGameObjectToChilds(*iter))
			iter = bucket.erase(iter); //erase the gameobject from the parent if it fits in the child

		else
			++iter; //if the gameobject doesn't fit in the child, keep it as parent child
	}
}

bool QuadtreeNode::PutGameObjectToChilds(const GameObject * obj)
{
	//put the gameobject to the child
	uint intersections = 0;
	uint childpos = -1; //to avoid getting the first position

	for (uint i = 0; i < childs.size(); i++)
	{
		if (childs[i].node_box.Intersects(obj->bbox.aabb_global))
		{
			intersections++;
			childpos = i;
		}
	}
	if (intersections == 1)
	{
		childs[childpos].AddToQuadTreeNode(obj);
		return true;
	}
	else if (intersections == 0)
		LOG("ERROR - No child intersection found");

	return false;
}

void QuadtreeNode::RemoveChilds()
{
	std::vector<const GameObject*> bucket_childs;
	GetBucketChilds(bucket_childs, false);
	if (bucket_childs.size() + bucket.size() <= MAX_BUCKET_SIZE)
	{
		for (uint i = 0; i < bucket_childs.size(); i++)
		{
			bucket.push_back(bucket_childs[i]);
		}

		childs.clear(); //clear the child list
	}
	bucket_childs.clear();  //clear the bucket list
}

void QuadtreeNode::GetBucketChilds(std::vector<const GameObject*>& obj_vec, bool wantSelf) const
{
	if (wantSelf)
	{
		for (uint i = 0; i < bucket.size(); i++)
		{
			obj_vec.push_back(bucket[i]);
		}
	}

	//recursively
	for (uint i = 0; i < childs.size(); i++)
	{
		childs[i].GetBucketChilds(obj_vec, true);
	}
}



