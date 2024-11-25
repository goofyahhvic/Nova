#include "nova_pch.hpp"
#include "nova_core-system-set.hpp"

namespace nova {
	namespace system_set {
		core_t::core_t(neo::app_t* app)
		: window(1280, 720, "Voxploration"), app(app)
		{
			s_This = this;
		}

		void core_t::OnEvent(neo::event_t& e)
		{
			s_This->input.on_event(e);
			if (e.type == NEO_WINDOW_CLOSE_EVENT)
			{
				s_This->window.destroy();
				s_This->app->should_close = true;
			} else
			if (e.type == NEO_MOUSE_BUTTON_PRESSED_EVENT)
				NEO_TRACE_LOG("Button pressed!");
		}
	} // namespace system_sets
} // namespace nova
