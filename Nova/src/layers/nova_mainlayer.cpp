#include "nova_pch.hpp"
#include "nova_mainlayer.hpp"

enum novaComponentSlots {
    POSITION_COMPONENT = 0,
    SIZE_COMPONENT = 1,
    TEXTURE_COMPONENT = 2
};

namespace nova {
    static neo::Vertex vertices[3] = {
        { {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
        { { 0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
        { { 0.0f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} }
    };
    MainLayer::MainLayer(int32_t priority, uint8_t state)
    : neo::Layer(priority, state),
    m_Program({
        neo::gl::CreateShader({ neo::Shaders::default_vertex, GL_VERTEX_SHADER }),
        neo::gl::CreateShader({ neo::Shaders::default_fragment, GL_FRAGMENT_SHADER }) 
    }), m_Triangle(vertices) {
        m_Input.reset();
    }
    MainLayer::~MainLayer(void) {

    }

    void MainLayer::on_event(const neo::Event& e) {
        m_Input.on_event(e);
    }
    void MainLayer::update(void) {
        if (m_Input.key(NEO_KEY_0)) {
            m_Triangle.set_color( {0.0f, 0.0f, 1.0f, 1.0f},0);
        } else if (m_Input.key(NEO_KEY_1)) {
            m_Triangle.set_color( {0.6f, 0.0f, 0.460897f, 1.0f},0);
        } else if (m_Input.key(NEO_KEY_2)) {
            m_Triangle.set_color( {0.0f, 0.0f, 0.0f, 1.0f},0);
        }
    }
    void MainLayer::draw(size_t window_id) {
        m_Program.bind();
        m_Triangle.draw();
    }
} // namespace nova
