#include "nova_pch.hpp"
#include "layers/nova_mainlayer.hpp"

namespace nova {
    static int main(int argc, char** argv) {
        try {
            neo::App app({ argc, argv, NEO_RENDERERAPI_OPENGL });
            app.windows.create_window(1280, 720, "Neo-Infused Cybernetic Endeavors");
            app.layers.create_layer<MainLayer>(5000);
            app.run();
        } catch (const std::exception& e) {
            NEO_FATAL_LOG("Exception caught in main: {0}", e.what());
            return -1;
        }
        return 0;
    }
}

#if (defined(NEO_PLATFORM_WINDOWS) && defined(NEO_CONFIG_DIST))
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    return nova::main(__argc, __argv);
}
#else
int main(int argc, char* argv[]) {
    return nova::main(argc, argv);
}
#endif // NEO_PLATFORM_WINDOWS && NEO_CONFIG_DIST
