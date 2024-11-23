#include "nova_pch.hpp"
#include "nova_app.hpp"

namespace nova {
	static app_t* app = nullptr;
	app_t& app_t::Get(void) { return *app; }

	app_t::app_t(void)
	: window(1280, 720, "Neo-Infused")
	{
		NEO_ASSERT(!app, "Cannot create multiple instances of App!");
		app = this;
	}
	app_t::~app_t(void) noexcept(false) { app = nullptr; }

	void app_t::run(void)
	{
		while (window)
		{
			for (auto it = layers.begin(); it != layers.end(); it++)
				if ((*it)->state & NEO_LAYERSTATE_UPDATABLE)
					(*it)->update();

			for (auto it = layers.rbegin(); it != layers.rend(); it++)
				if ((*it)->state & NEO_LAYERSTATE_VISIBLE)
					(*it)->draw();

			for (neo::event_t& e : neo::event::poll())
			{
				if (e.type == NEO_WINDOW_CLOSE_EVENT)
				{
					window.destroy();
					return;
				}

				for (neo::layer_t* layer : layers)
				{
					if (!(layer->state & NEO_LAYERSTATE_ENABLED))
						continue;

					layer->on_event(e);

					if (e.handled)
						break;
				}
			}
		}
	}
} // namespace nova
