#include <mgdl/mgdl-fbx.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-scene.h>
#include <stdio.h>

bool gdl::FBXFile::LoadFile(std::string fbxFile)
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
		return false;
	}

	// What is in this file?

	for (ufbx_node* node : scene->nodes)
	{
		if (node->is_root)
		{
			// This is the root node
			continue;
		}
		printf("Node: %s\n", node->name.data);

		printf("Transform:");
		ufbx_vec3 t = node->local_transform.translation;
		ufbx_vec3 r = node->euler_rotation;
		printf("position: (%.2f, %.2f, %.2f)", t.x, t.y, t.z);
		printf("rotation: (%.2f, %.2f, %.2f)", r.x, r.y, r.z);
		printf("\n");

		size_t childAmount = node->children.count;
		printf("\t%zu children\n", childAmount);

		ufbx_mesh* mesh = node->mesh;
		if (mesh != nullptr)
		{
			printf("\tMesh %s with %zu faces\n", mesh->name.data, mesh->faces.count);
			if (mesh->vertex_normal.exists)
			{
				printf("\t%zu normals\n", mesh->vertex_normal.values.count);
			}
			if (mesh->vertex_uv.exists)
			{
				printf("\t%zu uvs\n", mesh->vertex_uv.values.count);
			}
			continue;
		}
		ufbx_light* light = node->light;
		if (light != nullptr)
		{
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

			continue;
		}
		ufbx_camera* camera = node->camera;
		if (camera != nullptr)
		{
			printf("\tCamera %s\n", camera->name.data);
			continue;
		}
		ufbx_bone* bone = node->bone;
		if (bone != nullptr)
		{
			printf("\tBone %s, radius: %.2f relative length: %.2f\n", bone->name.data, bone->radius, bone->relative_length);
			continue;
		}
	}

	return true;
}

ufbx_mesh * gdl::FBXFile::GetMesh ( int index )
{
	int meshIndex = 0;
	for (ufbx_node* node : scene->nodes)
	{
		if (node->is_root)
		{
			// This is the root node
			continue;
		}
		printf("Node: %s\n", node->name.data);

		printf("Transform:");
		ufbx_vec3 t = node->local_transform.translation;
		ufbx_vec3 r = node->euler_rotation;
		printf("position: (%.2f, %.2f, %.2f)", t.x, t.y, t.z);
		printf("rotation: (%.2f, %.2f, %.2f)", r.x, r.y, r.z);
		printf("\n");

		size_t childAmount = node->children.count;
		printf("\t%zu children\n", childAmount);

		ufbx_mesh* mesh = node->mesh;
		if (mesh != nullptr)
		{
			if (index == meshIndex)
			{
				printf("\tFound %d: %s\n", index, mesh->name.data);
				return mesh;
			}
			else
			{
				meshIndex++;
			}
		}
	}
	return nullptr;
}


gdl::Mesh * gdl::FBXFile::LoadMesh(ufbx_mesh* fbxMesh)
{
	gdl::Mesh *mesh = new gdl::Mesh();

	// Calculate how many indices
	for(ufbx_face face : fbxMesh->faces)
	{
		bool triangulate = face.num_indices == 4;
		if (triangulate)
		{
			mesh->indexCount += 6;
		}
		else
		{
			mesh->indexCount += 3;
		}
	}
	mesh->indices = new GLushort[mesh->indexCount];
	printf("LoadMesh: %d indices\n", mesh->indexCount);

	// Get indices
	size_t ii = 0;
	GLushort indices[4];
	for(ufbx_face face : fbxMesh->faces)
	{
		for(uint32_t corner = 0; corner < face.num_indices; corner++)
		{
			// The face index points to vertex_indices array
			// Read the actual indices
			uint32_t index = face.index_begin + corner;
			indices[corner] = static_cast<GLushort>(fbxMesh->vertex_indices[index]);
		}

		// Create 1 or 2 triangles


		// First triangle
		/*   2 ---1
			*    \   |
			*     \  |
			*      \ |
			*       \|
			*        0
			*/
		mesh->indices[ii] = indices[0];
		ii++;
		mesh->indices[ii] = indices[1];
		ii++;
		mesh->indices[ii] = indices[2];
		ii++;

		bool triangulate = face.num_indices == 4;
		if (triangulate)
		{
			// Second triangle
			/*   2
			 *   |\
			 *   | \
			 *   |  \
			 *   |   \
			 *   3----0
			 */

			mesh->indices[ii] = indices[0];
			ii++;
			mesh->indices[ii] = indices[2];
			ii++;
			mesh->indices[ii] = indices[3];
			ii++;
		}
	}

	// Indices read
	printf("LoadMesh indices read \n");


	// Read vertices
	{
		mesh->vertexCount = fbxMesh->vertices.count;
		mesh->positions = new GLfloat[fbxMesh->vertices.count * 3];
		size_t vpi = 0;
		for (uint32_t pi = 0; pi < fbxMesh->vertices.count; pi++)
		{
			ufbx_vec3 v = fbxMesh->vertices[pi];
			mesh->positions[vpi+0] = v.x;
			mesh->positions[vpi+1] = v.y;
			mesh->positions[vpi+2] = v.z;
			vpi += 3;
		}
	}

	// TODO The normals are indexed separately
	// not necessarily a normal per vertex
	if (fbxMesh->vertex_normal.exists)
	{
		size_t normalBytes = fbxMesh->vertex_normal.values.count * 3;
		size_t vni = 0;
		printf("LoadMesh read normals\n");
		mesh->normals = new GLfloat[normalBytes];
		for (uint32_t ni = 0; ni < fbxMesh->vertex_normal.values.count; ni++)
		{
			ufbx_vec3 n = fbxMesh->vertex_normal[ni];

			mesh->normals[vni+0] = n.x;
			mesh->normals[vni+1] = n.y;
			mesh->normals[vni+2] = n.z;
			vni += 3;
		}
	}

	// TODO The uvs are indexed separately
	// not necessarily a uv per vertex
	if (fbxMesh->vertex_uv.exists)
	{
		printf("LoadMesh read uvs\n");
		size_t vti = 0;
		mesh->uvs = new GLfloat[fbxMesh->vertex_uv.values.count * 3];
		for (uint32_t ti = 0; ti < fbxMesh->vertex_uv.values.count; ti++)
		{
			ufbx_vec2 t = fbxMesh->vertex_uv[ti];

			// Flip the y coordinates because in OpenGL images Y grows upwards
			float y = t.y;
			y -= 1.0f;
			y *= -1.0f;

			mesh->uvs[vti+0] = t.x;
			mesh->uvs[vti+1] = y;
			vti += 2;
		}
	}

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

