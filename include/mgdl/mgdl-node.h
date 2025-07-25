
#pragma once
#include <mgdl/mgdl-transform.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-material.h>
#include <mgdl/mgdl-light.h>

struct DynamicArray;

struct Node
{
	Transform* transform;
	Mesh* mesh;
	Material* material;
	Light* light;
	const char* name;
	struct DynamicArray* children;
};
typedef struct Node Node;

#ifdef __cplusplus
extern "C"
{
#endif

	Node* Node_Create(u8 childCapacity);
	void Node_SetTransform(Node* node, const char* name, V3f position, V3f rotationAngles);
	void Node_SetContent(Node* node, const char* name, Mesh* meshParam, Material* materialParam);
	void Node_Draw(Node* node);


#ifdef __cplusplus
}
#endif
