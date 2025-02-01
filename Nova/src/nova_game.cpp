#include "nova_pch.hpp"
#include "nova_game.hpp"

namespace Nova {
	static const std::filesystem::path& ShaderDir(void)
	{
		static std::filesystem::path shader_dir = std::filesystem::path(Neo::ExecDir() / "../../../NeoInfused/Influx/shaders/").lexically_normal();
		return shader_dir;
	}

	void Game::Create(void)
	{
		if (!s_Data)
			s_Data = Neo::tcalloc<GameData>();

		Neo::InitializeCore();

		new (&s_Data->app) Neo::App();

		s_Data->app.add_system(Game::_Update, Neo::App_SystemType::Update);
		s_Data->app.add_system(Game::_OnEvent, Neo::App_SystemType::OnEvent);

		g_Logger(Neo::Info, "Hello, world!");

		Influx::Initialize();
	}

	void Game::CreateWindow(uint32_t width, uint32_t height, const std::string_view& title)
	{
		new (&s_Data->window) Neo::Window(width, height, title);
	}

	void Game::CreateRenderer(void)
	{
		new (&s_Data->renderer) Influx::Renderer(&s_Data->window);

		Influx::Shader vertex(ShaderDir() / "vertex.glsl", Influx::Shader_Stage::Vertex);
		Influx::Shader fragment(ShaderDir() / "fragment.glsl", Influx::Shader_Stage::Fragment);

		s_Data->renderer.attach_shaders({ vertex, fragment });
		s_Data->renderer.init();
	}

	void Game::_OnEvent(Neo::Event& e)
	{
		s_Data->input.on_event(e);
		if (e.type == Neo::Event_Type::WindowClosed)
			s_Data->app.should_close = true;
	}

	void Game::_Update(double dt)
	{
		g_Logger.fmt(Neo::Trace, "{}fps", floor(1000.0 / dt));
	}

	void Game::Run(void)
	{
		s_Data->app.run();
	}

	void Game::DestroyRenderer(void)
	{
		s_Data->renderer.~Renderer();
	}

	void Game::DestroyWindow(void)
	{
		s_Data->window.~Window();
	}

	void Game::Destroy(void)
	{
		Game::DestroyRenderer();
		Game::DestroyWindow();

		s_Data->app.~App();
		Influx::Shutdown();
		Neo::ShutdownCore();
		Neo::free(s_Data);
	}
} // namespace Nova