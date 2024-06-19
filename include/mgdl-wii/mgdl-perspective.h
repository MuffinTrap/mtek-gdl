/*! \file mgdl-perspective.h
 * 	\brief Module for drawing 3D models in perspective
 */
#pragma once
#include <vector>
#include <ogc/gu.h>
#include "mgdl-wii/mgdl-image.h"
#include "mgdl-wii/mgdl-globals.h"


namespace gdl {
	static const guVector RIGHT = {1.0f, 0.0f, 0.0f};
	static const guVector UP = {0.0f, 1.0f, 0.0f};
	static const guVector FORWARD = {0.0f, 0.0f, -1.0f};

	struct MatrixContainer
	{
		Mtx m;
	};

	class Camera
	{
	public:
		guVector position;
		guVector direction;
	};

	class Perspective
	{
	public:
		static void InitMatrixStack();
		static void SetDefaultVertexAttributes();
		static void SetTexture(gdl::Image& texture);

		// Controls for the default camera
		static void InitDefaultCamera();
		static Camera& GetDefaultCamera();
		static void ApplyDefaultCamera();
	private:
	static std::vector<MatrixContainer> modelMatrixStack;
	static int stackTop;
	static Camera defaultCamera;

	};
};

