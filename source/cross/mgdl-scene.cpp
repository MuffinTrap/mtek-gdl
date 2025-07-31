#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-assert.h>

void Scene_Init(Scene* scene)
{
	scene->rootNode = nullptr;
	scene->materials = DynamicArray_CreateMaterial(4);
	scene->meshes = DynamicArray_CreateMesh(4);
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
		DynamicArray_AddNode(parent->children, child);
	}
}

void Scene_DebugDraw(Scene* scene,  Menu* menu, short x, short y, u32 debugFlags )
{
	if (scene->rootNode != nullptr)
	{
		short index = 0;
		Menu_SetActive(menu);
		Menu_Start(x, y, 100);
		_Scene_DebugDrawNode(scene->rootNode, 0, &index, debugFlags);
	}
}

void _Scene_DebugDrawNode ( Node* node, short depth, short* index, u32 debugFlags)
{
	if (node == nullptr)
	{
		return;
	}
	short drawIndex = *index;
	Menu_TextF("%d: %s", drawIndex, node->name);
	if ((debugFlags & Scene_DebugFlag::Position) > 0)
	{
		V3f &p = node->transform->position;
		Menu_TextF("P(%.1f,%.1f,%.1f)", drawIndex, p.x, p.y, p.z);
	}

	for(sizetype i = 0; i < DynamicArray_CountNode(node->children); i++)
	{
		drawIndex += 1;
		*index = drawIndex;
		_Scene_DebugDrawNode(DynamicArray_GetNode(node->children, i), depth+1, index, debugFlags);
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
		for(sizetype i = 0; i < DynamicArray_CountNode(node->children); i++)
		{
			Scene_DrawNode(DynamicArray_GetNode(node->children, i));
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
		Material* m = DynamicArray_GetMaterial(scene->materials, i);
		m->texture = texture;
	}
}

void Scene_AddMaterial ( Scene* scene, Material* material )
{
	DynamicArray_AddMaterial(scene->materials, material);
}

Node* Scene_GetRootNode(Scene* scene )
{
	return scene->rootNode;
}

Node* Scene_GetNodeByIndex ( Scene* scene, short targetIndex )
{
	short index = 0;
	return Scene_FindChildNodeByIndex(scene->rootNode, targetIndex, index);
}

Node* Scene_FindChildNodeByIndex (Node* parent, short targetIndex, short index )
{
	if (index == targetIndex)
	{
		return parent;
	}
	index++;
	Node* childNode = nullptr;
	for(sizetype i = 0; i < DynamicArray_CountNode(parent->children); i++)
	{
		childNode = Scene_FindChildNodeByIndex(DynamicArray_GetNode(parent->children, i), targetIndex, index);
		if (childNode != nullptr)
		{
			break;
		}
	}
	return childNode;
}

V3f Scene_GetNodePosition ( Scene* scene, Node* node )
{
	mat4x4 matrix;
	mat4x4Identity(matrix);
	V3f posOut;
	Scene_CalculateNodePosition(scene->rootNode, node, matrix, &posOut);

	return posOut;
}

bool Scene_GetNodeModelMatrix ( Scene* scene, Node* node, mat4x4 modelOut )
{
	mat4x4Identity(modelOut);
	return Scene_CalculateNodeModelMatrix(scene->rootNode, node, modelOut);
}


bool Scene_CalculateNodeModelMatrix (Node* parent, Node* target, mat4x4 model )
{
	V3f p = parent->transform->position;
	mat4x4Translate(model, V3f_Create(p.x, p.y, p.z));
	mat4x4RotateX(model, Deg2Rad(parent->transform->rotationDegrees.x));
	mat4x4RotateY(model, Deg2Rad(parent->transform->rotationDegrees.y));
	mat4x4RotateZ(model, Deg2Rad(parent->transform->rotationDegrees.z));
	if (parent == target)
	{
		return true;
	}

	// Need to store the matrix at this state
	// so that every child starts from the same matrix
	for(sizetype i = 0; i < DynamicArray_CountNode(parent->children); i++)
	{
		mat4x4 accumulated;
		mat4x4Copy(accumulated, model);
		if(Scene_CalculateNodeModelMatrix(DynamicArray_GetNode(parent->children, i),
			target, accumulated))
		{
			return true;
		}
	}
	return false;
}


bool Scene_CalculateNodePosition ( Node* parent, Node* target, mat4x4 world, V3f* posOut )
{

	V3f p = parent->transform->position;
	mat4x4Translate(world, V3f_Create(p.x, p.y, p.z));
	mat4x4RotateX(world, Deg2Rad(parent->transform->rotationDegrees.x));
	mat4x4RotateY(world, Deg2Rad(parent->transform->rotationDegrees.y));
	mat4x4RotateZ(world, Deg2Rad(parent->transform->rotationDegrees.z));
	if (parent == target)
	{
		vec4 origo = V4f_Create(0.0f, 0.0f, 0.0f, 1.0f);
		vec4 pos = mat4x4MultiplyVector(world, origo);
		*posOut = V3f_Create(pos.x, pos.y, pos.z);
		return true;
	}

	// Need to store the matrix at this state
	// so that every child starts from the same matrix
	for(sizetype i = 0; i < DynamicArray_CountNode(parent->children); i++)
	{
		mat4x4 accumulated;
		mat4x4Copy(accumulated, world);
		if(Scene_CalculateNodePosition(DynamicArray_GetNode(parent->children, i), target, accumulated, posOut))
		{
			return true;
		}
	}
	return false;
}

Node* Scene_GetNode (Scene* scene, const char* name )
{
	return Scene_FindChildNode(scene->rootNode, name);
}

Node* Scene_FindChildNode (Node* node, const char* nodeName )
{
	if (strcmp(node->name, nodeName) == 0)
	{
		return node;
	}
	Node* childNode = nullptr;
	for(sizetype i = 0; i < DynamicArray_CountNode(node->children); i++)
	{
		childNode = Scene_FindChildNode(DynamicArray_GetNode(node->children, i), nodeName);
		if (childNode != nullptr)
		{
			break;
		}
	}
	return childNode;
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
