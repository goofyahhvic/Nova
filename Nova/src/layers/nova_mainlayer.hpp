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
		void draw(void) override;
	private:
		neo::Input m_Input;
	};
} // namespace nova

#endif // NOVA_MAIN_LAYER_HPP