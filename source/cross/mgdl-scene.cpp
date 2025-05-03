#include <mgdl/mgdl-scene.h>
#include <glm/gtc/matrix_transform.hpp>
void Scene_Init(Scene* scene)
{
	scene->rootNode = nullptr;
}


void Scene_AddChildNode (Scene* scene, Node* parent, Node* child )
{
	if (parent == nullptr)
	{
		scene->rootNode = child;
	}
	else
	{
		parent->children.push_back(child);
	}
}

void Scene_DebugDraw(Scene* scene,  Menu* menu, u32 debugFlags )
{
	if (scene->rootNode != nullptr)
	{
		short index = 0;
		Menu_SetActive(menu);
		_Scene_DebugDrawNode(scene->rootNode, 0, &index, debugFlags);
	}
}

void _Scene_DebugDrawNode ( Node* node, short depth, short* index, u32 debugFlags)
{
	Menu_TextF("%d: %s", index, node->name);
	*index = *index + 1;
	if ((debugFlags & Scene_DebugFlag::Position) > 0)
	{
		vec3 &p = node->transform->position;
		Menu_TextF("P(%.1f,%.1f,%.1f)", index, p.x, p.y, p.z);
	}

	for(size_t i = 0; i < node->children.size(); i++)
	{
		_Scene_DebugDrawNode(node->children[i], depth+1, index, debugFlags);
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
		for(size_t i = 0; i < node->children.size(); i++)
		{
			Scene_DrawNode(node->children[i]);
		}
	glPopMatrix();
}

void Scene_SetMaterialTexture (Scene* scene, const char* materialName, Image* texture )
{
	Material* m = Scene_GetMaterial(scene, materialName);
	if (m != nullptr)
	{
		m->texture = texture;
	}
	else
	{
		printf("No material found with name %s\n", materialName);
		return;
	}
}

void Scene_SetAllMaterialTextures (Scene* scene, Image* texture )
{
	for(unsigned long i = 0; i < scene->materials.size(); i++)
	{
		scene->materials[i]->texture = texture;
	}
}

void Scene_AddMaterial ( Scene* scene, Material* material )
{
	scene->materials.push_back(material);
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

Node* Scene_FindChildNodeByIndex (Node* parent, short targetIndex, short& index )
{
	if (index == targetIndex)
	{
		return parent;
	}
	index++;
	Node* childNode = nullptr;
	for(size_t i = 0; i < parent->children.size(); i++)
	{
		childNode = Scene_FindChildNodeByIndex(parent->children[i], targetIndex, index);
		if (childNode != nullptr)
		{
			break;
		}
	}
	return childNode;
}

vec3 Scene_GetNodePosition ( Scene* scene, Node* node )
{
	mat4x4 matrix;
	mat4x4Identity(matrix);
	vec3 posOut;
	Scene_CalculateNodePosition(scene->rootNode, node, matrix, posOut);

	return posOut;
}

bool Scene_GetNodeModelMatrix ( Scene* scene, Node* node, mat4x4 modelOut )
{
	mat4x4Identity(modelOut);
	return Scene_CalculateNodeModelMatrix(scene->rootNode, node, modelOut);
}


bool Scene_CalculateNodeModelMatrix (Node* parent, Node* target, mat4x4 model )
{
	vec3 p = parent->transform->position;
	mat4x4Translate(model, vec3New(p.x, p.y, p.z));
	mat4x4RotateX(model, glm::radians(parent->transform->rotationDegrees.x));
	mat4x4RotateY(model, glm::radians(parent->transform->rotationDegrees.y));
	mat4x4RotateZ(model, glm::radians(parent->transform->rotationDegrees.z));
	if (parent == target)
	{
		return true;
	}

	// Need to store the matrix at this state
	// so that every child starts from the same matrix
	for(size_t i = 0; i < parent->children.size(); i++)
	{
		mat4x4 accumulated;
		mat4x4Copy(accumulated, model);
		if(Scene_CalculateNodeModelMatrix(parent->children[i], target, accumulated))
		{
			return true;
		}
	}
	return false;
}


bool Scene_CalculateNodePosition ( Node* parent, Node* target, mat4x4 world, vec3& posOut )
{

	vec3 p = parent->transform->position;
	mat4x4Translate(world, vec3New(p.x, p.y, p.z));
	mat4x4RotateX(world, glm::radians(parent->transform->rotationDegrees.x));
	mat4x4RotateY(world, glm::radians(parent->transform->rotationDegrees.y));
	mat4x4RotateZ(world, glm::radians(parent->transform->rotationDegrees.z));
	if (parent == target)
	{
		vec4 origo = vec4New(0.0f, 0.0f, 0.0f, 1.0f);
		vec4 pos = mat4x4MultiplyVector(world, origo);
		posOut = vec3New(pos.x, pos.y, pos.z);
		return true;
	}

	// Need to store the matrix at this state
	// so that every child starts from the same matrix
	for(size_t i = 0; i < parent->children.size(); i++)
	{
		mat4x4 accumulated;
		mat4x4Copy(accumulated, world);
		if(Scene_CalculateNodePosition(parent->children[i], target, accumulated, posOut))
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
	for(size_t i = 0; i < node->children.size(); i++)
	{
		childNode = Scene_FindChildNode(node->children[i], nodeName);
		if (childNode != nullptr)
		{
			break;
		}
	}
	return childNode;
}

Material* Scene_GetMaterial (Scene* scene, const char* materialName )
{
	for(size_t mi = 0; mi < scene->materials.size(); mi++)
	{
		if (strcmp(scene->materials[mi]->name, materialName) == 0)
		{
			return scene->materials[mi];
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
	for(size_t i = 0; i < node->children.size(); i++)
	{
		childMat = Scene_FindMaterial(scene, node->children[i], materialName);
		if (childMat != nullptr)
		{
			break;
		}
	}
	return childMat;
}
