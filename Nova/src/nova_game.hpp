#if !defined(NOVA_GAME_HPP)
#define NOVA_GAME_HPP

#include "layers/nova_mainlayer.hpp"

namespace nova {
    class Game {
    public:
        Game(void);
        ~Game(void) noexcept(false);
        void loop(void);
    private:
        neo::Window* m_Window;
        neo::LayerGroup m_Layers;
    };
    
} // namespace nova

#endif // NOVA_GAME_HPP