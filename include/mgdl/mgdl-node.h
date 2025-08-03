
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

enum NodeCloningFlags
{
	CloneTransform = 1,
	CloneMesh = 2,
	CloneMaterial = 4,
	CloneLight = 8,
	CloneChildren = 16
};
typedef enum NodeCloningFlags NodeCloningFlags;

#ifdef __cplusplus
extern "C"
{
#endif


	Node* Node_Create(u8 childCapacity);
	Node* Node_Clone(Node* source, u32 cloningFlags);
	void Node_SetTransform(Node* node, const char* name, V3f position, V3f rotationAngles);
	void Node_SetContent(Node* node, const char* name, Mesh* meshParam, Material* materialParam);
	void Node_Draw(Node* node);


#ifdef __cplusplus
}
#endif
