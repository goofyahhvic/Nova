#if !defined(NOVA_CORE_SYSTEM_SET_HPP)
#define NOVA_CORE_SYSTEM_SET_HPP

namespace nova {
	namespace system_set {
		class core_t {
		public:
			core_t(neo::app_t* app);
			inline ~core_t(void) { s_This = nullptr; }

			static void OnEvent(neo::event_t& e);
		public:
			neo::app_t* app;
			neo::window_t window;
			neo::input_t input;
		private:
			static inline core_t* s_This = nullptr;
		};
	} // namespace system_sets
} // namespace nova

#endif // NOVA_CORE_SYSTEM_SET_HPP