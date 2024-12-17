#include "nova_pch.hpp"

int main(int argc, char* argv[])
{
	try {
		neo::app_t app;

		std::filesystem::path shader_dir(neo::ExecDir() / "../../../NeoInfused/Influx/shaders/");
		shader_dir = shader_dir.lexically_normal();

		inf::Loader::build_shader(shader_dir / "vertex.glsl");
		inf::Loader::build_shader(shader_dir / "fragment.glsl");

		neo::app::system_set::window_t window_system_set;
		window_system_set.AddSystems();

		inf::InitializeAPI();
		inf::window_surface_t window_surface(&window_system_set.window);

		app.run();
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
