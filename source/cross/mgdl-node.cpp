#include <mgdl/mgdl-node.h>

gdl::Node::Node()
{

}

gdl::Node::Node ( std::string name, gdl::Mesh* meshParam, gdl::Material* materialParam )
{
	this->name = name;
	mesh = meshParam;
	material = materialParam;
}
