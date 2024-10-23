#include "nova_pch.hpp"
#include "nova_mainlayer.hpp"

namespace nova {
	main_layer_t::main_layer_t(int32_t priority, uint8_t state)
	: neo::layer_t(priority, state)
	{
		m_Input.reset();
	}
	main_layer_t::~main_layer_t(void)
	{
	
	}

	void main_layer_t::on_event(neo::event_t& e)
	{
		m_Input.on_event(e);
	}
	void main_layer_t::update(void)
	{
		if (m_Input.mouse_button(NEO_MOUSE_BUTTON_1))
		{
			NEO_TRACE_LOG("Button");
		}
	}
	void main_layer_t::draw(void)
	{
		
	}
} // namespace nova
