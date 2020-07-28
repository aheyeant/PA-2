/**
 * @file Implementation of classes: CBuildChest, CTailMaker,
 *                                  CInventoryTails, CBuildFluid,
 *                                  CBuildGround, CBuildTail,
 *                                  CBuildTailInterface.
 */
//======================================================================================================================
#include "CTails.h"
//======================================================================================================================

//===========================class CBuildTailInterface==================================================================
/**
 * @param id - tail id.
 * @param BG_id - background id.
 * @param koefPermeability - coefficient permeability.
 * @param health
 * @param healthType
 * @param armor
 */
CBuildTailInterface::CBuildTailInterface (uint8_t id,
                                          uint8_t BG_id,
                                          float   koefPermeability,
                                          float   health,
                                          float   healthType,
                                          float   armor):m_id               (id),
                                                         m_BG_id            (BG_id),
                                                         m_coefPermeability (koefPermeability),
                                                         m_health           (health),
                                                         m_healthType       (healthType),
                                                         m_armor            (armor) {
    m_underSky = false;
}

//----------------------------------------------------------------------------------------------------------------------
CBuildTailInterface::~CBuildTailInterface () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * Change the flag.
 * @param flag
 */
void CBuildTailInterface::setUnderSky (bool flag) {
    m_underSky = flag;
}
//======================================================================================================================

//===========================class CBuildTail===========================================================================
/**
 * @param id - tail id.
 * @param BG_id - background id.
 * @param koefPermeability - coefficient permeability.
 * @param health
 * @param healthType
 * @param armor
 * @param texture
 */
CBuildTail::CBuildTail (uint8_t         id,
                        uint8_t         BG_id,
                        float           koefPermeability,
                        float           health,
                        float           healthType,
                        float           armor,
                        SDL_Texture    *texture): CBuildTailInterface(id,
                                                                      BG_id,
                                                                      koefPermeability,
                                                                      health,
                                                                      healthType,
                                                                      armor),
                                                  m_texture          (texture) {}

//----------------------------------------------------------------------------------------------------------------------
CBuildTail::~CBuildTail () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * Changes with respect to time.
 * @param time
 */
void CBuildTail::update (float time) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * @return - current texture.
 */
SDL_Texture* CBuildTail::getTexture () const {
    return m_texture;
}
//======================================================================================================================

//===========================class CBuildGround=========================================================================
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
CBuildGround::CBuildGround (uint8_t        id,
                            uint8_t        BG_id,
                            float          koefPermeability,
                            float          health,
                            float          healthType,
                            float          armor,
                            SDL_Texture   *ground,
                            SDL_Texture   *greenGround): CBuildTailInterface(id,
                                                                             BG_id,
                                                                             koefPermeability,
                                                                             health,
                                                                             healthType,
                                                                             armor),
                                                         m_ground           (ground),
                                                         m_greenGround      (greenGround),
                                                         m_time             (0) {}

//----------------------------------------------------------------------------------------------------------------------
CBuildGround::~CBuildGround () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * Changes with respect to time.
 * @param time
 */
