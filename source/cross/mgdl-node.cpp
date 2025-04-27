#include <mgdl/mgdl-node.h>

void Node_SetTransform(Node* node, const char* name, vec3 position, vec3 rotationAngles)
{
	node->name = name;
	node->transform = gdl::Transform(position, rotationAngles, vec3New(1,1,1));
}


void Node_SetContent (Node* node, const char* name, Mesh* meshParam, Material* materialParam )
{
	node->transform = gdl::Transform();
	node->name = name;
	node->mesh = meshParam;
	node->material = materialParam;
}

void Node_Draw(Node* node)
{

	const vec3& t = node->transform.position;
	glTranslatef(t.x, t.y, t.z);

	const vec3& r = node->transform.rotationDegrees;
	glRotatef(r.x, 1.0f, 0.0f, 0.0f);
	glRotatef(r.y, 0.0f, 1.0f, 0.0f);
	glRotatef(r.z, 0.0f, 0.0f, 1.0f);

	const vec3& s = node->transform.scale;
	glScalef(s.x, s.y, s.z);

	Mesh* m = node->mesh;
	if (m != nullptr)
	{
		if (node->material != nullptr)
		{
			// TODO do not reapply same material
			Material_Apply(node->material);
			if (node->material->type == gdl::MaterialType::Matcap)
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

