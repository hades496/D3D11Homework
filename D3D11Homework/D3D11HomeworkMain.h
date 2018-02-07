#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\SampleFpsTextRenderer.h"
#include "Content\SceneRenderer.h"

// 在屏幕上呈现 Direct2D 和 3D 内容。
namespace D3D11Homework
{
	class D3D11HomeworkMain : public DX::IDeviceNotify
	{
	public:
		D3D11HomeworkMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~D3D11HomeworkMain();
		void CreateWindowSizeDependentResources();
		void Start();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// 缓存的设备资源指针。
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: 替换为你自己的内容呈现器。
		std::unique_ptr<SceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		DirectionalLight m_dirLight;
		PointLight m_pointLight;
		SpotLight m_spotLight;
		Material m_cubeboxMat;
		Material m_sphereMat;
		
		// 渲染循环计时器。
		DX::StepTimer m_timer;
	};
}