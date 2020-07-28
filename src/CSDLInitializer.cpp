/**
 * @file Implementation of classes: CSDLInitializer.
 */
//=============================================================================================================
#include "CSDLInitializer.h"
//=============================================================================================================

//=============================================================================================================
/**
 * Initializes the window and renderer. Initialization control.
 */
CSDLInitializer::CSDLInitializer () {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cout << "ERROR of initialized SDL: " << SDL_GetError() << std::endl;
        m_good = false;
        SDL_Quit();
        return;
    }
    m_window = SDL_CreateWindow("Craft 2D", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!m_window) {
        std::cout << "ERROR of created window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        m_good = false;
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!m_renderer) {
        std::cout << "ERROR of created window screen: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        m_good = false;
        return;
    }

    m_good = true;
}

//-------------------------------------------------------------------------------------------------------------
/**
 * Destroy the window, renderer and quit SDL if control flag is true.
 */
void CSDLInitializer::destroy () {
    if (m_good) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
}

//-------------------------------------------------------------------------------------------------------------
/**
 * Rreturns an initialized renderer.
 * @return initialized renderer.
 */
SDL_Renderer *CSDLInitializer::getRenderer () {
    return m_renderer;
}

//-------------------------------------------------------------------------------------------------------------
/**
 * Returns initialization control flag.
 * @return true if window and surface are initialized or false if not.
 */
bool CSDLInitializer::isGood () const {
    return m_good;
}
//=============================================================================================================
