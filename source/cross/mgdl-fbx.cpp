#include <mgdl/mgdl-fbx.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-model.h>
#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-node.h>
#include <mgdl/mgdl-console.h>
#include <stdio.h>

ufbx_scene* FBX_LoadScene(const char* fbxFile)
{
	ufbx_load_opts opts = {};
	// Right handed for OpenGL
	// Y is up
	opts.target_axes = ufbx_axes_right_handed_y_up;
	opts.target_camera_axes = ufbx_axes_right_handed_y_up;
	opts.target_light_axes = ufbx_axes_right_handed_y_up;
	opts.target_unit_meters = 1.0f;

	// Different 3D Programs export differently
	// This is the best setting for Blender
	opts.space_conversion = UFBX_SPACE_CONVERSION_MODIFY_GEOMETRY;

	ufbx_error error;
	Log_InfoF("Reading fbx file %s\n", fbxFile);
	ufbx_scene* scene = ufbx_load_file(fbxFile, &opts, &error);
	if (scene == nullptr)
	{
		Log_ErrorF("Cannot load fbx: %s\n", error.description.data);
	}

	return scene;

}

Scene* FBX_Load(const char* fbxFile)
{
	ufbx_scene* scene = FBX_LoadScene(fbxFile);
	if (scene == nullptr)
	{
		return nullptr;
	}

	Scene* gdlScene = Scene_CreateEmpty();
	// What is in this file?
	gdlScene->ufbx = scene;

	// Start from the root
	ufbx_node* root = scene->root_node;
	m_FBX_LoadNode(gdlScene, Scene_GetRootNode(gdlScene), root, 0, false);

	return gdlScene;
}

void Indent(short depth)
{
	for (short i = 0; i < depth; i++)
	{
		Log_Info("  ");
	}
}

