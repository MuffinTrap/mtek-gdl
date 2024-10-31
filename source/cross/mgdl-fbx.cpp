#include <mgdl/mgdl-fbx.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-scene.h>
#include <stdio.h>

gdl::Scene* gdl::FBXFile::LoadFile(std::string fbxFile)
{
	// Right handed for OpenGL
	// Y is up
	ufbx_load_opts opts = {};
	opts.target_axes = ufbx_axes_right_handed_y_up;
	opts.target_unit_meters = 1.0f;
	ufbx_error error;
	printf("Reading fbx file %s\n", fbxFile.c_str());
	scene = ufbx_load_file(fbxFile.c_str(), &opts, &error);
	gdl_assert_printf(scene != nullptr, "Cannot load fbx: %s\n", error.description.data);
	if (scene == nullptr)
	{
		return nullptr;
	}

	gdl::Scene* gdlScene = new gdl::Scene();
	// What is in this file?

	// Start from the root
	ufbx_node* root = scene->root_node;
	{
		gdlScene->SetActiveParentNode(nullptr);
		LoadNode(gdlScene, root, 0);
	}

	return gdlScene;
}

void Indent(short depth)
{
	for (short i = 0; i < depth; i++)
	{
		printf("\t");
	}
}

bool gdl::FBXFile::LoadNode ( gdl::Scene* gdlScene, ufbx_node* node, short depth )
{
	Indent(depth);
	printf("Node: %s\n", node->name.data);

	Indent(depth);
	printf("Transform:");
	ufbx_vec3 t = node->local_transform.translation;
	ufbx_vec3 r = node->euler_rotation;
	printf("position: (%.2f, %.2f, %.2f)", t.x, t.y, t.z);
	printf("rotation: (%.2f, %.2f, %.2f)", r.x, r.y, r.z);
	printf("\n");

	gdl::Node* n = new gdl::Node();
	n->name = std::string(node->name.data);
	n->transform.position = gdl::vec3(t.x, t.y, t.z);
	n->transform.rotationRadians = gdl::vec3(r.x, r.y, r.z);

	Indent(depth);
	if (node->mesh != nullptr)
	{
		ufbx_mesh* mesh = node->mesh;
		printf("Mesh %s with %zu faces", mesh->name.data, mesh->faces.count);
		if (mesh->vertex_normal.exists)
		{
			printf(", %zu normals", mesh->vertex_normal.values.count);
		}
		if (mesh->vertex_uv.exists)
		{
			printf(",%zu uvs", mesh->vertex_uv.values.count);
		}
		printf("\n");

		// TODO
		// Is this mesh loaded already?

		gdl::Mesh* m = LoadMesh(mesh);
		n->mesh = m;

		// Does this node have materials?
		for(size_t mi = 0; mi < node->materials.count; mi++)
		{
			// TODO How to load the textures automatically
			// or if loaded later, match them to the meshes?

			ufbx_material* material = node->materials[mi];
			Indent(depth);
			printf("Material: %s\n", material->name.data);

			// Has this material been loaded already?
			std::string matName = std::string(material->name.data);
			gdl::Material* mat = gdlScene->GetMaterial(matName);
			if (mat == nullptr)
			{
				mat = new gdl::Material();
				mat->name = matName;
				gdlScene->AddMaterial(mat);
			}
			n->material = mat;
			printf("Added material %s\n", n->material->name.c_str());
		}


	}
	else if (node->light != nullptr)
	{
		ufbx_light* light = node->light;
		printf("\tLight %s, color: (%.2f, %.2f, %.2f) intensity: %.2f type: ", light->name.data, light->color.x, light->color.y, light->color.z, light->intensity);
		if (light->type == UFBX_LIGHT_POINT)
		{
			printf("point");
		}
		else if (light->type == UFBX_LIGHT_SPOT)
		{
			printf("spot");
		}
		else if (light->type == UFBX_LIGHT_DIRECTIONAL)
		{
			printf("directional");
		}
		else if (light->type == UFBX_LIGHT_AREA)
		{
			printf("area");
		}
		else if (light->type == UFBX_LIGHT_VOLUME)
		{
			printf("volumetric");
		}
		printf("\n");

	}
	else if (node->camera != nullptr)
	{
		ufbx_camera* camera = node->camera;
		printf("\tCamera %s\n", camera->name.data);
	}
	else if (node->bone != nullptr)
	{
		ufbx_bone* bone = node->bone;
		printf("\tBone %s, radius: %.2f relative length: %.2f\n", bone->name.data, bone->radius, bone->relative_length);
	}

	gdlScene->PushChildNode(n);

	size_t childAmount = node->children.count;
	if (childAmount > 0)
	{
		Indent(depth);
		printf("%zu children\n", childAmount);
		gdlScene->SetActiveParentNode(n);
		for(size_t i = 0; i < node->children.count; i++)
		{
			LoadNode(gdlScene, node->children[i], depth+1);
		}
	}

	return true;
}

void PushPosition(gdl::Mesh* mesh, size_t index, ufbx_vec3 pos)
{
	size_t vpi = index * 3;
	mesh->positions[vpi+0] = pos.x;
	mesh->positions[vpi+1] = pos.y;
	mesh->positions[vpi+2] = pos.z;
}

