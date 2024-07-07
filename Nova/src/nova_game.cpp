#include "nova_pch.hpp"
#include "nova_game.hpp"

#include <NeoInfused/core/neo_event.hpp>

namespace nova {
    Game::Game(void) {
        m_Window = neo::Window::New(1280, 720, "Neo-Infused Cybernetic Endeavors");
        m_Window->on_event = [this](const neo::Event* e) {
            if (e->type() == neo::EventType::WindowResize) {
                NEO_CONTEXT()->set_viewport(
                    m_Window,
                    ((neo::WindowResizeEvent*)e)->width(),
                    ((neo::WindowResizeEvent*)e)->height()
                );
            }
            for (auto layer : m_Layers) {
                if (e->handled) break;
                if ((layer->state & NEO_LAYERSTATE_ENABLED) != NEO_LAYERSTATE_ENABLED) continue;
                layer->on_event(e);
            }
        };

        m_Layers.create<MainLayer>(5000);
    }
    Game::~Game(void) noexcept(false) {
        neo::Window::Delete(m_Window);
    }
    void Game::loop(void) {
        while (!m_Window->should_close) {
            neo::PollEvents();

            for (auto layer : m_Layers)
                if ((layer->state & NEO_LAYERSTATE_UPDATABLE) == NEO_LAYERSTATE_UPDATABLE)
                    layer->update();

            NEO_CONTEXT()->new_frame(m_Window, {0.5f, 0.5f, 1.0f, 1.0f});
            for (auto layer : m_Layers) 
                if ((layer->state & NEO_LAYERSTATE_VISIBLE) == NEO_LAYERSTATE_VISIBLE)
                    layer->draw();

            NEO_CONTEXT()->present(m_Window);
        }
    }
} // namespace nova
