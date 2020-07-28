/**
 * @file Implementation of classes: CMap, CMapLoader, CMapCreater, CTestMapCreater.
 */
//======================================================================================================================
#include "CMap.h"
//======================================================================================================================

//==================class CMap==========================================================================================
/**
 * Creates a new map.
 * @param number - number of currently map.
 */
CMap::CMap (uint8_t number):m_number(number) {
    m_selIn[0].x = 0;
    m_selIn[0].y = 0;
    m_selIn[0].w = 2;
    m_selIn[0].h = 32;

    m_selIn[1].x = 2;
    m_selIn[1].y = 0;
    m_selIn[1].w = 2;
    m_selIn[1].h = 32;

    m_selIn[2].x = 4;
    m_selIn[2].y = 0;
    m_selIn[2].w = 28;
    m_selIn[2].h = 2;

    m_selIn[3].x = 4;
    m_selIn[3].y = 2;
    m_selIn[3].w = 28;
    m_selIn[3].h = 2;

    m_selOut[0].w = 2;
    m_selOut[0].h = 32;

    m_selOut[1].w = 2;
    m_selOut[1].h = 32;

    m_selOut[2].w = 28;
    m_selOut[2].h = 2;

    m_selOut[3].w = 28;
    m_selOut[3].h = 2;

    m_in[0].x = 0;
    m_in[0].y = 13;
    m_in[0].w = 6;
    m_in[0].h = 20;

    m_in[1].x = 21;
    m_in[1].y = 13;
    m_in[1].w = 6;
    m_in[1].h = 20;

    m_in[2].x = 6;
    m_in[2].y = 0;
    m_in[2].w = 15;
    m_in[2].h = 50;


    m_out[0].w = 6;
    m_out[0].h = 20;

    m_out[1].w = 6;
    m_out[1].h = 20;

    m_out[2].w = 15;
    m_out[2].h = 50;

    m_damage = 0;
    m_NPCs.clear();
    m_map = nullptr;
    m_player = nullptr;
    m_maker = nullptr;
    m_selectedTail = {0, 0};
    m_currentlyDamage = false;
    m_updateTime = 0;
    m_inicialize = false;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * No deeper deletions are required, the destructor itself
 * frees all map elements before exiting.
 */
CMap::~CMap () {
    if (!m_inicialize)
        return;
    for (int i = 0; i < LOAD_HEIGHT; i++) {
        for (int j = 0; j < LOAD_WIDTH; j++) {
            delete m_map[i][j];
        }
        delete[] m_map[i];
    }
    delete[] m_map;
    for (auto item : m_NPCs) {
        delete item;
    }
    delete m_maker;
    m_player->inventory->destroy();
    delete m_player->inventory;
    delete m_player;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Overwrites the file with number @m_number by the established rule:
 * @note ID of elements from the map, not including the top and
 *       bottom 20 tiles and 30 left and 30 right tiles.
 * 1 - ID (8bit) the tiles of the map, starting from the top left
 *     edge moving to the right.
 *
 * 2 - number (8bit) of chests on the map.
 *
 * 3 - the inventory of chests at a time, starting with the upper
 *     left edge and moving to the right: ID (8bit), count (8bit).
 *     Delimiter 0x00.
 *
 * 4 - the player's inventory: ID (8bit), count (8bit).
 *     Delimiter 0x00.
 *
 * 5 - the coordinates of the NPCs: type (8bit),
 *     x-coordinate (16bit), y-coordinate (16bit).
 *
 * 6 - the coordinates of the player: type (8bit),
 *     x-coordinate (16bit), y-coordinate (16bit).
 *
 * 7 - end symbol 0xFF.
 *
 * @throw CMapSaveException
 */
void CMap::save () {
    uint8_t min = 0x00;
    uint8_t max = 0xFF;
    std::ostringstream oss;
    oss << (int)m_number;
    std::ofstream fout (MAP_NAME_PATH_BEGIN + oss.str() + MAP_NAME_PATH_END, std::ios_base::binary | std::ios_base::trunc);
    if (fout.fail()) {
        fout.close();
        throw CMapSaveException ("");
    }
    std::vector<CBuildChest *> chest;
    for (int i = 20; i < LOAD_HEIGHT - 20; i++) {
        for (int j = 30; j < LOAD_WIDTH - 30; j++) {
            fout << m_map[i][j]->m_id;
            if (m_map[i][j]->m_id == 10) {
                chest.emplace_back(dynamic_cast<CBuildChest *>(m_map[i][j]));
            }
        }
    }
    fout << static_cast<uint8_t>(chest.size());
    for (auto item : chest) {
        for (auto itemChest : item->m_inventory) {
            if (itemChest)
                fout << itemChest->m_id << itemChest->m_count;
        }
        fout << min;
    }
    auto itemInventory = m_player->inventory->getItems();
    for (auto item1 : itemInventory) {
        fout << item1.first << item1.second;
    }
    fout << min;
    for (auto item2 : m_NPCs) {
        if (dynamic_cast<CZombie *>(item2)) {
            fout << static_cast<uint8_t>(1);
            uint16_t x = static_cast<uint16_t>(static_cast<int>(item2->m_position.x) / 32 - 30);
            uint16_t y = static_cast<uint16_t>(static_cast<int>(item2->m_position.y) / 32 - 20);
            fout.write((char*)&x, sizeof(uint16_t));
            fout.write((char*)&y, sizeof(uint16_t));
        }
        if (dynamic_cast<CWarrior *>(item2)) {
            fout << static_cast<uint8_t>(2);
            uint16_t x = static_cast<uint16_t>(static_cast<int>(item2->m_position.x) / 32 - 30);
            uint16_t y = static_cast<uint16_t>(static_cast<int>(item2->m_position.y) / 32 - 20);
            fout.write((char*)&x, sizeof(uint16_t));
            fout.write((char*)&y, sizeof(uint16_t));
        }
    }
    fout << static_cast<uint8_t>(0);
    uint16_t x = static_cast<uint16_t>(static_cast<int>(m_player->m_position.x) / 32 - 30);
    uint16_t y = static_cast<uint16_t>(static_cast<int>(m_player->m_position.y) / 32 - 20);
    fout.write((char*)&x, sizeof(uint16_t));
    fout.write((char*)&y, sizeof(uint16_t));
    fout << max;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Set currently renderer.
 * @param renderer - currently renderer.
 */
void CMap::setRenderer (SDL_Renderer *renderer) {
    m_renderer = renderer;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 *          head->  _____
 * x, y ->     |   |     |   |
 *             |   |     |   |
 *             |   |     |   |
 *             |___|     |___|
 *             |             |
 *             |             |
 *             |   |     |   |
 *             |   |     |   |
 *             |             |
 *             |     ||      |
 *             |     ||      |
 *             |_____||______|
 *
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
void CMap::setOutRect (int x, int y) {
    m_out[0].x = x;
    m_out[0].y = y + 13;

    m_out[1].x = x + 21;
    m_out[1].y = y + 13;

    m_out[2].x = x + 6;
    m_out[2].y = y;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 *      map tail
 *           _______
 * x, y - > |       |
 *          |       |
 *          |_______|
 *
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
void CMap::setSelectRect (int x, int y) {
    m_selOut[0].x = x;
    m_selOut[0].y = y;

    m_selOut[1].x = x + 30;
    m_selOut[1].y = y;

    m_selOut[2].x = x + 2;
    m_selOut[2].y = y;

    m_selOut[3].x = x + 2;
    m_selOut[3].y = y + 30;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Damage from center player body.
 * @param x - x start coordinate.
 * @param y - y start coordinate.
 * @param dir - num.
 *                      0
 *                11    |    1
 *                      |
 *              9 ----- o ----- 3
 *                      |
 *                7     |    5
 *                      6
 * @param damage - damage count.
 */
void CMap::damageWave (int x, int y, uint8_t dir, uint8_t damage) {
    m_currentlyDamage = true;
    m_damage = damage;
    m_damageVector.clear();
    switch (dir) {
        case 0:
            m_damageVector.emplace_back(std::make_pair(x, y - 25));
            m_damageVector.emplace_back(std::make_pair(x, y - 57));
            m_damageVector.emplace_back(std::make_pair(x, y - 89));
            break;
        case 1:
            m_damageVector.emplace_back(std::make_pair(x + 15, y - 15));
            m_damageVector.emplace_back(std::make_pair(x + 48, y - 48));
            m_damageVector.emplace_back(std::make_pair(x + 81, y - 81));
            break;
        case 3:
            m_damageVector.emplace_back(std::make_pair(x + 14, y));
            m_damageVector.emplace_back(std::make_pair(x + 46, y));
            m_damageVector.emplace_back(std::make_pair(x + 78, y));
            break;
        case 5:
            m_damageVector.emplace_back(std::make_pair(x + 15, y + 15));
            m_damageVector.emplace_back(std::make_pair(x + 48, y + 48));
            m_damageVector.emplace_back(std::make_pair(x + 81, y + 81));
            break;
        case 6:
            m_damageVector.emplace_back(std::make_pair(x, y + 25));
            m_damageVector.emplace_back(std::make_pair(x, y + 57));
            m_damageVector.emplace_back(std::make_pair(x, y + 89));
            break;
        case 7:
            m_damageVector.emplace_back(std::make_pair(x - 15, y + 15));
            m_damageVector.emplace_back(std::make_pair(x - 48, y + 48));
            m_damageVector.emplace_back(std::make_pair(x - 81, y + 81));
            break;
        case 9:
            m_damageVector.emplace_back(std::make_pair(x - 14, y));
            m_damageVector.emplace_back(std::make_pair(x - 46, y));
            m_damageVector.emplace_back(std::make_pair(x - 78, y));
            break;
        case 11:
            m_damageVector.emplace_back(std::make_pair(x - 15, y - 15));
            m_damageVector.emplace_back(std::make_pair(x - 48, y - 48));
            m_damageVector.emplace_back(std::make_pair(x - 81, y - 81));
            break;
        default:
            break;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draws the currently selected tail on the map.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
void CMap::drawSelectedTail (int x, int y) {
    setSelectRect(x, y);
    SDL_RenderCopy(m_renderer, m_maker->m_image->t_selecter, &m_selIn[0], &m_selOut[0]);
    SDL_RenderCopy(m_renderer, m_maker->m_image->t_selecter, &m_selIn[1], &m_selOut[1]);
    SDL_RenderCopy(m_renderer, m_maker->m_image->t_selecter, &m_selIn[2], &m_selOut[2]);
    SDL_RenderCopy(m_renderer, m_maker->m_image->t_selecter, &m_selIn[3], &m_selOut[3]);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draws the texture of the player on the map.
 * @param playerInterface - current player.
 */
void CMap::drawPlayer (CPlayerInterface * playerInterface) {
    setOutRect(playerInterface->m_position.x, playerInterface->m_position.y);
    SDL_RenderCopy(m_renderer, playerInterface->m_texture, &m_in[0], &m_out[0]);
    SDL_RenderCopy(m_renderer, playerInterface->m_texture, &m_in[1], &m_out[1]);
    SDL_RenderCopy(m_renderer, playerInterface->m_texture, &m_in[2], &m_out[2]);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Check whether damage to nip.
 * @param playerInterface - current player.
 */
void CMap::damageControl (CPlayerInterface * playerInterface) {
    if (playerInterface->m_position.x - m_damageVector[0].first > 100 ||
        m_damageVector[0].first - playerInterface->m_position.x > 100)
        return;
    if (playerInterface->m_position.y - m_damageVector[0].second > 100 ||
        m_damageVector[0].second - playerInterface->m_position.y > 100)
        return;
    for (auto item : m_damageVector) {
        if (item.first >= playerInterface->m_position.x       &&
            item.first <= playerInterface->m_position.x + 27  &&
            item.second >= playerInterface->m_position.y      &&
            item.second <= playerInterface->m_position.y + 50) {
            playerInterface->getDamage(m_damage, item.first < playerInterface->m_position.x);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw one frame based on the current position of the player.
 * @param time - time interval.
 */
void CMap::draw (float time, int x_sel, int y_sel) {
    m_player->update(time);
    int x = static_cast<int>(m_player->m_position.x) / 32;
    int y = static_cast<int>(m_player->m_position.y) / 32;
    int x_lost = static_cast<int>(m_player->m_position.x) - x * 32;
    int y_lost = static_cast<int>(m_player->m_position.y) - y * 32;
    int x_selected = (static_cast<int>(m_player->m_position.x) + (x_sel - 960)) / 32;
    int y_selected = (static_cast<int>(m_player->m_position.y) + (y_sel - 478)) / 32;
    m_select = false;
    if (x_selected >= (x - 2) && x_selected <= (x + 2) && y_selected >= (y - 2) && y_selected <= (y + 3)) {
        m_selectedTail.first = x_selected;
        m_selectedTail.second = y_selected;
        if (m_map[y_selected][x_selected]->m_coefPermeability == 1) {
            m_select = true;
        }
    } else {
        m_selectedTail.first =  0;
    }
    m_updateTime += time;
    bool updateFluid = false;
    if (m_updateTime > 3000) {
        m_updateTime = 0;
        updateFluid = true;
    }
    SDL_Rect rect = {0, 0, 32, 32};
    for (int i = y - 15; i < y + 17; i++) {
        for (int j = x - 30; j < x + 31; j++) {
            rect.x = 32 * (j - x + 30) - x_lost;
            rect.y = 32 * (i - y + 15) - y_lost;
            if (updateFluid) {
                updateFluidFunction(j, i);
            }
            m_map[i][j]->update(time);
            SDL_RenderCopy(m_renderer, m_map[i][j]->getTexture(), NULL, &rect);
            if (m_select && i == y_selected && j == x_selected) {
                drawSelectedTail(rect.x, rect.y);
            }
        }
    }
    m_out[0].x = 960;
    m_out[0].y = 491;
    m_out[1].x = 981;
    m_out[1].y = 491;
    m_out[2].x = 966;
    m_out[2].y = 478;
    SDL_RenderCopy(m_renderer, m_player->m_texture, &m_in[0], &m_out[0]);
    SDL_RenderCopy(m_renderer, m_player->m_texture, &m_in[1], &m_out[1]);
    SDL_RenderCopy(m_renderer, m_player->m_texture, &m_in[2], &m_out[2]);

    for (auto item : m_NPCs) {
        if (item->m_isAlive) {
            item->update(time);
            if (m_damage) {
                damageControl(item);
            }
            //drawPlayer(item);
        }
    }
    m_currentlyDamage = false;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Provide liquid transfusion.
 * @param x - x-coordinate.
 * @param y - y-coordinate.
 */
void CMap::updateFluidFunction (int x, int y) {
    // 0 - ok;
    // 1 - none;
    // 2 - change;
    if (m_map[y][x]->m_id == 18 || m_map[y][x]->m_id == 19) { //18 - water, 19 - lava
        uint8_t left  = static_cast<uint8_t>(m_map[y][x - 1]->m_coefPermeability == 0 ? 0 : m_map[y][x - 1]->m_coefPermeability == 1 ? 1 : 2);
        uint8_t right = static_cast<uint8_t>(m_map[y][x + 1]->m_coefPermeability == 0 ? 0 : m_map[y][x + 1]->m_coefPermeability == 1 ? 1 : 2);
        uint8_t down  = static_cast<uint8_t>(m_map[y + 1][x]->m_coefPermeability == 0 ? 0 : m_map[y + 1][x]->m_coefPermeability == 1 ? 1 : 2);
        if (down == 0) {
            uint8_t bg = m_map[y + 1][x]->m_BG_id;
            delete m_map[y + 1][x];
            m_map[y + 1][x] = m_maker->getBuildTail(m_map[y][x]->m_id, bg);
        }
        if (down == 2) {
            if (m_map[y][x]->m_id != m_map[y + 1][x]->m_id){
                uint8_t bg = m_map[y + 1][x]->m_BG_id;
                delete m_map[y + 1][x];
                m_map[y + 1][x] = m_maker->getBuildTail(4, bg);
                down = 1;
            }
        }
        if (down == 1) {
            if (left == 0) {
                uint8_t bg = m_map[y][x - 1]->m_BG_id;
                delete m_map[y][x - 1];
                m_map[y][x - 1] = m_maker->getBuildTail(m_map[y][x]->m_id, bg);
            }
            if (left == 2) {
                if (m_map[y][x]->m_id != m_map[y][x - 1]->m_id) {
                    uint8_t bg = m_map[y][x - 1]->m_BG_id;
                    delete m_map[y][x - 1];
                    m_map[y][x - 1] = m_maker->getBuildTail(4, bg);
                }
            }
            if (right == 0) {
                uint8_t bg = m_map[y][x + 1]->m_BG_id;
                delete m_map[y][x + 1];
                m_map[y][x + 1] = m_maker->getBuildTail(m_map[y][x]->m_id, bg);
            }
            if (right == 2) {
                if (m_map[y][x]->m_id != m_map[y][x + 1]->m_id) {
                    uint8_t bg = m_map[y][x + 1]->m_BG_id;
                    delete m_map[y][x + 1];
                    m_map[y][x + 1] = m_maker->getBuildTail(4, bg);
                }
            }
        }
    }
}
//======================================================================================================================

//==================class CMapLoader====================================================================================
/**
 * @param renderer - current renderer.
 */
CMapLoader::CMapLoader (SDL_Renderer * renderer):m_renderer  (renderer),
                                                 m_player    (nullptr),
                                                 m_maker     (nullptr),
                                                 m_returnMap (nullptr),
                                                 m_map       (nullptr){}

//----------------------------------------------------------------------------------------------------------------------
CMapLoader::~CMapLoader () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * Initializes the map.
 * @return - map created.
 * @throw CErrorMapFileException
 * @throw CExistFileException.
 */
CMap * CMapLoader::returnMap () {
    readSelect();
    allocateData();
    try {
        readMap();
    } catch (const CException & exception) {
        throw CException(exception);
    }
    m_returnMap->m_map = m_map;
    m_returnMap->m_player = m_player;
    m_returnMap->m_NPCs = m_NPCs;
    m_returnMap->m_maker = m_maker;
    m_returnMap->setRenderer(m_renderer);
    m_returnMap->m_inicialize = true;
    return m_returnMap;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Allocate memory for all objects in the class.
 * @throw CBMPLoadExcepion.
 */
void CMapLoader::allocateData () {
    m_maker = new CTailMaker (m_renderer);
    m_returnMap = new CMap(m_selectMap);
    m_returnMap->m_inicialize = false;
    m_map = new CBuildTailInterface ** [LOAD_HEIGHT];
    for (int i = 0; i < LOAD_HEIGHT; i++) {
        m_map[i] = new CBuildTailInterface * [LOAD_WIDTH];
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < LOAD_WIDTH; j++) {
            m_map[i][j] = m_maker->getBuildTail(7);
        }
    }
    for (int i = 520; i < 540; i++) {
        for (int j = 0; j < LOAD_WIDTH; j++) {
            m_map[i][j] = m_maker->getBuildTail(7);
        }
    }
    for (int i = 0; i < 30; i++) {
        for (int j = 20; j < 520; j++) {
            m_map[j][i] = m_maker->getBuildTail(7);
        }
    }
    for (int i = 1030; i < LOAD_WIDTH; i++) {
        for (int j = 20; j < 520; j++) {
            m_map[j][i] = m_maker->getBuildTail(7);
        }
    }
    for (int i = 20; i < LOAD_HEIGHT - 20; i++) {
        for (int j = 30; j < LOAD_WIDTH - 30; j++) {
            m_map[i][j] = nullptr;
        }
    }
    m_NPCs.clear();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Emergency exit if there is an exception.
 */
void CMapLoader::emergencyEscape () {
    delete m_maker;
    delete m_player;
    for (auto item : m_NPCs) {
        delete item;
    }
    delete m_returnMap;
    for (int i = 0; i < LOAD_HEIGHT; i++) {
        for (int j = 0; j < LOAD_WIDTH; j++) {
            delete m_map[i][j];
        }
        delete[] m_map[i];
    }
    delete[] m_map;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Reads the selected map from a file.
 */
void CMapLoader::readSelect () {
    std::ifstream fin(MAP_SELECT_PATH, std::ios_base::binary);
    if (fin.fail()) {
        fin.close();
        m_selectMap = 1;
        return;
    }
    if (fin.read((char*)&m_selectMap, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
        m_selectMap = 1;
        fin.close();
        return;
    }
    fin.close();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Read the data of the currently selected map.
 *
 * @note If you need to add new NIPs, you need to
 * rewrite the lower part of the code, where there
 * is a choice of type NIPs, now support only for
 * ID 1 and ID 2, any other ID will cause.
 *
 * @throw CExistFileException.
 * @throw CErrorMapFileException
 */
void CMapLoader::readMap () {
    std::ostringstream oss;
    oss << (int)m_selectMap;
    std::ifstream fin(MAP_NAME_PATH_BEGIN + oss.str() + MAP_NAME_PATH_END, std::ios_base::binary);
    if (!fin.good()) {
        fin.close();
        emergencyEscape();
        throw CExistFileException (MAP_NAME_PATH_BEGIN + oss.str() + MAP_NAME_PATH_END);
    }
    std::vector<CBuildChest *> chests;

    uint8_t tail = 0;
    uint8_t count = 0;
    for (int i = 20; i < LOAD_HEIGHT - 20; i++) {
        for (int j = 30; j < LOAD_WIDTH - 30; j++) {
            if (fin.read((char*)&tail, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
                emergencyEscape();
                std::ostringstream oss1;
                oss1 << (int)m_selectMap;
                std::string s;
                s.append(MAP_NAME_PATH_BEGIN).append(oss1.str()).append(MAP_NAME_PATH_END);
                throw CErrorMapFileException (s);
            }
            m_map[i][j] = m_maker->getBuildTail(tail);
            if (tail == 10) {
                chests.emplace_back(dynamic_cast<CBuildChest *>(m_map[i][j]));
            }
        }
    }
    if (fin.read((char*)&count, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
        emergencyEscape();
        std::ostringstream oss1;
        oss1 << (int)m_selectMap;
        std::string s;
        s.append(MAP_NAME_PATH_BEGIN).append(oss1.str()).append(MAP_NAME_PATH_END);
        throw CErrorMapFileException (s);
    }
    int chestCount = static_cast<int>(chests.size());
    for (int i = 0; i < chestCount; i++) {
        bool back = false;
        int currentlyPos = 0;
        while (!back) {
            if (fin.read((char*)&tail, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
                emergencyEscape();
                std::ostringstream oss1;
                oss1 << (int)m_selectMap;
                std::string s;
                s.append(MAP_NAME_PATH_BEGIN).append(oss1.str()).append(MAP_NAME_PATH_END);
                throw CErrorMapFileException (s);
            }
            if (tail == 0) {
                back = true;
                continue;
            }
            if (fin.read((char*)&count, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
                emergencyEscape();
                std::ostringstream oss1;
                oss1 << (int)m_selectMap;
                std::string s;
                s.append(MAP_NAME_PATH_BEGIN).append(oss1.str()).append(MAP_NAME_PATH_END);
                throw CErrorMapFileException (s);
            }
            chests[i]->m_inventory[currentlyPos++] = m_maker->getInventoryTail(tail, count);
        }
    }
    bool back = false;
    std::vector<std::pair<uint8_t, uint8_t>> playerInventory;
    while (!back) {
        if (fin.read((char*)&tail, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
            emergencyEscape();
            std::ostringstream oss1;
            oss1 << (int)m_selectMap;
            std::string s;
            s.append(MAP_NAME_PATH_BEGIN).append(oss1.str()).append(MAP_NAME_PATH_END);
            throw CErrorMapFileException (s);
        }
        if (tail == 0) {
            back = true;
            continue;
        }
        if (fin.read((char*)&count, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
            emergencyEscape();
            std::ostringstream oss1;
            oss1 << (int)m_selectMap;
            std::string s;
            s.append(MAP_NAME_PATH_BEGIN).append(oss1.str()).append(MAP_NAME_PATH_END);
            throw CErrorMapFileException (s);
        }


        playerInventory.emplace_back(std::make_pair(tail, count));
    }
    back = false;
    uint16_t x = 1;
    uint16_t y = 1;
    uint8_t  type = 1;
    std::vector<std::pair<uint8_t, std::pair<uint16_t , uint16_t>>> NPCs;
    while (!back) {
        if (fin.read((char*)&type, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
            emergencyEscape();
            std::ostringstream oss1;
            oss1 << (int)m_selectMap;
            std::string s;
            s.append(MAP_NAME_PATH_BEGIN).append(oss1.str()).append(MAP_NAME_PATH_END);
            throw CErrorMapFileException(s);
        }
        if (type == 0xFF) {
            back = true;
            continue;
        }
        if (fin.read((char*)&x, sizeof(uint16_t)).gcount() != sizeof(uint16_t)) {
            emergencyEscape();
            std::ostringstream oss1;
            oss1 << (int)m_selectMap;
            std::string s;
            s.append(MAP_NAME_PATH_BEGIN).append(oss1.str()).append(MAP_NAME_PATH_END);
            throw CErrorMapFileException(s);
        }
        if (fin.read((char*)&y, sizeof(uint16_t)).gcount() != sizeof(uint16_t)) {
            emergencyEscape();
            std::ostringstream oss1;
            oss1 << (int)m_selectMap;
            std::string s;
            s.append(MAP_NAME_PATH_BEGIN).append(oss1.str()).append(MAP_NAME_PATH_END);
            throw CErrorMapFileException(s);
        }
        NPCs.emplace_back(std::make_pair(type, std::make_pair(x, y)));
    }
    for (int i = 0; i < static_cast<int>(NPCs.size() - 1); i++) {
        type = NPCs[i].first;
        x = NPCs[i].second.first;
        y = NPCs[i].second.second;
        CPlayerInterface * anInterface = nullptr;
        if (type == 1) {
            anInterface = new CZombie (type, m_map, m_maker->m_image->playerImage->t_zombie, x, y);
        }
        if (type == 2) {
            anInterface = new CWarrior(type, m_map, m_maker->m_image->playerImage->t_warrior, x, y);
        }
        m_NPCs.emplace_back(anInterface);
    }
    type = NPCs[NPCs.size() - 1].first;
    x = NPCs[NPCs.size() - 1].second.first;
    y = NPCs[NPCs.size() - 1].second.second;

    m_player = new CPlayer (0, m_map, m_maker->m_image->playerImage->t_player, (int)x, (int)y);
    CInventory * inventory = new CInventory(m_renderer,
                                            m_maker,
                                            &m_player->m_health,
                                            &m_player->m_damage,
                                            &m_player->m_armor);
    for (auto item : playerInventory) {
        inventory->initialize(item.first, item.second);
    }
    m_player->inventory = inventory;
    fin.close();
}
//======================================================================================================================

//==================class CMapCreater===================================================================================
/**
 * Generates a new map and writes it to a new file named "map (number)"
 * where the number is the first free card number starting from one.
 * @throw CMapCreateException.
 */
CMapCreater::CMapCreater () {
    m_map               = nullptr;
    m_chestInventory    = nullptr;
    m_NPCs              = {};
    m_chestCount        = 0;
    //srand(time(0));
    try {
        readCount();
    } catch (const CException & exception) {
        std::cout << exception.what();
        destroyData();
        throw CMapCreateException ("error: read map count");
    }
    createMap();
    try {
        writeMap();
    } catch (const CException & exception1) {
        std::cout << exception1.what();
        destroyData();
        throw CMapCreateException ("error: write map");
    }
    destroyData();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Will create map size of CREATE_WIDTH x CREATE_HEIGHT.
 */
void CMapCreater::allocateMap () {
    m_map = new uint8_t*[CREATE_HEIGHT];
    for (int i = 0; i < CREATE_HEIGHT; i++) {
        m_map[i] = new uint8_t[CREATE_WIDTH];
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Destroy all allocated data.
 */
void CMapCreater::destroyData () {
    if (m_map) {
        for (int i = 0; i < CREATE_HEIGHT; i++) {
            delete[] m_map[i];
        }
        delete[] m_map;
    }
    if (m_chestInventory) {
        for (int i = 0; i < m_chestCount; i++) {
            for (int j = 0; ; j++) {
                if (!m_chestInventory[i][j][0]) {
                    delete[] m_chestInventory[i][j];
                    break;
                } else {
                    delete[] m_chestInventory[i][j];
                }
            }
            delete[] m_chestInventory[i];
        }
        delete[] m_chestInventory;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * The function returns the inventory for a new chest,
 * if the flag typeFlag(default 0) type is set to 0
 * in the chest will be a few rare things, if 1 rare
 * things will be more.
 * Used for chests at different depths.
 * @param type - 0-ordinary things, 1-rare things;
 * @return - pointer to possible inventory.
 */
unsigned char** CMapCreater::getChestContent (bool typeFlag) {
    uint8_t numOfType = static_cast<uint8_t >(rand() % 5);
    if (typeFlag) {
        numOfType += 5;
    }
    switch (numOfType) {
        case 0: {
            uint8_t **tmp = new uint8_t*[9];
            for (int i = 0; i < 9; i++) {
                tmp[i] = new uint8_t[2];
            }
            tmp[0][0] = 181; //iron helmet
            tmp[0][1] = 1;
            tmp[1][0] = 102; //stone
            tmp[1][1] = 22;
            tmp[2][0] = 151; //wood pick
            tmp[2][1] = 1;
            tmp[3][0] = 101; //ground
            tmp[3][1] = 12;
            tmp[4][0] = 107; //coal
            tmp[4][1] = 38;
            tmp[5][0] = 101; //ground
            tmp[5][1] = 14;
            tmp[6][0] = 161; //wood blade
            tmp[6][1] = 1;
            tmp[7][0] = 113; //stick
            tmp[7][1] = 10;
            tmp[8][0] = 0;
            tmp[8][1] = 0;
            return tmp;
        }
        case 1: {
            uint8_t **tmp = new uint8_t*[7];
            for (int i = 0; i < 7; i++) {
                tmp[i] = new uint8_t[2];
            }
            tmp[0][0] = 182; //gold helmet
            tmp[0][1] = 1;
            tmp[1][0] = 171; //wood axe
            tmp[1][1] = 1;
            tmp[2][0] = 152; //stone pick
            tmp[2][1] = 1;
            tmp[3][0] = 103; //sand
            tmp[3][1] = 45;
            tmp[4][0] = 108; //iron
            tmp[4][1] = 5;
            tmp[5][0] = 101; //ground
            tmp[5][1] = 94;
            tmp[6][0] = 0;
            tmp[6][1] = 0;
            return tmp;
        }
        case 2: {
            uint8_t **tmp = new uint8_t*[8];
            for (int i = 0; i < 8; i++) {
                tmp[i] = new uint8_t[2];
            }
            tmp[0][0] = 192; //gold m_armor
            tmp[0][1] = 1;
            tmp[1][0] = 101; //ground
            tmp[1][1] = 35;
            tmp[2][0] = 153; //iron pick
            tmp[2][1] = 1;
            tmp[3][0] = 104; //moss stone
            tmp[3][1] = 18;
            tmp[4][0] = 112; //workbench
            tmp[4][1] = 1;
            tmp[5][0] = 102; //stone
            tmp[5][1] = 14;
            tmp[6][0] = 164; //gold blade
            tmp[6][1] = 1;
            tmp[7][0] = 0;
            tmp[7][1] = 0;
            return tmp;
        }
        case 3: {
            uint8_t **tmp = new uint8_t*[9];
            for (int i = 0; i < 9; i++) {
                tmp[i] = new uint8_t[2];
            }
            tmp[0][0] = 109; //gold
            tmp[0][1] = 3;
            tmp[1][0] = 103; //sand
            tmp[1][1] = 1;
            tmp[2][0] = 173; //iron axe
            tmp[2][1] = 1;
            tmp[3][0] = 101; //ground
            tmp[3][1] = 97;
            tmp[4][0] = 107; //coal
            tmp[4][1] = 8;
            tmp[5][0] = 201; //iron boot
            tmp[5][1] = 1;
            tmp[6][0] = 164; //gold blade
            tmp[6][1] = 1;
            tmp[7][0] = 113; //stick
            tmp[7][1] = 13;
            tmp[8][0] = 0;
            tmp[8][1] = 0;
            return tmp;
        }
        case 4: {
            uint8_t **tmp = new uint8_t*[8];
            for (int i = 0; i < 8; i++) {
                tmp[i] = new uint8_t[2];
            }
            tmp[0][0] = 192; //gold m_armor
            tmp[0][1] = 1;
            tmp[1][0] = 104; //moss stone
            tmp[1][1] = 1;
            tmp[2][0] = 152; //stone pick
            tmp[2][1] = 1;
            tmp[3][0] = 110; //diamond
            tmp[3][1] = 2;
            tmp[4][0] = 111; //chest
            tmp[4][1] = 1;
            tmp[5][0] = 101; //ground
            tmp[5][1] = 99;
            tmp[6][0] = 201; //iron boot
            tmp[6][1] = 1;
            tmp[7][0] = 0;
            tmp[7][1] = 0;
            return tmp;
        }
        case 5: {
            uint8_t **tmp = new uint8_t*[6];
            for (int i = 0; i < 6; i++) {
                tmp[i] = new uint8_t[2];
            }
            tmp[0][0] = 183; //diamond helmet
            tmp[0][1] = 1;
            tmp[1][0] = 110; //diamond
            tmp[1][1] = 4;
            tmp[2][0] = 105; //wood
            tmp[2][1] = 11;
            tmp[3][0] = 111; //chest
            tmp[3][1] = 1;
            tmp[4][0] = 113; //stick
            tmp[4][1] = 38;
            tmp[5][0] = 0;
            tmp[5][1] = 0;
            return tmp;
        }
        case 6: {
            uint8_t **tmp = new uint8_t*[5];
            for (int i = 0; i < 5; i++) {
                tmp[i] = new uint8_t[2];
            }
            tmp[0][0] = 183; //diamond helmet
            tmp[0][1] = 1;
            tmp[1][0] = 102; //stone
            tmp[1][1] = 98;
            tmp[2][0] = 155; //diamond pick
            tmp[2][1] = 1;
            tmp[3][0] = 109; //gold
            tmp[3][1] = 12;
            tmp[4][0] = 0;
            tmp[4][1] = 0;
            return tmp;
        }
        case 7: {
            uint8_t **tmp = new uint8_t*[6];
            for (int i = 0; i < 6; i++) {
                tmp[i] = new uint8_t[2];
            }
            tmp[0][0] = 193; //diamond m_armor
            tmp[0][1] = 1;
            tmp[1][0] = 110; //diamond
            tmp[1][1] = 22;
            tmp[2][0] = 155; //diamond pick
            tmp[2][1] = 1;
            tmp[3][0] = 101; //ground
            tmp[3][1] = 12;
            tmp[4][0] = 112; //workbench
            tmp[4][1] = 1;
            tmp[5][0] = 0;
            tmp[5][1] = 0;
            return tmp;
        }
        case 8: {
            uint8_t **tmp = new uint8_t*[6];
            for (int i = 0; i < 6; i++) {
                tmp[i] = new uint8_t[2];
            }
            tmp[0][0] = 203; //diamond boot
            tmp[0][1] = 1;
            tmp[1][0] = 102; //stone
            tmp[1][1] = 22;
            tmp[2][0] = 165; //diamond blade
            tmp[2][1] = 1;
            tmp[3][0] = 104; //moss stone
            tmp[3][1] = 32;
            tmp[4][0] = 107; //coal
            tmp[4][1] = 38;
            tmp[5][0] = 0;
            tmp[5][1] = 0;
            return tmp;
        }
        case 9: {
            uint8_t **tmp = new uint8_t*[9];
            for (int i = 0; i < 9; i++) {
                tmp[i] = new uint8_t[2];
            }
            tmp[0][0] = 183; //diamond helmet
            tmp[0][1] = 1;
            tmp[1][0] = 193; //diamond m_armor
            tmp[1][1] = 1;
            tmp[2][0] = 203; //diamond boot
            tmp[2][1] = 1;
            tmp[3][0] = 155; //diamond pick
            tmp[3][1] = 1;
            tmp[4][0] = 165; //diamond blade
            tmp[4][1] = 1;
            tmp[5][0] = 175; //diamond axe
            tmp[5][1] = 14;
            tmp[6][0] = 111; //chest
            tmp[6][1] = 1;
            tmp[7][0] = 106; //foliage
            tmp[7][1] = 99;
            tmp[8][0] = 0;
            tmp[8][1] = 0;
            return tmp;
        }
        default:
            return nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Reads from a file the number of existing maps if the file
 * does not exist creates a new file and writes 1 if it fails
 * to write issues exception: CWriteFileException.
 * If there were no errors, writes the increment of the read
 * number to the file if unable to write issues exception:
 *
 * @throw CWriteFileException.
 */
void CMapCreater::readCount () {
    std::ifstream fin(MAP_COUNT_PATH, std::ios_base::binary);
    if (fin.fail()) {
        fin.close();
        std::ofstream fout (MAP_COUNT_PATH, std::ios_base::binary | std::ios_base::trunc);
        if (fout.fail()) {
            fout.close();
            throw CWriteFileException(MAP_COUNT_PATH);
        }
        m_mapNumber = 1;
        fout << m_mapNumber;
        fout.close();

        std::ofstream ffout (MAP_SELECT_PATH, std::ios_base::trunc | std::ios_base::binary);
        if (ffout.fail()) {
            ffout.close();
        } else {
            ffout << m_mapNumber;
            ffout.close();
        }
        return;
    }
    if (fin.read((char *)&m_mapNumber, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
        fin.close();
        std::ofstream fout (MAP_COUNT_PATH, std::ios_base::binary | std::ios_base::trunc);
        if (fout.fail()) {
            fout.close();
            throw CWriteFileException(MAP_COUNT_PATH);
        }
        m_mapNumber = 1;
        fout << m_mapNumber;
        fout.close();

        std::ofstream ffout (MAP_SELECT_PATH, std::ios_base::trunc | std::ios_base::binary);
        if (ffout.fail()) {
            ffout.close();
        } else {
            ffout << m_mapNumber;
            ffout.close();
        }
    }
    fin.close();
    std::ofstream fout (MAP_COUNT_PATH, std::ios_base::binary | std::ios_base::trunc);
    if (fout.fail()) {
        fout.close();
        throw CWriteFileException(MAP_COUNT_PATH);
    }
    m_mapNumber++;
    fout << m_mapNumber;
    fout.close();


    std::ofstream ffout (MAP_SELECT_PATH, std::ios_base::trunc | std::ios_base::binary);
    if (ffout.fail()) {
        ffout.close();
    } else {
        ffout << m_mapNumber;
        ffout.close();
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Will write the created map to the file with the number m_map
 * number, the data will be written bitwise starting from the
 * upper left edge moving to the right, after the number of
 * chests on the map, after the ID of each element in the chest,
 * chests are numbered starting from the upper left edge and
 * moving to the right divider between chests: 0x00.
 * Then there will be data about the player's inventory,
 * the delimiter 0x00, data on Nips: ID, coordinate x,
 * coordinate y, delimiter 0x00. Data on player: ID,
 * coordinate x, coordinate y.
 * At the end of the file 0xFF.
 *
 * In case of an error writing to the file will be
 * an exception: @throw CWriteFileException.
 *
 * P.S. data about inventory item: (uint8_t) ID, (uint8_t) count.
 */
void CMapCreater::writeMap () {
    std::ostringstream oss;
    oss << (int)m_mapNumber;
    uint8_t min = 0x00;
    uint8_t max = 0xFF;
    std::string fileName;
    fileName = MAP_NAME_PATH_BEGIN + oss.str() + MAP_NAME_PATH_END;
    std::ofstream fout (fileName, std::ios_base::binary | std::ios_base::trunc);
    if (fout.fail()) {
        destroyData();
        fout.close();
        throw CWriteFileException(fileName);
    }
    for (int i = 0; i < CREATE_HEIGHT; i++) {
        for (int j = 0; j < CREATE_WIDTH; j++) {
            fout << m_map[i][j];
        }
    }
    fout << m_chestCount;
    for (int i = 0; i < m_chestCount; i++) {
        for (int j = 0; ; j++) {
            if (!m_chestInventory[i][j][0]) {
                fout << min;
                break;
            } else {
                fout << m_chestInventory[i][j][0] << m_chestInventory[i][j][1];
            }
        }
    }
    fout << min;
    for (int i = 0; i < (int)m_NPCs.size(); i++) {
        fout << m_NPCs[i].first;
        fout.write((char*)&m_NPCs[i].second.first, sizeof(uint16_t));
        fout.write((char*)&m_NPCs[i].second.second, sizeof(uint16_t));
    }
    fout << m_player.first;
    fout.write((char*)&m_player.second.first, sizeof(uint16_t));
    fout.write((char*)&m_player.second.second, sizeof(uint16_t));
    fout << max;
    fout.close();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Creates a new map.
 */
void CMapCreater::createMap () {
    allocateMap();
    int skyHorizon   = CREATE_HEIGHT / 5 * 2;
    for (int i = 0; i < skyHorizon; i++) {
        for (int j = 0; j < CREATE_WIDTH; j++) {
            m_map[i][j] = 8;
        }
    }
    for (int i = skyHorizon; i < CREATE_HEIGHT; i++) {
        for (int j = 0; j < CREATE_WIDTH; j++) {
            m_map[i][j] = 1;
        }
    }
    int groundHorizon = (CREATE_HEIGHT - skyHorizon) / 5 * 2 + skyHorizon;
    for (int i = groundHorizon; i < CREATE_HEIGHT; i++) {
        for (int j = 0; j < CREATE_WIDTH; j++) {
            m_map[i][j] = 4;
        }
    }
    makeGroundRelief(skyHorizon);
    makeStoneRelief(groundHorizon);
    addGround(groundHorizon);
    addStones(groundHorizon);
    scatterMinerals(skyHorizon);
    spillFluid(skyHorizon);
    makeCave();
    plantTree();
    makeGreenGround();
    addNPCs(MAX_NPCS);
    addPlayer();
    fixBugs();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Creates a ground mountainous terrain.
 * @param skyHorizon - the boundary between heaven and earth.
 */
void CMapCreater::makeGroundRelief (int skyHorizon) {
    skyHorizon--;
    for (int i = 0; i < CREATE_WIDTH; i++) {
        if (!(rand() % CHANCE_CREATE_GROUND_HILL)) {
            int length = rand() % MAX_LENGTH_GROUND_HILL;
            if (i + length >= CREATE_WIDTH) {
                length = CREATE_WIDTH - i - 1;
            }
            int height = rand() % (skyHorizon / 2);
            if (skyHorizon - height < 0) {
                height = skyHorizon;
            }
            int start = i;
            int finish = i + length;
            i += length;

            if (rand() % 2) {
                for (int j = 0; j < height; j++) {
                    while (rand() % CHANCE_STEP_WHEN_CREATE_GROUND_HILL) {
                        start++;
                        length--;
                    }
                    while (rand() % CHANCE_STEP_WHEN_CREATE_GROUND_HILL) {
                        finish--;
                        length--;
                    }
                    if (!length)
                        break;
                    for (int k = start; k <= finish; k++) {
                        m_map[skyHorizon - j][k] = 1;
                    }
                }

                int oldHorizon = skyHorizon;
                skyHorizon += (rand() % 2 ? -1 : 1) * (rand() % MAX_MOVE_GROUND_HORIZON);

                //will consider change of horizon
                if (oldHorizon > skyHorizon) {
                    for (int p = 0; p < oldHorizon - skyHorizon; p++) {
                        for (int j = i + p; j < CREATE_WIDTH; j++) {
                            m_map[oldHorizon - p][j] = 8;
                        }
                    }
                }

                //will consider change of horizon
                if (oldHorizon < skyHorizon) {
                    for (int p = 0; p < skyHorizon - oldHorizon; p++) {
                        for (int j = i + p; j < CREATE_WIDTH; j++) {
                            m_map[oldHorizon + p][j] = 1;
                        }
                    }
                }
            } else {
                skyHorizon--;
                for (int j = 0; j < height; j++){
                    while (rand() % CHANCE_STEP_WHEN_CREATE_GROUND_HILL) {
                        start++;
                        length--;
                    }
                    while (rand() % CHANCE_STEP_WHEN_CREATE_GROUND_HILL) {
                        finish--;
                        length--;
                    }
                    if (!length)
                        break;
                    for (int k = start; k <= finish; k++) {
                        m_map[skyHorizon + j][k] = 8;
                    }
                }
                int oldHorizon = skyHorizon;
                skyHorizon += (rand() % 2 ? -1 : 1) * (rand() % MAX_MOVE_GROUND_HORIZON);

                //will consider change of horizon
                if (oldHorizon > skyHorizon) {
                    for (int p = 0; p < oldHorizon - skyHorizon; p++) {
                        for (int j = i + p; j < CREATE_WIDTH; j++) {
                            m_map[oldHorizon - p][j] = 8;
                        }
                    }
                }

                //will consider change of horizon
                if (oldHorizon < skyHorizon) {
                    for (int p = 0; p < skyHorizon - oldHorizon; p++) {
                        for (int j = i + p; j < CREATE_WIDTH; j++) {
                            m_map[oldHorizon + p][j] = 1;
                        }
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Creates a stone mountainous terrain.
 * @param groundHorizon - the level of the horizon between the earth and the stones.
 */
void CMapCreater::makeStoneRelief (int groundHorizon){
    groundHorizon--;
    for (int i = 0; i < CREATE_WIDTH; i++) {
        if (!(rand() % CHANCE_CREATE_STONE_HILL)) {
            int length = rand() % MAX_LENGTH_STONE_HILL;
            if (i + length >= CREATE_WIDTH) {
                length = CREATE_WIDTH - i - 1;
            }
            int height = rand() % (groundHorizon / 2);
            if (groundHorizon - height < 0) {
                height = groundHorizon;
            }
            int start = i;
            int finish = i + length;
            i += length;

            if (rand() % 2) {
                for (int j = 0; j < height; j++) {
                    while (rand() % CHANCE_STEP_WHEN_CREATE_STONE_HILL) {
                        start++;
                        length--;
                    }
                    while (rand() % CHANCE_STEP_WHEN_CREATE_STONE_HILL) {
                        finish--;
                        length--;
                    }
                    if (!length)
                        break;
                    for (int k = start; k <= finish; k++) {
                        m_map[groundHorizon - j][k] = 4;
                    }
                }

                int oldHorizon = groundHorizon;
                groundHorizon += (rand() % 2 ? -1 : 1) * (rand() % MAX_MOVE_STONE_HORIZON);

                //will consider change of horizon
                if (oldHorizon > groundHorizon) {
                    for (int p = 0; p < oldHorizon - groundHorizon; p++) {
                        for (int j = i + p; j < CREATE_WIDTH; j++) {
                            m_map[oldHorizon - p][j] = 1;
                        }
                    }
                }

                //will consider change of horizon
                if (oldHorizon < groundHorizon) {
                    for (int p = 0; p < groundHorizon - oldHorizon; p++) {
                        for (int j = i + p; j < CREATE_WIDTH; j++) {
                            m_map[oldHorizon + p][j] = 4;
                        }
                    }
                }
            } else {
                groundHorizon--;
                for (int j = 0; j < height; j++){
                    while (rand() % CHANCE_STEP_WHEN_CREATE_STONE_HILL) {
                        start++;
                        length--;
                    }
                    while (rand() % CHANCE_STEP_WHEN_CREATE_STONE_HILL) {
                        finish--;
                        length--;
                    }
                    if (!length)
                        break;
                    for (int k = start; k <= finish; k++) {
                        m_map[groundHorizon + j][k] = 1;
                    }
                }
                int oldHorizon = groundHorizon;
                groundHorizon += (rand() % 2 ? -1 : 1) * (rand() % MAX_MOVE_GROUND_HORIZON);

                //will consider change of horizon
                if (oldHorizon > groundHorizon) {
                    for (int p = 0; p < oldHorizon - groundHorizon; p++) {
                        for (int j = i + p; j < CREATE_WIDTH; j++) {
                            m_map[oldHorizon - p][j] = 1;
                        }
                    }
                }

                //will consider change of horizon
                if (oldHorizon < groundHorizon) {
                    for (int p = 0; p < groundHorizon - oldHorizon; p++) {
                        for (int j = i + p; j < CREATE_WIDTH; j++) {
                            m_map[oldHorizon + p][j] = 4;
                        }
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Add stones on map.
 * @param groundHorizon - the level of the horizon between the earth and the stones.
 */
void CMapCreater::addStones (int groundHorizon) {
    for (int i = 0; i < groundHorizon; i++) {
        for (int j = 0; j < CREATE_WIDTH; j++) {
            if (m_map[i][j] == 1) {
                if (!(rand() % ((groundHorizon - i)) * NOISE_LEVEL_STONE)) {
                    m_map[i][j] = 4;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Add stones on map under the level groundHorizon.
 * @param groundHorizon - the level of the horizon between the earth and the stones.
 */
void CMapCreater::addGround (int groundHorizon) {
    for (int i = groundHorizon; i < CREATE_HEIGHT; i++) {
        for (int j = 0; j < CREATE_WIDTH; j++) {
            if (m_map[i][j] == 4) {
                if (!(rand() % ((CREATE_HEIGHT - i) * NOISE_LEVEL_GROUND))) {
                    m_map[i][j] = 1;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Add minerals to the map.
 * @param skyHorizon - start layer.
 */
void CMapCreater::scatterMinerals (int skyHorizon) {
    for (int i = skyHorizon + 20; (i + 3) < CREATE_HEIGHT; i += 3) {
        for (int j = 0; j < CREATE_WIDTH - 4; j++) {
            if (!(rand() % CHANCE_SCATTER_MINERALS)) {
                setMinerals(j, i, (i > (CREATE_HEIGHT / 10 * 7) ? 1 : 0));
                j += 5;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Add randomly generated minerals.
 * @param x - x-coordinate left top corner.
 * @param y - y-coordinate left top corner.
 * @param type - if it's not 0, can add diamonds.
 */
void CMapCreater::setMinerals (int x, int y, int type) {
    uint8_t typeMineral = static_cast<uint8_t >(11 + (rand() % 4));
    if (!type && typeMineral == 13)
        typeMineral = 14;

    switch (rand() % 5) {
        case 0: {
            m_map[y    ][x    ] = typeMineral;
            m_map[y    ][x + 1] = typeMineral;
            m_map[y    ][x + 2] = typeMineral;
            m_map[y + 1][x    ] = typeMineral;
            m_map[y + 1][x + 1] = typeMineral;
            m_map[y + 1][x + 2] = typeMineral;
        }
        case 1: {
            m_map[y    ][x + 1] = typeMineral;
            m_map[y + 1][x    ] = typeMineral;
            m_map[y + 1][x + 1] = typeMineral;
            m_map[y + 1][x + 2] = typeMineral;
            m_map[y + 2][x + 1] = typeMineral;
        }
        case 2: {
            m_map[y    ][x    ] = typeMineral;
            m_map[y    ][x + 1] = typeMineral;
            m_map[y + 1][x    ] = typeMineral;
            m_map[y + 1][x + 1] = typeMineral;
            m_map[y + 1][x + 2] = typeMineral;
            m_map[y + 2][x + 1] = typeMineral;
            m_map[y + 2][x + 2] = typeMineral;
        }
        case 3: {
            m_map[y    ][x    ] = typeMineral;
            m_map[y    ][x + 1] = typeMineral;
            m_map[y    ][x + 2] = typeMineral;
            m_map[y + 1][x    ] = typeMineral;
            m_map[y + 1][x + 1] = typeMineral;
            m_map[y + 1][x + 2] = typeMineral;
            m_map[y + 2][x    ] = typeMineral;
            m_map[y + 2][x + 1] = typeMineral;
            m_map[y + 2][x + 2] = typeMineral;
        }
        case 4: {
            m_map[y    ][x    ] = typeMineral;
            m_map[y    ][x + 1] = typeMineral;
            m_map[y    ][x + 2] = typeMineral;
            m_map[y + 1][x    ] = typeMineral;
            m_map[y + 1][x + 1] = typeMineral;
            m_map[y + 1][x + 2] = typeMineral;
            m_map[y + 2][x    ] = typeMineral;
            m_map[y + 2][x + 2] = typeMineral;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Create caves on the map.
 */
void CMapCreater::makeCave () {
    std::vector<bool> chest;
    chest.clear();
    for (int i = 10; i < CREATE_WIDTH - 100; i++) {
        int startY = 0;
        int startX = i;
        for (int j = 0; j < CREATE_HEIGHT; j++){
            if (m_map[j][i] != 8) {
                startY = j;
                break;
            }
        }
        int size = CREATE_HEIGHT - startY - (rand() % 100) - 10;
        for (int y = 0; y < size; y++) {
            int tmpSize = (rand() % 5);
            for (int x = startX; x < startX + tmpSize; x++) {
                if (x == CREATE_WIDTH - 1)
                    break;
                switch (m_map[startY][x]) {
                    case 8:
                        break;
                    case 1:
                        m_map[startY][x] = 16;
                    default:
                        m_map[startY][x] = 17;
                }
            }
            startX++;
            startY++;
            if (startX == CREATE_WIDTH - 1)
                break;
        }
        int dir = (rand() % 2); // 0 - left, 1 - right
        if (startX + 16 > CREATE_WIDTH)
            dir = 0;
        if (dir) {
            for (int ii = startY; ii < startY + 4; ii++) {
                for (int jj = startX; jj < startX + 15; jj++) {
                    m_map[ii][jj] = 17;
                }
            }
            for (int ii = startX; ii < startX + 15; ii++) {
                    m_map[startY][ii] = 5;
                    m_map[startY + 5][ii] = 5;
                }
                m_map[startY + 1][startX + 15] = 5;
                m_map[startY + 2][startX + 15] = 5;
                m_map[startY + 3][startX + 15] = 5;
                m_map[startY + 4][startX + 15] = 5;
                m_map[startY + 3][startX + 7] = 10;
                m_map[startY + 3][startX + 9] = 9;
            chest.emplace_back(startY > CREATE_HEIGHT / 10 * 7);
            m_chestCount++;
            i = startX - STEP_BACK_WHEN_CREATE_CAVE;
        } else {
            for (int ii = startY; ii < startY + 4; ii++) {
                for (int jj = startX; jj > startX - 15; jj--) {
                    m_map[ii][jj] = 17;
                }
            }
            for (int ii = startX; ii > startX - 15; ii--) {
                m_map[startY][ii] = 5;
                m_map[startY + 5][ii] = 5;
            }
            m_map[startY + 1][startX - 15] = 5;
            m_map[startY + 2][startX - 15] = 5;
            m_map[startY + 3][startX - 15] = 5;
            m_map[startY + 4][startX - 15] = 5;
            m_map[startY + 3][startX - 7] = 10;
            m_map[startY + 3][startX - 9] = 9;
            chest.emplace_back(startY > CREATE_HEIGHT / 10 * 7);
            m_chestCount++;
            i = startX - STEP_BACK_WHEN_CREATE_CAVE;
        }
    }
    if (chest.size()) {
        m_chestInventory = new uint8_t **[chest.size()];
        for (int i = 0; i < static_cast<int>(chest.size()); i++) {
            m_chestInventory[i] = getChestContent(chest[i]);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Add trees to the map.
 */
void CMapCreater::plantTree () {
    int startY;
    for (int i = 10; i < CREATE_WIDTH - 10; i++) {
        if (!(rand() % CHANCE_PLANT_TREE)) {
            startY = 0;
            for (int j = 10; j < CREATE_HEIGHT + 10; j++) {
                if (m_map[j][i] != 8) {
                    startY = j - 1;
                    break;
                }
            }
            if (startY) {
                switch (rand() % 3) {
                    case 0: {
                        m_map[startY][i] = 6;
                        m_map[startY - 1][i] = 6;
                        m_map[startY - 2][i] = 6;
                        m_map[startY - 3][i] = 6;
                        m_map[startY - 4][i] = 6;

                        m_map[startY - 5][i] = 15;
                        m_map[startY - 6][i] = 15;
                        m_map[startY - 5][i + 1] = 15;
                        m_map[startY - 5][i - 1] = 15;
                        m_map[startY - 4][i - 1] = 15;
                        m_map[startY - 4][i - 2] = 15;
                        m_map[startY - 4][i + 1] = 15;
                        m_map[startY - 4][i + 2] = 15;
                        m_map[startY - 3][i - 1] = 15;
                        m_map[startY - 3][i - 2] = 15;
                        m_map[startY - 3][i + 1] = 15;
                        m_map[startY - 3][i + 2] = 15;
                        m_map[startY - 2][i - 1] = 15;
                        m_map[startY - 2][i + 1] = 15;
                    }
                    case 1: {
                        m_map[startY][i] = 6;
                        m_map[startY - 1][i] = 6;
                        m_map[startY - 2][i] = 6;
                        m_map[startY - 3][i] = 6;
                        m_map[startY - 4][i] = 6;

                        m_map[startY - 5][i] = 15;
                        m_map[startY - 6][i] = 15;
                        m_map[startY - 7][i] = 15;
                        m_map[startY - 7][i - 1] = 15;
                        m_map[startY - 7][i + 1] = 15;
                        m_map[startY - 6][i + 1] = 15;
                        m_map[startY - 6][i + 2] = 15;
                        m_map[startY - 6][i - 1] = 15;
                        m_map[startY - 6][i - 2] = 15;
                        m_map[startY - 5][i + 1] = 15;
                        m_map[startY - 5][i - 1] = 15;
                        m_map[startY - 5][i + 2] = 15;
                        m_map[startY - 5][i - 2] = 15;
                        m_map[startY - 4][i - 1] = 15;
                        m_map[startY - 4][i - 2] = 15;
                        m_map[startY - 4][i - 3] = 15;
                        m_map[startY - 4][i + 3] = 15;
                        m_map[startY - 4][i + 1] = 15;
                        m_map[startY - 4][i + 2] = 15;
                        m_map[startY - 3][i - 1] = 15;
                        m_map[startY - 3][i - 2] = 15;
                        m_map[startY - 3][i - 3] = 15;
                        m_map[startY - 3][i + 3] = 15;
                        m_map[startY - 3][i + 1] = 15;
                        m_map[startY - 3][i + 2] = 15;
                        m_map[startY - 2][i - 1] = 15;
                        m_map[startY - 2][i + 1] = 15;
                        m_map[startY - 2][i + 2] = 15;
                        m_map[startY - 2][i - 2] = 15;
                    }
                    default: {
                        m_map[startY][i] = 6;
                        m_map[startY - 1][i] = 6;
                        m_map[startY - 2][i] = 6;

                        m_map[startY - 3][i] = 15;
                        m_map[startY - 4][i] = 15;
                        m_map[startY - 3][i - 1] = 15;
                        m_map[startY - 3][i + 1] = 15;
                        m_map[startY - 2][i - 1] = 15;
                        m_map[startY - 2][i + 1] = 15;
                        m_map[startY - 2][i + 2] = 15;
                        m_map[startY - 2][i - 2] = 15;
                        m_map[startY - 1][i + 1] = 15;
                        m_map[startY - 1][i - 1] = 15;
                    }
                }
            }
            i += STEP_BETWEEN_TREE;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Add water or lava to a map starting in position x,y.
 * @param startX - x-coordinate.
 * @param startY - y-coordinate.
 * @param type - 1 - lava, 0 - water;
 */
void CMapCreater::spillFluidInPosition (int startX, int startY, uint8_t type) {
    m_map[startY][startX] = static_cast<uint8_t>(18 + type);
    std::queue<std::pair<int, int>> FIFO;
    FIFO.push(std::make_pair(startX, startY));
    while (!FIFO.empty()) {
        int x = FIFO.front().first;
        int y = FIFO.front().second;
        FIFO.pop();
        if ((type == 0 && m_map[y][x + 1] == 19) || (type == 1 && m_map[y][x + 1] == 18)) {
            m_map[y][x + 1] = 4;
        }
        if ((type == 0 && m_map[y][x - 1] == 19) || (type == 1 && m_map[y][x - 1] == 18)) {
            m_map[y][x - 1] = 4;
        }
        if ((type == 0 && m_map[y + 1][x] == 19) || (type == 1 && m_map[y + 1][x] == 18)) {
            m_map[y + 1][x] = 4;
        }
        if (m_map[y][x + 1] == 8 || m_map[y][x + 1] == 16 || m_map[y][x + 1] == 17) {
            m_map[y][x + 1] = static_cast<uint8_t>(18 + type);
            FIFO.push(std::make_pair(x + 1, y));
        }
        if (m_map[y][x - 1] == 8 || m_map[y][x - 1] == 16 || m_map[y][x - 1] == 17) {
            m_map[y][x - 1] = static_cast<uint8_t>(18 + type);
            FIFO.push(std::make_pair(x - 1, y));
        }
        if (m_map[y + 1][x] == 8 || m_map[y + 1][x] == 16 || m_map[y + 1][x] == 17) {
            m_map[y + 1][x] = static_cast<uint8_t>(18 + type);
            FIFO.push(std::make_pair(x, y + 1));
        }
    }
}

//------------------------------------------------------------------------------------------------------------------
/**
 * Add water or lava to a map.
 * @param skyHorizon - the initial level section of the earth and sky.
 */
void CMapCreater::spillFluid (int skyHorizon) {
    skyHorizon += 50;
    while (skyHorizon < CREATE_HEIGHT - 10) {
        for (int i = 10; i < CREATE_WIDTH - 20; i++) {
            if (!(rand() % CHANCE_SPILL_FLUID)) {
                bool go = true;
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 10; j++) {
                        if (m_map[skyHorizon + i][i - 5 + 10] == 10)
                            go = false;
                    }
                }
                if (!go)
                    continue;

                m_map[skyHorizon][i] = 4;
                m_map[skyHorizon + 1][i + 1] = 4;
                m_map[skyHorizon + 1][i + 2] = 4;
                m_map[skyHorizon + 1][i - 1] = 4;
                m_map[skyHorizon + 1][i - 2] = 4;
                m_map[skyHorizon + 2][i + 3] = 4;
                m_map[skyHorizon + 2][i - 3] = 4;
                m_map[skyHorizon + 3][i + 4] = 4;
                m_map[skyHorizon + 3][i - 4] = 4;
                m_map[skyHorizon + 4][i + 5] = 4;
                m_map[skyHorizon + 4][i - 5] = 4;
                m_map[skyHorizon + 5][i + 5] = 4;
                m_map[skyHorizon + 5][i - 2] = 4;
                m_map[skyHorizon + 5][i - 3] = 4;
                m_map[skyHorizon + 5][i - 4] = 4;
                m_map[skyHorizon + 6][i - 1] = 4;
                m_map[skyHorizon + 6][i    ] = 4;
                m_map[skyHorizon + 6][i + 1] = 4;
                m_map[skyHorizon + 6][i + 2] = 4;
                m_map[skyHorizon + 6][i + 3] = 4;
                m_map[skyHorizon + 6][i + 4] = 4;
                m_map[skyHorizon + 6][i + 5] = 4;


                m_map[skyHorizon + 1][i] = static_cast<uint8_t>(17);
                for (int j = 0; j < 5; j++) {
                    m_map[skyHorizon + 2][i - 2 + j] = static_cast<uint8_t>(17);
                }
                for (int j = 0; j < 7; j++) {
                    m_map[skyHorizon + 3][i - 3 + j] = static_cast<uint8_t>(17);
                }
                for (int j = 0; j < 9; j++) {
                    m_map[skyHorizon + 4][i - 4 + j] = static_cast<uint8_t>(17);
                }
                for (int j = 0; j < 6; j++) {
                    m_map[skyHorizon + 5][i - 1 + j] = static_cast<uint8_t>(17);
                }
                spillFluidInPosition(i, skyHorizon + 2, static_cast<uint8_t >(rand() % 2));
                i += 25 + (rand() % 20);
            }
        }
        skyHorizon += 15 + (rand() % 20);
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Add one nip to or near a specific position if there is a conflict.
 * @param type - type of NPC.
 * @param x - x-coordinate left top.
 * @param y - y-coordinate left top.
 */
int CMapCreater::addNPCInPosition (int type, int x, int y) {
    int count = 5;
    while (count != 0) {
        if ((m_map[y    ][x    ] == 8 || m_map[y    ][x    ] == 16 || m_map[y    ][x    ] == 17) &&
            (m_map[y    ][x + 1] == 8 || m_map[y    ][x + 1] == 16 || m_map[y    ][x + 1] == 17) &&
            (m_map[y + 1][x    ] == 8 || m_map[y + 1][x    ] == 16 || m_map[y + 1][x    ] == 17) &&
            (m_map[y + 1][x + 1] == 8 || m_map[y + 1][x + 1] == 16 || m_map[y + 1][x + 1] == 17)) {
            break;
        }
        switch (count) {
            case 5:
                x += 1;
                break;
            case 4:
                x -= 2;
                break;
            case 3:
                x++;
                y++;
                break;
            case 2:
                y -= 2;
                break;
            default:
                break;
        }
        count--;
    }
    if (count) {
        m_NPCs.emplace_back(std::make_pair(type, std::make_pair(x, y)));
        return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Adds new NPCs or any of the specified types in any place of the map.
 * @param count - NPSs count.
 * @param type - 0 - any type, > 0 - type of NPC.
 */
void CMapCreater::addNPCs (int count, uint8_t type) {
    for (int i = 0 ; i < CREATE_WIDTH; i++) {
        if (!count) {
            break;
        }
        if (!(rand() % CHANCE_SEED_NPCS)) {
            uint8_t typeNPC = type;
            if (!type) {
                typeNPC = static_cast<uint8_t >((rand() % COUNT_TYPES_NPCS) + 1);
            }
            for (int j = 2; j < CREATE_HEIGHT; j++) {
                if (m_map[j][i] != 8 || m_map[j][i] != 16 || m_map[j][i] != 17) {
                    if ((m_map[j - 1][i] == 8 || m_map[j - 1][i] == 16 || m_map[j - 1][i] == 17) &&
                        (m_map[j - 2][i] == 8 || m_map[j - 2][i] == 16 || m_map[j - 2][i] == 17)) {
                        count -= addNPCInPosition(typeNPC, i, j);
                        j += 10;
                        if (!count) {
                            break;
                        }
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Will make the earth nearest to the sky green.
 */
void CMapCreater::makeGreenGround () {
    for (int i = 0; i < CREATE_WIDTH; i++) {
        for (int j = 0; j < CREATE_HEIGHT - 1; j++) {
            if (m_map[j][i] == 8 && m_map[j + 1][i] == 1) {
                m_map[j + 1][i] = 2;
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
/**
 * Add a player in the middle of the map.
 */
void CMapCreater::addPlayer () {
    int centerX = CREATE_WIDTH / 2;
    int centerY = 5;
    for (int i = 0; i < CREATE_HEIGHT; i++) {
        if (m_map[i][centerX] != 8 && m_map[i][centerX] != 16 && m_map[i][centerX] != 17) {
            centerY = i;
            break;
        }
    }
    m_map[centerY    ][centerX    ] = 9;
    m_map[centerY - 1][centerX    ] = 8;
    m_map[centerY - 1][centerX + 1] = 8;
    m_map[centerY - 2][centerX    ] = 8;
    m_map[centerY - 2][centerX + 1] = 8;
    m_player = std::make_pair(0, std::make_pair(centerX, centerY - 2));
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Fix imperfections in the map.
 */
void CMapCreater::fixBugs () {
    for (int i = 0; i < CREATE_HEIGHT; i++) {
        if (m_map[i][0] == 10) {
            m_map[i][1] = 10;
        }
        m_map[i][0] = 7;
        if (m_map[i][CREATE_WIDTH - 1] == 10) {
            m_map[i][CREATE_WIDTH - 2] = 10;
        }
        m_map[i][CREATE_WIDTH - 1] = 7;
    }
}
//======================================================================================================================

//==================class CTestMapCreater===============================================================================
/**
 * Generates a new map and writes it to a new file named "map (number)"
 * where the number is the first free card number starting from one.
 * @throw CMapCreateException.
 */
CTestMapCreater::CTestMapCreater () {
    m_type = 0;
    m_x_coordinate = CREATE_WIDTH / 2;
    m_y_coordinate = 200;
    m_item = new uint8_t *[CREATE_HEIGHT];
    for (int i = 0; i < CREATE_HEIGHT; i++) {
        m_item[i] = new uint8_t[CREATE_WIDTH];
    }
    m_chestCount = 2;
    m_chests = new std::vector <std::pair<uint8_t, uint8_t>> [m_chestCount];
    try {
        readCount();
    } catch (const CException & exception) {
        std::cout << exception.what();
        destroyData();
        throw CMapCreateException ("error: read map count");
    }
    createMap();
    try {
        writeMap();
    } catch (const CException & exception1) {
        std::cout << exception1.what();
        destroyData();
        throw CMapCreateException ("error: write map");
    }
    destroyData();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Will write the created map to the file with the number m_map
 * number, the data will be written bitwise starting from the
 * upper left edge moving to the right, after the number of
 * chests on the map, after the ID of each element in the chest,
 * chests are numbered starting from the upper left edge and
 * moving to the right divider between chests: 0x00.
 * Then there will be data about the player's inventory,
 * the delimiter 0x00, data on Nips: ID, coordinate x,
 * coordinate y, delimiter 0x00. Data on player: ID,
 * coordinate x, coordinate y.
 * At the end of the file 0xFF.
 *
 * In case of an error writing to the file will be
 * an exception: @throw CWriteFileException.
 *
 * P.S. data about inventory item: (uint8_t) ID, (uint8_t) count.
 */
void CTestMapCreater::writeMap () {
    std::ostringstream oss;
    oss << (int)m_mapNumber;
    uint8_t min = 0x00;
    uint8_t max = 0xFF;
    std::string fileName;
    fileName = MAP_NAME_PATH_BEGIN + oss.str() + MAP_NAME_PATH_END;
    std::ofstream fout (fileName, std::ios_base::binary | std::ios_base::trunc);
    if (fout.fail()) {
        destroyData();
        fout.close();
        throw CWriteFileException(fileName);
    }
    for (int i = 0; i < CREATE_HEIGHT; i++) {
        for (int j = 0; j < CREATE_WIDTH; j++) {
            fout << m_item[i][j];
        }
    }
    fout << m_chestCount;
    for (int i = 0; i < m_chestCount; i++) {
        for (int j = 0; j < static_cast<int>(m_chests[i].size()); j++) {
            fout << m_chests[i][j].first << m_chests[i][j].second;
        }
        fout << min;
    }
    //inventory player;
    fout << min;

    //NPCs coordinates BEGIN;
    {
        uint16_t x = 100;
        uint16_t y = 100;
        uint8_t type = 1;
        fout << type;
        fout.write((char*)&x, sizeof(uint16_t));
        fout.write((char*)&y, sizeof(uint16_t));
    }
    //NPCs coordinates END;

    fout << m_type;
    fout.write((char*)&m_x_coordinate, sizeof(uint16_t));
    fout.write((char*)&m_y_coordinate, sizeof(uint16_t));
    fout << max;
    fout.close();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Reads from a file the number of existing maps if the file
 * does not exist creates a new file and writes 1 if it fails
 * to write issues exception: CWriteFileException.
 * If there were no errors, writes the increment of the read
 * number to the file if unable to write issues exception:
 *
 * @throw CWriteFileException.
 */
void CTestMapCreater::readCount () {
    std::ifstream fin(MAP_COUNT_PATH, std::ios_base::binary);
    if (fin.fail()) {
        fin.close();
        std::ofstream fout (MAP_COUNT_PATH, std::ios_base::binary | std::ios_base::trunc);
        if (fout.fail()) {
            fout.close();
            throw CWriteFileException(MAP_COUNT_PATH);
        }
        m_mapNumber = 1;
        fout << m_mapNumber;
        fout.close();

        std::ofstream ffout (MAP_SELECT_PATH, std::ios_base::trunc | std::ios_base::binary);
        if (ffout.fail()) {
            ffout.close();
        } else {
            ffout << m_mapNumber;
            ffout.close();
        }
        return;
    }
    if (fin.read((char *)&m_mapNumber, sizeof(uint8_t)).gcount() != sizeof(uint8_t)) {
        fin.close();
        std::ofstream fout (MAP_COUNT_PATH, std::ios_base::binary | std::ios_base::trunc);
        if (fout.fail()) {
            fout.close();
            throw CWriteFileException(MAP_COUNT_PATH);
        }
        m_mapNumber = 1;
        fout << m_mapNumber;
        fout.close();

        std::ofstream ffout (MAP_SELECT_PATH, std::ios_base::trunc | std::ios_base::binary);
        if (ffout.fail()) {
            ffout.close();
        } else {
            ffout << m_mapNumber;
            ffout.close();
        }
    }
    fin.close();
    std::ofstream fout (MAP_COUNT_PATH, std::ios_base::binary | std::ios_base::trunc);
    if (fout.fail()) {
        fout.close();
        throw CWriteFileException(MAP_COUNT_PATH);
    }
    m_mapNumber++;
    fout << m_mapNumber;
    fout.close();


    std::ofstream ffout (MAP_SELECT_PATH, std::ios_base::trunc | std::ios_base::binary);
    if (ffout.fail()) {
        ffout.close();
    } else {
        ffout << m_mapNumber;
        ffout.close();
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Destroy all allocated data.
 */
void CTestMapCreater::destroyData () {
    delete[] m_chests;
    for (int i = 0; i < CREATE_HEIGHT; i++) {
        delete[] m_item[i];
    }
    delete[] m_item;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Creates a new map.
 */
void CTestMapCreater::createMap () {
    m_chests[0].emplace_back(std::make_pair(151, 1));
    m_chests[0].emplace_back(std::make_pair(152, 1));
    m_chests[0].emplace_back(std::make_pair(153, 1));
    m_chests[0].emplace_back(std::make_pair(154, 1));
    m_chests[0].emplace_back(std::make_pair(155, 1));
    m_chests[0].emplace_back(std::make_pair(161, 1));
    m_chests[0].emplace_back(std::make_pair(162, 1));
    m_chests[0].emplace_back(std::make_pair(163, 1));
    m_chests[0].emplace_back(std::make_pair(164, 1));
    m_chests[0].emplace_back(std::make_pair(165, 1));
    m_chests[0].emplace_back(std::make_pair(171, 1));
    m_chests[0].emplace_back(std::make_pair(172, 1));
    m_chests[0].emplace_back(std::make_pair(173, 1));
    m_chests[0].emplace_back(std::make_pair(174, 1));
    m_chests[0].emplace_back(std::make_pair(175, 1));

    m_chests[0].emplace_back(std::make_pair(181, 1));
    m_chests[0].emplace_back(std::make_pair(182, 1));
    m_chests[0].emplace_back(std::make_pair(183, 1));
    m_chests[0].emplace_back(std::make_pair(191, 1));
    m_chests[0].emplace_back(std::make_pair(192, 1));
    m_chests[0].emplace_back(std::make_pair(193, 1));
    m_chests[0].emplace_back(std::make_pair(201, 1));
    m_chests[0].emplace_back(std::make_pair(202, 1));
    m_chests[0].emplace_back(std::make_pair(203, 1));


    m_chests[1].emplace_back(std::make_pair(101, 99));
    m_chests[1].emplace_back(std::make_pair(102, 99));
    m_chests[1].emplace_back(std::make_pair(103, 99));
    m_chests[1].emplace_back(std::make_pair(104, 99));
    m_chests[1].emplace_back(std::make_pair(105, 99));
    m_chests[1].emplace_back(std::make_pair(106, 99));
    m_chests[1].emplace_back(std::make_pair(107, 99));
    m_chests[1].emplace_back(std::make_pair(108, 99));
    m_chests[1].emplace_back(std::make_pair(109, 99));
    m_chests[1].emplace_back(std::make_pair(110, 99));
    m_chests[1].emplace_back(std::make_pair(113, 99));

    m_chests[1].emplace_back(std::make_pair(111, 1));
    m_chests[1].emplace_back(std::make_pair(112, 1));


    for (int i = 0; i < m_y_coordinate; i++) {
        for (int j = 0; j < CREATE_WIDTH; j++) {
            m_item[i][j] = 8;
        }
    }
    for (int i = m_y_coordinate; i < CREATE_HEIGHT; i++) {
        for (int j = 0; j < CREATE_WIDTH; j++) {
            m_item[i][j] = 1;
        }
    }
    m_item[m_y_coordinate - 1][m_x_coordinate + 5] = 10;
    m_item[m_y_coordinate - 1][m_x_coordinate - 5] = 10;
    int step = 5;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 3;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 4;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 5;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 6;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 7;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 9;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 11;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 12;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 13;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 14;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 15;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 16;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 17;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 18;
        }
    }
    step += 3;
    for (int i = m_y_coordinate; i < m_y_coordinate + 3; i++) {
        for (int j = m_x_coordinate + step; j < m_x_coordinate + step + 3; j++) {
            m_item[i][j] = 19;
        }
    }
    m_y_coordinate -= 2;

    step += 10;
    for (int i = m_y_coordinate; i < m_y_coordinate + 15; i++) {
        m_item[i][m_x_coordinate + step] = 8;
    }
    fixBugs();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Fix imperfections in the map.
 */
void CTestMapCreater::fixBugs () {
    for (int i = 0; i < CREATE_HEIGHT; i++) {
        m_item[i][0] = 7;
        m_item[i][CREATE_WIDTH - 1] = 7;
    }
}
//======================================================================================================================