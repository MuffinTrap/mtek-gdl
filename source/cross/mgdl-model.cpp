#include <mgdl/mgdl-model.h>
#include <mgdl/mgdl-memory.h>
#include <mgdl/mgdl-mesh.h>
#include <mgdl/mgdl-material.h>


	Model* Model_Create()
	{
		Model* model = (Model*)mgdl_AllocateGraphicsMemory(sizeof(Model));
		return model;
	}
	Model* Model_Destroy(Model* model)
	{
		mgdl_FreeGraphicsMemory(model);
		return nullptr;
	}

	void Model_Draw(Model* model)
	{
		if (model->m_material != nullptr)
		{
			Material_Apply(model->m_material);
		}
		Mesh_DrawArrays(model->m_mesh);
		if (model->m_material != nullptr)
		{
			Material_UnApply(model->m_material);
		}
	}
