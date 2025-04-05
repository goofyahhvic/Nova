#include "nova_pch.hpp"
#include "nova_game.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

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
		glm::vec3 pos;
		glm::vec3 color;
	};

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	Vertex VBO1[] = 
	{
		{ { 0.5f,  0.5f, 0.0f}, { 0.2f, 0.9f, 0.2f } },
		{ { 0.5f, -0.5f, 0.0f}, { 0.9f, 0.2f, 0.2f } },
		{ {-0.5f, -0.5f, 0.0f}, { 0.2f, 0.2f, 0.9f } },
		{ {-0.5f,  0.5f, 0.0f}, { 0.2f, 0.2f, 0.9f } }
	};

	uint32_t IBO2[] = 
	{
		0, 1, 2,
		2, 3, 0
	};

	Vertex VBO2[8] = {
		{ {-0.5f, -0.5f,  0.5f}, { 0.9f, 0.2f, 0.2f } }, // 01 45
		{ { 0.5f, -0.5f,  0.5f}, { 0.2f, 0.2f, 0.9f } }, // 32 76
		{ { 0.5f,  0.5f,  0.5f}, { 0.2f, 0.2f, 0.9f } }, // 
		{ {-0.5f,  0.5f,  0.5f}, { 0.9f, 0.2f, 0.2f } }, //
		{ {-0.5f, -0.5f, -0.5f}, { 0.9f, 0.2f, 0.2f } }, //
		{ { 0.5f, -0.5f, -0.5f}, { 0.2f, 0.2f, 0.9f } }, //
		{ { 0.5f,  0.5f, -0.5f}, { 0.2f, 0.2f, 0.9f } }, //
		{ {-0.5f,  0.5f, -0.5f}, { 0.9f, 0.2f, 0.2f } }  //
	};

	uint32_t IBO[6 * 3 * 2] = {
		// Front
		0, 1, 2,
		2, 3, 0,

		// Top
		0, 1, 4,
		4, 5, 0,

		// Right
		1, 2, 5,
		5, 6, 1,

		// Left
		0, 3, 4,
		4, 7, 0,

		// Bottom
		2, 3, 6,
		6, 7, 2
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
				Influx::VertexAttribute{ 0, Influx::VertexAttribute_Type::Vec3 },
				Influx::VertexAttribute{ 1, Influx::VertexAttribute_Type::Vec3 }
			},
			{
				Influx::DescriptorBinding{ 0, Influx::DescriptorBinding_Type::UniformBuffer, 1, Influx::Shader_Stage::Vertex }
			}
		);
		s_Data->renderer.bind_pipeline(s_Data->pipeline);

		new (&s_Data->vbo1) Influx::VertexBuffer(s_Data->renderer, VBO1, 4, sizeof(Vertex));
		new (&s_Data->ibo1) Influx::IndexBuffer(s_Data->renderer, IBO2, 6);
		new (&s_Data->ubo1) Influx::UniformBuffer(s_Data->renderer, sizeof(UniformBufferObject), 0);
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

	void Game::_Update(double dt)
	{
		//g_Logger.fmt(Neo::Trace, "{}fps", floor(1000.0 / dt));

		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo{};

		float angle = fmod(
			(float)dt * glm::radians(90.0f),
			glm::two_pi<float>()
		);
		ubo.model = glm::rotate(
			glm::mat4(1.0f),
			angle,
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

		ubo.view = glm::lookAt(
			glm::vec3(2.0f, 2.0f, 2.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

		ubo.proj = glm::perspective(
			glm::radians(45.0f),
			s_Data->renderer.width() / (float)s_Data->renderer.height(),
			0.1f,
			10.0f
		);
		ubo.proj[1][1] *= -1; 

		//ubo.view = glm::mat4(1.0f);
		//ubo.proj = glm::mat4(1.0f);

		static uint32_t logged = 0;
		if (logged < 3)
		{
			logged++;
			g_Logger.fmt(Neo::Trace, "view: {}", glm::to_string(ubo.view));
			g_Logger.fmt(Neo::Trace, "proj: {}", glm::to_string(ubo.proj));
		}

		s_Data->ubo1.update((void*)&ubo, s_Data->renderer);

		s_Data->renderer.clear({ 0.11f, 0.11f, 0.13f, 1.0f });

		s_Data->renderer.bind_ubo(s_Data->ubo1);
		s_Data->ibo1.draw(s_Data->renderer, s_Data->vbo1);
		s_Data->renderer.present();
	}

	void Game::Run(void)
	{
		s_Data->app.run();
	}

	void Game::DestroyRenderer(void)
	{
		s_Data->ubo1.~UniformBuffer();
		s_Data->ibo1.~IndexBuffer();
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