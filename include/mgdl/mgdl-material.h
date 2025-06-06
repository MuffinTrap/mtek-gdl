#pragma once

#include <string>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-image.h>

namespace gdl
{

	/* Material used by a node. Contains a
	 * pointer to Image and material properties
	 * used in rendering
	 */
	class Material
	{
	public:

		std::string name;
		GLfloat shininess = 1.0f;
		vec3 emissiveColor;
		gdl::Image* texture;
		gdl::MaterialType type;
		Material();
		Material(std::string name);
		Material(std::string name, gdl::Image* texture, gdl::MaterialType type);
		void Apply();
	};
}
