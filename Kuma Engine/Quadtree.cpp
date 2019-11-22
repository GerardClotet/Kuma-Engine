#include "Quadtree.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"



Quadtree::Quadtree()
{

}

Quadtree::~Quadtree()
{
	delete root;
}

void Quadtree::Create(const AABB limits)
{
	root = new QuadtreeNode();
	root->node_box.SetFrom(limits);
}

void Quadtree::Draw()
{
	root->Draw();
}

void Quadtree::Clear()
{
	root->Clear();
}

void Quadtree::AddGameObject(const GameObject * gameObject)
{
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

QuadtreeNode::QuadtreeNode()
{

}

QuadtreeNode::~QuadtreeNode()
{
}

void QuadtreeNode::Create(const AABB limits)
{
	node_box.SetFrom(limits);
}

void QuadtreeNode::Clear()
{
	
}

bool QuadtreeNode::AddGameObject(const GameObject * gameObject)
{
	return false;
}



