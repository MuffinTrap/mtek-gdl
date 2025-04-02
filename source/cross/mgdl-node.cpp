#include <mgdl/mgdl-node.h>

gdl::Node::Node()
{
	transform = gdl::Transform();

}

gdl::Node::Node(std::string name, vec3 position, vec3 rotationAngles)
{
	this->name = name;
	transform = gdl::Transform(position, rotationAngles, vec3New(1,1,1));
}


gdl::Node::Node ( std::string name, gdl::Mesh* meshParam, gdl::Material* materialParam )
{
	transform = gdl::Transform();
	this->name = name;
	mesh = meshParam;
	material = materialParam;
}

void gdl::Node::Draw()
{

	const vec3& t = transform.position;
	glTranslatef(t.x, t.y, t.z);

	const vec3& r = transform.rotationDegrees;
	glRotatef(r.x, 1.0f, 0.0f, 0.0f);
	glRotatef(r.y, 0.0f, 1.0f, 0.0f);
	glRotatef(r.z, 0.0f, 0.0f, 1.0f);

	const vec3& s = transform.scale;
	glScalef(s.x, s.y, s.z);

	gdl::Mesh* m = mesh;
	if (m != nullptr)
	{
		if (material != nullptr)
		{
			// TODO do not reapply same material
			material->Apply();
			if (material->type == gdl::MaterialType::Matcap)
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

				m->CalculateMatcapUVs(modelView, normalMatrix);
			}
		}
		m->DrawElements();
	}
}

