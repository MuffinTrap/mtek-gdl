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
