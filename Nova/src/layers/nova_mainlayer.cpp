#include "nova_pch.hpp"
#include "nova_mainlayer.hpp"

namespace nova {
	struct Vertex {
		glm::vec3 pos;
		glm::vec4 color;
	};
	static Vertex vertices[3]{
		{ {-0.5f, -0.5f, 0.0f}, { 0.9f, 0.4f, 0.4f, 1.0f } },
		{ { 0.5f, -0.5f, 0.0f}, { 0.4f, 0.9f, 0.4f, 1.0f } },
		{ { 0.0f,  0.5f, 0.0f}, { 0.4f, 0.4f, 0.9f, 1.0f } }
	};
	MainLayer::MainLayer(int32_t priority, uint8_t state)
	: neo::Layer(priority, state),
	m_Mesh(neo::Mesh::Create(
		vertices, 3 * sizeof(Vertex),
		{ { 3, neo::Type::Float }, { 4, neo::Type::Float } },
		{ 0, 1, 2 })
	), m_Program(neo::ShaderProgram::Create())
	{
		m_Input.reset();
		neo::GetScenes().create_scene<neo::Scene>(50);
	}
	MainLayer::~MainLayer(void)
	{}

	void MainLayer::on_event(const neo::Event& e)
	{
		m_Input.on_event(e);
	}
	void MainLayer::update(void)
	{
		if (m_Input.mouse_button(NEO_MOUSE_BUTTON_1))
		{
			NEO_TRACE_LOG("Button");
		}
	}
	void MainLayer::draw(void)
	{
		m_Program->bind();
		m_Mesh->draw();
	}
} // namespace nova
