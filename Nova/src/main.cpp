#include "nova_pch.hpp"
#include "nova_game.hpp"



int main(int argc, char* argv[])
{
	try {
		nova::Game_Create();
		nova::Game_CreateWindow(1280, 720, "Neo-Infused Cybernetic Endeavors");
		nova::Game_CreateRenderer();

		nova::Game_Run();

		nova::Game_DestroyRenderer();
		nova::Game_DestroyWindow();
		nova::Game_Destroy();
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
