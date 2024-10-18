#include <mgdl/mgdl-scene.h>

bool gdl::Scene::LoadFromFBX(gdl::FBXFile* fbx)
{
	ufbx_mesh* mesh = fbx->GetMesh(0);

	if (mesh != nullptr)
	{
		gdl::Model* mod = new gdl::Model();
		gdl::Mesh *loadedMesh = fbx->LoadMesh(mesh);
		if (loadedMesh != nullptr)
		{
			mod->AddMesh(loadedMesh);
			models.push_back(mod);
			return true;
		}
	}
	return false;
}
