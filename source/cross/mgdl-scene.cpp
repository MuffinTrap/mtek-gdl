#include <mgdl/mgdl-scene.h>
#include <glm/gtc/matrix_transform.hpp>
gdl::Scene::Scene()
{
	rootNode = nullptr;
}


void gdl::Scene::AddChildNode (gdl::Node* parent, gdl::Node* child )
{
	if (parent == nullptr)
	{
		rootNode = child;
	}
	else
	{
		parent->children.push_back(child);
	}
}

void gdl::Scene::DebugDraw( gdl::Font* font, short x, short y, u32 debugFlags )
{
	if (rootNode != nullptr)
	{
		short index = 0;
		DebugDrawNode(rootNode, font, x, y, 0, index, debugFlags);
	}
}

void gdl::Scene::DebugDrawNode ( gdl::Node* node, gdl::Font* font, short x, short& dy, short depth, short& index, u32 debugFlags)
{
	font->Printf(gdl::Colors::White, x + depth*16, dy, 16.0f, gdl::LJustify, gdl::LJustify, "%d: %s", index, node->name.c_str());
	index++;
	dy -= 18;
	if ((debugFlags & DebugFlag::Position) > 0)
	{
		vec3 &p = node->transform.position;
		font->Printf(gdl::Colors::White, x + depth*16, dy, 16.0f, gdl::LJustify, gdl::LJustify, "P(%.1f,%.1f,%.1f)", index, p.x, p.y, p.z);
		dy -= 18;
	}

	for(size_t i = 0; i < node->children.size(); i++)
	{
		DebugDrawNode(node->children[i], font, x, dy, depth+1, index, debugFlags);
	}
}

void gdl::Scene::Draw()
{
	if (rootNode != nullptr)
	{
		DrawNode(rootNode);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void gdl::Scene::DrawNode ( gdl::Node* node )
{
	glPushMatrix();
		node->Draw();
		for(size_t i = 0; i < node->children.size(); i++)
		{
			DrawNode(node->children[i]);
		}
	glPopMatrix();
}

void gdl::Scene::SetMaterialTexture (const std::string& materialName, gdl::Image* texture )
{
	gdl::Material* m = GetMaterial(materialName);
	if (m != nullptr)
	{
		m->texture = texture;
	}
	else
	{
		printf("No material found with name %s\n", materialName.c_str());
		return;
	}
}

void gdl::Scene::SetAllMaterialTextures (gdl::Image* texture )
{
	for(unsigned long i = 0; i < materials.size(); i++)
	{
		materials[i]->texture = texture;
	}
}

void gdl::Scene::AddMaterial ( gdl::Material* material )
{
	materials.push_back(material);
}

gdl::Node * gdl::Scene::GetRootNode()
{
	return rootNode;
}

gdl::Node * gdl::Scene::GetNodeByIndex ( short targetIndex )
{
	short index = 0;
	return FindNodeByIndex(rootNode, targetIndex, index);
}

gdl::Node * gdl::Scene::FindNodeByIndex ( gdl::Node* parent, short targetIndex, short& index )
{
	if (index == targetIndex)
	{
		return parent;
	}
	index++;
	gdl::Node* childNode = nullptr;
	for(size_t i = 0; i < parent->children.size(); i++)
	{
		childNode = FindNodeByIndex(parent->children[i], targetIndex, index);
		if (childNode != nullptr)
		{
			break;
		}
	}
	return childNode;
}

vec3 gdl::Scene::GetWorldPosition ( gdl::Node* node )
{
	mat4x4 matrix;
	mat4x4Identity(matrix);
	vec3 posOut;
	CalculateWorldPosition(rootNode, node, matrix, posOut);
	return posOut;
}

bool gdl::Scene::GetModelMatrix ( gdl::Node* node, mat4x4 modelOut )
{
	mat4x4Identity(modelOut);
	return CalculateModelMatrix(rootNode, node, modelOut);
}


bool gdl::Scene::CalculateModelMatrix ( gdl::Node* parent, gdl::Node* target, mat4x4 model )
{
	vec3 p = parent->transform.position;
	mat4x4Translate(model, vec3New(p.x, p.y, p.z));
	mat4x4RotateX(model, glm::radians(parent->transform.rotationDegrees.x));
	mat4x4RotateY(model, glm::radians(parent->transform.rotationDegrees.y));
	mat4x4RotateZ(model, glm::radians(parent->transform.rotationDegrees.z));
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
		if(CalculateModelMatrix(parent->children[i], target, accumulated))
		{
			return true;
		}
	}
	return false;
}


bool gdl::Scene::CalculateWorldPosition ( gdl::Node* parent, gdl::Node* target, mat4x4 world, vec3& posOut )
{

	vec3 p = parent->transform.position;
	mat4x4Translate(world, vec3New(p.x, p.y, p.z));
	mat4x4RotateX(world, glm::radians(parent->transform.rotationDegrees.x));
	mat4x4RotateY(world, glm::radians(parent->transform.rotationDegrees.y));
	mat4x4RotateZ(world, glm::radians(parent->transform.rotationDegrees.z));
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
		if(CalculateWorldPosition(parent->children[i], target, accumulated, posOut))
		{
			return true;
		}
	}
	return false;
}

gdl::Node * gdl::Scene::GetNode (const std::string& name )
{
	return FindNode(rootNode, name);
}

gdl::Node * gdl::Scene::FindNode (gdl::Node* node, const std::string& nodeName )
{
	if (node->name.compare(nodeName) == 0)
	{
		return node;
	}
	gdl::Node* childNode = nullptr;
	for(size_t i = 0; i < node->children.size(); i++)
	{
		childNode = FindNode(node->children[i], nodeName);
		if (childNode != nullptr)
		{
			break;
		}
	}
	return childNode;
}

gdl::Material * gdl::Scene::GetMaterial (const std::string& materialName )
{
	for(size_t mi = 0; mi < materials.size(); mi++)
	{
		if (materials[mi]->name.compare(materialName) == 0)
		{
			return materials[mi];
		}
	}
	return nullptr;
}

gdl::Material * gdl::Scene::FindMaterial ( gdl::Node* node, const std::string& materialName )
{
	if (node->material != nullptr)
	{
		if (node->material->name.compare(materialName) == 0)
		{
			return node->material;
		}
	}
	gdl::Material* childMat = nullptr;
	for(size_t i = 0; i < node->children.size(); i++)
	{
		childMat = FindMaterial(node->children[i], materialName);
		if (childMat != nullptr)
		{
			break;
		}
	}
	return childMat;
}
