#include <mgdl/mgdl-fbx.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <stdio.h>

Scene* FBX_Load(const char* fbxFile)
{
	// Right handed for OpenGL
	// Y is up
	ufbx_load_opts opts = {};
	opts.target_axes = ufbx_axes_right_handed_y_up;
	opts.target_unit_meters = 1.0f;
	ufbx_error error;
	Log_InfoF("Reading fbx file %s\n", fbxFile);
	ufbx_scene* scene = ufbx_load_file(fbxFile, &opts, &error);
	mgdl_assert_printf(scene != nullptr, "Cannot load fbx: %s\n", error.description.data);
	if (scene == nullptr)
	{
		return nullptr;
	}

	Scene* gdlScene = Scene_CreateEmpty();
	// What is in this file?

	// Start from the root
	ufbx_node* root = scene->root_node;
	_FBX_LoadNode(gdlScene, Scene_GetRootNode(gdlScene), root, 0);

	// DANGER ZONE
	// TODO copy only the necessary data so that this can be freed
	//ufbx_free_scene(scene);

	return gdlScene;
}

void Indent(short depth)
{
	for (short i = 0; i < depth; i++)
	{
		Log_Info("\t");
	}
}

bool _FBX_LoadNode ( Scene* gdlScene, Node* parentNode, ufbx_node* node, short depth )
{
	mgdl_assert_print(node != nullptr, "Tried to load null node");
	mgdl_assert_print(gdlScene != nullptr, "No scene to load nodes to");
	ufbx_vec3 t = node->local_transform.translation;
	ufbx_vec3 r = node->euler_rotation;

	if (true)
	{
		Indent(depth);
		Log_InfoF("Node: %s\n", node->name.data);

		Indent(depth);
		Log_Info("Transform:");
		Log_InfoF("position: (%.2f, %.2f, %.2f)", t.x, t.y, t.z);
		Log_InfoF("rotation: (%.2f, %.2f, %.2f)", r.x, r.y, r.z);
		Log_Info("\n");
	}

	size_t childAmount = node->children.count;
	mgdl_assert_print(childAmount <= 255, "UFBX node has too many children > 255");
	if (childAmount == 0) // TODO FIX THIS
	{
		childAmount = 1;
	}
	Node* n = Node_Create((u8)childAmount);
	mgdl_assert_print(n != nullptr, "Could not create new Node");

	Node_SetTransform(n, node->name.data,
								 V3f_Create(t.x, t.y, t.z),
								 V3f_Create(r.x, r.y, r.z));

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
			Log_InfoF(",%zu uvs", mesh->vertex_uv.values.count);
		}
		Log_Info("\n");

		// Is this mesh loaded already?
		// Cannot use name: if multiple meshes have the same name
		// the ufbx will postfix _1 etc.
		// element_id is not unique either. Need to compare ufbx* mesh directly?
		n->mesh = _FBX_LoadMesh(mesh);


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
					Log_InfoF("Creating default material\n");
					Texture* defaultTex = Texture_GenerateCheckerBoard();
					mat = Material_Load(material->name.data, defaultTex, MaterialType::Diffuse);
					Scene_AddMaterial(gdlScene, mat);
				}
				else
				{
					Log_InfoF("Material was already loaded\n");
				}
				n->material = mat;
			}
		}
		else {
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

		Light* gdlLight = _FBX_LoadLight(light);
		n->light = gdlLight;

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

	Scene_AddChildNode(gdlScene, parentNode, n);

	if (childAmount > 0)
	{
		for(size_t i = 0; i < node->children.count; i++)
		{
			_FBX_LoadNode(gdlScene, n, node->children[i], depth+1);
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

Mesh * _FBX_AllocateMesh ( ufbx_mesh* fbxMesh )
{
	Mesh *mesh = Mesh_CreateEmpty();
	sizetype vertices = fbxMesh->num_triangles * 3;
	bool normals = fbxMesh->vertex_normal.exists;
	bool uvs = fbxMesh->vertex_uv.exists;
	u32 creationFlags = 0;
	if (normals)
	{
		creationFlags += FlagNormals;
	}
	if (uvs)
	{
		creationFlags += FlagUVs;
	}
	Mesh_Init(mesh, vertices, vertices, creationFlags);
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

Mesh * _FBX_LoadMesh(ufbx_mesh* fbxMesh)
{
	Mesh* mesh = _FBX_AllocateMesh(fbxMesh);

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

Light* _FBX_LoadLight(ufbx_light* fbxLight)
{
	Light* light = new Light();

	Color4f c = {fbxLight->color.x, fbxLight->color.y, fbxLight->color.z, 1.0f};
	Light_SetColor(light, &c);
	light->intensity = fbxLight->intensity;
	light->name = fbxLight->name.data;

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
	return light;
}
