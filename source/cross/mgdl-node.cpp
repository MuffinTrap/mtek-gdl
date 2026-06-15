#include <mgdl/mgdl-node.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-gui.h>

DYNAMIC_ARRAY_IMPL(Node)

static const s16 MGDL_EMPTY_CONTENT = -1;

Node* Node_Create(u8 childCapacity, u16 id)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->id = id;
	node->contentId = MGDL_EMPTY_CONTENT;
	node->flags = (NodeActive | NodeApplyTransform);
	node->transform = Transform_CreateZero();
	node->parent = nullptr;
	node->childNodes = DynamicArray_CreatePtrNode(childCapacity);

	return node;
}

Node* Node_AddChild(Node* parent, u8 childCapacity, u16 id, s16 contentId, Transform transform)
{
	if (parent)
	{
		Node* child = Node_Create(childCapacity, id);
		child->contentId = contentId;
		child->parent = parent;
		child->transform = transform;
		DynamicArray_AddPtrNode(parent->childNodes, child);
		return child;
	}
	return nullptr;
}

void Node_SetEnabled(Node* node, NodeFlagField elements)
{
	node->flags = Flag_SetAll(node->flags, elements);
}

void Node_SetDisabled(Node* node, NodeFlagField elements)
{
	node->flags = Flag_UnsetAll(node->flags, elements);
}


Node* Node_Clone(Node* source, NodeFlagField cloningFlags, u16 id)
{
	sizetype childAmount = DynamicArray_Count(source->childNodes);
	Node* clone = Node_Create(childAmount, id);
	clone->parent = source->parent;
	clone->transform = Transform_Clone(&source->transform);
	clone->contentId = source->contentId;

	if (Flag_IsSetAny(cloningFlags, NodeChildren))
	{
		for(sizetype i = 0; i < childAmount; i++)
		{
			Node* childNode = DynamicArray_GetPtrNode(source->childNodes, i);
			if (childNode != nullptr)
			{
				DynamicArray_AddPtrNode(clone->childNodes, Node_Clone(childNode, cloningFlags, id+i+1));
			}
		}
	}

	return clone;
}

void Node_SetTransform(Node* node, Vector3 position, Vector3 rotationAngles, Vector3 scale)
{
	node->transform = Transform_Create(position, rotationAngles, scale);
}


void Node_SetContent (Node* node, s16 contentId)
{
	node->contentId = contentId;
}

Node* Node_FindChildByIndex ( Node* parent, s16 index )
{
	s16 indexCounter = 0;
	return m_Node_FindChildByIndexRecursive(parent, index, &indexCounter);
}

Node* Node_FindChildByIndexRecursive (Node* parent, s16 index, s16* indexCounter )
{
	if ((*indexCounter) == index)
	{
		return parent;
	}
	for(sizetype i = 0; i < DynamicArray_Count(parent->childNodes); i++)
	{
		(*indexCounter) += 1;
		Node* childNode =  Node_FindChildByIndexRecursive(DynamicArray_GetNode(parent->childNodes, i), index, indexCounter);
		if (childNode != nullptr)
		{
			return childNode;
		}
	}
	return nullptr;
}

Node* Node_FindChildById (Node* node, u16 id)
{
	if (node->id == id)
	{
		return node;
	}
	for(sizetype i = 0; i < DynamicArray_Count(node->childNodes); i++)
	{
		Node* childNode = Node_FindChildById(DynamicArray_GetNode(node->childNodes, i), id);
		if (childNode != nullptr)
		{
			return childNode;
		}
	}
	return nullptr;
}

Node* Node_GetParent(Node* node)
{
	return node->parent;
}

void Node_ApplyTransform(Node* node)
{
	Transform_Apply(&node->transform);
}

void Node_WalkGraph(Node* node, HandleContentIdCallback contentCallback)
{
	if (Flag_IsSetAll(node->flags, NodeActive))
	{
		if (Flag_IsSetAll(node->flags, NodeApplyTransform))
		{
			glPushMatrix();
			Node_ApplyTransform(node);
		}
		if (node->contentId != MGDL_EMPTY_CONTENT)
		{
			contentCallback(node->contentId);
		}
		for (sizetype i = 0; i < DynamicArray_Count(node->childNodes); i++)
		{
			Node* child = DynamicArray_GetPtrNode(node->childNodes, i);
			Node_WalkGraph(child, contentCallback);
		}
		if (Flag_IsSetAll(node->flags, NodeApplyTransform))
		{
			glPopMatrix();
		}
	}
}


