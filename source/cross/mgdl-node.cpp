#include <mgdl/mgdl-node.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

Node* Node_Create(u8 childCapacity)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->transform = Transform_CreateZero();
	node->mesh = nullptr;
	node->material = nullptr;
	node->light = nullptr;
	node->name = nullptr;
	node->enabledElements = (NodeTransform |NodeMesh| NodeMaterial| NodeLight| NodeChildren);
	node->children = DynamicArray_CreateNode(childCapacity);

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
	if (Flag_IsSet(cloningFlags, NodeChildren))
	{
		sizetype childCount = DynamicArray_CountNode(source->children);
		if (childCount > 1)
		{
			childAmount = childCount;
		}
	}
	clone = Node_Create(childAmount);

	if (Flag_IsSet(cloningFlags, NodeChildren))
	{
		for(sizetype i = 0; i < DynamicArray_CountNode(source->children); i++)
		{
			Node* childNode = DynamicArray_GetNode(source->children, i);
			if (childNode != nullptr)
			{
				DynamicArray_AddNode(clone->children, Node_Clone(childNode, cloningFlags));
			}
		}
	}

	if (Flag_IsSet(cloningFlags, NodeTransform))
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
	clone->name = source->name;
	return clone;
}

void Node_SetTransform(Node* node, const char* name, V3f position, V3f rotationAngles)
{
	node->name = name;
	node->transform = Transform_Create(position, rotationAngles, V3f_Create(1,1,1));
}


void Node_SetContent (Node* node, const char* name, Mesh* meshParam, Material* materialParam )
{
	node->transform = Transform_CreateZero();
	node->name = name;
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
	for(sizetype i = 0; i < DynamicArray_CountNode(parent->children); i++)
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
	if (strcmp(node->name, nodeName) == 0)
	{
		return node;
	}
	for(sizetype i = 0; i < DynamicArray_CountNode(node->children); i++)
	{
		Node* childNode = Node_FindChildByName(DynamicArray_GetNode(node->children, i), nodeName);
		if (childNode != nullptr)
		{
			return childNode;
		}
	}
	return nullptr;
}

void Node_Draw(Node* node)
{
	if (Flag_IsSet(node->enabledElements, NodeTransform) )
	{
		const V3f t = node->transform->position;
		glTranslatef(t.x, t.y, t.z);

		const V3f r = node->transform->rotationDegrees;
		glRotatef(r.x, 1.0f, 0.0f, 0.0f);
		glRotatef(r.y, 0.0f, 1.0f, 0.0f);
		glRotatef(r.z, 0.0f, 0.0f, 1.0f);

		const V3f& s = node->transform->scale;
		glScalef(s.x, s.y, s.z);
	}

	Mesh* m = node->mesh;
	if (m != nullptr && Flag_IsSet(node->enabledElements, NodeMesh))
	{
		if (node->material != nullptr && Flag_IsSet(node->enabledElements, NodeMaterial))
		{
			// TODO do not reapply same material
			Material_Apply(node->material);
			if (node->material->type == MaterialType::Matcap)
			{
				// If material is matcap material
				float modelViewMatrix[16];
				glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
				mat4x4 modelView;
				mat4x4Identity(modelView);
				// Copy the values

				// Different ways
				// Normal
				// transposed

				for (int row = 0; row < 4; row++)
				{
					for (int col = 0; col < 4; col++)
					{
						modelView[row][col] = modelViewMatrix[row * 4 + col];
					}
				}
				mat4x4 inverseView;
				mat4x4 normalMatrix;
				mat4x4Identity(inverseView);
				mat4x4Identity(normalMatrix);

				// Target is on the left
				mat4x4Inverse(inverseView, modelView);
				mat4x4Transpose(normalMatrix, inverseView);

				Mesh_CalculateMatcapUVs(m, modelView, normalMatrix);
			}
		}
		Mesh_DrawElements(m);
	}
}
