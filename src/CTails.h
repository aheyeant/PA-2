/**
 * @file Contains declaration of classes: CBuildChest, CTailMaker,
 *                                        CInventoryTails, CBuildFluid,
 *                                        CBuildGround, CBuildTail,
 *                                        CBuildTailInterface.
 */
//======================================================================================================================
#pragma once
//======================================================================================================================


#include <cstdint>
#include <SDL2/SDL_system.h>
#include <vector>
#include <iostream>

#include "CVars.h"
#include "CImageLoader.h"
#include "CExceptions.h"

//===========================class CBuildTailInterface==================================================================
class CBuildTailInterface {
public:
    /**
     * @param id - tail id.
     * @param BG_id - background id.
     * @param koefPermeability - coefficient permeability.
     * @param health
     * @param healthType
     * @param armor
     */
    CBuildTailInterface (uint8_t id,
                         uint8_t BG_id,
                         float   koefPermeability,
                         float   health,
                         float   healthType,
                         float   armor);

    //------------------------------------------------------------------------------------------------------------------
    virtual ~CBuildTailInterface ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Changes with respect to time.
     * @param time
     */
    virtual void update (float time) = 0;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * @return - current texture.
     */
    virtual SDL_Texture * getTexture () const = 0;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Change the flag.
     * @param flag
     */
    void setUnderSky (bool flag);

    //------------------------------------------------------------------------------------------------------------------
    uint8_t m_id;
    uint8_t m_BG_id;
    float m_coefPermeability; // 0 - air , 0.5 - fluid, 1 - stone
    float m_health;
    float m_healthType;
    float m_armor;
    bool  m_underSky;
};
//======================================================================================================================

//===========================class CBuildTail===========================================================================
class CBuildTail : public CBuildTailInterface {
public:
    /**
     * @param id - tail id.
     * @param BG_id - background id.
     * @param koefPermeability - coefficient permeability.
     * @param health
     * @param healthType
     * @param armor
     * @param texture
     */
    CBuildTail (uint8_t         id,
                uint8_t         BG_id,
                float           koefPermeability,
                float           health,
                float           healthType,
                float           armor,
                SDL_Texture    *texture);

    //------------------------------------------------------------------------------------------------------------------
    ~CBuildTail () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Changes with respect to time.
     * @param time
     */
    void update (float time) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * @return - current texture.
     */
    SDL_Texture * getTexture () const override;

private:
    SDL_Texture * m_texture;
};
//======================================================================================================================

//===========================class CBuildGround=========================================================================
class CBuildGround : public CBuildTailInterface {
public:
    /**
     * @param id - tail id.
     * @param BG_id - background id.
     * @param koefPermeability - coefficient permeability.
     * @param health
     * @param healthType
     * @param armor
     * @param ground - ground texture.
     * @param greenGround - green ground texture.
     */
    CBuildGround (uint8_t        id,
                  uint8_t        BG_id,
                  float          koefPermeability,
                  float          health,
                  float          healthType,
                  float          armor,
                  SDL_Texture   *ground,
                  SDL_Texture   *greenGround);

    //------------------------------------------------------------------------------------------------------------------
    ~CBuildGround () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Changes with respect to time.
     * @param time
     */
    void update (float time) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * @return - current texture.
     */
    SDL_Texture * getTexture () const override;

private:
    SDL_Texture * m_ground;
    SDL_Texture * m_greenGround;
    float m_time;
};
//======================================================================================================================

//===========================class CBuildFluid==========================================================================
class CBuildFluid : public CBuildTailInterface {
public:
    /**
     * @param id - tail id.
     * @param BG_id - background id.
     * @param koefPermeability - coefficient permeability.
     * @param health
     * @param healthType
     * @param armor
     * @param t_1 - texture 1.
     * @param t_2 - texture 2.
     * @param t_3 - texture 3.
     * @param t_4 - texture 4.
     */
    CBuildFluid  (uint8_t        id,
                  uint8_t        BG_id,
                  float          koefPermeability,
                  float          health,
                  float          healthType,
                  float          armor,
                  SDL_Texture   *t_1,
                  SDL_Texture   *t_2,
                  SDL_Texture   *t_3,
                  SDL_Texture   *t_4);

