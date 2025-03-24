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

		s_Data->app.add_system((void*)Game::_Update, Neo::App_SystemType::Update);
		s_Data->app.add_system((void*)Game::_OnEvent, Neo::App_SystemType::OnEvent);

		g_Logger(Neo::Info, "Hello, world!");

		Influx::Initialize();
	}

	void Game::CreateWindow(uint32_t width, uint32_t height, const std::string_view& title)
	{
		new (&s_Data->window) Neo::Window(width, height, title);
	}

	struct Vertex {
		glm::vec2 pos;
		glm::vec4 color;
	};

	Vertex VBO1[3] = {
		{ { 0.0f, -0.5f}, { 0.9f, 0.2f, 0.2f, 1.0f } },
		{ { 0.5f,  0.5f}, { 0.2f, 0.9f, 0.2f, 1.0f } },
		{ {-0.5f,  0.5f}, { 0.2f, 0.2f, 0.9f, 1.0f } }
	};

	Vertex VBO2[6] = {
		{ {-0.5f, -0.5f}, { 0.9f, 0.2f, 0.2f, 1.0f } },
		{ { 0.5f, -0.5f}, { 0.2f, 0.9f, 0.2f, 1.0f } },
		{ { 0.5f,  0.5f}, { 0.2f, 0.2f, 0.9f, 1.0f } },
		{ {-0.5f, -0.5f}, { 0.9f, 0.2f, 0.2f, 1.0f } },
		{ {-0.5f,  0.5f}, { 0.2f, 0.9f, 0.2f, 1.0f } },
		{ { 0.5f,  0.5f}, { 0.2f, 0.2f, 0.9f, 1.0f } }
	};

	Vertex VBO3[3] = {
		{ {-0.5f, -0.5f}, { 0.9f, 0.2f, 0.2f, 1.0f } },
		{ {-0.5f,  0.5f}, { 0.2f, 0.9f, 0.2f, 1.0f } },
		{ { 0.5f,  0.5f}, { 0.2f, 0.2f, 0.9f, 1.0f } }
	};

	void Game::CreateRenderer(void)
	{
		new (&s_Data->renderer) Influx::Renderer(s_Data->window.native());

		Influx::Shader vertex(ShaderDir() / "vertex.glsl", Influx::Shader_Stage::Vertex);
		Influx::Shader fragment(ShaderDir() / "fragment.glsl", Influx::Shader_Stage::Fragment);

		new (&s_Data->pipeline) Influx::Pipeline(
			s_Data->renderer,
			{ vertex, fragment },
			{
				{ 0, Influx::VertexAttribute_Type::Vec2 },
				{ 1, Influx::VertexAttribute_Type::Vec4 }
			}
		);
		s_Data->renderer.bind_pipeline(s_Data->pipeline);

		new (&s_Data->vbo1) Influx::VertexBuffer(VBO1, 3, sizeof(Vertex));
		new (&s_Data->vbo2) Influx::VertexBuffer(VBO2, 6, sizeof(Vertex));
		new (&s_Data->vbo3) Influx::VertexBuffer(VBO3, 3, sizeof(Vertex));
	}

	void Game::_OnEvent(Neo::Event& e)
	{
		s_Data->input.on_event(e);
		switch (e.type)
		{
		case Neo::Event_Type::WindowClosed:
			s_Data->app.should_close = true;
			break;
		case Neo::Event_Type::WindowResized:
			s_Data->renderer.update_size();
			break;
		}
	}

	static uint32_t buffer = 1;
	void Game::_Update(double dt)
	{
		//g_Logger.fmt(Neo::Trace, "{}fps", floor(1000.0 / dt));
		
		//s_Data->renderer.set_viewport({ 0.0f, 0.0f, (float)s_Data->window.width(), (float)s_Data->window.height()});
		s_Data->renderer.clear({ 0.15f, 0.15f, 0.2f, 1.0f });

		VBO1[0].pos.y = s_Data->window.height() / s_Data->input.mouse_y();
		//s_Data->vbo1.set_memory(VBO1);

		s_Data->renderer.bind_vbo(s_Data->vbo1);
		s_Data->renderer.present();
	}

	void Game::Run(void)
	{
		s_Data->app.run();
	}

	void Game::DestroyRenderer(void)
	{
		s_Data->vbo3.~VertexBuffer();
		s_Data->vbo2.~VertexBuffer();
		s_Data->vbo1.~VertexBuffer();
		s_Data->pipeline.~Pipeline();
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
		free(s_Data);
		s_Data = nullptr;
	}
} // namespace Nova