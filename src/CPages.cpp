/**
 * @file Implementation of classes: CPageInterface, CRespawnPage,
 *                                  CMainPage, CKeysPage,
 *                                  CSettingsPage, CExitPage,
 *                                  CContinuePage, CGameMenuPage,
 *                                  CNewGamePage, CLoadingGamePage.
 */
//======================================================================================================================
#include "CPages.h"
#include "CMap.h"
//======================================================================================================================

//===========================class CPageInterface=======================================================================
/**
 * Creates a texture from a PNG file that is named fileName.
 * @param fileName - file path.
 * @return a pointer to created texture.
 */
SDL_Texture *CPageInterface::createTexture (const std::string &fileName) const {
    SDL_Surface *dst = SDL_LoadBMP(fileName.c_str());
    if (!dst) {
        std::cout << "ERROR of loading BMP file \"" << fileName << "\": " << SDL_GetError() << std::endl;
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, dst);
    SDL_FreeSurface(dst);
    if (!texture) {
        std::cout << "ERROR create texture: " << SDL_GetError() << std::endl;
    }
    return texture;
}

//----------------------------------------------------------------------------------------------------------------------
/**
* Sets the rectangle parameters to the specified x, y, w(width), h(height).
* @param src - rectangle that is changing.
* @param x - coordinate of the point x of the upper left corner.
* @param y - coordinate of the point y of the upper left corner.
* @param w - width.
* @param h - height.
*/
void CPageInterface::setRect (SDL_Rect *src, int x, int y, int w, int h) const {
    src->x = x;
    src->y = y;
    src->w = w;
    src->h = h;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draws an image of a specific \ full size on a renderer at a specific location
 * or starting from the top left if dstRect == NULL.
 * @param texture - where does the image come from.
 * @param srcRect - what is the size of the drawn image.
 * @param dstRect - what size will be drawn.
 */
void CPageInterface::drawInPozition (SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect) const {
    SDL_RenderCopy(m_renderer, texture, srcRect, dstRect);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Clear the renderer inside the window.
 */
void CPageInterface::clearScreen () const {
    SDL_RenderClear(m_renderer);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Refresh the renderer inside the window.
 */
void CPageInterface::drawScreen () const {
    SDL_RenderPresent(m_renderer);
}
//======================================================================================================================

//===========================class CMainPage============================================================================
/**
 * @param sdl_renderer - current renderer.
 */
CMainPage::CMainPage (SDL_Renderer *sdl_renderer):CPageInterface    (sdl_renderer),
                                                  m_good            (true),
                                                  m_selected        (0),
                                                  m_selectedReturn  (5) {
    setRect(&m_selectedRect, 0, 102, 1000, 100);
    setRect(&m_unselectedRect, 0, 0, 1000, 100);
    loadingImages();
    setOutputRects();
}

//----------------------------------------------------------------------------------------------------------------------
CMainPage::~CMainPage () {
    if (m_backgroundTexture)
        SDL_DestroyTexture(m_backgroundTexture);
    for (int i = 0; i < 5; i++)
        if (m_textures[i])
            SDL_DestroyTexture(m_textures[i]);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Start page functionality.
 * @return - next action.
 */
EAction CMainPage::run () {
    if (!m_good)
        return EAction::ERROR;
    drawFirstFrame();
    CTimer timer;
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (timer.getTime() > 200) {
                if (event.type == SDL_QUIT)
                    return EAction::EXIT;
                checkMouse(&event);
                if (m_selectedReturn != 5)
                    return switchReturns();
                checkKeyboard(&event);
                if (m_selectedReturn != 5)
                    return switchReturns();
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Loading all images used.
 */
void CMainPage::loadingImages () {
    m_backgroundTexture = createTexture(BACK_GROUND_PATH);
    if (!m_backgroundTexture) {
        m_good = false;
        return;
    }
    m_textures[0] = createTexture(START_PAGE_NEW_GAME_PATH);
    if (!m_textures[0]) {
        m_good = false;
        return;
    }
    m_textures[1] = createTexture(START_PAGE_CONTINUE_PATH);
    if (!m_textures[1]) {
        m_good = false;
        return;
    }
    m_textures[2] = createTexture(START_PAGE_SETTINGS_PATH);
    if (!m_textures[2]) {
        m_good = false;
        return;
    }
    m_textures[3] = createTexture(START_PAGE_KEYS_PATH);
    if (!m_textures[3]) {
        m_good = false;
        return;
    }
    m_textures[4] = createTexture(START_PAGE_EXIT_PATH);
    if (!m_textures[4]) {
        m_good = false;
        return;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Given the initial location of the images.
 */
void CMainPage::setOutputRects () {
    setRect(&m_outTexturesRects[0], 460, 240, 1000, 100);
    setRect(&m_outTexturesRects[1], 460, 380, 1000, 100);
    setRect(&m_outTexturesRects[2], 460, 520, 1000, 100);
    setRect(&m_outTexturesRects[3], 460, 660, 1000, 100);
    setRect(&m_outTexturesRects[4], 460, 800, 1000, 100);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Sets the initial appearance of the images.
 */
void CMainPage::drawFirstFrame () {
    clearScreen();
    drawInPozition(m_backgroundTexture);
    for (uint8_t i = 0; i < 5; i++)
        drawInPozition(m_textures[i], (i == m_selected ? &m_selectedRect : &m_unselectedRect), &m_outTexturesRects[i]);
    drawScreen();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function to check for keystrokes.
 * @param event - actually event for control.
 */
void CMainPage::checkKeyboard (SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_w || event->key.keysym.sym == SDLK_UP) {
            reselected(static_cast<uint8_t >(!m_selected ? 4 : m_selected - 1));
        }
        if (event->key.keysym.sym == SDLK_s || event->key.keysym.sym == SDLK_DOWN) {
            reselected(static_cast<uint8_t >(m_selected == 4 ? 0 : m_selected + 1));
        }
        if (event->key.keysym.sym == SDLK_RETURN) {
            m_selectedReturn = m_selected;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function for validation of movements and clicks of the mouse.
 * @param event - actually event for control.
 */
void CMainPage::checkMouse (SDL_Event *event) {

    if (event->type == SDL_MOUSEMOTION) {
        uint8_t potentiallySelected = understandClick(event->button.x, event->button.y);
        if (potentiallySelected == 5 || potentiallySelected == m_selected) {

        } else {
            reselected(potentiallySelected);
        }
    }
    if (event->type == SDL_MOUSEBUTTONUP) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            m_selectedReturn = understandClick(event->button.x, event->button.y);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Understand where was the click of the mouse.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 * @return - click position.
 */
uint8_t CMainPage::understandClick (int x, int y) const {
    if (x < 460 || x > 1460)
        return 5;
    if (y >= 240 && y <= 340)
        return 0;
    if (y >= 380 && y <= 480)
        return 1;
    if (y >= 520 && y <= 620)
        return 2;
    if (y >= 660 && y <= 760)
        return 3;
    if (y >= 800 && y <= 900)
        return 4;
    return 5;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Decide what action will be next.
 * @return - action.
 */
EAction CMainPage::switchReturns () const {
    switch (m_selectedReturn) {
        case 0 :
            return EAction::NEW_GAME_PAGE;
        case 1 :
            return EAction::CONTINUE_PAGE;
        case 2 :
            return EAction::SETTINGS_PAGE;
        case 3 :
            return EAction::KEYS_PAGE;
    }
    return EAction::EXIT;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Changes the selected item.
 * @param newSelected
 */
void CMainPage::reselected (uint8_t newSelected) {
    drawInPozition(m_textures[m_selected], &m_unselectedRect, &m_outTexturesRects[m_selected]);
    m_selected = newSelected;
    drawInPozition(m_textures[m_selected], &m_selectedRect, &m_outTexturesRects[m_selected]);
    drawScreen();
}
//======================================================================================================================

//===========================class CKeysPage============================================================================
/**
 * @param sdl_renderer - current render.
 */
CKeysPage::CKeysPage (SDL_Renderer *sdl_renderer):CPageInterface    (sdl_renderer),
                                                  m_good            (true),
                                                  m_back            (false) {
    setOutputRects();
    loadingImages();
}

//----------------------------------------------------------------------------------------------------------------------
CKeysPage::~CKeysPage () {
    if (m_backgroundTexture)
        SDL_DestroyTexture(m_backgroundTexture);
    if (m_keysImage)
        SDL_DestroyTexture(m_keysImage);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Start page functionality.
 * @return - next action.
 */
EAction CKeysPage::run () {
    if (!m_good)
        return EAction::ERROR;
    drawFirstFrame();
    SDL_Event event;

    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return EAction::EXIT;
            checkMouse(&event);
            checkKeyboard(&event);
            if (m_back)
                return EAction::MAIN_PAGE;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function to check for keystrokes.
 * @param event - actually event for control.
 */
void CKeysPage::checkKeyboard (SDL_Event *event) {
    if (event->type == SDL_KEYDOWN)
        m_back = (event->key.keysym.sym == SDLK_ESCAPE);

}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function for validation of movements and clicks of the mouse.
 * @param event - actually event for control.
 */
void CKeysPage::checkMouse (SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONUP)
        if (event->button.button == SDL_BUTTON_LEFT)
            m_back = cursorInTarget(event->button.x, event->button.y);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Loading all images used.
 */
void CKeysPage::loadingImages () {
    m_backgroundTexture = createTexture(BACK_GROUND_PATH);
    if (!m_backgroundTexture) {
        m_good = false;
        return;
    }
    m_keysImage = createTexture(KEYS_PATH);
    if (!m_keysImage) {
        m_good = false;
        return;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Given the initial location of the images.
 */
void CKeysPage::setOutputRects () {
    setRect(&m_keysOutRect, 560, 196, 800, 600);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Sets the initial appearance of the images.
 */
void CKeysPage::drawFirstFrame () {
    clearScreen();
    drawInPozition(m_backgroundTexture, NULL, NULL);
    drawInPozition(m_keysImage, NULL, &m_keysOutRect);
    drawScreen();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Understands the cursor on the target or not.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 * @return - true or false.
 */
bool CKeysPage::cursorInTarget (int x, int y) const {
    if (x >= 862 && x <= 1063)
        if (y >= 738 && y <= 769)
            return true;
    return false;
}
//======================================================================================================================

//===========================class CSettingsPage========================================================================
/**
 * @param sdl_renderer - current render.
 */
CSettingsPage::CSettingsPage (SDL_Renderer *sdl_renderer):CPageInterface(sdl_renderer) {
    m_selected = 0;
    m_good = true;
    m_back = false;
    loadingImages();
    setOutputRects();
}

//----------------------------------------------------------------------------------------------------------------------
CSettingsPage::~CSettingsPage () {
    if (m_selecter)
        SDL_DestroyTexture(m_selecter);
    if (m_settings)
        SDL_DestroyTexture(m_settings);
    if (m_background)
        SDL_DestroyTexture(m_background);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Start page functionality.
 * @return - next action.
 */
EAction CSettingsPage::run () {
    if (!m_good)
        return EAction::ERROR;
    drawFirstFrame();
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return EAction::EXIT;
            checkMouse(&event);
            checkKeyboard(&event);
            if (m_back) {
                return EAction::MAIN_PAGE;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Loading all images used.
 */
void CSettingsPage::loadingImages () {
    m_background = createTexture(BACK_GROUND_PATH);
    if (!m_background) {
        m_good = false;
        return;
    }
    m_settings = createTexture(SETTINGS_MENU_SKIN_PATH);
    if (!m_settings) {
        m_good = false;
        return;
    }
    m_selecter = createTexture(SETTINGS_MENU_SELECTER_PATH);
    if (!m_selecter) {
        m_good = false;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Given the initial location of the images.
 */
void CSettingsPage::setOutputRects () {
    setRect(&m_settingsRect, 560, 211, 800, 600);
    setRect(&m_selecterRect[0], 660, 333, 15, 25);
    setRect(&m_selecterRect[1], 660, 463, 15, 25);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Sets the initial appearance of the images.
 */
void CSettingsPage::drawFirstFrame () {
    clearScreen();
    drawInPozition(m_background, NULL, NULL);
    drawInPozition(m_settings, NULL, &m_settingsRect);
    drawInPozition(m_selecter, NULL, &m_selecterRect[m_selected]);
    drawScreen();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function for validation of movements and clicks of the mouse.
 * @param event - actually event for control.
 */
void CSettingsPage::checkMouse (SDL_Event *event) {
    if (event->button.button == SDL_BUTTON_LEFT) {
        clickDetected(event->button.x, event->button.y);
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function to check for keystrokes.
 * @param event - actually event for control.
 */
void CSettingsPage::checkKeyboard (SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        m_back = (event->key.keysym.sym == SDLK_ESCAPE);
        if (event->key.keysym.sym == SDLK_RETURN) {
            m_back = true;
            try {
                writeSelectedType();
            } catch (const CException &exeption) {
                std::cout << exeption.what();
            }
        }
        if (event->key.keysym.sym == SDLK_w || event->key.keysym.sym == SDLK_UP || event->key.keysym.sym == SDLK_s ||
            event->key.keysym.sym == SDLK_DOWN) {
            m_selected = static_cast<uint8_t >(m_selected ? 0 : 1);
            drawFirstFrame();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Writes the selected type to the file.
 */
void CSettingsPage::writeSelectedType () {
    std::ofstream fout(SKY_SETTINGS_PATH, std::ios_base::binary | std::ios_base::trunc);
    if (fout.fail()) {
        fout.close();
        throw CWriteFileException(SKY_SETTINGS_PATH);
    }
    fout << m_selected;
    fout.close();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Detected mouse click.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
void CSettingsPage::clickDetected (int x, int y) {
    if (x >= 660 && x <= 1210) {
        if (y >= 316 && y <= 381) {
            m_selected = 0;
            drawFirstFrame();
            return;
        }
        if (y >= 436 && y <= 483) {
            m_selected = 1;
            drawFirstFrame();
            return;
        }
    }
    if (y >= 756 && y <= 791) {
        if (x >= 700 && x <= 910) {
            m_back = true;
            return;
        }
        if (x >= 1010 && x <= 1210) {
            try {
                writeSelectedType();
            } catch (const CException &exeption) {
                std::cout << exeption.what();
            }
            m_back = true;
        }
    }
}
//======================================================================================================================

//===========================class CExitPage============================================================================
/**
 * @param sdl_renderer - current render.
 */
CExitPage::CExitPage (SDL_Renderer *sdl_renderer):CPageInterface(sdl_renderer) {
    m_selected = 1;
    m_back = false;
    m_good = true;
    loadingImages();
    setOutputRects();
}

//----------------------------------------------------------------------------------------------------------------------
CExitPage::~CExitPage () {
    if (m_textures)
        SDL_DestroyTexture(m_textures);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Start page functionality.
 * @return - next action.
 */
EAction CExitPage::run () {
    if (!m_good) {
        return EAction::ERROR;
    }
    drawFirstFrame();
    CTimer timer;
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (timer.getTime() > 200) {
                if (event.type == SDL_QUIT) {
                    return EAction::QUIT;
                }
                checkMouse(&event);
                if (m_back) {
                    if (m_selected) {
                        return EAction::CONTINUE;
                    }
                    return EAction::QUIT;
                }
                checkKeyboard(&event);
                if (m_back) {
                    if (m_selected) {
                        return EAction::CONTINUE;
                    }
                    return EAction::QUIT;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Loading all images used.
 */
void CExitPage::loadingImages () {
    m_textures = createTexture(EXIT_PATH);
    if (!m_textures) {
        m_good = false;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Given the initial location of the images.
 */
void CExitPage::setOutputRects () {
    setRect(&m_restsOut[0], 710, 500, 500, 200);
    setRect(&m_restsOut[1], 970, 650, 98, 28);
    setRect(&m_restsOut[2], 1090, 650, 98, 28);
    setRect(&m_restsIn[0], 0, 0, 500, 200);
    setRect(&m_restsIn[1], 5, 220, 98, 28);
    setRect(&m_restsIn[2], 5, 260, 98, 28);
    setRect(&m_restsIn[3], 250, 220, 98, 28);
    setRect(&m_restsIn[4], 250, 260, 98, 28);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Sets the initial appearance of the images.
 */
void CExitPage::drawFirstFrame () {
    drawInPozition(m_textures, &m_restsIn[0], &m_restsOut[0]);
    drawInPozition(m_textures, &m_restsIn[1], &m_restsOut[1]);
    drawInPozition(m_textures, &m_restsIn[4], &m_restsOut[2]);
    drawScreen();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function for validation of movements and clicks of the mouse.
 * @param event - actually event for control.
 */
void CExitPage::checkMouse (SDL_Event *event) {
    if (event->type == SDL_MOUSEMOTION) {
        understandMouse(event->button.x, event->button.y);
    }
    if (event->button.button == SDL_BUTTON_LEFT) {
        understandClick(event->button.x, event->button.y);
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function to check for keystrokes.
 * @param event - actually event for control.
 */
void CExitPage::checkKeyboard (SDL_Event *event) {
    if (event->key.keysym.sym == SDLK_a || event->key.keysym.sym == SDLK_LEFT || event->key.keysym.sym == SDLK_d ||
        event->key.keysym.sym == SDLK_RIGHT) {
        m_selected = static_cast<uint8_t>(m_selected ? 0 : 1);
        draw();
        CTimer timer;
        while (timer.getTime() < 200) {
            SDL_PollEvent(event);
        }
    }
    if (event->key.keysym.sym == SDLK_RETURN) {
        m_back = true;
    }
    if (event->key.keysym.sym == SDLK_ESCAPE) {
        m_selected = 1;
        m_back = true;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Understands the movement of the mouse.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
void CExitPage::understandMouse (int x, int y) {
    if (y >= 650 && y <= 678) {
        if (x >= 970 && x <= 1068) {
            m_selected = 0;
            draw();
            return;
        }
        if (x >= 1090 && x <= 1188) {
            m_selected = 1;
            draw();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Understands the click of a mouse.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
void CExitPage::understandClick (int x, int y) {
    if (y >= 650 && y <= 678) {
        if (x >= 970 && x <= 1068) {
            m_selected = 0;
            m_back = true;
            return;
        }
        if (x >= 1090 && x <= 1188) {
            m_selected = 1;
            m_back = true;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw the actual frame.
 */
void CExitPage::draw () {
    if (m_selected) {
        drawInPozition(m_textures, &m_restsIn[1], &m_restsOut[1]);
        drawInPozition(m_textures, &m_restsIn[4], &m_restsOut[2]);
        drawScreen();
        return;
    }
    drawInPozition(m_textures, &m_restsIn[2], &m_restsOut[1]);
    drawInPozition(m_textures, &m_restsIn[3], &m_restsOut[2]);
    drawScreen();
}
//======================================================================================================================

//===========================class CContinuePage========================================================================
/**
 * @param sdl_renderer - current render.
 */
CContinuePage::CContinuePage (SDL_Renderer *sdl_renderer):CPageInterface(sdl_renderer) {
    m_good = true;
    m_back = false;
    m_select = false;
    m_count = 0;
    m_selected = 0;
    m_top = 0;
    readCount();
    loadingImages();
    setOutputRects();
}

//----------------------------------------------------------------------------------------------------------------------
CContinuePage::~CContinuePage () {
    if (m_backgroundTexture)
        SDL_DestroyTexture(m_backgroundTexture);
    if (m_selectedTexture)
        SDL_DestroyTexture(m_selectedTexture);
    if (m_numsTexture)
        SDL_DestroyTexture(m_numsTexture);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Start page functionality.
 * @return - next action.
 */
EAction CContinuePage::run () {
    if (!m_good)
        return EAction::ERROR;
    drawFirstFrame();

    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return EAction::EXIT;
            }
            checkMouse(&event);

            if (m_select) {
                try {
                    writeSelect();
                } catch (const CException &exception) {
                    std::cout << exception.what();
                    return EAction::ERROR;
                }
                return EAction::LOADING_GAME_PAGE;
            }
            if (m_back) {
                return EAction::MAIN_PAGE;
            }
            checkKeyboard(&event);
            if (m_select) {
                try {
                    writeSelect();
                } catch (const CException &exception) {
                    std::cout << exception.what();
                    return EAction::ERROR;
                }
                return EAction::LOADING_GAME_PAGE;
            }
            if (m_back) {
                return EAction::MAIN_PAGE;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Loading all images used.
 */
void CContinuePage::loadingImages () {
    m_backgroundTexture = createTexture(BACK_GROUND_PATH);
    if (!m_backgroundTexture) {
        m_good = false;
        return;
    }
    m_selectedTexture = createTexture(CONTINUE_MENU_SKIN_PATH);
    if (!m_selectedTexture) {
        m_good = false;
        return;
    }
    m_numsTexture = createTexture(CONTINUE_MENU_NUMS_PATH);
    if (!m_numsTexture) {
        m_good = false;
    }

}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Given the initial location of the images.
 */
void CContinuePage::setOutputRects () {
    setRect(&m_outSelectedTextureRect, 560, 200, 800, 600);
    setRect(&m_mapRect, 150, 0, 55, 25);
    setRect(&m_selecterRect, 205, 0, 15, 25);

    setRect(&m_numsRect[0], 135, 0, 15, 25);
    setRect(&m_numsRect[1], 0, 0, 15, 25);
    setRect(&m_numsRect[2], 15, 0, 15, 25);
    setRect(&m_numsRect[3], 30, 0, 15, 25);
    setRect(&m_numsRect[4], 45, 0, 15, 25);
    setRect(&m_numsRect[5], 60, 0, 15, 25);
    setRect(&m_numsRect[6], 75, 0, 15, 25);
    setRect(&m_numsRect[7], 90, 0, 15, 25);
    setRect(&m_numsRect[8], 105, 0, 15, 25);
    setRect(&m_numsRect[9], 120, 0, 15, 25);

    setRect(&m_outMapRects[0], 660, 283, 55, 25);
    setRect(&m_outMapRects[1], 660, 328, 55, 25);
    setRect(&m_outMapRects[2], 660, 376, 55, 25);
    setRect(&m_outMapRects[3], 660, 420, 55, 25);
    setRect(&m_outMapRects[4], 660, 462, 55, 25);
    setRect(&m_outMapRects[5], 660, 505, 55, 25);
    setRect(&m_outMapRects[6], 660, 547, 55, 25);
    setRect(&m_outMapRects[7], 660, 593, 55, 25);
    setRect(&m_outMapRects[8], 660, 641, 55, 25);
    setRect(&m_outMapRects[9], 660, 690, 55, 25);

    setRect(&m_outSelecterRects[0], 615, 283, 15, 25);
    setRect(&m_outSelecterRects[1], 615, 328, 15, 25);
    setRect(&m_outSelecterRects[2], 615, 376, 15, 25);
    setRect(&m_outSelecterRects[3], 615, 420, 15, 25);
    setRect(&m_outSelecterRects[4], 615, 462, 15, 25);
    setRect(&m_outSelecterRects[5], 615, 505, 15, 25);
    setRect(&m_outSelecterRects[6], 615, 547, 15, 25);
    setRect(&m_outSelecterRects[7], 615, 593, 15, 25);
    setRect(&m_outSelecterRects[8], 615, 641, 15, 25);
    setRect(&m_outSelecterRects[9], 615, 690, 15, 25);

    setRect(&m_outFirstNumRects[0], 715, 283, 15, 25);
    setRect(&m_outFirstNumRects[1], 715, 328, 15, 25);
    setRect(&m_outFirstNumRects[2], 715, 376, 15, 25);
    setRect(&m_outFirstNumRects[3], 715, 420, 15, 25);
    setRect(&m_outFirstNumRects[4], 715, 462, 15, 25);
    setRect(&m_outFirstNumRects[5], 715, 505, 15, 25);
    setRect(&m_outFirstNumRects[6], 715, 547, 15, 25);
    setRect(&m_outFirstNumRects[7], 715, 593, 15, 25);
    setRect(&m_outFirstNumRects[8], 715, 641, 15, 25);
    setRect(&m_outFirstNumRects[9], 715, 690, 15, 25);

    setRect(&m_outSecondNumRects[0], 730, 283, 15, 25);
    setRect(&m_outSecondNumRects[1], 730, 328, 15, 25);
    setRect(&m_outSecondNumRects[2], 730, 376, 15, 25);
    setRect(&m_outSecondNumRects[3], 730, 420, 15, 25);
    setRect(&m_outSecondNumRects[4], 730, 462, 15, 25);
    setRect(&m_outSecondNumRects[5], 730, 505, 15, 25);
    setRect(&m_outSecondNumRects[6], 730, 547, 15, 25);
    setRect(&m_outSecondNumRects[7], 730, 593, 15, 25);
    setRect(&m_outSecondNumRects[8], 730, 641, 15, 25);
    setRect(&m_outSecondNumRects[9], 730, 690, 15, 25);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Sets the initial appearance of the images.
 */
void CContinuePage::drawFirstFrame () {
    clearScreen();
    drawInPozition(m_backgroundTexture);
    drawInPozition(m_selectedTexture, NULL, &m_outSelectedTextureRect);
    if (m_count > 10) {
        m_selected = 1;
        m_top = 1;
        for (int i = 0; i < 10; i++) {
            if (i + 1 == m_selected) {
                drawInPozition(m_numsTexture, &m_selecterRect, &m_outSelecterRects[i]);
            }
            drawMapNum(i);
        }
        drawScreen();
        return;
    }
    if (!m_count) {
        drawScreen();
        return;
    }
    m_selected = 1;
    m_top = 1;
    for (int i = 0; i < m_count; i++) {
        if (i + 1 == m_selected) {
            drawInPozition(m_numsTexture, &m_selecterRect, &m_outSelecterRects[i]);
        }
        drawMapNum(i);
    }
    drawScreen();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function for validation of movements and clicks of the mouse.
 * @param event - actually event for control.
 */
void CContinuePage::checkMouse (SDL_Event *event) {
    if (event->type == SDL_MOUSEBUTTONUP)
        if (event->button.button == SDL_BUTTON_LEFT) {
            understandClick(event->button.x, event->button.y);
            drawText();
            return;
        }
    if (event->type == SDL_MOUSEWHEEL) {
        if (event->wheel.y > 0) {
            if (m_selected != 1) {
                m_selected--;
                drawText();
                return;
            }
            if (m_top > 1 && m_selected == 1) {
                m_top--;
                drawText();
            }
        }
        if (event->wheel.y < 0) {
            if (m_selected == 10 && m_count - m_top > 9) {
                m_top++;
                drawText();
                return;
            }
            if (m_selected != 10 && m_selected != m_count) {
                m_selected++;
                drawText();
                return;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function to check for keystrokes.
 * @param event - actually event for control.
 */
void CContinuePage::checkKeyboard (SDL_Event *event) {
    if (event->type == SDL_KEYDOWN && m_count) {
        if (event->key.keysym.sym == SDLK_w || event->key.keysym.sym == SDLK_UP) {
            if (m_selected != 1) {
                m_selected--;
                drawText();
                return;
            }
            if (m_top > 1 && m_selected == 1) {
                m_top--;
                drawText();
            }
        }
        if (event->key.keysym.sym == SDLK_s || event->key.keysym.sym == SDLK_DOWN) {
            if (m_selected == 10 && m_count - m_top > 9) {
                m_top++;
                drawText();
                return;
            }
            if (m_selected != 10 && m_selected != m_count) {
                m_selected++;
                drawText();
                return;
            }
        }
        if (event->key.keysym.sym == SDLK_RETURN) {
            if (m_selected)
                m_select = true;
            return;
        }
    }
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_ESCAPE) {
            m_back = true;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Write the text to select the map.
 */
void CContinuePage::drawText () {
    clearScreen();
    drawInPozition(m_backgroundTexture);
    drawInPozition(m_selectedTexture, NULL, &m_outSelectedTextureRect);

    if (m_count > 10) {
        for (int i = 0; i < 10; i++) {
            if (i + 1 == m_selected) {
                drawInPozition(m_numsTexture, &m_selecterRect, &m_outSelecterRects[i]);
            }
            drawMapNum(static_cast<uint8_t>(i));
        }
        drawScreen();
        return;
    }
    if (!m_count) {
        drawScreen();
        return;
    }
    m_top = 1;
    for (int i = 0; i < m_count; i++) {
        if (i + 1 == m_selected) {
            drawInPozition(m_numsTexture, &m_selecterRect, &m_outSelecterRects[i]);
        }
        drawMapNum(i);
    }
    drawScreen();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Reads the number of maps created from the file.
 */
void CContinuePage::readCount () {
    std::ifstream fin(MAP_COUNT_PATH, std::ios_base::binary);
    if (fin.fail()) {
        fin.close();
        m_count = 0;
        std::ofstream fout(MAP_COUNT_PATH, std::ios_base::binary | std::ios_base::trunc);
        if (fout.fail()) {
            fout.close();
            throw CWriteFileException(MAP_COUNT_PATH);
        }
        fout << m_count;
        fout.close();
        return;
    }
    if (fin.read((char *) &m_count, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
        fin.close();
        throw CReadFileException(MAP_COUNT_PATH);
    }
    fin.close();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Understands the click of a mouse.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
void CContinuePage::understandClick (int x, int y) {
    if (x >= 606 && x <= 1300) {
        if (y >= 277 && y <= 318 && m_count > 0) {
            m_selected = 1;
            return;
        }
        if (y >= 327 && y <= 364 && m_count > 1) {
            m_selected = 2;
            return;
        }
        if (y >= 373 && y <= 409 && m_count > 2) {
            m_selected = 3;
            return;
        }
        if (y >= 417 && y <= 452 && m_count > 3) {
            m_selected = 4;
            return;
        }
        if (y >= 460 && y <= 494 && m_count > 4) {
            m_selected = 5;
            return;
        }
        if (y >= 501 && y <= 538 && m_count > 5) {
            m_selected = 6;
            return;
        }
        if (y >= 546 && y <= 580 && m_count > 6) {
            m_selected = 7;
            return;
        }
        if (y >= 588 && y <= 628 && m_count > 7) {
            m_selected = 8;
            return;
        }
        if (y >= 636 && y <= 676 && m_count > 8) {
            m_selected = 9;
            return;
        }
        if (y >= 682 && y <= 726 && m_count > 9) {
            m_selected = 10;
            return;
        }
    }
    if (y >= 745 && y <= 780) {
        if (x >= 710 && x <= 910) {
            m_back = true;
            return;
        }
        if (x >= 1010 && x <= 1210) {
            if (m_selected && m_count)
                m_select = true;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Writes the selected map to a file/
 */
void CContinuePage::writeSelect () {
    std::ofstream fout(MAP_SELECT_PATH, std::ios_base::binary | std::ios_base::trunc);
    if (fout.fail()) {
        fout.close();
        throw CWriteFileException(MAP_SELECT_PATH);
    }
    m_selected += m_top - 1;
    fout << m_selected;
    fout.close();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw a number to the desired position.
 * @param pos - current position.
 */
void CContinuePage::drawMapNum (uint8_t pos) {
    drawInPozition(m_numsTexture, &m_mapRect, &m_outMapRects[pos]);

    if (m_top + pos < 10) {
        drawInPozition(m_numsTexture, &m_numsRect[(m_top + pos)], &m_outFirstNumRects[pos]);
        return;
    }
    drawInPozition(m_numsTexture, &m_numsRect[(m_top + pos) / 10], &m_outFirstNumRects[pos]);
    drawInPozition(m_numsTexture, &m_numsRect[(m_top + pos) % 10], &m_outSecondNumRects[pos]);
}
//======================================================================================================================

//===========================class CGameMenuPage========================================================================
/**
 * @param sdl_renderer - current render.
 */
CGameMenuPage::CGameMenuPage (SDL_Renderer * renderer):CPageInterface(renderer) {
    m_good = true;
    m_back = false;
    m_select = 0;
    setOutputRects();
    loadingImages();
}

//----------------------------------------------------------------------------------------------------------------------
CGameMenuPage::~CGameMenuPage () {
    if (m_continue) {
        SDL_DestroyTexture(m_continue);
    }
    if (m_save) {
        SDL_DestroyTexture(m_save);
    }
    if (m_exit) {
        SDL_DestroyTexture(m_exit);
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Given the initial location of the images.
 */
void CGameMenuPage::setOutputRects () {
    setRect(&m_r_texture[0], 5, 5  , 495, 85);
    setRect(&m_r_texture[1], 5, 100, 495, 85);
    setRect(&m_r_out[0], 720, 300, 495, 85);
    setRect(&m_r_out[1], 720, 400, 495, 85);
    setRect(&m_r_out[2], 720, 500, 495, 85);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Loading all images used.
 */
void CGameMenuPage::loadingImages () {
    m_continue = createTexture(GAME_MENU_CONTINUE_PATH);
    if (!m_continue) {
        m_good = false;
        return;
    }
    m_save = createTexture(GAME_MENU_SAVE_PATH);
    if (!m_save) {
        m_good = false;
        return;
    }
    m_exit = createTexture(GAME_MENU_EXIT_PATH);
    if (!m_exit) {
        m_good = false;
        return;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Sets the initial appearance of the images.
 */
void CGameMenuPage::drawFirstFrame () {
    drawFrame();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Check mouse position.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 * @return - 0 - CONTINUE, 1 - SAVE, 2 - EXIT, 3 - NO FOUND.
 */
int CGameMenuPage::understandMouse (int x, int y) const {
    if (x < 720 || x > 1215) {
        return 3;
    }
    if (y < 300 || y > 585) {
        return 3;
    }
    if (y >= 300 && y <= 385) {
        return 0;
    }
    if (y >= 400 && y <= 485) {
        return 1;
    }
    if (y >= 500 && y <= 585) {
        return 2;
    }
    return 3;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw currently frame.
 */
void CGameMenuPage::drawFrame () {
    drawInPozition(m_continue, (m_select == 0 ? &m_r_texture[1] : &m_r_texture[0]), &m_r_out[0]);
    drawInPozition(m_save, (m_select == 1 ? &m_r_texture[1] : &m_r_texture[0]), &m_r_out[1]);
    drawInPozition(m_exit, (m_select == 2 ? &m_r_texture[1] : &m_r_texture[0]), &m_r_out[2]);
    drawScreen();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Returns the action depending on the selection.
 * @return - action.
 */
EAction CGameMenuPage::identify () const {
    if (m_select == 0) return EAction::CONTINUE;
    if (m_select == 1) return EAction::SAVE;
    return EAction::EXIT;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function for validation of movements and clicks of the mouse.
 * @param event - actually event for control.
 */
void CGameMenuPage::checkMouse (SDL_Event *event) {
    if (event->type == SDL_MOUSEWHEEL) {
        if (event->wheel.y > 0) {
            if (m_select == 0) {
                m_select = 2;
                drawFrame();
                return;
            }
            m_select--;
            drawFrame();
            return;
        }
        if (event->wheel.y < 0) {
            if (m_select == 2) {
                m_select = 0;
                drawFrame();
                return;
            }
            m_select++;
            drawFrame();
            return;
        }
    }

    if (event->type == SDL_MOUSEBUTTONUP) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            int var = understandMouse(event->button.x, event->button.y);
            if (var != 3) {
                m_back = true;
                m_select = static_cast<uint8_t>(var);
                return;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function to check for keystrokes.
 * @param event - actually event for control.
 */
void CGameMenuPage::checkKeyboard (SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_w || event->key.keysym.sym == SDLK_UP) {
            if (m_select == 0) {
                m_select = 2;
                drawFrame();
                return;
            }
            m_select--;
            drawFrame();
            return;
        }
        if (event->key.keysym.sym == SDLK_s || event->key.keysym.sym == SDLK_DOWN) {
            if (m_select == 2) {
                m_select = 0;
                drawFrame();
                return;
            }
            m_select++;
            drawFrame();
            return;
        }
        if (event->key.keysym.sym == SDLK_RETURN) {
            m_back = true;
            return;
        }
        if (event->key.keysym.sym == SDLK_ESCAPE) {
            m_select = 0;
            m_back = true;
            return;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Controller for CGameMenuPage.
 * @return - QUIT if event SDL_QUIT or CONTINUE, SAVE, EXIT(to main menu).
 */
EAction CGameMenuPage::run () {
    if (!m_good)
        return EAction::ERROR;
    drawFrame();
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return EAction::QUIT;
            }
            checkKeyboard(&event);
            if (m_back) {
                return identify();
            }
            checkMouse(&event);
            if (m_back) {
                return identify();
            }
        }
    }
}
//======================================================================================================================

//===========================class CNewGamePage=========================================================================
/**
 * @param sdl_renderer - current render.
 */
CNewGamePage::CNewGamePage (SDL_Renderer *renderer):CPageInterface(renderer) {
    m_good = true;
    loadingImages();
}

//----------------------------------------------------------------------------------------------------------------------
CNewGamePage::~CNewGamePage () {
    if (m_background)
        SDL_DestroyTexture(m_background);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Loading all images used.
 */
void CNewGamePage::loadingImages () {
    m_background = createTexture(BACK_GROUND_CREATING_PATH);
    if (!m_background) {
        m_good = false;
        return;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Sets the initial appearance of the images.
 */
void CNewGamePage::drawFirstFrame () {
    clearScreen();
    drawInPozition(m_background, NULL, NULL);
    drawScreen();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Given the initial location of the images.
 */
void CNewGamePage::setOutputRects () {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function for validation of movements and clicks of the mouse.
 * @param event - actually event for control.
 */
void CNewGamePage::checkMouse (SDL_Event *event) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function to check for keystrokes.
 * @param event - actually event for control.
 */
void CNewGamePage::checkKeyboard (SDL_Event *event) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Controller for CGameMenuPage.
 * @return - EXIT if event SDL_QUIT or LOADING_GAME_PAGE.
 */
EAction CNewGamePage::run () {
    if (!m_good)
        return EAction::ERROR;
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                return EAction::EXIT;
            }
            drawFirstFrame();
            try {
                //create test map;
                //CTestMapCreater();

                //create new map;
                CMapCreater();
            } catch (const CException & exception) {
                std::cout << exception.what();
                return EAction::ERROR;
            }
            CTimer timer;
            timer.setTimer(1000);
            SDL_PushEvent(&event);
            return EAction::LOADING_GAME_PAGE;
        }
    }
}
//======================================================================================================================

//===========================class CLoadingGamePage=====================================================================
/**
 * @param renderer - current renderer.
 * @param map - game map for create.
 */
CLoadingGamePages::CLoadingGamePages (SDL_Renderer *renderer, CMap **map):CPageInterface (renderer),
                                                                          m_map          (map),
                                                                          m_renderer     (renderer) {
    m_good = true;
    loadingImages();
}

//----------------------------------------------------------------------------------------------------------------------
CLoadingGamePages::~CLoadingGamePages () {
    if (m_background)
        SDL_DestroyTexture(m_background);
}

//----------------------------------------------------------------------------------------------------------------------
/**
     * Loading all images used.
     */
void CLoadingGamePages::loadingImages () {
    m_background = createTexture(BACK_GROUND_LOADING_PATH);
    if (!m_background) {
        m_good = false;
        return;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Given the initial location of the images.
 */
void CLoadingGamePages::setOutputRects () {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Sets the initial appearance of the images.
 */
void CLoadingGamePages::drawFirstFrame () {
    clearScreen();
    drawInPozition(m_background, NULL, NULL);
    drawScreen();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function for validation of movements and clicks of the mouse.
 * @param event - actually event for control.
 */
void CLoadingGamePages::checkMouse (SDL_Event *event) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function to check for keystrokes.
 * @param event - actually event for control.
 */
void CLoadingGamePages::checkKeyboard (SDL_Event *event) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Controller for CLoadingGamePages.
 * @return - EXIT if event SDL_QUIT or NEW_GAME.
 */
EAction CLoadingGamePages::run () {
    if (!m_good)
        return EAction::ERROR;
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT) {
                return EAction::EXIT;
            }
            drawFirstFrame();

            CMapLoader cMapLoader = CMapLoader(m_renderer);
            try {
                *m_map = cMapLoader.returnMap();
            } catch (const CException &exception) {
                std::cout << exception.what();
                m_map = nullptr;
                return EAction::MAIN_PAGE;
            }
            CTimer timer;
            timer.setTimer(1000);
            return EAction::START_GAME;
        }
    }
}
//======================================================================================================================

//===========================class CRespawnPage=========================================================================
/**
 * @param sdl_renderer - current render.
 */
CRespawnPage::CRespawnPage (SDL_Renderer *renderer):CPageInterface(renderer) {
    m_good = true;
    m_currentFrame = 4;
    loadingImages();
    setOutputRects();
}

//----------------------------------------------------------------------------------------------------------------------
CRespawnPage::~CRespawnPage () {
    if (m_background)
        SDL_DestroyTexture(m_background);
    for (auto item : m_time) {
        if (item)
            SDL_DestroyTexture(item);
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Given the initial location of the images.
 */
void CRespawnPage::setOutputRects () {
    setRect(&m_outRect, 658, 234, 600, 600);
}

//----------------------------------------------------------------------------------------------------------------------
/**
     * Loading all images used.
     */
void CRespawnPage::loadingImages () {
    m_background = createTexture(RESPAWN_BACK_GROUND);
    if (!m_background) {
        m_good = false;
        return;
    }
    m_time[0] = createTexture(RESPAWN_TIME_1);
    if (!m_time[0]) {
        m_good = false;
        return;
    }
    m_time[1] = createTexture(RESPAWN_TIME_2);
    if (!m_time[1]) {
        m_good = false;
        return;
    }
    m_time[2] = createTexture(RESPAWN_TIME_3);
    if (!m_time[2]) {
        m_good = false;
        return;
    }
    m_time[3] = createTexture(RESPAWN_TIME_4);
    if (!m_time[3]) {
        m_good = false;
        return;
    }
    m_time[4] = createTexture(RESPAWN_TIME_5);
    if (!m_time[4]) {
        m_good = false;
        return;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function to check for keystrokes.
 * @param event - actually event for control.
 */
void CRespawnPage::checkKeyboard (SDL_Event *event) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overridable function for validation of movements and clicks of the mouse.
 * @param event - actually event for control.
 */
void CRespawnPage::checkMouse (SDL_Event *event) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Sets the initial appearance of the images.
 */
void CRespawnPage::drawFirstFrame () {
    clearScreen();
    drawInPozition(m_background, NULL, NULL);
    drawInPozition(m_time[m_currentFrame], NULL, &m_outRect);
    drawScreen();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Controller for CLoadingGamePages.
 * @return - EXIT if event SDL_QUIT or LOADING_GAME_PAGE.
 */
EAction CRespawnPage::run () {
    if (!m_good)
        return EAction::ERROR;
    SDL_Event event;
    uint32_t time = SDL_GetTicks();
    uint32_t currentTime = 0;
    while (true) {
        while (SDL_PollEvent(&event)) {
            drawFirstFrame();
            if (event.type == SDL_QUIT) {
                return EAction::EXIT;
            }

            currentTime += SDL_GetTicks() - time;
            time = SDL_GetTicks();
            if (currentTime > 1000) {
                currentTime = 0;
                m_currentFrame--;
            }
            SDL_PushEvent(&event);
            if (m_currentFrame == -1) {
                return EAction::LOADING_GAME_PAGE;
            }
        }
    }
}
//======================================================================================================================