    //------------------------------------------------------------------------------------------------------------------
    ~CBuildFluid () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Changes with respect to time.
     * @param time
     */
    void update (float time) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * @return - current texture.
     */
    SDL_Texture * getTexture () const override;

private:
    SDL_Texture * m_texture[4];
    float m_currentFrame;
};
//======================================================================================================================

//===========================class CInventoryTails======================================================================
class CInventoryTails {
public:
    /**
     * @param id - tail id.
     * @param count
     * @param maxCount
     * @param damage
     * @param damageType
     * @param forMap - true == can be put on map.
     * @param texture
     */
    CInventoryTails (uint8_t        id,
                     uint8_t        count,
                     uint8_t        maxCount,
                     uint8_t        damage,
                     uint8_t        damageType,
                     bool           forMap,
                     SDL_Texture   *texture);

    //------------------------------------------------------------------------------------------------------------------
    uint8_t         m_id;
    uint8_t         m_count;
    uint8_t         m_maxCount;
    uint8_t         m_damage;
    uint8_t         m_damageType;
    bool            m_forMap;
    SDL_Texture    *m_texture;
};
//======================================================================================================================

//===========================class CTailMaker===========================================================================
class CTailMaker {
public:
    /**
     * @param renderer - current render.
     */
    explicit CTailMaker (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~CTailMaker ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Create a new item for the map.
     * @param id - tail id.
     * @param BG_id - background id.
     * @return - map item.
     */
    CBuildTailInterface * getBuildTail (uint8_t id, uint8_t BG_id = 0);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Create a new item for inventory.
     * @param id - tail id.
     * @param count
     * @return - inventory item.
     */
    CInventoryTails * getInventoryTail (uint8_t id, uint8_t count = 1);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Create a new item for the map from the inventory item.
     * @param id - tail id.
     * @param BG_id - background id.
     * @return - map item.
     */
    CBuildTailInterface * getBuildTailFromInventory (uint8_t id, uint8_t BG_id = 0);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Create a new item for inventory from a map item.
     * @param id - tail id.
     * @param count
     * @return - inventory item.
     */
    CInventoryTails * getInventoryTailFromBuild (uint8_t id, uint8_t count = 1);

    //------------------------------------------------------------------------------------------------------------------
    TImages * m_image;
};
//======================================================================================================================

//===========================class CBuildChest==========================================================================
class CBuildChest : public CBuildTailInterface {
public:
    /**
     * @param id - tail id.
     * @param BG_id - background id.
     * @param koefPermeability - coefficient permeability.
     * @param health
     * @param healthType
     * @param armor
     * @param texture
     */
    CBuildChest  (uint8_t        id,
                  uint8_t        BG_id,
                  float          koefPermeability,
                  float          health,
                  float          healthType,
                  float          armor,
                  SDL_Texture   *texture);

    //------------------------------------------------------------------------------------------------------------------
    ~CBuildChest () override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Changes with respect to time.
     * @param time
     */
    void update (float time) override;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * @return - current texture.
     */
    SDL_Texture * getTexture () const override;

    //------------------------------------------------------------------------------------------------------------------
    std::vector<CInventoryTails *> m_inventory = {nullptr, nullptr, nullptr, nullptr, nullptr,
                                                  nullptr, nullptr, nullptr, nullptr, nullptr,
                                                  nullptr, nullptr, nullptr, nullptr, nullptr,
                                                  nullptr, nullptr, nullptr, nullptr, nullptr,
                                                  nullptr, nullptr, nullptr, nullptr, nullptr,
                                                  nullptr, nullptr, nullptr, nullptr, nullptr};
private:
    SDL_Texture * m_texture;
};
//======================================================================================================================