void PushNormal(gdl::Mesh* mesh, size_t index, ufbx_vec3 n)
{
	// Where the vec3 begins in array
	// every vertex has 3 floats
	size_t vni = index * 3;
	mesh->normals[vni+0] = n.x;
	mesh->normals[vni+1] = n.y;
	mesh->normals[vni+2] = n.z;
}

void PushUV(gdl::Mesh* mesh, size_t index, ufbx_vec2 uv)
{
	// Flip the y coordinates because in OpenGL images Y grows upwards
	float y = uv.y;
	y -= 1.0f;
	y *= -1.0f;

	// Every vertex has 2 floats for uv
	size_t vti = index * 2;
	mesh->uvs[vti+0] = uv.x;
	mesh->uvs[vti+1] = y;
}

gdl::Mesh * gdl::FBXFile::AllocateMesh ( ufbx_mesh* fbxMesh )
{
	gdl::Mesh *mesh = new gdl::Mesh();

	int tris = fbxMesh->num_triangles;
	mesh->indexCount = tris * 3;
	printf("Mesh has %d triangles\n", tris);

	// Reserve space
	int byteCount = 0;

	mesh->indices = new GLushort[mesh->indexCount];
	byteCount += mesh->indexCount * sizeof(float);

	size_t vertexAmount = tris * 3;
	mesh->vertexCount = vertexAmount;
	{
		// 3 floats per position
		size_t positionFloats = vertexAmount * 3;
		mesh->positions = new GLfloat[positionFloats];
		byteCount += positionFloats * sizeof(float);
	}

	bool normals = fbxMesh->vertex_normal.exists;
	if (normals)
	{
		// 3 floats per normal
		size_t normalFloats = vertexAmount * 3;
		mesh->normals = new GLfloat[normalFloats];
		byteCount += normalFloats * sizeof(float);
	}
	bool uvs = fbxMesh->vertex_uv.exists;
	if (uvs)
	{
		// 2 floats per uv
		size_t uvFloats = vertexAmount * 2;
		mesh->uvs = new GLfloat[uvFloats];
		byteCount += uvFloats * sizeof(float);
	}
	printf("Allocated %d bytes for the mesh\n", byteCount);

	return mesh;
}


void PushVertex(ufbx_mesh* fbxMesh, gdl::Mesh* mesh, uint32_t faceIndex, size_t arrayIndex)
{
	ufbx_vec3 position = fbxMesh->vertex_position[faceIndex];
	ufbx_vec3 normal = fbxMesh->vertex_normal[faceIndex];
	ufbx_vec2 uv = fbxMesh->vertex_uv[faceIndex];

	PushPosition(mesh, arrayIndex, position);
	PushNormal(mesh, arrayIndex, normal);
	PushUV(mesh, arrayIndex, uv);
}

gdl::Mesh * gdl::FBXFile::LoadMesh(ufbx_mesh* fbxMesh)
{
	gdl::Mesh* mesh = AllocateMesh(fbxMesh);

	size_t vertexArrayIndex = 0;
	size_t indiceArrayIndex = 0;
	GLushort drawIndex = 0;
	// Read each face and triangulate if needed
	for(ufbx_face face : fbxMesh->faces)
	{
		// Ufbx indices that belong to a single face and
		// are used to refer to vertex data
		uint32_t faceIndices[4];

		// This can be 3 or 4 indices;
		for(uint32_t corner = 0; corner < face.num_indices; corner++)
		{
			uint32_t index = face.index_begin + corner;
			// ufbx face index used later for uvs and normals
			faceIndices[corner] = index;
		}

		// TODO
		// Unique vertex is the position, normal and uv index
		// together.
		// If a vertex does not exist yet, push it into
		// array and add a new index for it.
		// If it exists, add only its index

		// 3 new unique vertices. New indice for each one
		PushVertex(fbxMesh, mesh, faceIndices[0], vertexArrayIndex);
		mesh->indices[indiceArrayIndex] = drawIndex;

		PushVertex(fbxMesh, mesh, faceIndices[1], vertexArrayIndex+1);
		mesh->indices[indiceArrayIndex+1] = drawIndex+1;

		PushVertex(fbxMesh, mesh, faceIndices[2], vertexArrayIndex+2);
		mesh->indices[indiceArrayIndex+2] = drawIndex+2;

		// One more unique vertex. Use the previous indices with it
		if (face.num_indices == 4)
		{
			mesh->indices[indiceArrayIndex+3] = drawIndex;
			mesh->indices[indiceArrayIndex+4] = drawIndex+2;

			PushVertex(fbxMesh, mesh, faceIndices[3], vertexArrayIndex+3);
			mesh->indices[indiceArrayIndex+5] = drawIndex+3;
		}

		// Get ready for next face
		if (face.num_indices == 4)
		{
			// This was a quad
			drawIndex += 4; 		// Drew the face with 4 vertices
			vertexArrayIndex += 4; 	// Added 4 unique vertices
			indiceArrayIndex += 6; 	// Added 6 new indices
		}
		else
		{
			// This was a triangle
			drawIndex += 3; 		 // Drew the face with 3 vertices
			vertexArrayIndex += 3;	 // Added 3 unique vertices
			indiceArrayIndex += 3;	 // Added 3 new indices
		}
	}
	printf("Loaded mesh\n");
	return mesh;
}

void gdl::FBXFile::DeleteData()
{
	if (scene != nullptr)
	{
		ufbx_free_scene(scene);
		scene = nullptr;
	}
}

