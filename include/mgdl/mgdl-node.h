
#pragma once
#include <mgdl/mgdl-transform.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-material.h>
#include <mgdl/mgdl-light.h>
#include <mgdl/mgdl-dynamic_array.h>

struct Menu;

// TODO DANGER
// Test this code

/**
 * @file mgdl-node.h
 * @ingroup scene
 * @brief Node class that can be used to create a scene graph
 */

enum NodeFlags
{
	NodeActive = 0, /**< Is active. */
	NodeApplyTransform = 1,
	NodeChildren = 16 /**< Can have child nodes */
};
typedef enum NodeFlags NodeFlags;
typedef u32 NodeFlagField;

enum Node_DebugFlag : u32
{
	Index = 1,
	UniqueId = 2,
	Position = 4,
	Rotation = 8
};

typedef void (*HandleContentIdCallback)(s16);

struct Node
{
	u16 id; /**< Unique id for node, set by user */
	s16 contentId; /**< Content id for node, set by user */
	NodeFlagField flags; /**< What flags are on. All by default */
	Transform transform; /**< Transform of the node relative to parent */
	Node* parent; /**< Node's parent. Null if root node */
	struct DynamicArray* childNodes; /**< Children of the node */
};
typedef struct Node Node;

DYNAMIC_ARRAY(Node)

#ifdef __cplusplus
extern "C"
{
#endif

	Node* Node_Create(u8 childCapacity, u16 id);
	Node* Node_Clone(Node* source, NodeFlagField cloningFlags, u16 id);
	Node* Node_AddChild(Node* parent, u8 childCapacity, u16 id, s16 contentId, Transform transform);

	void Node_SetEnabled(Node* node, NodeFlagField elements);
	void Node_SetDisabled(Node* node, NodeFlagField elements);
	void Node_SetTransform(Node* node, u16 id, Vector3 position, Vector3 rotationAngles, Vector3 scale);
	void Node_SetContent(Node* node, u16 id, s16 contentId);
	void Node_ApplyTransform(Node* node);

	// Graph navigation
	Node* Node_GetParent(Node* node);
	Node* Node_GetChildByIndex(Node* node, s16 index);
	Node* Node_FindChildById(Node* parent, u16 id);
	bool Node_IsLeaf(Node* node);
	bool Node_IsEmpty(Node* node);
	bool Node_IsRoot(Node* node);
	sizetype Node_GetChildCount(Node* node);
	Node* FindRoot(Node* node);


	/**
	 * @brief Starts walking the node graph recursively from given root node.
	 * @details The function will do a walk of the graph. It will modify the OpenGL matrix stack as it goes. When it reaches a node with a valid content id, it will call the callback function with that content id.
	 * @param root The root node. Can be any node in the graph
	 * @param contentCallback This callback is called with content id when node is processed.
	 */
	void Node_WalkGraph(Node* node, HandleContentIdCallback contentCallback);

	void Node_DebugDraw(Node* node, Menu* menu, short x, short y, u32 debugFlags);

	Vector3 Node_GetPosition(Node* node);
	bool Node_GetModelMatrix(Node* node, Matrix modelOut);
	bool Node_CalculatePosition ( Node* parent, Node* target, Matrix world, Vector3* posOut );
	bool Node_CalculateModelMatrix (Node* parent, Node* target, Matrix model );

	// Private functions
	Node* m_Node_FindChildByIndexRecursive(Node* parent, s16 index, s16* indexCounter);
	void m_DebugDrawNode(Node* node, Menu* menu, short depth, short* index, u32 drawFlags );


#ifdef __cplusplus
}
#endif
