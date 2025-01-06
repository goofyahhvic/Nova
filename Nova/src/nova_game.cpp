#include "nova_pch.hpp"
#include "nova_game.hpp"

namespace nova {
	static const std::filesystem::path& ShaderDir(void)
	{
		static std::filesystem::path shader_dir = std::filesystem::path(neo::ExecDir() / "../../../NeoInfused/Influx/shaders/").lexically_normal();
		return shader_dir;
	}

	struct game_t {
		neo::App app;
		neo::Window window;
		neo::Input input;
		inf::Renderer renderer;
	};
	game_t* game = nullptr;

	void Game_OnEvent(neo::Event& e);
	void Game_Update(double dt);

	void Game_Create(void)
	{
		if (game)
			exit(-1);

		game = neo::tmalloc<game_t>(1);
		neo::InitCore();

		new (&game->app) neo::App();

		game->app.add_system(neo::App_SystemType::Update, Game_Update);
		game->app.add_system(neo::App_SystemType::OnEvent, Game_OnEvent);
	}

	void Game_CreateWindow(uint32_t width, uint32_t height, const char* title)
	{
		new (&game->window) neo::Window(width, height, title);
	}

	void Game_CreateRenderer(void)
	{
		new (&game->renderer) inf::Renderer(game->window);

		inf::Shader vertex(ShaderDir() / "vertex.glsl", inf::Shader_Stage::Vertex);
		inf::Shader fragment(ShaderDir() / "fragment.glsl", inf::Shader_Stage::Fragment);

		game->renderer.attach_shaders({ vertex, fragment });
		game->renderer.init();
	}

	void Game_OnEvent(neo::Event& e)
	{
		game->input.on_event(e);
		if (e.type == neo::Event_Type::WindowClosed)
			game->app.should_close = true;
	}

	void Game_Update(double dt)
	{
		std::cout << (uint16_t)(1000.0 / dt) << "fps\n";
	}

	void Game_Run(void)
	{
		game->app.run();
	}

	void Game_DestroyRenderer(void)
	{
		game->renderer.~Renderer();
	}

	void Game_DestroyWindow(void)
	{
		game->window.~Window();
	}

	void Game_Destroy(void)
	{
		Game_DestroyRenderer();
		Game_DestroyWindow();

		game->app.~App();
		neo::ShutdownCore();
		free(game);
		game = nullptr;
	}
} // namespace nova