
#pragma once
#include <mgdl/mgdl-transform.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-material.h>
#include <mgdl/mgdl-light.h>

/**
 * @file mgdl-node.h
 * @ingroup scene
 * @brief Node class for scene graph
 */

struct DynamicArray;

enum NodeFlags
{
	NodeNone = 0,
	NodeTransform = 1,
	NodeMesh = 2,
	NodeMaterial = 4,
	NodeLight = 8,
	NodeChildren = 16
};
typedef enum NodeFlags NodeFlags;
typedef u32 NodeFlagField;

struct Node
{
	Transform* transform;
	Mesh* mesh;
	Material* material;
	Light* light;
	const char* name;
	NodeFlagField enabledElements;
	struct DynamicArray* children;

};
typedef struct Node Node;


#ifdef __cplusplus
extern "C"
{
#endif

	Node* Node_Create(u8 childCapacity);
	Node* Node_Clone(Node* source, NodeFlagField cloningFlags);
	Node* Node_FindChildByName(Node* parent, const char* nodeName);
	Node* Node_FindChildByIndex(Node* node, short index);
	Node* Node_FindChildByIndexRecursive_(Node* parent, short index, short* indexCounter);
	void Node_SetEnabled(Node* node, NodeFlagField elements);
	void Node_SetDisabled(Node* node, NodeFlagField elements);
	void Node_EnableDrawing(Node* node);
	void Node_DisableDrawing(Node* node);
	void Node_SetTransform(Node* node, const char* name, V3f position, V3f rotationAngles);
	void Node_SetContent(Node* node, const char* name, Mesh* meshParam, Material* materialParam);
	void Node_Draw(Node* node);

#ifdef __cplusplus
}
#endif
