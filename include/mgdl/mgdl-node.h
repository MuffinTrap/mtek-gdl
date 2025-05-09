
#pragma once
#include <vector>
#include <string>
#include <mgdl/mgdl-transform.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-material.h>
#include <mgdl/mgdl-light.h>

using namespace gdl;

struct Node
{
	Transform* transform;
	Mesh* mesh = nullptr;
	Material* material = nullptr;
	Light* light = nullptr;
	const char* name = nullptr;
	std::vector<Node*> children;
};

extern "C"
{

	Node* Node_Create();
	void Node_SetTransform(Node* node, const char* name, vec3 position, vec3 rotationAngles);
	void Node_SetContent(Node* node, const char* name, Mesh* meshParam, Material* materialParam);

	void Node_Draw(Node* node);

}
