#include <mgdl/mgdl-node.h>

gdl::Node::Node()
{
	transform = gdl::Transform();

}

gdl::Node::Node ( std::string name, gdl::Mesh* meshParam, gdl::Material* materialParam )
{
	transform = gdl::Transform();
	this->name = name;
	mesh = meshParam;
	material = materialParam;
}
