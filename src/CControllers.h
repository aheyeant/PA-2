/**
 * @file Contains declaration of classes: CGameController, CPageController, EGameAction.
 */
//======================================================================================================================
#pragma once
//======================================================================================================================

//======================================================================================================================
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <list>

#include "CMap.h"
#include "CPages.h"
#include "CExceptions.h"
#include "CSDLInitializer.h"

//======================enum EGameAction================================================================================
enum class EGameAction {
    RESTART,
    FINISH
};

//======================class CGameController===========================================================================
class CGameController {
public:
    /**
     * @param map - the game map
     */
    explicit CGameController (CMap * map = nullptr);

    //------------------------------------------------------------------------------------------------------------------
    ~CGameController ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Controls the gameplay.
     */
    EGameAction run ();

private:
    CMap * m_map;
};
//======================================================================================================================

//======================class CPageController===========================================================================
class CPageController {
public:
    //------------------------------------------------------------------------------------------------------------------
    CPageController ();

    //------------------------------------------------------------------------------------------------------------------
    ~CPageController ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Controls the menu and start the game.
     */
    void run ();
};
//======================================================================================================================