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
#include <mgdl/mgdl-console.h>
#include <mgdl/mgdl-opengl_util.h>
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
	// This is the best setting for Blockbench
	// TODO Load scene without content and try to
	// set this by exporter
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
	if (mgdl_DoesFileExist(fbxFile) == false)
	{
		Log_ErrorF("No such file: %s\n", fbxFile);
		return nullptr;
	}
	ufbx_scene* ufbx = FBX_LoadScene(fbxFile);
	if (ufbx == nullptr)
	{
		return nullptr;
	}

	Scene* scene = Scene_CreateEmpty();
	Scene_InitArrays(scene, 8, 8, 8);
	Scene_SetUFBX(scene, ufbx);

	// Start from the root
	ufbx_node* root = ufbx->root_node;
	m_FBX_LoadNode(scene, root, 0);

	Scene_LogInfo(scene);

	return scene;
}

static void Indent(short depth)
{
	for (short i = 0; i < depth; i++)
	{
		Log_Info("  ");
	}
}

bool m_FBX_LoadNode(Scene* scene, ufbx_node* node, s16 depth)
{
	ufbx_vec3 t = node->local_transform.translation;
	ufbx_vec3 r = ufbx_quat_to_euler(node->local_transform.rotation, UFBX_ROTATION_ORDER_XYZ);
	ufbx_vec3 s = node->local_transform.scale;

	Indent(depth);
	Console_SetTextColor(ConsoleText_Cyan);
	Log_InfoF("Node: %s\n", node->name.data);
	Console_ResetTextColor();

	Indent(depth);
	Log_InfoF("position: (%.2f, %.2f, %.2f) ", t.x, t.y, t.z);
	Log_InfoF("rotation: (%.2f, %.2f, %.2f) ", r.x, r.y, r.z);
	Log_InfoF("scale: (%.2f, %.2f, %.2f) ", s.x, s.y, s.z);
	Log_Info("\n");

	if (node->mesh != nullptr)
	{
		ufbx_mesh* mesh = node->mesh;

		Indent(depth);
		Console_SetTextColor(ConsoleText_Cyan);
		Log_InfoF("Mesh %s ", mesh->name.data);
		Console_ResetTextColor();

		Log_InfoF("id:%u with %zu faces",  mesh->typed_id, mesh->faces.count);
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

		if (Scene_HasMesh(scene, mesh->typed_id) == false)
		{
			Indent(depth);
			Scene_AddMesh(scene, m_FBX_LoadMeshUFBX(mesh));
		}

		for(size_t mi = 0; mi < node->materials.count; mi++)
		{
			ufbx_material* material = node->materials[mi];

			Indent(depth);
			Console_SetTextColor(ConsoleText_Cyan);
			Log_InfoF("Material: %s\n", material->name.data);
			Console_ResetTextColor();

			// Has this material been loaded already?
			mgdl_assert_print(scene->materials != nullptr, "No materials array in scene");
			mgdl_assert_print(scene->materials->data != nullptr, "Scene materials array is nullptr");

			if (Scene_HasMaterial(scene, material->typed_id) == false)
			{
				Material* mat = FBX_LoadNodeMaterial(node, mi, nullptr);
				Scene_AddMaterial(scene, mat);
			}
		}
	}
	else if (node->light != nullptr)
	{
		ufbx_light* light = node->light;
		Indent(depth);
		Console_SetTextColor(ConsoleText_Cyan);
		Log_InfoF("Light %s", light->name.data);
		Console_ResetTextColor();

		Log_InfoF("color: (%.2f, %.2f, %.2f) intensity: %.2f decay enum: %d type: ", light->color.x, light->color.y, light->color.z, (int)light->decay, light->intensity);
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
		Log_Info("\n");

		Light* gdlLight = m_FBX_LoadLight(light);
		Scene_AddLight(scene, gdlLight);
	}
	else if (node->camera != nullptr)
	{
		ufbx_camera* camera = node->camera;
		Indent(depth);
		Console_SetTextColor(ConsoleText_Cyan);
		Log_InfoF("Camera %s\n", camera->name.data);
		Console_ResetTextColor();
	}
	else if (node->bone != nullptr)
	{
		ufbx_bone* bone = node->bone;
		Indent(depth);
		Console_SetTextColor(ConsoleText_Cyan);
		Log_InfoF("Bone %s", bone->name.data);
		Console_ResetTextColor();

		Log_InfoF(" radius: %.2f relative length: %.2f\n",  bone->radius, bone->relative_length);
	}

	for(size_t i = node->children.count; i > 0; i--)
	{
		m_FBX_LoadNode(scene, node->children[i-1], depth+1);
	}

	return true;
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

// Loads the mesh like in ufbx example
Mesh* m_FBX_LoadMeshUFBX(ufbx_mesh* fbxMesh)
{
	// TODO Use general memory for allocations
	Vector3 minPos = Vector3New(10000, 10000, 10000);
	Vector3 maxPos = Vector3New(-10000, -10000, -10000);
	const bool normals = fbxMesh->vertex_normal.exists;
	const bool uvs = fbxMesh->vertex_uv.exists;

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
			if (normals)
			{
				v->normal = ufbx_get_vertex_vec3(&fbxMesh->vertex_normal, index);
			}
			if (uvs)
			{
				v->uv = ufbx_get_vertex_vec2(&fbxMesh->vertex_uv, index);
			}

			// Bounding box calculation
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
	if (normals)
	{
		flags = Flag_SetAll(flags, FlagNormals);
	}
	if (uvs)
	{
		flags = Flag_SetAll(flags, FlagUVs);
	}
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
		if (normals)
		{
			Mesh_AddNormal(m_mesh, Vector3New(v->normal.x, v->normal.y, v->normal.z));
		}
		if (uvs)
		{
			Mesh_AddUV(m_mesh, Vector2New(v->uv.x, v->uv.y));
		}
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

Light* m_FBX_LoadLight(ufbx_light* fbxLight)
{
	Light* light = Light_Create();

	Vector3 c = Vector3New(fbxLight->color.x, fbxLight->color.y, fbxLight->color.z);
	Light_SetColorV(light, c);
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
	Light_Enable(light);

	return light;
}
