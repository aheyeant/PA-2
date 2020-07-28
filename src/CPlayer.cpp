/**
 * @file Implementation of classes: CPlayerInterface, CPlayer, CZombie, CWarrior.
 */
//======================================================================================================================
#include "CPlayer.h"
//======================================================================================================================

//=================================class CPlayerInterface===============================================================
/**
 * Creates a nip based on the type.
 * @param type - type of NPC.
 * @param map - actually map.
 * @param texture - texture NPC.
 * @param x - x-coordinate in pxl.
 * @param y - y-coordinate in pxl.
 */
CPlayerInterface::CPlayerInterface (uint8_t                  type,
                                    CBuildTailInterface   ***map,
                                    SDL_Texture             *texture,
                                    int                      x,
                                    int                      y):
                                                                m_texture (texture),
                                                                m_map     (map) {
    if (type == 0) {
        m_health = PLAYER_HEALTH;
        m_armor = 0;
        m_damage = PLAYER_HAND_DAMAGE;
    }
    if (type == 1) {
        m_health = ZOMBIE_HEALTH;
        m_armor = 0;
        m_damage = 8;
    }
    if (type == 2) {
        m_health = WARRIOR_HEALTH;
        m_armor = 2;
        m_damage = 10;
    }
    m_position.x = (x + 30) * 32;
    m_position.y = (y + 20) * 32;
    m_position.w = 27.0f;
    m_position.h = 50.0f;
    //dx = dy = 0.1f;
    dx = dy = 0.0f;
    m_onGround = false;
    m_isAlive = true;
}

//----------------------------------------------------------------------------------------------------------------------
CPlayerInterface::~CPlayerInterface () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * Controls the permissible movements of the NPC.
 * @param dir - 0 - x-coordinate, 1 - y-coordinate,
 */
void CPlayerInterface::collision (uint8_t dir, uint32_t time) {
    if (dir) {
        if (dy > 0) {
            int j = static_cast<int>(m_position.y + m_position.h) / 32;
            for (int i = static_cast<int>(m_position.x) / 32; i <= static_cast<int>(m_position.x - 1 + m_position.w) / 32; i++) {
                if (m_map[j][i]->m_coefPermeability == 1) {
                    m_position.y = j * 32 - m_position.h;
                    dy = 0;
                    m_onGround = true;
                    break;
                }
            }
        }
        if (dy < 0) {
            int j = static_cast<int>(m_position.y) / 32;
            for (int i = static_cast<int>(m_position.x) / 32; i <= static_cast<int>(m_position.x - 1 + m_position.w) / 32; i++) {
                if (m_map[j][i]->m_coefPermeability == 1) {
                    m_position.y = j * 32 + 32;
                    break;
                }
            }
        }
    } else {
        if (dx > 0) {
            int j = static_cast<int>(m_position.x + m_position.w) / 32;
            for (int i = static_cast<int>(m_position.y) / 32; i < static_cast<int>(m_position.y + m_position.h) / 32; i++) {
                if (m_map[i][j]->m_coefPermeability == 1) {
                    m_position.x = j * 32 - m_position.w;
                    break;
                }
            }
        }
        if (dx < 0) {
            int j = static_cast<int>(m_position.x) / 32;
            for (int i = static_cast<int>(m_position.y) / 32; i < static_cast<int>(m_position.y + m_position.h) / 32; i++) {
                if (m_map[i][j]->m_coefPermeability == 1) {
                    m_position.x = j * 32 + 32;
                    break;
                }
            }
        }
    }

    if (dir) {
        for (int i = static_cast<int>(m_position.y) / 32;
             i <= static_cast<int>(m_position.y + m_position.h) / 32; i++) {
            for (int j = static_cast<int>(m_position.x) / 32;
                 j <= static_cast<int>(m_position.x + m_position.w) / 32; j++) {
                if (m_map[i][j]->m_id == 19) {
                    getDamage(LAVA_DAMAGE * time / 1000, true);
                    break;
                }
            }
        }
        if (dy > AIR_DAMGE_ACCELERATION) {
            getDamage(AIR_DAMGE * time / 1000, true);
            return;
        }
    }
}
//======================================================================================================================

//=================================class CPlayer========================================================================
/**
 * @param type - personage type.
 * @param map - game map.
 * @param texture - personage texture.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
CPlayer::CPlayer (uint8_t                type,
                  CBuildTailInterface ***map,
                  SDL_Texture           *texture,
                  int                    x,
                  int                    y):CPlayerInterface(type,
                                                             map,
                                                             texture,
                                                             x,
                                                             y) {}

//----------------------------------------------------------------------------------------------------------------------
CPlayer::~CPlayer () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * NPC controller.
 * @param time
 */
void CPlayer::update (uint32_t time) {
    m_position.x += dx * time;
    collision(0, time);
    if (!m_onGround)
        dy = static_cast<float>(dy + 0.0005 * time);
    m_position.y += dy * time;
    m_onGround = false;
    collision(1, time);
    dx = 0;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Take damage.
 * @param damage - the size of the damage.
 */
void CPlayer::getDamage (float damage, bool left) {
    m_health -= damage;
    if (m_health <= 0) {
        m_health = 0;
        m_isAlive = false;
    }
    /*if (left) {
        dx += STEP_PLAYER;
    } else {
        dx -= STEP_PLAYER;
    }
    dy -= 0.01f;*/
}
//======================================================================================================================

//=================================class CZombie========================================================================
/**
 * @param type - personage type.
 * @param map - game map.
 * @param texture - personage texture.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
CZombie::CZombie (uint8_t                type,
                  CBuildTailInterface ***map,
                  SDL_Texture           *texture,
                  int                    x,
                  int                    y):CPlayerInterface(type,
                                                             map,
                                                             texture,
                                                             x,
                                                             y) {}

//----------------------------------------------------------------------------------------------------------------------
CZombie::~CZombie () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * NPC controller.
 * @param time
 */
void CZombie::update (uint32_t time) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Take damage.
 * @param damage - the size of the damage.
 */
void CZombie::getDamage (float damage, bool left) {}
//======================================================================================================================

//=================================class CWarrior=======================================================================
/**
 * @param type - personage type.
 * @param map - game map.
 * @param texture - personage texture.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
CWarrior::CWarrior (uint8_t                type,
                    CBuildTailInterface ***map,
                    SDL_Texture           *texture,
                    int                    x,
                    int                    y):CPlayerInterface(type,
                                                               map,
                                                               texture,
                                                               x,
                                                               y) {}

//----------------------------------------------------------------------------------------------------------------------
CWarrior::~CWarrior () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * NPC controller.
 * @param time
 */
void CWarrior::update (uint32_t time) {}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Take damage.
 * @param damage - the size of the damage.
 */
void CWarrior::getDamage (float damage, bool left) {}
//======================================================================================================================