#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include <iostream>
#include "Walnut/Image.h"
#include <cstdlib>

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
		Render();
		
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
			delete[] overlay;
			m_ImageData = new uint32_t[m_viewportWidth * m_viewportHeight];
			overlay = m_Overlay(m_viewportWidth, m_viewportHeight);
		}

		for (uint32_t i = 0; i < m_viewportWidth * m_viewportHeight; i++)
		{
			if (overlay[i] != 0)
			{
				m_ImageData[i] = overlay[i];
				//std::cout << "test";
			}
			else
			{
				m_ImageData[i] = 0xffff0000;
			}
		}

		m_Image->SetData(m_ImageData);
	}

	uint32_t* m_Overlay(uint32_t overlayWidth, uint32_t overlayHeight)
	{
		uint32_t* out = new uint32_t[overlayWidth * overlayHeight];
		/*
		for (uint32_t i = 0; i < overlayHeight; i++)
		{
			for (uint32_t j = 0; j < overlayWidth; j++)
			{
				if (j % 40 < 3)
				{
					out[i * j] = 0xffffffff;
				}
				//if (j % 40 < 3)
				//{
				//	out[i * j] = 0xffffffff;
				//}
				else
				{

					out[i*j] = 0x00000000;
				}
			}
		}
		*/
		
		for (uint32_t i = 0; i < overlayWidth * overlayHeight; i++)
		{

			if (i / overlayWidth % 40 < 1) // dream to use one if statement but had bugs.  
			{
				out[i] = 0xffffffff;
			}
			else if ((i - (overlayWidth * (i / overlayWidth))) % 40 < 1) //i % 40 < 1 && i < overlayWidth * overlayHeight / 2
			{
				out[i] = 0xffffffff; //((i / overlayWidth) * (overlayWidth % 40) + i)
			}
			else if (out[i] == 0xffffffff)
			{
				continue;
			}
			else
			{
				out[i] = 0x00000000;
			}
		}


		
		return out;
	}



private:
	std::shared_ptr<Image> m_Image;
	uint32_t* m_ImageData = nullptr;
	uint32_t* overlay = nullptr;
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