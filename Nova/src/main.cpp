#include "nova_pch.hpp"
#include "nova_game.hpp"

int main(int argc, char* argv[])
{
	try {
		Nova::Game::Create();
		Nova::Game::CreateWindow(1280u, 720u, "Neo-Infused Cybernetic Endeavors"sv);
		Nova::Game::CreateRenderer();

		Nova::Game::Run();

		Nova::Game::DestroyRenderer();
		Nova::Game::DestroyWindow();
		Nova::Game::Destroy();
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