void CBuildGround::update (float time) {
    if (m_underSky) {
        m_time += time;
    } else {
        m_time = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * @return - current texture.
 */
SDL_Texture* CBuildGround::getTexture () const {
    return (m_time > 10000 && m_underSky ? m_greenGround : m_ground);
}
//======================================================================================================================

//===========================class CBuildFluid==========================================================================
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
CBuildFluid::CBuildFluid (uint8_t        id,
                          uint8_t        BG_id,
                          float          koefPermeability,
                          float          health,
                          float          healthType,
                          float          armor,
                          SDL_Texture   *t_1,
                          SDL_Texture   *t_2,
                          SDL_Texture   *t_3,
                          SDL_Texture   *t_4): CBuildTailInterface(id,
                                                                   BG_id,
                                                                   koefPermeability,
                                                                   health,
                                                                   healthType,
                                                                   armor),
                                               m_currentFrame      (0) {
    m_texture[0] = t_1;
    m_texture[1] = t_2;
    m_texture[2] = t_3;
    m_texture[3] = t_4;
}

//----------------------------------------------------------------------------------------------------------------------
CBuildFluid::~CBuildFluid () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * @return - current texture.
 */
SDL_Texture* CBuildFluid::getTexture () const {
    return m_texture[(int)m_currentFrame];
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Changes with respect to time.
 * @param time
 */
void CBuildFluid::update (float time) {
    m_currentFrame += time * CHANGE_FRAME_SPEED / 4000;
    if (m_currentFrame >= 4) {
        m_currentFrame -= 4;
    }
}
//======================================================================================================================

//===========================class CInventoryTails======================================================================
/**
 * @param id - tail id.
 * @param count
 * @param maxCount
 * @param damage
 * @param damageType
 * @param forMap - true == can be put on map.
 * @param texture
 */
CInventoryTails::CInventoryTails (uint8_t       id,
                                  uint8_t       count,
                                  uint8_t       maxCount,
                                  uint8_t       damage,
                                  uint8_t       damageType,
                                  bool          forMap,
                                  SDL_Texture  *texture):
                m_id         (id),
                m_count      (count),
                m_maxCount   (maxCount),
                m_damage     (damage),
                m_damageType (damageType),
                m_forMap     (forMap),
                m_texture    (texture) {
    if (m_count > m_maxCount) {
        m_count = m_maxCount;
    }
}
//======================================================================================================================

//===========================class CTailMaker===========================================================================
/**
 * @param renderer - current render.
 */
CTailMaker::CTailMaker (SDL_Renderer * renderer) {
    CImageLoader cImageLoader(renderer);
    m_image = cImageLoader.getImages();
}

//----------------------------------------------------------------------------------------------------------------------
CTailMaker::~CTailMaker () {
    delete m_image;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Create a new item for the map.
 * @param id - tail id.
 * @param BG_id - background id.
 * @return - map item.
 */
CBuildTailInterface* CTailMaker::getBuildTail (uint8_t id, uint8_t BG_id) {
    CBuildGround * tmp;
    switch (id) {
        case 1:
            return new CBuildGround (1, (!BG_id ? (uint8_t)16 : BG_id), 1, TAIL_HEALTH_UPPER * 15, 1, 0,
                                     m_image->mapTail->t_ground, m_image->mapTail->t_greenGround);
        case 2:
            tmp = new CBuildGround (1, (!BG_id ? (uint8_t)16 : BG_id), 1, TAIL_HEALTH_UPPER * 15, 1, 0,
                                         m_image->mapTail->t_ground, m_image->mapTail->t_greenGround);
            tmp->setUnderSky(true);
            tmp->update(10000);
            return tmp;
        case 3:
            return new CBuildTail (3,  (!BG_id ? (uint8_t)16 : BG_id), 1, TAIL_HEALTH_UPPER * 10,  1, 0,   m_image->mapTail->t_sand);
        case 4:
            return new CBuildTail (4,  (!BG_id ? (uint8_t)17 : BG_id), 1, TAIL_HEALTH_UPPER * 10,  2, 0,   m_image->mapTail->t_stone);
        case 5:
            return new CBuildTail (5,  (!BG_id ? (uint8_t)17 : BG_id), 1, TAIL_HEALTH_UPPER * 10,  2, 0,   m_image->mapTail->t_mossStone);
        case 6:
            return new CBuildTail (6,  (!BG_id ? (uint8_t)8 : BG_id),  1, TAIL_HEALTH_UPPER * 15,  3, 0,   m_image->mapTail->t_wood);
        case 7:
            return new CBuildTail (7,  (!BG_id ? (uint8_t)17 : BG_id), 1, TAIL_HEALTH_UPPER * 100, 1, 100, m_image->mapTail->t_lavaStone);
        case 8:
            return new CBuildTail (8,  (!BG_id ? (uint8_t)8 : BG_id),  0, TAIL_HEALTH_UPPER * 100, 1, 100, m_image->mapTail->t_BG_sky);
        case 9:
            return new CBuildTail (9,  (!BG_id ? (uint8_t)17 : BG_id), 1, TAIL_HEALTH_UPPER * 8,   3, 0,   m_image->mapTail->t_workbench);
        case 10:
            return new CBuildChest(10, (!BG_id ? (uint8_t)17 : BG_id), 1, TAIL_HEALTH_UPPER * 8,   3, 0,   m_image->mapTail->t_chest);
        case 11:
            return new CBuildTail (11, (!BG_id ? (uint8_t)17 : BG_id), 1, TAIL_HEALTH_UPPER * 12,  2, 4,   m_image->mapTail->t_iron);
        case 12:
            return new CBuildTail (12, (!BG_id ? (uint8_t)17 : BG_id), 1, TAIL_HEALTH_UPPER * 12,  2, 4,   m_image->mapTail->t_gold);
        case 13:
            return new CBuildTail (13, (!BG_id ? (uint8_t)17 : BG_id), 1, TAIL_HEALTH_UPPER * 12,  2, 16,  m_image->mapTail->t_diamond);
        case 14:
            return new CBuildTail (14, (!BG_id ? (uint8_t)17 : BG_id), 1, TAIL_HEALTH_UPPER * 12,  2, 4,   m_image->mapTail->t_coal);
        case 15:
            return new CBuildTail (15, (!BG_id ? (uint8_t)8 : BG_id),  1, TAIL_HEALTH_UPPER * 4,   1, 0,   m_image->mapTail->t_foliage);
        case 16:
            return new CBuildTail (16, (!BG_id ? (uint8_t)16 : BG_id), 0, TAIL_HEALTH_UPPER * 100, 1, 100, m_image->mapTail->t_BG_ground);
        case 17:
            return new CBuildTail (17, (!BG_id ? (uint8_t)17 : BG_id), 0, TAIL_HEALTH_UPPER * 100, 1, 100, m_image->mapTail->t_BG_stone);
        case 19:
            return new CBuildFluid (19, (!BG_id ? (uint8_t)17 : BG_id), 0.5, TAIL_HEALTH_UPPER * 100, 1, 100,
                                    m_image->mapTail->lava->t_lava_1, m_image->mapTail->lava->t_lava_2,
                                    m_image->mapTail->lava->t_lava_3, m_image->mapTail->lava->t_lava_4);
        case 18:
            return new CBuildFluid (18, (!BG_id ? (uint8_t)17 : BG_id), 0.5, TAIL_HEALTH_UPPER * 100, 1, 100,
                                    m_image->mapTail->water->t_water_1, m_image->mapTail->water->t_water_2,
                                    m_image->mapTail->water->t_water_3, m_image->mapTail->water->t_water_4);
        default:
            return new CBuildGround (1, (!BG_id ? (uint8_t)16 : BG_id), 1,   TAIL_HEALTH_UPPER * 15, 1, 0,
                                     m_image->mapTail->t_ground, m_image->mapTail->t_greenGround);
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Create a new item for inventory.
 * @param id - tail id.
 * @param count
 * @return - inventory item.
 */
CInventoryTails* CTailMaker::getInventoryTail (uint8_t id, uint8_t count) {
    switch (id) {
        case 101:
            return new CInventoryTails (101, count, 99, 8, 1, true,  m_image->inventoryTail->tails->t_ground);
        case 102:
            return new CInventoryTails (102, count, 99, 8, 1, true,  m_image->inventoryTail->tails->t_stone);
        case 103:
            return new CInventoryTails (103, count, 99, 8, 1, true,  m_image->inventoryTail->tails->t_sand);
        case 104:
            return new CInventoryTails (104, count, 99, 8, 1, true,  m_image->inventoryTail->tails->t_mossStone);
        case 105:
            return new CInventoryTails (105, count, 99, 8, 1, true,  m_image->inventoryTail->tails->t_wood);
        case 106:
            return new CInventoryTails (106, count, 99, 8, 1, true,  m_image->inventoryTail->tails->t_foliage);
        case 107:
            return new CInventoryTails (107, count, 99, 8, 1, false, m_image->inventoryTail->tails->t_coal);
        case 108:
            return new CInventoryTails (108, count, 99, 8, 1, false, m_image->inventoryTail->tails->t_iron);
        case 109:
            return new CInventoryTails (109, count, 99, 8, 1, false, m_image->inventoryTail->tails->t_gold);
        case 110:
            return new CInventoryTails (110, count, 99, 8, 1, false, m_image->inventoryTail->tails->t_diamond);
        case 111:
            return new CInventoryTails (111, count, 1,  8, 1, true,  m_image->inventoryTail->tails->t_chest);
        case 112:
            return new CInventoryTails (112, count, 99, 8, 1, true,  m_image->inventoryTail->tails->t_workbench);
        case 113:
            return new CInventoryTails (113, count, 99, 8, 1, false, m_image->inventoryTail->t_stick);

        case 151:
            return new CInventoryTails (151, count, 1, 16, 2, false, m_image->inventoryTail->picks->t_woodPick);
        case 152:
            return new CInventoryTails (152, count, 1, 20, 2, false, m_image->inventoryTail->picks->t_stonePick);
        case 153:
            return new CInventoryTails (153, count, 1, 24, 2, false, m_image->inventoryTail->picks->t_ironPick);
        case 154:
            return new CInventoryTails (154, count, 1, 28, 2, false, m_image->inventoryTail->picks->t_goldPick);
        case 155:
            return new CInventoryTails (155, count, 1, 34, 2, false, m_image->inventoryTail->picks->t_diamondPick);

        case 161:
            return new CInventoryTails (161, count, 1, 16, 1, false, m_image->inventoryTail->blades->t_woodBlade);
        case 162:
            return new CInventoryTails (162, count, 1, 20, 1, false, m_image->inventoryTail->blades->t_stoneBlade);
        case 163:
            return new CInventoryTails (163, count, 1, 24, 1, false, m_image->inventoryTail->blades->t_ironBlade);
        case 164:
            return new CInventoryTails (164, count, 1, 28, 1, false, m_image->inventoryTail->blades->t_goldBlade);
        case 165:
            return new CInventoryTails (165, count, 1, 34, 1, false, m_image->inventoryTail->blades->t_diamondBlade);

        case 171:
            return new CInventoryTails (171, count, 1, 16, 3, false, m_image->inventoryTail->axes->t_woodAxe);
        case 172:
            return new CInventoryTails (172, count, 1, 20, 3, false, m_image->inventoryTail->axes->t_stoneAxe);
        case 173:
            return new CInventoryTails (173, count, 1, 24, 3, false, m_image->inventoryTail->axes->t_ironAxe);
        case 174:
            return new CInventoryTails (174, count, 1, 28, 3, false, m_image->inventoryTail->axes->t_goldAxe);
        case 175:
            return new CInventoryTails (175, count, 1, 34, 3, false, m_image->inventoryTail->axes->t_diamondAxe);


        case 181:
            return new CInventoryTails (181, count, 1, 8, 1, false, m_image->inventoryTail->helmets->t_ironHelmet);
        case 182:
            return new CInventoryTails (182, count, 1, 8, 1, false, m_image->inventoryTail->helmets->t_goldHelmet);
        case 183:
            return new CInventoryTails (183, count, 1, 8, 1, false, m_image->inventoryTail->helmets->t_diamondHelmet);

        case 191:
            return new CInventoryTails (191, count, 1, 8, 1, false, m_image->inventoryTail->armors->t_ironArmor);
        case 192:
            return new CInventoryTails (192, count, 1, 8, 1, false, m_image->inventoryTail->armors->t_goldArmor);
        case 193:
            return new CInventoryTails (193, count, 1, 8, 1, false, m_image->inventoryTail->armors->t_diamondArmor);

        case 201:
            return new CInventoryTails (201, count, 1, 8, 1, false, m_image->inventoryTail->boots->t_ironBoot);
        case 202:
            return new CInventoryTails (202, count, 1, 8, 1, false, m_image->inventoryTail->boots->t_goldBoot);
        case 203:
            return new CInventoryTails (203, count, 1, 8, 1, false, m_image->inventoryTail->boots->t_diamondBoot);

        default:
            return new CInventoryTails (101, count, 99, 8, 1, false, m_image->inventoryTail->tails->t_ground);
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Create a new item for the map from the inventory item.
 * @param id - tail id.
 * @param BG_id - background id.
 * @return - map item.
 */
CBuildTailInterface* CTailMaker::getBuildTailFromInventory (uint8_t id, uint8_t BG_id) {
    switch (id) {
        case 101:
            return getBuildTail( 1, BG_id);
        case 102:
            return getBuildTail( 4, BG_id);
        case 103:
            return getBuildTail( 3, BG_id);
        case 104:
            return getBuildTail( 5, BG_id);
        case 105:
            return getBuildTail( 6, BG_id);
        case 106:
            return getBuildTail(15, BG_id);
        case 111:
            return getBuildTail(10, BG_id);
        case 112:
            return getBuildTail( 9, BG_id);
        default:
            return getBuildTail( 1, BG_id);
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Create a new item for inventory from a map item.
 * @param id - tail id.
 * @param count
 * @return - inventory item.
 */
CInventoryTails* CTailMaker::getInventoryTailFromBuild (uint8_t id, uint8_t count) {
    switch (id) {
        case  1:
            return getInventoryTail(101);
        case  2:
            return getInventoryTail(101);
        case  3:
            return getInventoryTail(103);
        case  4:
            return getInventoryTail(102);
        case  5:
            return getInventoryTail(104);
        case  6:
            return getInventoryTail(105);
        case  9:
            return getInventoryTail(112);
        case 10:
            return getInventoryTail(111);
        case 11:
            return getInventoryTail(108);
        case 12:
            return getInventoryTail(109);
        case 13:
            return getInventoryTail(110);
        case 14:
            return getInventoryTail(107);
        case 15:
            return getInventoryTail(106);
        default:
            return getInventoryTail(101);
    }


}
//======================================================================================================================

//===========================class CBuildChest==========================================================================
/**
 * @param id - tail id.
 * @param BG_id - background id.
 * @param koefPermeability - coefficient permeability.
 * @param health
 * @param healthType
 * @param armor
 * @param texture
 */
CBuildChest::CBuildChest (uint8_t       id,
                          uint8_t       BG_id,
                          float         koefPermeability,
                          float         health,
                          float         healthType,
                          float         armor,
                          SDL_Texture  *texture):CBuildTailInterface(id,
                                                                     BG_id,
                                                                     koefPermeability,
                                                                     health,
                                                                     healthType,
                                                                     armor),
                                                 m_texture          (texture) {}

//----------------------------------------------------------------------------------------------------------------------
CBuildChest::~CBuildChest () {
    for (auto item : m_inventory) {
        if (item) {
            delete item;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Changes with respect to time.
 * @param time
 */
void CBuildChest::update (float time) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * @return - current texture.
 */
SDL_Texture* CBuildChest::getTexture () const {
    return m_texture;
}
//======================================================================================================================