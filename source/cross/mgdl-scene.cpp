#include <mgdl/mgdl-scene.h>
#include <glm/gtc/matrix_transform.hpp>
gdl::Scene::Scene()
{
	parent = nullptr;
	rootNode = nullptr;
}

void gdl::Scene::SetActiveParentNode(gdl::Node* node)
{
	parent = node;
}

void gdl::Scene::PushChildNode ( gdl::Node* node )
{
	if (parent != nullptr)
	{
		parent->children.push_back(node);
	}
	else
	{
		node->name = "ROOT";
		rootNode = node;
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
		gdl::vec3 &p = node->transform.position;
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
	const gdl::vec3& t = node->transform.position;
	glPushMatrix();
		glTranslatef(t.x, t.y, t.z);

		const gdl::vec3& r = node->transform.rotationDegrees;
		glRotatef(r.x, 1.0f, 0.0f, 0.0f);
		glRotatef(r.y, 0.0f, 1.0f, 0.0f);
		glRotatef(r.z, 0.0f, 0.0f, 1.0f);

		const gdl::vec3& s = node->transform.scale;
		glScalef(s.x, s.y, s.z);

		gdl::Mesh* m = node->mesh;
		if (m != nullptr)
		{
			if (node->material != nullptr)
			{
				// TODO do not reapply same material
				node->material->Apply();
			}
			m->DrawElements();
		}
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

void gdl::Scene::AddMaterial ( gdl::Material* material )
{
	materials.push_back(material);
}

void gdl::Scene::AddMesh ( gdl::Mesh* mesh )
{
	meshes.push_back(mesh);
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

gdl::vec3 gdl::Scene::GetWorldPosition ( gdl::Node* node )
{
	glm::mat4 matrix = glm::mat4(1.0f);
	gdl::vec3 posOut;
	CalculateWorldPosition(rootNode, node, matrix, posOut);
	return posOut;
}

glm::mat4 gdl::Scene::GetModelMatrix ( gdl::Node* node )
{
	glm::mat4 matrix = glm::mat4(1.0f);
	CalculateModelMatrix(rootNode, node, matrix);
	return matrix;
}


bool gdl::Scene::CalculateModelMatrix ( gdl::Node* parent, gdl::Node* target, glm::mat4& matrixRef )
{
	gdl::vec3 p = parent->transform.position;
	matrixRef = glm::translate(matrixRef, glm::vec3(p.x, p.y, p.z));
	matrixRef = glm::rotate(matrixRef, glm::radians(parent->transform.rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrixRef = glm::rotate(matrixRef, glm::radians(parent->transform.rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrixRef = glm::rotate(matrixRef, glm::radians(parent->transform.rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0f));
	if (parent == target)
	{
		return true;
	}

	// Need to store the matrix at this state
	// so that every child starts from the same matrix
	for(size_t i = 0; i < parent->children.size(); i++)
	{
		glm::mat4 accumulated = matrixRef;
		if(CalculateModelMatrix(parent->children[i], target, accumulated))
		{
			return true;
		}
	}
	return false;

}


bool gdl::Scene::CalculateWorldPosition ( gdl::Node* parent, gdl::Node* target, glm::mat4& matrix, gdl::vec3& posOut )
{

	gdl::vec3 p = parent->transform.position;
	matrix = glm::translate(matrix, glm::vec3(p.x, p.y, p.z));
	matrix = glm::rotate(matrix, glm::radians(parent->transform.rotationDegrees.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(parent->transform.rotationDegrees.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(parent->transform.rotationDegrees.z), glm::vec3(0.0f, 0.0f, 1.0f));
	if (parent == target)
	{
		glm::vec4 origo = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 pos = matrix * origo;
		posOut = gdl::vec3(pos.x, pos.y, pos.z);
		return true;
	}

	// Need to store the matrix at this state
	// so that every child starts from the same matrix
	for(size_t i = 0; i < parent->children.size(); i++)
	{
		glm::mat4 accumulated = matrix;
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

gdl::Mesh * gdl::Scene::GetMeshByUniqueId ( uint32_t uniqueId )
{
	for(size_t mi = 0; mi < meshes.size(); mi++)
	{
		if (meshes[mi]->uniqueId == uniqueId)
		{
			return meshes[mi];
		}
	}
	return nullptr;
}


gdl::Mesh * gdl::Scene::GetMesh ( const std::string& meshName )
{
	for(size_t mi = 0; mi < meshes.size(); mi++)
	{
		if (meshes[mi]->name.compare(meshName) == 0)
		{
			return meshes[mi];
		}
	}
	return nullptr;
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
