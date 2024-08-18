#include "nova_pch.hpp"
#include "nova_mainlayer.hpp"

namespace nova {
	MainLayer::MainLayer(int32_t priority, uint8_t state)
	: neo::Layer(priority, state)
	{
		m_Input.reset();
		neo::GetScenes().create_scene<neo::Scene>(50);
		inf::init();
		inf::hello_world();
	}
	MainLayer::~MainLayer(void)
	{
	
	}

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
		
	}
} // namespace nova
