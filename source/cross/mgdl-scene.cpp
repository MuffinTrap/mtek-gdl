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

void gdl::Scene::Draw(gdl::Image* texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
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

		// TODO gdl::vec3 r = node->transform.rotationRadians;

		gdl::Mesh* m = node->mesh;
		if (m != nullptr)
		{
			m->DrawElements();
		}
		for(size_t i = 0; i < node->children.size(); i++)
		{
			DrawNode(node->children[i]);
		}
	glPopMatrix();
}


