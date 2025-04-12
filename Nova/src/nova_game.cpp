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

		Neo::InitializeCore(Influx::RendererApi::Vulkan);

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
	Vertex VBO1[] = {
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.7f, 0.2f, 0.7f), }, 
		{ glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.2f, 0.2f, 0.7f), }, 
		{ glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.2f, 0.7f, 0.2f), }, 
		{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.2f, 0.7f, 0.7f), }, 
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.7f, 0.2f, 0.2f), },
		{ glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.7f, 0.2f, 0.7f), },
		{ glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.7f, 0.7f, 0.2f), }, 
		{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.7f, 0.2f, 0.7f)  } 
	};

	uint32_t IBO1[] = 
	{
		// Front face (0, 1, 2, 3)
	0, 1, 2,
	2, 3, 0,

	// Back face (4, 5, 6, 7)
	5, 4, 7,
	7, 6, 5,

	// Left face (4, 0, 3, 7)
	4, 0, 3,
	3, 7, 4,

	// Right face (1, 5, 6, 2)
	1, 5, 6,
	6, 2, 1,

	// Top face (3, 2, 6, 7)
	3, 2, 6,
	6, 7, 3,

	// Bottom face (0, 4, 5, 1)
	0, 4, 5,
	5, 1, 0
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

		new (&s_Data->vbo1) Influx::VertexBuffer(s_Data->renderer, VBO1, 8, sizeof(Vertex));
		new (&s_Data->ibo1) Influx::IndexBuffer(s_Data->renderer, IBO1, 36);
		new (&s_Data->ubo1) Influx::UniformBuffer(s_Data->renderer, sizeof(UniformBufferObject), 0);

		Influx::Image img = Influx::Image::Load(Neo::ExecDir() / "schlatt.jpg");
		if (!img)
			Nova::g_Logger(Neo::Error, "Ah shucks");

		img.destroy();
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
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo{};

		ubo.model = glm::rotate(
			glm::mat4(1.0f),
			time * glm::radians(90.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

		ubo.view = glm::lookAt(
			glm::vec3(2.5f, 2.5f, 2.5f),
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