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
	ufbx_scene* scene = ufbx_load_file(fbxFile.c_str(), &opts, &error);
	gdl_assert_printf(scene != nullptr, "Cannot load fbx: %s\n", error.description.data);
	if (scene == nullptr)
	{
		return nullptr;
	}

	gdl::Scene* gdlScene = new gdl::Scene();
	// What is in this file?

	// Start from the root
	ufbx_node* root = scene->root_node;
	LoadNode(gdlScene, gdlScene->GetRootNode(), root, 0);

	ufbx_free_scene(scene);

	return gdlScene;
}

void Indent(short depth)
{
	for (short i = 0; i < depth; i++)
	{
		printf("\t");
	}
}

bool gdl::FBXFile::LoadNode ( gdl::Scene* gdlScene, gdl::Node* parentNode, ufbx_node* node, short depth )
{
	gdl_assert_print(node != nullptr, "Tried to load null node");
	gdl_assert_print(gdlScene != nullptr, "No scene to load nodes to");
	ufbx_vec3 t = node->local_transform.translation;
	ufbx_vec3 r = node->euler_rotation;

	if (false)
	{
		Indent(depth);
		printf("Node: %s\n", node->name.data);

		Indent(depth);
		printf("Transform:");
		printf("position: (%.2f, %.2f, %.2f)", t.x, t.y, t.z);
		printf("rotation: (%.2f, %.2f, %.2f)", r.x, r.y, r.z);
		printf("\n");
	}

	gdl::Node* n = new gdl::Node(std::string(node->name.data),
								 vec3New(t.x, t.y, t.z),
								 vec3New(r.x, r.y, r.z));
	gdl_assert_print(n != nullptr, "Could not create new Node");

	Indent(depth);
	if (node->mesh != nullptr)
	{
		ufbx_mesh* mesh = node->mesh;
		if (false)
		{
			printf("Mesh %s (%u) with %zu faces", mesh->name.data, mesh->element_id, mesh->faces.count);
			if (mesh->vertex_normal.exists)
			{
				printf(", %zu normals", mesh->vertex_normal.values.count);
			}
			if (mesh->vertex_uv.exists)
			{
				printf(",%zu uvs", mesh->vertex_uv.values.count);
			}
			printf("\n");
		}

		// Is this mesh loaded already?
		// Cannot use name: if multiple meshes have the same name
		// the ufbx will postfix _1 etc.
		// element_id is not unique either. Need to compare ufbx* mesh directly?
		n->mesh = LoadMesh(mesh);


		// Does this node have materials?
		for(size_t mi = 0; mi < node->materials.count; mi++)
		{
			// TODO How to load the textures automatically
			// or if loaded later, match them to the meshes?

			ufbx_material* material = node->materials[mi];
			if (false)
			{
				Indent(depth);
				printf("Material: %s\n", material->name.data);
			}

			// Has this material been loaded already?
			std::string matName = std::string(material->name.data);
			gdl::Material* mat = gdlScene->GetMaterial(matName);
			if (mat == nullptr)
			{
				mat = new gdl::Material(matName);
				gdlScene->AddMaterial(mat);
			}
			n->material = mat;
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

		gdl::Light* gdlLight = LoadLight(light);
		n->light = gdlLight;

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

	gdlScene->AddChildNode(parentNode, n);

	size_t childAmount = node->children.count;
	if (childAmount > 0)
	{
		if (false)
		{
			Indent(depth);
			printf("%zu children\n", childAmount);
		}
		for(size_t i = 0; i < node->children.count; i++)
		{
			LoadNode(gdlScene, n, node->children[i], depth+1);
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
	gdl_assert_print(mesh->normals != nullptr, "Cannot push normal to nullptr");
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
	sizetype vertices = fbxMesh->num_triangles * 3;
	bool normals = fbxMesh->vertex_normal.exists;
	bool uvs = fbxMesh->vertex_uv.exists;
	gdl::Mesh *mesh = new gdl::Mesh();
	mesh->Allocate(vertices, vertices, normals, uvs);
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
	if (false)
	{
		printf("Loaded mesh\n");
	}
	mesh->name = fbxMesh->name.data;
	return mesh;
}

gdl::Light* gdl::FBXFile::LoadLight(ufbx_light* fbxLight)
{
	gdl::Light* light = new gdl::Light();

	light->color = vec3New(fbxLight->color.x, fbxLight->color.y, fbxLight->color.z);
	light->intensity = fbxLight->intensity;
	light->name = std::string(fbxLight->name.data);

	// Light is a spot in OpenGL if this is less than 90
	// Light is point or directional if this is 180
	light->spotHalfAngle = 180.0f;

	if (fbxLight->type == UFBX_LIGHT_POINT)
	{
		light->type = gdl::LightType::Point;
	}
	else if (fbxLight->type == UFBX_LIGHT_SPOT)
	{
		light->type = gdl::LightType::Spot;
		light->spotHalfAngle = fbxLight->outer_angle;
	}
	else if (fbxLight->type == UFBX_LIGHT_DIRECTIONAL)
	{
		// Light is a directional if the W component of position is 0.0f
		light->type = gdl::LightType::Directional;
	}
	else if (fbxLight->type == UFBX_LIGHT_AREA)
	{
		printf("Area lights not supported\n");
		light->type = gdl::LightType::Point;
	}
	else if (fbxLight->type == UFBX_LIGHT_VOLUME)
	{
		printf("Volumetric lights not supported\n");
		light->type = gdl::LightType::Point;
	}

	switch(fbxLight->decay)
	{
		case UFBX_LIGHT_DECAY_NONE:
			light->constantAttenuation = 1.0f;
			light->LinearAttenuation = 0.0f;
			light->QuadraticAttenuation = 0.0f;
		break;
		case UFBX_LIGHT_DECAY_LINEAR:
			light->constantAttenuation = 0.0f;
			light->LinearAttenuation = 1.0f;
			light->QuadraticAttenuation = 0.0f;
		break;
		case UFBX_LIGHT_DECAY_QUADRATIC:
		case UFBX_LIGHT_DECAY_CUBIC:
			light->constantAttenuation = 0.0f;
			light->LinearAttenuation = 0.0f;
			light->QuadraticAttenuation = 1.0f;
		break;
		default:

		break;
	}
	return light;
}