void Node_DebugDraw(Node* node,  Menu* menu, short x, short y, u32 debugFlags )
{
	short index = 0;
	Menu_Start(menu, x, y, 100);
	m_DebugDrawNode(node, menu, 0, &index, debugFlags);
}

void m_DebugDrawNode( Node* node, Menu* menu, short depth, short* index, u32 debugFlags)
{
	if (node == nullptr)
	{
		return;
	}
	short drawIndex = *index;
		Menu_TextF(menu, "%d: %d", drawIndex, node->id);

	if (Flag_IsSetAny(debugFlags, Node_DebugFlag::Position))
	{
		Vector3 &p = node->transform.position;
		Menu_TextF(menu, "P(%.1f,%.1f,%.1f)", drawIndex, p.x, p.y, p.z);
	}
	// What does this node have?
	if (node->contentId != MGDL_EMPTY_CONTENT)
	{
		Menu_TextF(menu, "%d", node->contentId);
	}

	for(sizetype i = 0; i < DynamicArray_Count(node->childNodes); i++)
	{
		drawIndex += 1;
		*index = drawIndex;
		m_DebugDrawNode(DynamicArray_GetPtrNode(node->childNodes, i), menu, depth+1, index, debugFlags);
	}
}

Node* FindRoot(Node* node)
{
	while(true)
	{
		Node* parent = node->parent;
		if (parent == nullptr)
		{
			return node;
		}
		else
		{
			node = parent;
		}
	}
}

Vector3 Node_GetPosition ( Node* node )
{
	Matrix matrix = MatrixIdentity();
	Vector3 posOut;
	Node* root = FindRoot(node);
	Node_CalculatePosition(root, node, matrix, &posOut);

	return posOut;
}

bool Node_GetNodeModelMatrix ( Node* node, Matrix modelOut )
{
	modelOut = MatrixIdentity();
	Node* root = FindRoot(node);
	return Node_CalculateModelMatrix(root, node, modelOut);
}

bool Node_CalculateModelMatrix (Node* parent, Node* target, Matrix model )
{
	Vector3 p = parent->transform.position;
	Vector3 rotationRad = Vector3Scale(parent->transform.rotationDegrees, DEG2RAD);
	model = MatrixMultiply(model, MatrixTranslate(p.x, p.y, p.z));
	model = MatrixMultiply(model, MatrixRotateXYZ(rotationRad));
	if (parent == target)
	{
		return true;
	}

	// Need to store the matrix at this state
	// so that every child starts from the same matrix
	for(sizetype i = 0; i < DynamicArray_Count(parent->childNodes); i++)
	{
		Matrix accumulated =  model;
		if(Node_CalculateModelMatrix(
			DynamicArray_GetNode(parent->childNodes, i), // New parent
			target,
			accumulated))
		{
			return true;
		}
	}
	return false;
}


bool Node_CalculatePosition ( Node* parent, Node* target, Matrix world, Vector3* posOut )
{

	Vector3 p = parent->transform.position;
	Vector3 rotationRad = Vector3Scale(parent->transform.rotationDegrees, DEG2RAD);
	world = MatrixMultiply(world, MatrixTranslate(p.x, p.y, p.z));
	world = MatrixMultiply(world, MatrixRotateXYZ(rotationRad));
	if (parent == target)
	{
		Vector3 origo = Vector3Zero();
		*posOut = Vector3Transform(origo, world);
		return true;
	}

	// Need to store the matrix at this state
	// so that every child starts from the same matrix
	for(sizetype i = 0; i < DynamicArray_Count(parent->childNodes); i++)
	{
		Matrix accumulated = world;
		if(Node_CalculatePosition(DynamicArray_GetNode(parent->childNodes, i), target, accumulated, posOut))
		{
			return true;
		}
	}
	return false;
}

bool Node_IsRoot(Node* node){
	return node->parent == nullptr;
}
bool Node_IsLeaf(Node* node){
	return DynamicArray_Count(node->childNodes) == 0;
}
bool Node_IsEmpty(Node* node) {
	return node->contentId == MGDL_EMPTY_CONTENT;
}
sizetype Node_GetChildCount(Node* node) {
	return DynamicArray_Count(node->childNodes);
}
