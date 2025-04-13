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
		glm::vec2 tex_coord;
	};

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};
	Vertex VBO1[] = {
		{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, }, // 0
		{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }, }, // 1
		{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, }, // 2
		{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }, }, // 3

		{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }, }, // 4
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }, }, // 5
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }, }, // 6
		{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }, }, // 7

		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, }, // 8
		{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, }, // 9
		{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, }, // 10
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, }, // 11

		{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }, }, // 12
		{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }, }, // 13
		{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }, }, // 14
		{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }, }, // 15

		{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, }, // 16
		{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, }, // 17
		{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, }, // 18
		{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, }, // 19

		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }, }, // 20
		{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }, }, // 21
		{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, }, // 22
		{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }  } // 23
	};

	uint32_t IBO1[] = 
	{
		// Front face (0, 1, 2, 3)
		0, 1, 2,
		2, 3, 0,

		// Back face (4, 5, 6, 7)
		4, 5, 6,
		6, 7, 4,

		// Left face (8, 9, 10, 11)
		8, 9, 10,
		10, 11, 8,

		// Right face (12, 13, 14, 15)
		12, 13, 14,
		14, 15, 12,

		// Top face (16, 17, 18, 19)
		16, 17, 18,
		18, 19, 16,

		// Bottom face (20, 21, 22, 23)
		20, 21, 22,
		22, 23, 20
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
				Influx::VertexAttribute{
					0, // location
					Influx::VertexAttribute_Type::Vec3
				},
				Influx::VertexAttribute{
					1, // location
					Influx::VertexAttribute_Type::Vec3
				},
				Influx::VertexAttribute{
					2, // location
					Influx::VertexAttribute_Type::Vec2
				}
			},
			{
				Influx::DescriptorBinding{
					0, // location
					Influx::DescriptorBinding_Type::UniformBuffer,
					1, // count
					Influx::Shader_Stage::Vertex
				},
				Influx::DescriptorBinding{
					1, // location
					Influx::DescriptorBinding_Type::TextureSampler,
					1, // count
					Influx::Shader_Stage::Fragment
				}
			}
		);
		s_Data->renderer.bind_pipeline(s_Data->pipeline);

		new (&s_Data->vbo1) Influx::VertexBuffer(
			s_Data->renderer,
			VBO1,
			sizeof(VBO1) / sizeof(Vertex),
			sizeof(Vertex)
		);
		new (&s_Data->ibo1) Influx::IndexBuffer(s_Data->renderer, IBO1, 36);
		new (&s_Data->ubo1) Influx::UniformBuffer(s_Data->renderer, sizeof(UniformBufferObject), 0);

		Influx::Image img = Influx::Image::Load(Neo::ExecDir() / "tex.png");
		if (!img)
			Nova::g_Logger.fmt(
				Neo::Error,
				"Failed to load {}",
					(Neo::ExecDir() / "tex.png").C_STR()
			);

		new (&s_Data->texture) Influx::Texture(img, s_Data->renderer);

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
		s_Data->ubo1.destroy();
		s_Data->ibo1.destroy();
		s_Data->vbo1.destroy();

		s_Data->pipeline.destroy();
		s_Data->renderer.destroy();

		s_Data->texture.destroy();
	}

	void Game::DestroyWindow(void)
	{
		s_Data->window.destroy();
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