bool m_FBX_LoadNode(Scene* gdlScene, Node* parentNode, ufbx_node* node, s16 depth, const bool createNodes)
{
	mgdl_assert_print(node != nullptr, "Tried to load null node");
	mgdl_assert_print(gdlScene != nullptr, "No scene to load nodes to");
	ufbx_vec3 t = node->local_transform.translation;
	ufbx_vec3 r = ufbx_quat_to_euler(node->local_transform.rotation, UFBX_ROTATION_ORDER_XYZ);
	ufbx_vec3 s = node->local_transform.scale;

	if (true)
	{
		Indent(depth);
		Console_SetTextColor(ConsoleText_Cyan);
		Log_InfoF("Node: %s\n", node->name.data);

		Console_ResetTextColor();
		Indent(depth);
		Log_InfoF("position: (%.2f, %.2f, %.2f) ", t.x, t.y, t.z);
		Log_InfoF("rotation: (%.2f, %.2f, %.2f) ", r.x, r.y, r.z);
		Log_InfoF("scale: (%.2f, %.2f, %.2f) ", s.x, s.y, s.z);
		Log_Info("\n");
	}

	size_t childAmount = node->children.count;
	mgdl_assert_print(childAmount <= 255, "UFBX node has too many children > 255");
	if (childAmount == 0) // TODO FIX THIS
	{
		childAmount = 1;
	}
	Node* n = nullptr;
	if (createNodes)
	{
		n = Node_Create((u8)childAmount);
		mgdl_assert_print(n != nullptr, "Could not create new Node");

		Node_SetTransform(n, node->name.data,
									Vector3New(t.x, t.y, t.z),
									Vector3New(r.x, r.y, r.z));

	}
	Indent(depth);
	if (node->mesh != nullptr)
	{
		ufbx_mesh* mesh = node->mesh;

		Log_InfoF("Mesh %s (%u) with %zu faces", mesh->name.data, mesh->element_id, mesh->faces.count);
		if (mesh->vertex_normal.exists)
		{
			Log_InfoF(", %zu normals", mesh->vertex_normal.values.count);
		}
		if (mesh->vertex_uv.exists)
		{
			Log_InfoF(", %zu uvs", mesh->vertex_uv.values.count);
		}
		if (mesh->num_indices > 0)
		{
			Log_InfoF(", %zu indices", mesh->num_indices);
		}
		Log_Info("\n");

		// Is this mesh loaded already?
		// Cannot use name: if multiple meshes have the same name
		// the ufbx will postfix _1 etc.
		// element_id is not unique either. Need to compare ufbx* mesh directly?
		if (createNodes)
		{
			n->mesh = m_FBX_LoadMeshUFBX(mesh);
		}
		else
		{
			Scene_AddMesh(gdlScene, m_FBX_LoadMeshUFBX(mesh));
		}


		// Does this node have materials?
		if (node->materials.count > 0)
		{
			for(size_t mi = 0; mi < node->materials.count; mi++)
			{
				// TODO How to load the textures automatically
				// or if loaded later, match them to the meshes?

				ufbx_material* material = node->materials[mi];

				if (true)
				{
					Indent(depth);
					Log_InfoF("Material: %s\n", material->name.data);
				}

				// Has this material been loaded already?
				mgdl_assert_print(gdlScene->materials != nullptr, "No materials array in scene");
				mgdl_assert_print(gdlScene->materials->data != nullptr, "Scene materials array is nullptr");


				Material* mat = Scene_GetMaterial(gdlScene, material->name.data);
				if (mat == nullptr)
				{
					mat = FBX_LoadNodeMaterial(node, mi, nullptr);
					Scene_AddMaterial(gdlScene, mat);
				}
				else
				{
					Log_InfoF("Material was already loaded\n");
				}
				if (createNodes)
				{
					n->material = mat;
				}
			}
		}
		else if (createNodes)
		{
			// Set default material for safety?
			Log_Warning("Node has mesh but no material, setting default material");
			Log_InfoF("Creating default material\n");
			Texture* defaultTex = Texture_GenerateCheckerBoard();
			Material* mat = Material_Load("Checkboard", defaultTex, MaterialType::Diffuse);
			Scene_AddMaterial(gdlScene, mat);
			n->material = mat;
		}


	}
	else if (node->light != nullptr)
	{
		ufbx_light* light = node->light;
		Log_InfoF("\tLight %s, color: (%.2f, %.2f, %.2f) intensity: %.2f type: ", light->name.data, light->color.x, light->color.y, light->color.z, light->intensity);
		if (light->type == UFBX_LIGHT_POINT)
		{
			Log_Info("point");
		}
		else if (light->type == UFBX_LIGHT_SPOT)
		{
			Log_Info("spot");
		}
		else if (light->type == UFBX_LIGHT_DIRECTIONAL)
		{
			Log_Info("directional");
		}
		else if (light->type == UFBX_LIGHT_AREA)
		{
			Log_Info("area");
		}
		else if (light->type == UFBX_LIGHT_VOLUME)
		{
			Log_Info("volumetric");
		}

		Light* gdlLight = m_FBX_LoadLight(light);
		if (createNodes)
		{
			n->light = gdlLight;
		}
		else
		{
			Scene_AddLight(gdlScene, gdlLight);
		}

		Log_Info("\n");

	}
	else if (node->camera != nullptr)
	{
		ufbx_camera* camera = node->camera;
		Log_InfoF("\tCamera %s\n", camera->name.data);
	}
	else if (node->bone != nullptr)
	{
		ufbx_bone* bone = node->bone;
		Log_InfoF("\tBone %s, radius: %.2f relative length: %.2f\n", bone->name.data, bone->radius, bone->relative_length);
	}

	if (createNodes)
	{
		Scene_AddChildNode(gdlScene, parentNode, n);
	}

	if (childAmount > 0)
	{
		for(size_t i = node->children.count; i > 0; i--)
		{
			m_FBX_LoadNode(gdlScene, n, node->children[i-1], depth+1, createNodes);
		}
	}

	return true;
}

void PushPosition(Mesh* mesh, size_t index, ufbx_vec3 pos)
{
	size_t vpi = index * 3;
	mesh->positions[vpi+0] = pos.x;
	mesh->positions[vpi+1] = pos.y;
	mesh->positions[vpi+2] = pos.z;
}

void PushNormal(Mesh* mesh, size_t index, ufbx_vec3 n)
{
	mgdl_assert_print(mesh->normals != nullptr, "Cannot push normal to nullptr");
	// Where the V3f begins in array
	// every vertex has 3 floats
	size_t vni = index * 3;
	mesh->normals[vni+0] = n.x;
	mesh->normals[vni+1] = n.y;
	mesh->normals[vni+2] = n.z;
}

