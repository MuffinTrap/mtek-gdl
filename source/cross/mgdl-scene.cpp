#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-node.h>
#include <mgdl/mgdl-camera.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-console.h>
#include <mgdl/mgdl-memory.h>

POINTER_ARRAY_IMPLEMENT(Mesh)
POINTER_ARRAY_IMPLEMENT(Material)
POINTER_ARRAY_IMPLEMENT(Light)

void Scene_InitArrays(Scene* scene, int meshCapacity, int materialCapacity, int lightCapacity)
{
	scene->materials = PointerArray_Create_Material(materialCapacity);
	scene->meshes = PointerArray_Create_Mesh(meshCapacity);
	// Wii has max 8 lights
	if (lightCapacity > 8)
	{
		lightCapacity = 8;
	}
	scene->lights = PointerArray_Create_Light(lightCapacity);
}

Scene* Scene_CreateEmpty()
{
	Scene* scene = (Scene*)mgdl_AllocateGraphicsMemory(sizeof(Scene));
	scene->materials = nullptr;
	scene->meshes = nullptr;
	scene->lights = nullptr;
	scene->ufbx = nullptr;
	return scene;
}

void Scene_SetUFBX(Scene* scene, ufbx_scene* ufbx)
{
	scene->ufbx = ufbx;
}

static Mesh* m_GetMeshById(Scene* scene, uint32_t mesh_id)
{
	for(sizetype i = 0; i < PointerArray_Count(scene->meshes); i++)
	{
		Mesh* mptr = PointerArray_Get_Mesh(scene->meshes, i);
		if (mptr != nullptr)
		{
			if(mptr->ufbx_id == mesh_id)
			{
				return mptr;
			}
		}
	}
	return nullptr;
}

static Material* m_GetMaterialById(Scene* scene, uint32_t material_id)
{

	for(sizetype i = 0; i < PointerArray_Count(scene->materials); i++)
	{
		Material* mptr = PointerArray_Get_Material(scene->materials, i);
		if (mptr != nullptr)
		{
			if(mptr->ufbx_id == material_id)
			{
				return mptr;
			}
		}
	}
	return nullptr;
}

static Light* m_GetLightById(Scene* scene, uint32_t light_id)
{
	for(sizetype i = 0; i < PointerArray_Count(scene->lights); i++)
	{
		Light* mptr = PointerArray_Get_Light(scene->lights, i);
		if(mptr->ufbx_id == light_id)
		{
			return mptr;
		}
	}
	return nullptr;
}

static void m_ApplyUFBXTransform(ufbx_transform* transform)
{
	ufbx_vec3 rotations = ufbx_quat_to_euler(transform->rotation, UFBX_ROTATION_ORDER_XYZ);
	glTranslatef(transform->translation.x, transform->translation.y, transform->translation.z);
	glRotatef(rotations.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rotations.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotations.z, 0.0f, 0.0f, 1.0f);
	glScalef(transform->scale.x, transform->scale.y, transform->scale.z);
}

static void s_DrawUFBXNode(Scene* scene, ufbx_node* node, bool animate, float elapsedSeconds)
{
	glPushMatrix();

	if (animate)
	{
		ufbx_transform animatedPos = ufbx_evaluate_transform(scene->ufbx->anim, node, elapsedSeconds);
		m_ApplyUFBXTransform(&animatedPos);
	}
	else
	{
		m_ApplyUFBXTransform(&node->local_transform);
	}

	if (node->mesh != nullptr)
	{
		uint32_t mesh_id = node->mesh->typed_id;
		Mesh* mesh = m_GetMeshById(scene, mesh_id);
		if (node->materials.count > 0)
		{
			ufbx_material* uMaterial = node->materials[0];
			Material* mMaterial = m_GetMaterialById(scene, uMaterial->typed_id);
			if (mMaterial != nullptr)
			{
				Material_Apply(mMaterial);

				// TODO Material apply also takes the mesh as parameter
				// to do matcap?
				if (mMaterial->type == MaterialType::Matcap)
				{
					// If material is matcap material
					GLfloat modelViewMatrix[16];
					glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
					Matrix modelView = MatrixFromGL(modelViewMatrix);
					// Copy the values
					// TODO

					// Different ways
					// Normal
					// transposed

					// Destination matrix is on the left, source on right
					Matrix inverseView = MatrixInvert(modelView);
					Matrix normalMatrix = MatrixTranspose(inverseView);

					Mesh_CalculateMatcapUVs(mesh, modelView, normalMatrix);
				}
			}
		}
		else
		{
			glColor3f(1.0f, 1.0f, 1.0f);
		}

		if (mesh != nullptr)
		{
			Mesh_DrawArrays(mesh);
		}
	}
	else if(node->light != nullptr)
	{
		Light* mLight = m_GetLightById(scene, node->light->typed_id);
		if (mLight != nullptr)
		{
			// NOTE
			// Lights are not affected by glTranslatef
			ufbx_vec3 p = node->local_transform.translation;
			Vector3 position = Vector3New(p.x, p.y, p.z);
			Light_SetPosition(mLight, position);
		}
	}

	for(size_t i = 0; i < node->children.count; i++)
	{
		s_DrawUFBXNode(scene, node->children[i]);
	}
	glPopMatrix();
}

