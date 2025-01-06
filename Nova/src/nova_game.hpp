#if !defined(NOVA_GAME_HPP)
#define NOVA_GAME_HPP

#include "NeoInfused/NeoInfused.hpp"

namespace nova {
	void Game_Create(void);
	void Game_CreateWindow(uint32_t width, uint32_t height, const char* title);
	void Game_CreateRenderer(void);
	void Game_Run(void);
	void Game_DestroyRenderer(void);
	void Game_DestroyWindow(void);
	void Game_Destroy(void);
} // namespace nova

#endif // NOVA_GAME_HPP