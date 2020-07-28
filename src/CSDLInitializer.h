/**
 * @file Contains declaration of classes: CSDLInitializer.
 */
//=================================================================================================
#pragma once
//=================================================================================================

//=================================================================================================
#include <SDL2/SDL.h>
#include <iostream>

#include "CVars.h"
//=================================================================================================

/**
 * This class initialisere window size 1920х992
 * and creates an renderer for later placement of frames.
 */
class CSDLInitializer{
public:
    //---------------------------------------------------------------------------------------------
    /**
     * Initializes the window and renderer. Initialization control.
     */
    CSDLInitializer ();

    //---------------------------------------------------------------------------------------------
    /**
     * Destroy the window, renderer and quit SDL if control flag is true.
     */
    void destroy ();

    //---------------------------------------------------------------------------------------------
    /**
     * Rreturns an initialized renderer.
     * @return initialized renderer.
     */
    SDL_Renderer * getRenderer ();

    //---------------------------------------------------------------------------------------------
    /**
     * Returns initialization control flag.
     * @return true if window and surface are initialized or false if not.
     */
    bool isGood () const;

    //---------------------------------------------------------------------------------------------
private:
    bool            m_good; //success flag
    SDL_Window     *m_window; //game window
    SDL_Renderer   *m_renderer; // renderer of window
};
//=================================================================================================