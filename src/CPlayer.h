/**
 * @file Contains declaration of classes: CPlayerInterface, CPlayer, CZombie, CWarrior.
 */
//======================================================================================================================
#pragma once
//======================================================================================================================

//======================================================================================================================
#include <SDL2/SDL_system.h>
#include <SDL2/SDL_events.h>

#include "CTails.h"
#include "CPages.h"
#include "CInventory.h"
//======================================================================================================================

//=================================struct TRect=========================================================================
/**
 * The structure that stores the coordinates of the characters.
 */
struct TRect {
    float x;
    float y;
    float w;
    float h;
};

//=================================class CPlayerInterface===============================================================
class CPlayerInterface {
public:
     /**
     * Creates a nip based on the type.
     * @param type - type of NPC.
     * @param map - actually map.
     * @param texture - texture NPC.
     * @param x - x-coordinate in pxl.
     * @param y - y-coordinate in pxl.
     */
    CPlayerInterface (uint8_t                type,
                      CBuildTailInterface ***map,
                      SDL_Texture           *texture,
                      int                    x,
                      int                    y);

    //------------------------------------------------------------------------------------------------------------------
    virtual ~CPlayerInterface ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * NPC controller.
     * @param time
     */
    virtual void update (uint32_t time) = 0;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Take damage.
     * @param damage - the size of the damage.
     */
    virtual void getDamage (float damage, bool left) = 0;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Controls the permissible movements of the NPC.
     * @param dir - 0 - x-coordinate, 1 - y-coordinate,
     */
    void collision (uint8_t dir, uint32_t time);

    //------------------------------------------------------------------------------------------------------------------
    TRect                    m_position;
    float                    m_health;
    float                    m_armor;
    float                    m_damage;
    float                    dx;
    float                    dy;
    bool                     m_onGround;
    bool                     m_isAlive;
    SDL_Texture             *m_texture;
private:
    CBuildTailInterface   ***m_map;
};
//======================================================================================================================

//=================================class CPlayer========================================================================
class CPlayer : public CPlayerInterface {       // id = 0
public:
    /**
     * @param type - personage type.
     * @param map - game map.
     * @param texture - personage texture.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     */
    CPlayer (uint8_t                type,
             CBuildTailInterface ***map,
             SDL_Texture           *texture,
             int                    x,
             int                    y);

    //------------------------------------------------------------------------------------------------------------------
    ~CPlayer () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * NPC controller.
     * @param time
     */
    void update (uint32_t time) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Take damage.
     * @param damage - the size of the damage.
     */
    void getDamage (float damage, bool left) override;

    //------------------------------------------------------------------------------------------------------------------
    CInventory * inventory;
};
//======================================================================================================================

//=================================class CZombie========================================================================
class CZombie : public CPlayerInterface {       // id = 1
public:
    /**
     * @param type - personage type.
     * @param map - game map.
     * @param texture - personage texture.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     */
    CZombie (uint8_t                type,
             CBuildTailInterface ***map,
             SDL_Texture           *texture,
             int                    x,
             int                    y);

    //------------------------------------------------------------------------------------------------------------------
    ~CZombie () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * NPC controller.
     * @param time
     */
    void update (uint32_t time) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Take damage.
     * @param damage - the size of the damage.
     */
    void getDamage (float damage, bool left) override;
};
//======================================================================================================================

//=================================class CWarrior=======================================================================
class CWarrior : public CPlayerInterface {       // id = 2
public:
    /**
     * @param type - personage type.
     * @param map - game map.
     * @param texture - personage texture.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     */
    CWarrior (uint8_t                type,
              CBuildTailInterface ***map,
              SDL_Texture           *texture,
              int                    x,
              int                    y);

    //------------------------------------------------------------------------------------------------------------------
    ~CWarrior () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * NPC controller.
     * @param time
     */
    void update (uint32_t time) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Take damage.
     * @param damage - the size of the damage.
     */
    void getDamage (float damage, bool left) override;
};
//======================================================================================================================