#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "Content\CubeBox.h"
#include "Content\Snowman.h"
#include "Content\Plane.h"
#include "Content\SkyBox.h"
#include "Model.h"
#include "InputClass.h"
#include "MoveLookController.h"
#include <vector>


namespace D3D11Homework
{
	// 场景渲染
	struct ModelStruct
	{
		std::unique_ptr<Model> model;
		std::string type;
		ModelStruct(std::unique_ptr<Model> model, std::string type) :model(std::move(model)), type(type) {}
	};

	class SceneRenderer
	{
	public:
		SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void Update(DX::StepTimer const& timer);
		void Render();
		void Add(std::string type, std::unique_ptr<Model> model);
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		void ReleaseDeviceDependentResources();
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		
	private:
		std::vector<ModelStruct> m_modelManager; //模型管理器
		Agile<MoveLookController> m_moveLookController;
	};
}

