#if !defined(NOVA_MAIN_LAYER_HPP)
#define NOVA_MAIN_LAYER_HPP

#include <NeoInfused/NeoInfused.hpp>

namespace nova {
    class MainLayer : public neo::Layer {
    public:
        MainLayer(int32_t priority, uint8_t state = NEO_LAYERSTATE_ALL);
        ~MainLayer(void);
    public:
        void on_event(const neo::Event& e) override;
        void update(void) override;
        void draw(size_t window_id) override;
    private:
        neo::Input m_Input;
        std::unique_ptr<neo::ShaderProgram> m_Program;
        std::unique_ptr<neo::VertexBuffer> m_VertexBuffer;
        std::unique_ptr<neo::IndexBuffer> m_IndexBuffer;
    };
} // namespace nova

#endif // NOVA_MAIN_LAYER_HPP