void PushUV(Mesh* mesh, size_t index, ufbx_vec2 uv)
{
	// NOTE This might be because Blockbench and Blender think about
	// this fdifferently
	// Flip the y coordinates because in OpenGL images Y grows upwards
	float y = uv.y;
	/*
	y -= 1.0f;
	y *= -1.0f;
	*/

	// Every vertex has 2 floats for uv
	size_t vti = index * 2;
	mesh->uvs[vti+0] = uv.x;
	mesh->uvs[vti+1] = y;
}

Mesh * m_FBX_AllocateMesh ( ufbx_mesh* fbxMesh )
{
	Mesh *mesh = Mesh_CreateEmpty();
	sizetype vertices = fbxMesh->num_triangles * 3;
	bool normals = fbxMesh->vertex_normal.exists;
	bool uvs = fbxMesh->vertex_uv.exists;
	sizetype indices = fbxMesh->max_face_triangles * 3; // NOTE This handles Quads
	u32 creationFlags = 0;
	if (normals)
	{
		creationFlags += FlagNormals;
	}
	if (uvs)
	{
		creationFlags += FlagUVs;
	}
	Mesh_Init(mesh, vertices, indices, creationFlags);
	return mesh;
}


void PushVertex(ufbx_mesh* fbxMesh, Mesh* mesh, uint32_t faceIndex, size_t arrayIndex)
{
	ufbx_vec3 position = fbxMesh->vertex_position[faceIndex];
	ufbx_vec3 normal = fbxMesh->vertex_normal[faceIndex];
	ufbx_vec2 uv = fbxMesh->vertex_uv[faceIndex];

	PushPosition(mesh, arrayIndex, position);
	PushNormal(mesh, arrayIndex, normal);
	PushUV(mesh, arrayIndex, uv);
}
ufbx_mesh* FBX_GetFirstMesh(ufbx_scene* scene)
{
	for (size_t i = 0; i < scene->nodes.count; i++)
	{
		ufbx_node *node = scene->nodes.data[i];
		if (node->mesh != nullptr)
		{
			Log_InfoF("Node %d : %s ", i, node->name.data);
			ufbx_mesh* mesh = node->mesh;

			Log_InfoF("Mesh %s (%u) with %zu faces", mesh->name.data, mesh->element_id, mesh->faces.count);
			if (mesh->vertex_normal.exists)
			{
				Log_InfoF(", %zu normals", mesh->vertex_normal.values.count);
			}
			if (mesh->vertex_uv.exists)
			{
				Log_InfoF(", %zu uvs", mesh->vertex_uv.values.count);
			}
			Log_Info("\n");
			return mesh;
		}
	}
    return nullptr;
}

Model* FBX_LoadFirstModel(const char* fbxFile)
{
	ufbx_scene* scene = FBX_LoadScene(fbxFile);
	if (scene == nullptr)
	{
		return nullptr;
	}

	ufbx_mesh* mesh = FBX_GetFirstMesh(scene);
	Material* material = nullptr;

	// Extract folder from file path
	zstr filepath = zstr_from(fbxFile);

	// Find '/'
	ptrdiff_t index = zstr_find(&filepath, "/");
	if (index > 0)
	{
		zstr_view pathview = zstr_as_view(&filepath);
		zstr_view filetype = zstr_sub(pathview, 0, index);
		zstr folder = zstr_from_view(filetype);
		material = FBX_LoadFirstMaterial(scene, zstr_cstr(&folder));
		zstr_free(&folder);
	}
	zstr_free(&filepath);

	if (material == nullptr)
	{
		material = FBX_LoadFirstMaterial(scene, nullptr);
	}

	if (mesh != nullptr)
	{
		Model* model = Model_Create();
		model->m_mesh = m_FBX_LoadMeshUFBX(mesh);
		model->m_material = material;

		ufbx_free_scene(scene);
		return model;
	}
	else
	{
		ufbx_free_scene(scene);
		return nullptr;
	}
}

Material* FBX_LoadFirstMaterial(ufbx_scene* scene, const char* searchfolder)
{
	for (size_t i = 0; i < scene->nodes.count; i++)
	{
		ufbx_node *node = scene->nodes.data[i];
		if (node->mesh != nullptr && node->materials.count > 0)
		{
			return FBX_LoadNodeMaterial(node, 0, searchfolder);
		}
	}
	return nullptr;
}

