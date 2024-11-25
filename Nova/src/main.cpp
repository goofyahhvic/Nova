#include "nova_pch.hpp"
#include "system_sets/nova_core-system-set.hpp"

int main(int argc, char* argv[])
{
	using namespace nova;
	try {
		neo::app_t app;

		system_set::core_t core(&app);
		app.add_system(NEO_SYSTEMTYPE_ONEVENT, core.OnEvent);

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
