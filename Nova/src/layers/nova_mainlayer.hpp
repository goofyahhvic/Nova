#if !defined(NOVA_LAYERS_HPP)
#define NOVA_LAYERS_HPP

#include <NeoInfused/NeoInfused.hpp>

namespace nova {
    class MainLayer : public neo::Layer {
    public:
        MainLayer(int32_t priority, uint8_t state = NEO_LAYERSTATE_ALL);
        ~MainLayer(void) = default;
    public:
        void on_event(const neo::Event* e) override;
        void update(void) override;
        void draw(void) override;
    private:
        neo::Input m_Input;
        neo::ShaderProgram m_Program;
        neo::Triangle m_Triangle;
    };
} // namespace nova

#endif // NOVA_LAYERS_HPP