Material* FBX_LoadNodeMaterial(ufbx_node* node, sizetype materialIndex, const char* searchfolder)
{
	if (materialIndex >= node->materials.count)
	{
		return nullptr;
	}
	ufbx_material* material = node->materials[materialIndex];
	if (material != nullptr)
	{
		// Try to load from assets folder

		TextureHandle materialTexture = MGDL_INVALID_HANDLE;
		if (searchfolder != nullptr)
		{
			Log_InfoF("Trying to load material from %s\n", searchfolder);
			mgdl_BufferPrintf("%s/%s", searchfolder, material->name.data);
			materialTexture = AssetManager_LoadTexture(mgdl_GetPrintfBuffer());
		}
		else
		{
			materialTexture = AssetManager_LoadTexture(material->name.data);
		}
		Texture* texture = nullptr;
		if (Handle_IsValid(materialTexture))
		{
			Log_InfoF("Material texture loaded\n");
			texture = AssetManager_GetTexture(materialTexture);
		}
		else
		{
			Log_InfoF("Material texture not found\n");
		}
		Material* mat = Material_Load(material->name.data, texture, MaterialType::Diffuse);
		mat->ufbx_id = material->typed_id;
		return mat;
	}
	return nullptr;
}

Mesh* FBX_LoadMeshTrianglesOnly(ufbx_mesh* fbxMesh)
{
	// NOTE Does not use indices
	if (fbxMesh->vertex_uv.exists == false)
	{
		Log_Warning("Mesh does not have UVs");
	}
	Mesh* mesh = m_FBX_AllocateMesh(fbxMesh);
	sizetype indexIndex = 0;
	for(ufbx_face face : fbxMesh->faces)
	{
		// Ufbx indices that belong to a single face and
		// are used to refer to vertex data
		for(uint32_t corner = 0; corner < face.num_indices; corner++)
		{
			uint32_t index = face.index_begin + corner;

			ufbx_vec3 position = ufbx_get_vertex_vec3(&fbxMesh->vertex_position, index);
			PushPosition(mesh, indexIndex, position);
			if (fbxMesh->vertex_normal.exists)
			{
				ufbx_vec3 normal = ufbx_get_vertex_vec3(&fbxMesh->vertex_normal, index);
				PushNormal(mesh, indexIndex, normal);
			}
			if (fbxMesh->vertex_uv.exists)
			{
				ufbx_vec2 uv = ufbx_get_vertex_vec2(&fbxMesh->vertex_uv, index);
				PushUV(mesh, indexIndex, uv);
			}
			indexIndex++;
		}
	}
	mesh->name = zstr_from(fbxMesh->name.data);
	return mesh;
}

