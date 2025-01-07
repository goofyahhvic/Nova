#include "nova_pch.hpp"
#include "nova_game.hpp"

namespace Nova {
	static const std::filesystem::path& ShaderDir(void)
	{
		static std::filesystem::path shader_dir = std::filesystem::path(Neo::ExecDir() / "../../../NeoInfused/Influx/shaders/").lexically_normal();
		return shader_dir;
	}

	struct Game {
		Neo::App app;
		Neo::Window window;
		Neo::Input input;
		Influx::Renderer renderer;
	};
	Game* game = nullptr;

	void Game_OnEvent(Neo::Event& e);
	void Game_Update(double dt);

	void Game_Create(void)
	{
		if (game)
			exit(-1);

		game = Neo::tcalloc<Game>();
		Neo::InitCore();

		new (&game->app) Neo::App();

		game->app.add_system(Game_Update, Neo::App_SystemType::Update);
		game->app.add_system(Game_OnEvent, Neo::App_SystemType::OnEvent);

		g_Logger(Neo::Info, "Hello, world!");
	}

	void Game_CreateWindow(uint32_t width, uint32_t height, const char* title)
	{
		new (&game->window) Neo::Window(width, height, title);
	}

	void Game_CreateRenderer(void)
	{
		new (&game->renderer) Influx::Renderer(game->window);

		Influx::Shader vertex(ShaderDir() / "vertex.glsl", Influx::Shader_Stage::Vertex);
		Influx::Shader fragment(ShaderDir() / "fragment.glsl", Influx::Shader_Stage::Fragment);

		game->renderer.attach_shaders({ vertex, fragment });
		game->renderer.init();
	}

	void Game_OnEvent(Neo::Event& e)
	{
		game->input.on_event(e);
		if (e.type == Neo::Event_Type::WindowClosed)
			game->app.should_close = true;
	}

	void Game_Update(double dt)
	{
		g_Logger.fmt(Neo::Trace, "{}fps", floor(1000.0 / dt));
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
		Neo::ShutdownCore();
		Neo::free(game);
	}
} // namespace Nova