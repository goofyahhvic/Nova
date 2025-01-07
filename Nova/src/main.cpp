#include "nova_pch.hpp"
#include "nova_game.hpp"

int main(int argc, char* argv[])
{
	try {
		Nova::Game_Create();
		Nova::Game_CreateWindow(1280, 720, "Neo-Infused Cybernetic Endeavors");
		Nova::Game_CreateRenderer();

		Nova::Game_Run();

		Nova::Game_DestroyRenderer();
		Nova::Game_DestroyWindow();
		Nova::Game_Destroy();
	} catch (const std::exception& e)
	{
		Neo::g_Logger.fmt(Neo::Fatal, "Exception caught in main: {}", e.what());
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
