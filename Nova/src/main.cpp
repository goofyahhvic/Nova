#include "nova_pch.hpp"

int main(int argc, char* argv[])
{
	try {
		neo::app_t app;
		
		neo::app::system_set::window_t window_system_set;
		window_system_set.AddSystems();

		inf::window_surface_t window_surface(&window_system_set.window);

		std::filesystem::path shader_dir(neo::ExecDir() / "../../../NeoInfused/Influx/shaders/");
		shader_dir = shader_dir.lexically_normal();

		inf::Loader::shader_pipeline_t* pipeline = inf::Loader::create_shader_pipeline(
			window_surface.native()
		);
		uint64_t vertex = inf::Loader::create_shader(shader_dir / "vertex.glsl", INF_SHADER_STAGE_VERTEX);
		uint64_t fragment = inf::Loader::create_shader(shader_dir / "fragment.glsl", INF_SHADER_STAGE_FRAGMENT);

		inf::Loader::attach_shader_to_pipeline(pipeline, vertex);
		inf::Loader::attach_shader_to_pipeline(pipeline, fragment);

		inf::Loader::init_shader_pipeline(pipeline);

		inf::Loader::destroy_shader(fragment);
		inf::Loader::destroy_shader(vertex);

		app.run();

		inf::Loader::destroy_shader_pipeline(pipeline);
	} catch (const std::exception& e)
	{
		NEO_FATAL_LOG("Exception caught in main: {}", e.what());
		return -1;
	}
	return 0;
}

#if (defined(NEO_PLATFORM_WINDOWS) && defined(NEO_CONFIG_DIST))
int APIENTRY WinMain(
	HINSTANCE hInst,
	HINSTANCE hInstPrev,
	PSTR cmdline,
	int cmdshow)
{
	return main(__argc, __argv);
}
#endif 
