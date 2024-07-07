#include "nova_pch.hpp"
#include "nova_game.hpp"

namespace nova {
    int Main(int argc, char** argv) {
        try {
            neo::Core::Init({argc, argv, NEO_RENDERERAPI_OPENGL});
            {
                Game game;
                game.loop();
            }
            neo::Core::Terminate();
        } catch (const std::exception& e) {
            NEO_FATAL_LOG("Exception caught in main: {0}", e.what());
            return -1;
        }
        return 0;
    }
}

#if (defined(NEO_PLATFORM_WINDOWS) && defined(NEO_CONFIG_DIST))
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    return nova::Main(__argc, __argv);
}
#else
int main(int argc, char* argv[]) {
    return nova::Main(argc, argv);
}
#endif // NEO_PLATFORM_WINDOWS && NEO_CONFIG_DIST