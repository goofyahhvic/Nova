#if !defined(NOVA_APP_HPP)
#define NOVA_APP_HPP

#include "NeoInfused/NeoInfused.hpp"

namespace nova {
	class app_t {
	public:
		app_t(void);
		~app_t(void) noexcept(false);
		void run(void);

		[[nodiscard]] static app_t& Get(void);
	public:
		neo::window_t window;
		neo::layer::storage_t layers;
	};
}

#endif // NOVA_APP_HPP