#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "TestTree2.h"

namespace test {

	TestTree2::TestTree2()
		: m_CubeObject("res/models/cube.obj"), m_Shader("res/shaders/basic_light.shader"),
		m_BaseTranslation(-10.0f, 0.0f, 0.0f),
		m_BaseRotation(1.0f, 1.0f, 1.0f),
		m_BaseScale(0.5f, 0.5f, 0.5f),
		m_LightPosition(0.0f, 10.0f, 40.0f), m_CameraPosition(0.0f, 10.0f, 60.0f),
		m_Cube(m_CubeObject), m_BaseAngle(1.0f)
	{

		// Camera parameters for view transform
		glm::vec3 cameraLookAt(0.0f, -0.1f, -1.0f);
		glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

		// Set projection matrix for shader, this won't change
		m_Proj = glm::perspective(70.0f,            // field of view in degrees
			16.0f / 9.0f,  // aspect ratio
			0.01f, 100.0f);   // near and far (near > 0)

		// Set initial view matrix
		m_View = lookAt(m_CameraPosition,  // eye
			m_CameraPosition + cameraLookAt,  // center
			cameraUp); // up

		GLCall(glEnable(GL_CULL_FACE));
		glEnable(GL_DEPTH_TEST);

		m_Shader.Bind();

		m_complexModel = new ComplexModel(m_Shader);
		m_complexModel->setRotation(45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		m_complexModel->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
		m_complexModel->computeModelMatrix();

		m_Tree2 = new Tree2(m_Shader, m_CubeObject);
	}

	TestTree2::~TestTree2()
	{
	}

	void TestTree2::OnUpdate(float deltaTime)
	{
	}

	void TestTree2::OnRender()
	{
		GLCall(glClearColor(0.502f, 0.502f, 0.502f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		Renderer renderer;

		glm::vec3 cameraLookAt(0.0f, -0.1f, -1.0f);
		glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
		// Set initial view matrix
		m_View = lookAt(m_CameraPosition,  // eye
			m_CameraPosition + cameraLookAt,  // center
			cameraUp); // up


		m_Shader.Bind();
		m_Shader.SetUniformMat4f("u_Projection", m_Proj);
		m_Shader.SetUniformMat4f("u_View", m_View);
		m_Shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		m_Shader.SetUniform3fv("u_LightPos", m_LightPosition);
		m_Shader.SetUniform3fv("u_ViewPos", m_CameraPosition);

		// m_complexModel->setTransforms(m_BaseTranslation, m_BaseAngle, m_BaseRotation, m_BaseScale);

		// m_complexModel->draw();

		m_Tree2->draw();
	}

	void TestTree2::OnImGuiRender()
	{
		ImGui::SliderFloat3("Camera", &m_CameraPosition.x, -50.0f, 50.0f);
		ImGui::SliderFloat3("Light", &m_LightPosition.x, -50.0f, 50.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}