// Loads the mesh like in ufbx example
Mesh* m_FBX_LoadMeshUFBX(ufbx_mesh* fbxMesh)
{
	// TODO Use general memory for allocations
	Vector3 minPos = Vector3New(10000, 10000, 10000);
	Vector3 maxPos = Vector3New(-10000, -10000, -10000);

	ufbx_mesh_part* part = &fbxMesh->material_parts[0];
	size_t num_triangles = part->num_triangles;
	Vertex *vertices = (Vertex*)calloc(num_triangles * 3, sizeof(Vertex));
    size_t num_vertices = 0;

    // Reserve space for the maximum triangle indices.
    size_t num_tri_indices = fbxMesh->max_face_triangles * 3;
    uint32_t *tri_indices = (uint32_t*)calloc(num_tri_indices, sizeof(uint32_t));

    // Iterate over each face using the specific material.
    for (size_t face_ix = 0; face_ix < part->num_faces; face_ix++) {
        ufbx_face face = fbxMesh->faces.data[part->face_indices.data[face_ix]];

        // Triangulate the face into `tri_indices[]`.
        uint32_t num_tris = ufbx_triangulate_face(tri_indices, num_tri_indices, fbxMesh, face);

        // Iterate over each triangle corner contiguously.
        for (size_t i = 0; i < num_tris * 3; i++) {
            uint32_t index = tri_indices[i];

            Vertex *v = &vertices[num_vertices++];
            v->position = ufbx_get_vertex_vec3(&fbxMesh->vertex_position, index);
            v->normal = ufbx_get_vertex_vec3(&fbxMesh->vertex_normal, index);
            v->uv = ufbx_get_vertex_vec2(&fbxMesh->vertex_uv, index);

			if (v->position.x > maxPos.x) { maxPos.x = v->position.x;}
			if (v->position.x > maxPos.y) { maxPos.y = v->position.y;}
			if (v->position.x > maxPos.z) { maxPos.z = v->position.z;}
			if (v->position.x < minPos.x) { minPos.x = v->position.x;}
			if (v->position.x < minPos.y) { minPos.y = v->position.y;}
			if (v->position.x < minPos.z) { minPos.z = v->position.z;}

        }
    }


    // Should have written all the vertices.
    free(tri_indices);
    assert(num_vertices == num_triangles * 3);

    // Generate the index buffer.
    ufbx_vertex_stream streams[1] = {
        { vertices, num_vertices, sizeof(Vertex) },
    };
    size_t num_indices = num_triangles * 3;
    uint32_t *indices = (uint32_t*)calloc(num_indices, sizeof(uint32_t));

    // This call will deduplicate vertices, modifying the arrays passed in `streams[]`,
    // indices are written in `indices[]` and the number of unique vertices is returned.
    num_vertices = ufbx_generate_indices(streams, 1, indices, num_indices, NULL, NULL);

    // create_vertex_buffer(vertices, num_vertices);
    //create_index_buffer(indices, num_indices);
    // Copy over the indices
	Mesh* m_mesh = Mesh_CreateEmpty();
	u32 flags = 0;
	flags = Flag_SetAll(flags, FlagNormals);
	flags = Flag_SetAll(flags, FlagUVs);
	Mesh_Init(m_mesh, num_vertices, num_indices, flags);
    for(size_t i = 0; i < num_indices; i++)
	{
		m_mesh->indices[i] = (GLushort)indices[i];
	}
	m_mesh->ufbx_id = fbxMesh->typed_id;

	for (size_t i = 0; i < num_vertices; i++)
	{
		Vertex* v = &vertices[i];

		Mesh_AddPosition(m_mesh, Vector3New(v->position.x, v->position.y, v->position.z));
		Mesh_AddNormal(m_mesh, Vector3New(v->normal.x, v->normal.y, v->normal.z));
		Mesh_AddUV(m_mesh, Vector2New(v->uv.x, v->uv.y));
	}

    free(indices);
    free(vertices);

	Log_InfoF("Mesh bounding box (%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)\n",
			  maxPos.x,
			  maxPos.y,
			  maxPos.z,
			  minPos.x,
			  minPos.y,
			  minPos.z
			  );

	return m_mesh;
}



// DANGER Delete this, has memory errors with quads
Mesh * m_FBX_LoadMesh(ufbx_mesh* fbxMesh)
{
	Mesh* mesh = m_FBX_AllocateMesh(fbxMesh);

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
	mesh->name = zstr_from(fbxMesh->name.data);
	return mesh;
}

Light* m_FBX_LoadLight(ufbx_light* fbxLight)
{
	Light* light = new Light();

	color32 c = Color_Create4f(fbxLight->color.x, fbxLight->color.y, fbxLight->color.z, 1.0f);
	Light_SetColor(light, c);
	light->intensity = fbxLight->intensity;
	light->name = zstr_from(fbxLight->name.data);

	// Light is a spot in OpenGL if this is less than 90
	// Light is point or directional if this is 180
	light->spotHalfAngle = 180.0f;

	if (fbxLight->type == UFBX_LIGHT_POINT)
	{
		light->type = LightType::Point;
	}
	else if (fbxLight->type == UFBX_LIGHT_SPOT)
	{
		light->type = LightType::Spot;
		light->spotHalfAngle = fbxLight->outer_angle;
	}
	else if (fbxLight->type == UFBX_LIGHT_DIRECTIONAL)
	{
		// Light is a directional if the W component of position is 0.0f
		light->type = LightType::Directional;
	}
	else if (fbxLight->type == UFBX_LIGHT_AREA)
	{
		Log_Warning("Area lights not supported\n");
		light->type = LightType::Point;
	}
	else if (fbxLight->type == UFBX_LIGHT_VOLUME)
	{
		Log_Warning("Volumetric lights not supported\n");
		light->type = LightType::Point;
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
	light->ufbx_id = fbxLight->typed_id;
	return light;
}
