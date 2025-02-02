
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
		std::string name;
		std::vector<Node*> children;

		Node();
		Node(std::string name, gdl::vec3 position, gdl::vec3 rotationAngles);
		Node(std::string name, gdl::Mesh* meshParam, gdl::Material* materialParam);
	};
}
