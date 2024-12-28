#include "nova_pch.hpp"

static std::filesystem::path ShaderDir(void)
{
	return std::filesystem::path(neo::ExecDir() / "../../../NeoInfused/Influx/shaders/").lexically_normal();
}

int main(int argc, char* argv[])
{
	try {
		neo::app_t app;
		
		neo::core_subsystem_t core_subsystem;

		neo::window_subsystem_t window_subsystem;
		window_subsystem.AddSystems();

		inf::renderer_t renderer(window_subsystem.window);

		std::filesystem::path shader_dir = ShaderDir();

		inf::shader_t vertex(shader_dir / "vertex.glsl", INF_SHADER_STAGE_VERTEX);
		inf::shader_t fragment(shader_dir / "fragment.glsl", INF_SHADER_STAGE_FRAGMENT);

		renderer.attach_shaders({ vertex.handle, fragment.handle });
		renderer.init();

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
