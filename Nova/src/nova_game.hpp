#if !defined(NOVA_GAME_HPP)
#define NOVA_GAME_HPP

#include "NeoInfused/NeoInfused.hpp"
#undef CreateWindow

namespace Nova {
	inline Neo::Logger<> g_Logger{"Nova", &std::clog};

	struct GameData {
		Neo::App app;
		Neo::Window window;
		Neo::Input input;

		Influx::Renderer renderer;
		Influx::Pipeline pipeline;

		Influx::VertexBuffer vbo1;
		Influx::IndexBuffer ibo1;
		Influx::UniformBuffer ubo1;

		Influx::Texture texture;
	};

	class Game {
	public:
		static void Create(void);
		static void CreateWindow(uint32_t width, uint32_t height, const std::string_view& title);
		static void CreateRenderer(void);
		static void Run(void);
		static void DestroyRenderer(void);
		static void DestroyWindow(void);
		static void Destroy(void);
	private:
		static void _OnEvent(Neo::Event& e);
		static void _Update(double dt);
	private:
		static inline GameData* s_Data = nullptr;
	};
} // namespace Nova

#endif // NOVA_GAME_HPP