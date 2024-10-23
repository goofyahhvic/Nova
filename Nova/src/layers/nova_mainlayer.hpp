#if !defined(NOVA_MAIN_LAYER_HPP)
#define NOVA_MAIN_LAYER_HPP

#include <NeoInfused/NeoInfused.hpp>
#include <influx/inf_loader.hpp>

namespace nova {
	class main_layer_t : public neo::layer_t {
	public:
		main_layer_t(int32_t priority, uint8_t state = NEO_LAYERSTATE_ALL);
		~main_layer_t(void);
	public:
		void on_event(neo::event_t& e) override;
		void update(void) override;
		void draw(void) override;
	private:
		neo::input_t m_Input;
	};
} // namespace nova

#endif // NOVA_MAIN_LAYER_HPP