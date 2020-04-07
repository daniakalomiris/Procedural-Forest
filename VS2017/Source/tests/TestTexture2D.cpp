#pragma once

#include "TestTexture2D.h"

namespace test {
	
	TestTexture2D::TestTexture2D()
		: m_TranslationA(200.0f, 200.0f, 0.0f), m_TranslationB(400.0f, 200.0f, 0.0f)
	{
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			- 50.0f,  50.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		


		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);  // position
		layout.Push<float>(2);  // texture uv
		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices)/sizeof(unsigned int));

		m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);
		m_Texture = std::make_unique<Texture>("res/textures/autumn_leaves.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}
	
	TestTexture2D::~TestTexture2D()
	{
	}
	
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);


		model = glm::translate(glm::mat4(1.0f), m_TranslationB);
		mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}
	
	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}