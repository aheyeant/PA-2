/**
 * @file Contains declaration of classes: EAction, CPageInterface,
 *                                        CMainPage, CKeysPage,
 *                                        CSettingsPage, CExitPage,
 *                                        CContinuePage, CGameMenuPage,
 *                                        CNewGamePage, CLoadingGamePage,
 *                                        CRespawnPage.
 */
//======================================================================================================================
#pragma once
//======================================================================================================================

//======================================================================================================================
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>

#include "CVars.h"
#include "CExceptions.h"
#include "CTimer.h"
//======================================================================================================================

//======================================================================================================================
class CPageController;
class CMap;
//===========================enum class EAction=========================================================================
enum class EAction {
    EXIT,
    MAIN_PAGE,
    NEW_GAME_PAGE,
    LOADING_GAME_PAGE,
    CONTINUE_PAGE,
    SETTINGS_PAGE,
    KEYS_PAGE,
    START_GAME,
    ERROR,
    CONTINUE,
    QUIT,
    SAVE
};
//======================================================================================================================

//===========================class CPageInterface=======================================================================
class CPageInterface {
public:
    /**
     *  Initial initialization of the work renderer.
     * @param sdl_renderer - work renderer.
     */
    explicit        CPageInterface      (SDL_Renderer * sdl_renderer):m_renderer(sdl_renderer) {}

    //---------------------------------------------------------------------------------------------
    /**
     * Virtual destructor.
     */
    virtual         ~CPageInterface     () = default;

    //---------------------------------------------------------------------------------------------
    /**
     * Start execution of the main code page.
     * @return the enumeration element that corresponds to the next selected page.
     */
    virtual EAction run                 () = 0;

    //---------------------------------------------------------------------------------------------
    /**
     * Loading all images used.
     */
    virtual void    loadingImages       () = 0;

    //---------------------------------------------------------------------------------------------
    /**
     * Given the initial location of the images.
     */
    virtual void    setOutputRects      () = 0;

    //---------------------------------------------------------------------------------------------
    /**
     * Sets the initial appearance of the images.
     */
    virtual void    drawFirstFrame () = 0;

    //---------------------------------------------------------------------------------------------
    /**
     * Overridable function to check for keystrokes.
     * @param event - actually event for control.
     */
    virtual void    checkKeyboard       (SDL_Event *event) = 0;

    //---------------------------------------------------------------------------------------------
    /**
     * Overridable function for validation of movements and clicks of the mouse.
     * @param event - actually event for control.
     */
    virtual void    checkMouse          (SDL_Event *event) = 0;

    //---------------------------------------------------------------------------------------------
    /**
     * Creates a texture from a PNG file that is named fileName.
     * @param fileName - file path.
     * @return a pointer to created texture.
     */
    SDL_Texture *   createTexture       (const std::string &fileName) const;

    //---------------------------------------------------------------------------------------------
    /**
    * Sets the rectangle parameters to the specified x, y, w(width), h(height).
    * @param src - rectangle that is changing.
    * @param x - coordinate of the point x of the upper left corner.
    * @param y - coordinate of the point y of the upper left corner.
    * @param w - width.
    * @param h - height.
    */
    void            setRect             (SDL_Rect *src,
                                         int       x = 0,
                                         int       y = 0,
                                         int       w = 32,
                                         int       h = 32) const;

    //---------------------------------------------------------------------------------------------
    /**
     * Draws an image of a specific \ full size on a renderer at a specific location
     * or starting from the top left if dstRect == NULL.
     * @param texture - where does the image come from.
     * @param srcRect - what is the size of the drawn image.
     * @param dstRect - what size will be drawn.
     */
    void            drawInPozition      (SDL_Texture    *texture,
                                         SDL_Rect       *srcRect = NULL,
                                         SDL_Rect       *dstRect = NULL) const;

    //---------------------------------------------------------------------------------------------
    /**
     * Clear the renderer inside the window.
     */
    void            clearScreen         () const;

