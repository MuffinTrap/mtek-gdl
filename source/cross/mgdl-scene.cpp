#include <mgdl/mgdl-scene.h>

void gdl::Scene::SetActiveParentNode(gdl::Node* node)
{
	parent = node;
}

void gdl::Scene::PushChildNode ( gdl::Node* node )
{
	if (parent != nullptr)
	{
		printf("Added child node\n");
		parent->children.push_back(node);
	}
	else
	{
		printf("Set root node\n");
		rootNode = node;
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
	gdl::vec3 t = node->transform.position;
	glPushMatrix();
		glTranslatef(t.x, t.y, t.z);

		gdl::vec3 r = node->transform.rotationRadians;
		glRotatef(r.x, 1.0f, 0.0f, 0.0f);
		glRotatef(r.y, 0.0f, 1.0f, 0.0f);
		glRotatef(r.z, 0.0f, 0.0f, 1.0f);

		float s = node->transform.scale;
		glScalef(s, s, s);

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






