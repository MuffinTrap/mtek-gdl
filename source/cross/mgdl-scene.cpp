#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-assert.h>

// TODO Move materials and meshes to AssetManager
DYNAMIC_ARRAY_IMPL(Mesh)
DYNAMIC_ARRAY_IMPL(Material)

void Scene_Init(Scene* scene)
{
	scene->rootNode = nullptr;
	scene->materials = DynamicArray_CreatePtrMaterial(4);
	scene->meshes = DynamicArray_CreatePtrMesh(4);
	scene->lights = nullptr;
}

Scene* Scene_CreateEmpty()
{
	Scene* scene = (Scene*)malloc(sizeof(Scene));
	Scene_Init(scene);
	return scene;
}


void Scene_AddChildNode (Scene* scene, Node* parent, Node* child )
{
	if (parent == nullptr)
	{
		scene->rootNode = child;
	}
	else
	{
		DynamicArray_AddPtrNode(parent->children, child);
	}
}

void Scene_DebugDraw(Scene* scene,  Menu* menu, short x, short y, u32 debugFlags )
{
	if (scene->rootNode != nullptr)
	{
		short index = 0;
		Menu_Start(menu, x, y, 100);
		Scene_DebugDrawNode_(scene->rootNode, menu, 0, &index, debugFlags);
	}
}

void Scene_DebugDrawNode_( Node* node, Menu* menu, short depth, short* index, u32 debugFlags)
{
	if (node == nullptr)
	{
		return;
	}
	short drawIndex = *index;
	if (strlen(node->name) > 0)
	{
		Menu_TextF(menu, "%d: %s", drawIndex, node->name);
	}
	else if (drawIndex == 0)
	{
		Menu_TextF(menu, "%d: %s", drawIndex, "ROOT");
	}
	if (Flag_IsSetAny(debugFlags, Scene_DebugFlag::Position))
	{
		Vector3 &p = node->transform->position;
		Menu_TextF(menu, "P(%.1f,%.1f,%.1f)", drawIndex, p.x, p.y, p.z);
	}
	// What does this node have?
	if (node->mesh != nullptr)
	{
		Menu_Text(menu, "- Mesh");
	}
	if (node->light != nullptr)
	{
		Menu_Text(menu, "- Light");
	}

	for(sizetype i = 0; i < DynamicArray_CountNode(node->children); i++)
	{
		drawIndex += 1;
		*index = drawIndex;
		Scene_DebugDrawNode_(DynamicArray_GetPtrNode(node->children, i), menu, depth+1, index, debugFlags);
	}
}

void Scene_Draw(Scene* scene)
{
	if (scene->rootNode != nullptr)
	{
		Scene_DrawNode(scene->rootNode);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void Scene_DrawNode ( Node* node )
{
	glPushMatrix();
		Node_Draw(node);
		if (Flag_IsSetAny(node->enabledElements, NodeChildren))
		{
			for(sizetype i = 0; i < DynamicArray_CountNode(node->children); i++)
			{
				Scene_DrawNode(DynamicArray_GetPtrNode(node->children, i));
			}
		}
	glPopMatrix();
}

void Scene_SetMaterialTexture (Scene* scene, const char* materialName, Texture* texture )
{
	Material* m = Scene_GetMaterial(scene, materialName);
	if (m != nullptr)
	{
		m->texture = texture;
	}
	else
	{
		Log_ErrorF("No material found with name %s\n", materialName);
		return;
	}
}

void Scene_SetAllMaterialTextures (Scene* scene, Texture* texture )
{
	for(sizetype i = 0; i < DynamicArray_CountMaterial(scene->materials); i++)
	{
		Material* m = DynamicArray_GetPtrMaterial(scene->materials, i);
		m->texture = texture;
	}
}

void Scene_AddMaterial ( Scene* scene, Material* material )
{
	DynamicArray_AddPtrMaterial(scene->materials, material);
}

Node* Scene_GetRootNode(Scene* scene )
{
	return scene->rootNode;
}


Vector3 Scene_GetNodePosition ( Scene* scene, Node* node )
{
	Matrix matrix = MatrixIdentity();
	Vector3 posOut;
	Scene_CalculateNodePosition(scene->rootNode, node, matrix, &posOut);

	return posOut;
}

bool Scene_GetNodeModelMatrix ( Scene* scene, Node* node, Matrix modelOut )
{
	modelOut = MatrixIdentity();
	return Scene_CalculateNodeModelMatrix(scene->rootNode, node, modelOut);
}


bool Scene_CalculateNodeModelMatrix (Node* parent, Node* target, Matrix model )
{
	Vector3 p = parent->transform->position;
	Vector3 rotationRad = Vector3Scale(parent->transform->rotationDegrees, DEG2RAD);
	model = MatrixMultiply(model, MatrixTranslate(p.x, p.y, p.z));
	model = MatrixMultiply(model, MatrixRotateXYZ(rotationRad));
	if (parent == target)
	{
		return true;
	}

	// Need to store the matrix at this state
	// so that every child starts from the same matrix
	for(sizetype i = 0; i < DynamicArray_CountNode(parent->children); i++)
	{
		Matrix accumulated =  model;
		if(Scene_CalculateNodeModelMatrix(DynamicArray_GetNode(parent->children, i),
			target, accumulated))
		{
			return true;
		}
	}
	return false;
}


bool Scene_CalculateNodePosition ( Node* parent, Node* target, Matrix world, Vector3* posOut )
{

	Vector3 p = parent->transform->position;
	Vector3 rotationRad = Vector3Scale(parent->transform->rotationDegrees, DEG2RAD);
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
	for(sizetype i = 0; i < DynamicArray_CountNode(parent->children); i++)
	{
		Matrix accumulated = world;
		if(Scene_CalculateNodePosition(DynamicArray_GetNode(parent->children, i), target, accumulated, posOut))
		{
			return true;
		}
	}
	return false;
}

Material* Scene_GetMaterial (Scene* scene, const char* materialName )
{
	mgdl_assert_print(materialName != nullptr, "Null material name");
	mgdl_assert_print(scene->materials != nullptr, "No materials array in scene");
	for(sizetype mi = 0; mi < DynamicArray_CountMaterial(scene->materials); mi++)
	{
		Material* m = DynamicArray_GetMaterial(scene->materials, mi);
		mgdl_assert_print(m != nullptr, "No material");
		mgdl_assert_print(m->name != nullptr, "No name on material");
		if (strcmp(m->name, materialName) == 0)
		{
			return m;
		}
	}
	return nullptr;
}

Material* Scene_FindMaterial ( Scene* scene, Node* node, const char* materialName )
{
	if (node->material != nullptr)
	{
		if (strcmp(node->material->name, materialName) == 0)
		{
			return node->material;
		}
	}
	Material* childMat = nullptr;
	for(sizetype i = 0; i < DynamicArray_CountNode(node->children); i++)
	{
		childMat = Scene_FindMaterial(scene, DynamicArray_GetNode(node->children, i), materialName);
		if (childMat != nullptr)
		{
			break;
		}
	}
	return childMat;
}
