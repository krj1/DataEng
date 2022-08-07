#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;
		
		if (m_Image)
		{
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });
		}
		ImGui::End();
		ImGui::PopStyleVar();
		

	}

	void Render()
	{
		if (!m_Image || m_viewportWidth != m_Image->GetWidth() || m_viewportHeight != m_Image->GetHeight())
		{
			m_Image = std::make_shared<Image>(m_viewportWidth, m_viewportHeight, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_viewportWidth * m_viewportHeight];
		}

		for (uint32_t i = 0; i < m_viewportWidth * m_viewportHeight; i++)
		{
			if (i < m_viewportWidth * m_viewportHeight)
			{
				m_ImageData[i] = 0xff00ffff;
			}
			else
			{
				m_ImageData[i] = 0xffff00ff;
			}


		}

		m_Image->SetData(m_ImageData);
	}



private:
	std::shared_ptr<Image> m_Image;
	uint32_t* m_ImageData = nullptr;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Data Engine";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}