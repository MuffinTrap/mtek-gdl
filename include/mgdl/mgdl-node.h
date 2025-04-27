
#pragma once
#include <vector>
#include <string>
#include <mgdl/mgdl-transform.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-material.h>
#include <mgdl/mgdl-light.h>

namespace gdl
{
	class Node
	{
	public:
		Transform transform;
		gdl::Mesh* mesh = nullptr;
		gdl::Material* material = nullptr;
		gdl::Light* light = nullptr;
		const char* name;
		std::vector<Node*> children;

		Node();
		Node(const char* name, vec3 position, vec3 rotationAngles);
		Node(const char* name, gdl::Mesh* meshParam, gdl::Material* materialParam);

		void Draw();
	};
}
