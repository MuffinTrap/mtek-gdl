#include <mgdl/mgdl-node.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

DYNAMIC_ARRAY_IMPL(Node)

Node* Node_Create(u8 childCapacity)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->transform = Transform_CreateZero();
	node->mesh = nullptr;
	node->material = nullptr;
	node->light = nullptr;
	node->name = zstr_init();
	node->enabledElements = (NodeTransform |NodeMesh| NodeMaterial| NodeLight| NodeChildren);
	node->children = DynamicArray_CreatePtrNode(childCapacity);

	return node;
}

void Node_SetEnabled(Node* node, NodeFlagField elements)
{
	node->enabledElements = Flag_SetAll(node->enabledElements, elements);
}

void Node_SetDisabled(Node* node, NodeFlagField elements)
{
	node->enabledElements = Flag_UnsetAll(node->enabledElements, elements);
}

void Node_EnableDrawing(Node* node)
{
	Node_SetEnabled(node, NodeMesh|NodeMaterial);

}
void Node_DisableDrawing(Node* node)
{
	Node_SetDisabled(node, NodeMesh|NodeMaterial);
}

Node* Node_Clone(Node* source, NodeFlagField cloningFlags)
{
	Node* clone = nullptr;
	sizetype childAmount = 1;
	if (Flag_IsSetAny(cloningFlags, NodeChildren))
	{
		sizetype childCount = DynamicArray_Count(source->children);
		if (childCount > 1)
		{
			childAmount = childCount;
		}
	}
	clone = Node_Create(childAmount);

	if (Flag_IsSetAny(cloningFlags, NodeChildren))
	{
		for(sizetype i = 0; i < DynamicArray_Count(source->children); i++)
		{
			Node* childNode = DynamicArray_GetPtrNode(source->children, i);
			if (childNode != nullptr)
			{
				DynamicArray_AddPtrNode(clone->children, Node_Clone(childNode, cloningFlags));
			}
		}
	}

	if (Flag_IsSetAny(cloningFlags, NodeTransform))
	{
		clone->transform = Transform_Clone(source->transform);
	}
	else
	{
		clone->transform = source->transform;
	}

	clone->mesh = source->mesh;
	clone->material = source->material;
	clone->light = source->light;
	clone->name = zstr_dup(&source->name);
	return clone;
}

void Node_SetTransform(Node* node, const char* name, Vector3 position, Vector3 rotationAngles)
{
	node->name = zstr_from(name);
	node->transform = Transform_Create(position, rotationAngles, Vector3New(1,1,1));
}


void Node_SetContent (Node* node, const char* name, Mesh* meshParam, Material* materialParam )
{
	node->transform = Transform_CreateZero();
	node->name = zstr_from(name);
	node->mesh = meshParam;
	node->material = materialParam;
}

Node* Node_FindChildByIndex ( Node* parent, short index )
{
	short indexCounter = 0;
	return Node_FindChildByIndexRecursive_(parent, index, &indexCounter);
}

Node* Node_FindChildByIndexRecursive_ (Node* parent, short index, short* indexCounter )
{
	if ((*indexCounter) == index)
	{
		return parent;
	}
	for(sizetype i = 0; i < DynamicArray_Count(parent->children); i++)
	{
		(*indexCounter) += 1;
		Node* childNode =  Node_FindChildByIndexRecursive_(DynamicArray_GetNode(parent->children, i), index, indexCounter);
		if (childNode != nullptr)
		{
			return childNode;
		}
	}
	return nullptr;
}

Node* Node_FindChildByName (Node* node, const char* nodeName )
{
	zstr_view myname = zstr_as_view(&node->name);
	if (zstr_view_eq(myname, nodeName) == 0)
	{
		return node;
	}
	for(sizetype i = 0; i < DynamicArray_Count(node->children); i++)
	{
		Node* childNode = Node_FindChildByName(DynamicArray_GetNode(node->children, i), nodeName);
		if (childNode != nullptr)
		{
			return childNode;
		}
	}
	return nullptr;
}

void Node_ApplyTransform(Node* node)
{
	Transform_Apply(node->transform);
}

void Node_Draw(Node* node)
{
	if (Flag_IsSetAny(node->enabledElements, NodeTransform) )
	{
		Transform_Apply(node->transform);
	}

	Mesh* m = node->mesh;
	if (m != nullptr && Flag_IsSetAny(node->enabledElements, NodeMesh))
	{
		if (node->material != nullptr && Flag_IsSetAny(node->enabledElements, NodeMaterial))
		{
			// TODO do not reapply same material
			Material_Apply(node->material);
			if (node->material->type == MaterialType::Matcap)
			{
				// If material is matcap material
				GLfloat modelViewMatrix[16];
				glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
				Matrix modelView = MatrixFromGL(modelViewMatrix);
				// Copy the values
				// TODO

				// Different ways
				// Normal
				// transposed

				// Destination matrix is on the left, source on right
				Matrix inverseView = MatrixInvert(modelView);
				Matrix normalMatrix = MatrixTranspose(inverseView);

				Mesh_CalculateMatcapUVs(m, modelView, normalMatrix);
			}
		}
		Mesh_DrawElements(m);
	}
}