    //---------------------------------------------------------------------------------------------
    /**
     * Refresh the renderer inside the window.
     */
    void            drawScreen          () const;

private:
    SDL_Renderer * m_renderer;
};
//======================================================================================================================

//===========================class CMainPage============================================================================
class CMainPage : private CPageInterface {
public:
    /**
     * @param sdl_renderer - current renderer.
     */
    explicit            CMainPage       (SDL_Renderer *sdl_renderer);

    //------------------------------------------------------------------------------------------------------------------
                        ~CMainPage      () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Start page functionality.
     * @return - next action.
     */
    EAction             run             () override;

    friend CPageController;
private:
    bool                m_good;
    uint8_t             m_selected;
    uint8_t             m_selectedReturn;
    SDL_Texture       * m_textures[5] {NULL, NULL, NULL, NULL, NULL};
    SDL_Rect            m_outTexturesRects[5] {};
    SDL_Texture       * m_backgroundTexture {};
    SDL_Rect            m_selectedRect {};
    SDL_Rect            m_unselectedRect {};

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Loading all images used.
     */
    void                loadingImages   () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Given the initial location of the images.
     */
    void                setOutputRects  () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Sets the initial appearance of the images.
     */
    void                drawFirstFrame () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function to check for keystrokes.
     * @param event - actually event for control.
     */
    void                checkKeyboard   (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function for validation of movements and clicks of the mouse.
     * @param event - actually event for control.
     */
    void                checkMouse      (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Understand where was the click of the mouse.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     * @return - click position.
     */
    uint8_t             understandClick (int    x,
                                         int    y) const;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Decide what action will be next.
     * @return - action.
     */
    EAction             switchReturns   () const;

    //------------------------------------------------------------------------------------------------------------------
    void                reselected      (uint8_t newSelected);
};
//======================================================================================================================

//===========================class CKeysPage============================================================================
class CKeysPage : private CPageInterface {
public:
    /**
     * @param sdl_renderer - current render.
     */
    explicit        CKeysPage       (SDL_Renderer *sdl_renderer);

    //------------------------------------------------------------------------------------------------------------------
                    ~CKeysPage      () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Start page functionality.
     * @return - next action.
     */
    EAction         run             () override;

    friend CPageController;
private:
    bool            m_good;
    bool            m_back;
    SDL_Texture   * m_backgroundTexture {};
    SDL_Texture   * m_keysImage {};
    SDL_Rect        m_keysOutRect {};

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function to check for keystrokes.
     * @param event - actually event for control.
     */
    void            checkKeyboard   (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function for validation of movements and clicks of the mouse.
     * @param event - actually event for control.
     */
    void            checkMouse      (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Loading all images used.
     */
    void            loadingImages   () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Given the initial location of the images.
     */
    void            setOutputRects  () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Sets the initial appearance of the images.
     */
    void            drawFirstFrame  () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Understands the cursor on the target or not.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     * @return - true or false.
     */
    bool            cursorInTarget  (int x, int y) const;
};
//======================================================================================================================

//===========================class CSettingsPage========================================================================
class CSettingsPage : private CPageInterface {
public:
    /**
     * @param sdl_renderer - current render.
     */
    explicit        CSettingsPage   (SDL_Renderer *sdl_renderer);

    //------------------------------------------------------------------------------------------------------------------
                    ~CSettingsPage  () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Start page functionality.
     * @return - next action.
     */
    EAction         run             () override;

    friend CPageController;
private:
    SDL_Texture     *m_settings;
    SDL_Texture     *m_selecter;
    SDL_Texture     *m_background;
    SDL_Rect        m_settingsRect;
    SDL_Rect        m_selecterRect[2];
    bool            m_good;
    uint8_t         m_selected;
    bool            m_back;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Loading all images used.
     */
    void            loadingImages   () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Given the initial location of the images.
     */
    void            setOutputRects  () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Sets the initial appearance of the images.
     */
    void            drawFirstFrame  () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function for validation of movements and clicks of the mouse.
     * @param event - actually event for control.
     */
    void            checkMouse      (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
    * Overridable function to check for keystrokes.
    * @param event - actually event for control.
    */
    void            checkKeyboard   (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Writes the selected type to the file.
     */
    void            writeSelectedType ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Detected mouse click.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     */
    void            clickDetected   (int x, int y);
};
//======================================================================================================================

//===========================class CExitPage============================================================================
class CExitPage : private CPageInterface {
public:
    /**
     * @param sdl_renderer - current render.
     */
    explicit CExitPage (SDL_Renderer *sdl_renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~CExitPage () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Start page functionality.
     * @return - next action.
     */
    EAction run () override;

    friend CPageController;
private:
    SDL_Texture     * m_textures{};
    SDL_Rect          m_restsOut[3]{};
    SDL_Rect          m_restsIn[5]{};
    uint8_t           m_selected;
    bool              m_back;
    bool              m_good;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Loading all images used.
     */
    void loadingImages () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Given the initial location of the images.
     */
    void setOutputRects () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Sets the initial appearance of the images.
     */
    void drawFirstFrame () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function for validation of movements and clicks of the mouse.
     * @param event - actually event for control.
     */
    void checkMouse (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function to check for keystrokes.
     * @param event - actually event for control.
     */
    void checkKeyboard (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Understands the movement of the mouse.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     */
    void understandMouse (int x, int y);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Understands the click of a mouse.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     */
    void understandClick (int x, int y);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw the actual frame.
     */
    void draw ();
};
//======================================================================================================================

//===========================class CContinuePage========================================================================
class CContinuePage : private CPageInterface {
public:
    //------------------------------------------------------------------------------------------------------------------
    /**
     * @param sdl_renderer - current render.
     */
    explicit CContinuePage (SDL_Renderer * sdl_renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~CContinuePage () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Start page functionality.
     * @return - next action.
     */
    EAction run () override;

    friend CPageController;
private:
    SDL_Texture   * m_backgroundTexture;
    SDL_Texture   * m_selectedTexture;
    SDL_Texture   * m_numsTexture;
    SDL_Rect        m_numsRect[10];
    SDL_Rect        m_mapRect;
    SDL_Rect        m_selecterRect;
    SDL_Rect        m_outSelectedTextureRect;
    SDL_Rect        m_outMapRects[10];
    SDL_Rect        m_outFirstNumRects[10];
    SDL_Rect        m_outSecondNumRects[10];
    SDL_Rect        m_outSelecterRects[10];
    bool            m_good;
    bool            m_back;
    bool            m_select;
    uint8_t         m_selected;
    uint8_t         m_count;
    uint8_t         m_top;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Loading all images used.
     */
    void loadingImages () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Given the initial location of the images.
     */
    void setOutputRects () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Sets the initial appearance of the images.
     */
    void drawFirstFrame () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function for validation of movements and clicks of the mouse.
     * @param event - actually event for control.
     */
    void checkMouse (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function to check for keystrokes.
     * @param event - actually event for control.
     */
    void checkKeyboard (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Write the text to select the map.
     */
    void drawText ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Reads the number of maps created from the file.
     */
    void readCount ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Understands the click of a mouse.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     */
    void understandClick (int x, int y);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Writes the selected map to a file/
     */
    void writeSelect ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw a number to the desired position.
     * @param pos - current position.
     */
    void drawMapNum (uint8_t pos);
};
//======================================================================================================================

//===========================class CGameMenuPage========================================================================
class CGameMenuPage : private CPageInterface {
public:
    /**
     * @param sdl_renderer - current render.
     */
    explicit CGameMenuPage (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~CGameMenuPage () override ;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Controller for CGameMenuPage.
     * @return - QUIT if event SDL_QUIT or CONTINUE, SAVE, EXIT(to main menu).
     */
    EAction run () override;

    friend CPageController;
private:
    SDL_Texture   * m_continue;
    SDL_Texture   * m_save;
    SDL_Texture   * m_exit;
    SDL_Rect        m_r_texture [2];
    SDL_Rect        m_r_out [3];
    bool            m_good;
    bool            m_back;
    uint8_t         m_select;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Loading all images used.
     */
    void loadingImages () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Given the initial location of the images.
     */
    void setOutputRects () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Sets the initial appearance of the images.
     */
    void drawFirstFrame () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function for validation of movements and clicks of the mouse.
     * @param event - actually event for control.
     */
    void checkMouse (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function to check for keystrokes.
     * @param event - actually event for control.
     */
    void checkKeyboard (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Check mouse position.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     * @return - 0 - CONTINUE, 1 - SAVE, 2 - EXIT, 3 - NO FOUND.
     */
    int understandMouse (int x, int y) const;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw currently frame.
     */
    void drawFrame ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Returns the action depending on the selection.
     * @return - action.
     */
    EAction identify () const;
};
//======================================================================================================================

//===========================class CNewGamePage=========================================================================
class CNewGamePage : private CPageInterface {
public:
    /**
     * @param sdl_renderer - current render.
     */
    explicit CNewGamePage (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~CNewGamePage () override ;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Controller for CGameMenuPage.
     * @return - EXIT if event SDL_QUIT or LOADING_GAME_PAGE.
     */
    EAction run () override;

    friend CPageController;
private:
    SDL_Texture     * m_background;
    bool              m_good;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Loading all images used.
     */
    void loadingImages () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Given the initial location of the images.
     */
    void setOutputRects () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Sets the initial appearance of the images.
     */
    void drawFirstFrame () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function for validation of movements and clicks of the mouse.
     * @param event - actually event for control.
     */
    void checkMouse (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function to check for keystrokes.
     * @param event - actually event for control.
     */
    void checkKeyboard (SDL_Event *event) override;
};
//======================================================================================================================

//===========================class CLoadingGamePage=====================================================================
class CLoadingGamePages : private CPageInterface {
public:
    /**
     * @param renderer - current renderer.
     * @param map - game map for create.
     */
    explicit CLoadingGamePages (SDL_Renderer * renderer, CMap ** map);

    //------------------------------------------------------------------------------------------------------------------
    ~CLoadingGamePages () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Controller for CLoadingGamePages.
     * @return - EXIT if event SDL_QUIT or NEW_GAME.
     */
    EAction run () override;

    friend CPageController;
private:
    CMap        ** m_map;
    SDL_Texture  * m_background;
    SDL_Renderer * m_renderer;
    bool           m_good;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Loading all images used.
     */
    void loadingImages () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Given the initial location of the images.
     */
    void setOutputRects () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Sets the initial appearance of the images.
     */
    void drawFirstFrame () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function for validation of movements and clicks of the mouse.
     * @param event - actually event for control.
     */
    void checkMouse (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function to check for keystrokes.
     * @param event - actually event for control.
     */
    void checkKeyboard (SDL_Event *event) override;
};
//======================================================================================================================

//===========================class CRespawnPage=========================================================================
class CRespawnPage : private CPageInterface {
public:
    /**
     * @param sdl_renderer - current render.
     */
    explicit CRespawnPage (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~CRespawnPage () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Controller for CLoadingGamePages.
     * @return - EXIT if event SDL_QUIT or LOADING_GAME_PAGE.
     */
    EAction run () override;

    friend CPageController;
private:
    SDL_Texture  * m_background;
    SDL_Texture  * m_time[5];
    SDL_Rect       m_outRect;
    int            m_currentFrame;
    bool           m_good;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Loading all images used.
     */
    void loadingImages () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Given the initial location of the images.
     */
    void setOutputRects () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Sets the initial appearance of the images.
     */
    void drawFirstFrame () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function for validation of movements and clicks of the mouse.
     * @param event - actually event for control.
     */
    void checkMouse (SDL_Event *event) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Overridable function to check for keystrokes.
     * @param event - actually event for control.
     */
    void checkKeyboard (SDL_Event *event) override;
};
//======================================================================================================================