void Scene_ApplyCamera(Scene* scene, u32 index)
{
	if (index < scene->ufbx->cameras.count)
	{
		ufbx_camera* camera = scene->ufbx->cameras.data[index];
		ufbx_node* cameraNode = camera->instances.data[0];
		ufbx_transform cameraTransform = cameraNode->local_transform;

		ufbx_vec3 forward_dir;
		forward_dir.x = 0.0f;
		forward_dir.y = 0.0f;
		forward_dir.z = -1.0f;
		ufbx_vec3 up_dir;
		up_dir.x = 0.0f;
		up_dir.y = 1.0f;
		up_dir.z = 0.0f;
		ufbx_vec3 cameraDir = ufbx_quat_rotate_vec3(cameraTransform.rotation, forward_dir);
		ufbx_vec3 cameraUp = ufbx_quat_rotate_vec3(cameraTransform.rotation, up_dir);

		mgdl_InitPerspectiveProjection(camera->field_of_view_deg.y, camera->near_plane, camera->far_plane);

		mgdl_InitCameraF(
			cameraTransform.translation.x,
			cameraTransform.translation.y,
			cameraTransform.translation.z,

			cameraTransform.translation.x + cameraDir.x,
			cameraTransform.translation.y + cameraDir.y,
			cameraTransform.translation.z + cameraDir.z,

			cameraUp.x,
			cameraUp.y,
			cameraUp.z
		);
	}
}

static bool s_BeginLights(Scene* scene)
{
	// If the scene has lights, enable lighting
	// and all the lights
	const bool hasLights = PointerArray_Count(scene->lights) > 0;
	if (hasLights)
	{
		mgdl_SetLightingEnabled(true);
		for (sizetype i = 0; i < PointerArray_Count(scene->lights); i++)
		{
			Light* l = PointerArray_Get_Light(scene->lights, i);
			Light_Apply(l);
		}
	}
	return hasLights;
}

static void s_EndLights(Scene* scene)
{
	for (sizetype i = 0; i < PointerArray_Count(scene->lights); i++)
	{
		// TODO Do we need to disable the lights?
		// Light* l = PointerArray_GetLight(scene->lights, i);
	}
	mgdl_SetLightingEnabled(false);
}

// NOTE
// New system. Store minimal amount of information
// use the ufbx scene for everything else
void Scene_Draw(Scene* scene)
{

	bool lightsOn = s_BeginLights(scene);

	ufbx_node* root = scene->ufbx->root_node;
	s_DrawUFBXNode(scene, root, false, 0.0f);

	if (lightsOn)
	{
		s_EndLights(scene);
	}
}

void Scene_DrawAnimated(Scene* scene, float elapsedSeconds)
{

	bool lightsOn = s_BeginLights(scene);

	ufbx_node* root = scene->ufbx->root_node;
	s_DrawUFBXNode(scene, root, true, elapsedSeconds);

	if (lightsOn)
	{
		s_EndLights(scene);
	}

}

void Scene_AddMaterial ( Scene* scene, Material* material )
{
	PointerArray_Add_Material(scene->materials, material);
}

void Scene_AddLight(Scene* scene, Light* light)
{
	Log_Info("Scene got light\n");
	Light_LogInfo(light);
	PointerArray_Add_Light(scene->lights, light);
}

void Scene_AddMesh(Scene* scene, Mesh* mesh)
{
	PointerArray_Add_Mesh(scene->meshes, mesh);
}

bool Scene_HasMaterial(Scene* scene, uint32_t ufbx_id)
{
	return (m_GetMaterialById(scene, ufbx_id) != nullptr);

}
bool Scene_HasMesh(Scene* scene, uint32_t ufbx_id)
{
	return (m_GetMeshById(scene, ufbx_id) != nullptr);
}


void Scene_SetMaterialTexture (Scene* scene, uint32_t ufbx_id, Texture* texture )
{
	Material* m = m_GetMaterialById(scene, ufbx_id);
	if (m != nullptr)
	{
		m->texture = texture;
	}
	else
	{
		Log_ErrorF("No material found with id %d\n", ufbx_id);
		return;
	}
}

void Scene_SetAllMaterialTextures (Scene* scene, Texture* texture )
{
	for(sizetype i = 0; i < PointerArray_Count(scene->materials); i++)
	{
		Material* m = PointerArray_Get_Material(scene->materials, i);
		m->texture = texture;
	}
}

void Scene_LogInfo(Scene* scene)
{
	Console_SetTextColor(ConsoleText_Green);
	Log_Info("Scene Info\n");
	Console_ResetTextColor();
	Log_Info("Lights:\n");
		for (sizetype i = 0; i < PointerArray_Count(scene->lights); i++)
		{
			Light* l = PointerArray_Get_Light(scene->lights, i);
			Light_LogInfo(l);
		}
}
