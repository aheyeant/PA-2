/**
 * @file Implementation of class CInventory.
 */
//======================================================================================================================
#include "CInventory.h"
//======================================================================================================================

//=================================class CInventory=====================================================================
/**
 * Creates an inventory class, fills in textures and forms.
 * @param renderer - currently renderer.
 * @param tailMaker - the class that creates tails.
 * @param health - pointer to player health.
 * @param damage - pointer to player damage.
 * @param armor - pointer to player armor.
 */
CInventory::CInventory (SDL_Renderer    *renderer,
                        CTailMaker      *tailMaker,
                        float           *health,
                        float           *damage,
                        float           *armor): m_renderer  (renderer),
                                                m_tailMaker (tailMaker),
                                                m_health    (health),
                                                m_damage    (damage),
                                                m_armor     (armor) {
    m_inInventoryHand = nullptr;
    m_back = false;
    m_images = tailMaker->m_image;
    m_chestInventory    = m_images->gameSkin->t_chestInventory;
    m_armorTable        = m_images->gameSkin->t_armorTable;
    m_craft             = m_images->gameSkin->t_craft;
    m_speedInventory    = m_images->gameSkin->t_speedInventory;
    m_healthPlayer      = m_images->gameSkin->t_healthPlayer;
    m_inventory         = m_images->gameSkin->t_inventory;
    m_selecterTexture   = m_images->gameSkin->t_selecterInventory;
    m_nums              = m_images->gameSkin->t_nums;
    m_background        = m_images->t_background;

    setRect(&m_r_healthIn, 0, 0, 150, 37);
    setRect(&m_r_healthFullIn, 0, 40, 15, 13);
    setRect(&m_r_healthHalfIn, 14, 40, 15, 13);
    setRect(&m_r_healthOut, 1750, 15, 150, 37);
    for (int i = 0; i < 10; i++) {
        setRect(&m_r_healthHearthOut[i], 1750 + (i * 15), 38, 15, 13);
    }

    setRect(&m_r_armorTable, 1860, 400, 42, 120);
    for (int i = 0; i < 3; i++) {
        setRect(&m_r_armorItems[i], 1866, 406 + (i * 38), 32, 32);
    }
    for (int i = 0; i < 10; i++) {
        setRect(&m_r_nums[i], 0 + (i * 10), 0, 10, 12);
    }

    setRect(&m_r_speedInventoryOut, SPEED_INVENTORY_X, SPEED_INVENTORY_Y, 440, 64);
    setRect(&m_r_inventoryOut, INVENTORY_X, INVENTORY_Y, 640, 254);
    setRect(&m_r_chestOut, CHEST_X, CHEST_Y, 440, 187);
    setRect(&m_r_craftOut, CRAFT_X, CRAFT_Y, 230, 167);

    for (int i = 0; i < 10; i++) {
        setRect(&m_r_speedInventoryItems[i], SPEED_INVENTORY_X + 6 + (i * 44), SPEED_INVENTORY_Y + 6, 32, 32);
    }

    for (int i = 0; i < 10; i++) {
        setRect(&m_r_inventoryItems[30 + i], INVENTORY_X + 6 + (i * 44), INVENTORY_Y + 59 , 32, 32);
        setRect(&m_r_inventoryItems[20 + i], INVENTORY_X + 6 + (i * 44), INVENTORY_Y + 103, 32, 32);
        setRect(&m_r_inventoryItems[10 + i], INVENTORY_X + 6 + (i * 44), INVENTORY_Y + 147, 32, 32);
        setRect(&m_r_inventoryItems[ 0 + i], INVENTORY_X + 6 + (i * 44), INVENTORY_Y + 196, 32, 32);
    }

    for (int i = 0; i < 10; i++) {
        setRect(&m_r_chestItems[ 0 + i], CHEST_X + 6 + (i * 44), CHEST_Y + 59 , 32, 32);
        setRect(&m_r_chestItems[10 + i], CHEST_X + 6 + (i * 44), CHEST_Y + 103, 32, 32);
        setRect(&m_r_chestItems[20 + i], CHEST_X + 6 + (i * 44), CHEST_Y + 147, 32, 32);
    }

    for (int i = 0; i < 3; i++) {
        setRect(&m_r_craftItems[0 + i], CRAFT_X + 6 + (i * 44), CRAFT_Y + 39 , 32, 32);
        setRect(&m_r_craftItems[3 + i], CRAFT_X + 6 + (i * 44), CRAFT_Y + 83 , 32, 32);
        setRect(&m_r_craftItems[6 + i], CRAFT_X + 6 + (i * 44), CRAFT_Y + 127, 32, 32);
    }
    setRect(&m_r_craftItems[9], CRAFT_X + 182, CRAFT_Y + 83, 32, 32);

    for (int i = 0; i < 2; i++) {
        setRect(&m_r_craftInventoryItems[0 + i], INVENTORY_X + 467 + (i * 44), INVENTORY_Y + 80 , 32, 32);
        setRect(&m_r_craftInventoryItems[2 + i], INVENTORY_X + 467 + (i * 44), INVENTORY_Y + 124, 32, 32);
    }
    setRect(&m_r_craftInventoryItems[4], INVENTORY_X + 584, INVENTORY_Y + 101, 32, 32);

    setRect(&m_r_selecterIn[0], 2, 3, 4, 38);
    setRect(&m_r_selecterIn[1], 7, 3, 4, 38);
    setRect(&m_r_selecterIn[2], 12, 3, 30, 4);
    setRect(&m_r_selecterIn[3], 12, 8, 30, 4);

    setRect(&m_r_selecterOut[0], SPEED_INVENTORY_X + 3 , SPEED_INVENTORY_Y + 3 , 4 , 38);
    setRect(&m_r_selecterOut[1], SPEED_INVENTORY_X + 37, SPEED_INVENTORY_Y + 3 , 4 , 38);
    setRect(&m_r_selecterOut[2], SPEED_INVENTORY_X + 7 , SPEED_INVENTORY_Y + 3 , 30, 4);
    setRect(&m_r_selecterOut[3], SPEED_INVENTORY_X + 7 , SPEED_INVENTORY_Y + 37, 30, 4);

    m_selectedInSpeedInventory = 0;
    m_inventoryItemsCount = 0;

    updateStats();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Destroy all data
 */
void CInventory::destroy () {
    if (m_inInventoryHand)
        delete m_inInventoryHand;
    for (int i = 0; i < INVENTORY_MAX_COUNT; i++) {
        if (m_items[i]) {
            delete m_items[i];
        }
    }
    for (int i = 0; i < 5; i++) {
        if (m_inventoryCraftItems[i]) {
            delete m_inventoryCraftItems[i];
        }
    }
    for (int i = 0; i < 10; i++) {
        if (m_craftItems[i]) {
            delete m_craftItems[i];
        }
    }
    for (int i = 0; i < 3; i++) {
        if (m_armorItems[i]) {
            delete m_armorItems[i];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
CInventory::~CInventory () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * Creates a vector of inventory items to be saved later.
 * @return - vector of inventory items.
 */
std::vector<std::pair<uint8_t, uint8_t>> CInventory::getItems () {
    std::vector<std::pair<uint8_t, uint8_t>> items;
    for (int i = 0; i < 3; i++) {
        if (m_armorItems[i]) {
            items.emplace_back(std::make_pair(m_armorItems[i]->m_id, m_armorItems[i]->m_count));
        }
    }
    for (int i = 0; i < INVENTORY_MAX_COUNT; i++) {
        if (m_items[i]) {
            items.emplace_back(std::make_pair(m_items[i]->m_id, m_items[i]->m_count));
        }
    }
    return items;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * A controller to operate the chest.
 * @return - EXIT if it was pressed QUIT or CONTINUE.
 */
EAction CInventory::chestInventory (CBuildChest *chest) {
    if (!chest) {
        return EAction::CONTINUE;
    }
    clear();
    drawBG();
    drawInventory();
    drawChest(chest);
    draw();
    m_back = false;
    m_inInventoryHand = nullptr;
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                clearLittleCraft();
                clearHand();
                return EAction::EXIT;
            }
            checkKeyboard(&event);
            if (m_back) {
                clearLittleCraft();
                clearHand();
                updateStats();
                return EAction::CONTINUE;
            }
            checkMouseCI(&event, chest);
            clear();
            drawBG();
            drawInventory();
            drawChest(chest);
            drawHandItem(&event);
            draw();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Mouse control for chest inventory menu.
 * @param event - currently event.
 */
void CInventory::checkMouseCI (SDL_Event * event, CBuildChest *chest) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            int x = event->button.x;
            int y = event->button.y;

            uint8_t button = understandMouseIninventory(x, y);
            if (button) {
                if (button >= 1 && button <= 40) {
                    swapLeft(m_items, static_cast<uint8_t>(button - 1));
                    updateStats();
                    return;
                }
                if (button >= 41 && button <= 44) {
                    swapLeft(m_inventoryCraftItems, static_cast<uint8_t>(button - 41));
                    littleCraftController();
                    return;
                }
                if (button == 45) {
                    getLittleCraftItem();
                    littleCraftController();
                    return;
                }
                if (button == 46) {
                    if (m_inInventoryHand) {
                        delete m_inInventoryHand;
                        m_inInventoryHand = nullptr;
                        return;
                    }
                }
            }
            button = understandMouseChest(x, y);
            if (button) {
                if (button == 31) {
                    for (int i = 0; i < CHEST_MAX_COUNT; i++) {
                        if (chest->m_inventory[i]) {
                            insertInCurrentPosition(chest->m_inventory[i]);
                            chest->m_inventory[i] = nullptr;
                        }
                    }
                    return;
                }
                swapLeft(chest->m_inventory, static_cast<uint8_t>(button - 1));
                return;
            }
        }
        if (event->button.button == SDL_BUTTON_RIGHT) {
            int x = event->button.x;
            int y = event->button.y;

            uint8_t button = understandMouseIninventory(x, y);
            if (button) {
                if (button >= 1 && button <= 40) {
                    swapRight(m_items, static_cast<uint8_t>(button - 1));
                    updateStats();
                    return;
                }
                if (button >= 41 && button <= 44) {
                    swapRight(m_inventoryCraftItems, static_cast<uint8_t>(button - 41));
                    littleCraftController();
                    return;
                }
                return;
            }
            button = understandMouseChest(x, y);
            if (button) {
                if (button >= 1 && button <= 30) {
                    swapRight(chest->m_inventory, static_cast<uint8_t>(button - 1));
                    return;
                }
                return;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * A controller to operate the workbench.
 * @return - EXIT if it was pressed QUIT or CONTINUE.
 */
EAction CInventory::workbenchInventory () {
    clear();
    drawBG();
    drawInventory();
    drawCraft();
    draw();
    m_inInventoryHand = nullptr;
    m_back = false;
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                clearHand();
                clearLittleCraft();
                clearBigCraft();
                return EAction::EXIT;
            }
            checkKeyboard(&event);
            if (m_back) {
                clearHand();
                clearLittleCraft();
                clearBigCraft();
                updateStats();
                return EAction::CONTINUE;
            }
            checkMouseWI(&event);
            clear();
            drawBG();
            drawInventory();
            drawCraft();
            drawHandItem(&event);
            draw();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Mouse control for workbench inventory menu.
 * @param event - currently event.
 */
void CInventory::checkMouseWI (SDL_Event * event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            int x = event->button.x;
            int y = event->button.y;

            uint8_t button = understandMouseIninventory(x, y);
            if (button) {
                if (button >= 1 && button <= 40) {
                    swapLeft(m_items, static_cast<uint8_t>(button - 1));
                    updateStats();
                    return;
                }
                if (button >= 41 && button <= 44) {
                    swapLeft(m_inventoryCraftItems, static_cast<uint8_t>(button - 41));
                    littleCraftController();
                    return;
                }
                if (button == 45) {
                    getLittleCraftItem();
                    littleCraftController();
                    return;
                }
                if (button == 46) {
                    if (m_inInventoryHand) {
                        delete m_inInventoryHand;
                        m_inInventoryHand = nullptr;
                    }
                    return;
                }
                return;
            }

            button = understandMouseCraft(x, y);
            if (button) {
                if (button == 10) {
                    getBigCraftItem();
                    bigCraftControl();
                    return;
                }
                swapLeft(m_craftItems, static_cast<uint8_t>(button - 1));
                bigCraftControl();
                return;
            }
            return;
        }
        if (event->button.button == SDL_BUTTON_RIGHT) {
            int x = event->button.x;
            int y = event->button.y;

            uint8_t button = understandMouseIninventory(x, y);
            if (button) {
                if (button >= 1 && button <= 40) {
                    swapRight(m_items, static_cast<uint8_t>(button - 1));
                    updateStats();
                    return;
                }
                if (button >= 41 && button <= 44) {
                    swapRight(m_inventoryCraftItems, static_cast<uint8_t>(button - 41));
                    littleCraftController();
                    return;
                }
                return;
            }

            button = understandMouseCraft(x, y);
            if (button) {
                if (button == 10) {
                    return;
                }
                swapRight(m_craftItems, static_cast<uint8_t>(button - 1));
                bigCraftControl();
                return;
            }
            return;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Clear the craft field.
 */
void CInventory::clearBigCraft () {
    for (int i = 0; i < 9; i++) {
        if (m_craftItems[i]) {
            insertInCurrentPosition(m_craftItems[i]);
            m_craftItems[i] = nullptr;
        }
    }
    if (m_craftItems[9]) {
        delete m_craftItems[9];
        m_craftItems[9] = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * To obtain a new item.
 */
void CInventory::getBigCraftItem () {
    if (m_inInventoryHand) {
        return;
    }
    auto & item = m_craftItems[9];
    auto & itemsC = m_craftItems;
    if (!item) {
        return;
    }
    if (item->m_id == 111) {
        for (int i = 0; i < 9; i++) {
            if (i != 4) {
                itemsC[i]->m_count--;
                if (itemsC[i]->m_count == 0) {
                    delete itemsC[i];
                    itemsC[i] = nullptr;
                }
            }
        }
        m_inInventoryHand = item;
        item = nullptr;
        return;
    }

    if (item->m_id == 112) {
        if (--itemsC[0]->m_count == 1) {
            delete itemsC[0];
            itemsC[0] = nullptr;
        }
        if (--itemsC[1]->m_count == 1) {
            delete itemsC[1];
            itemsC[1] = nullptr;
        }
        if (--itemsC[3]->m_count == 1) {
            delete itemsC[3];
            itemsC[3] = nullptr;
        }
        if (--itemsC[4]->m_count == 1) {
            delete itemsC[4];
            itemsC[4] = nullptr;
        }
        m_inInventoryHand = item;
        item = nullptr;
        return;
    }

    if (item->m_id == 113) {
        for (int i = 0; i < 9; i++) {
            if (itemsC[i]) {
                itemsC[i]->m_count -= static_cast<uint8_t>(item->m_count / 4);
                if (itemsC[i]->m_count == 0) {
                    delete itemsC[i];
                    itemsC[i] = nullptr;
                }
                m_inInventoryHand = item;
                item = nullptr;
                return;
            }
        }
    }

    if (item->m_id >= 151 && item->m_id <= 155) {
        if (--itemsC[0]->m_count == 1) {
            delete itemsC[0];
            itemsC[0] = nullptr;
        }
        if (--itemsC[1]->m_count == 1) {
            delete itemsC[1];
            itemsC[1] = nullptr;
        }
        if (--itemsC[2]->m_count == 1) {
            delete itemsC[2];
            itemsC[2] = nullptr;
        }
        if (--itemsC[4]->m_count == 1) {
            delete itemsC[4];
            itemsC[4] = nullptr;
        }
        if (--itemsC[7]->m_count == 1) {
            delete itemsC[7];
            itemsC[7] = nullptr;
        }
        m_inInventoryHand = item;
        item = nullptr;
        return;
    }

    if (item->m_id >= 161 && item->m_id <= 165) {
        if (--itemsC[1]->m_count == 1) {
            delete itemsC[1];
            itemsC[1] = nullptr;
        }
        if (--itemsC[4]->m_count == 1) {
            delete itemsC[4];
            itemsC[4] = nullptr;
        }
        if (--itemsC[7]->m_count == 1) {
            delete itemsC[7];
            itemsC[7] = nullptr;
        }
        m_inInventoryHand = item;
        item = nullptr;
        return;
    }

    if (item->m_id >= 171 && item->m_id <= 175) {
        if (--itemsC[0]->m_count == 1) {
            delete itemsC[0];
            itemsC[0] = nullptr;
        }
        if (--itemsC[1]->m_count == 1) {
            delete itemsC[1];
            itemsC[1] = nullptr;
        }
        if (--itemsC[3]->m_count == 1) {
            delete itemsC[3];
            itemsC[3] = nullptr;
        }
        if (--itemsC[4]->m_count == 1) {
            delete itemsC[4];
            itemsC[4] = nullptr;
        }
        if (--itemsC[7]->m_count == 1) {
            delete itemsC[7];
            itemsC[7] = nullptr;
        }
        m_inInventoryHand = item;
        item = nullptr;
        return;
    }

    if (item->m_id >= 181 && item->m_id <= 183) {
        if (--itemsC[0]->m_count == 1) {
            delete itemsC[0];
            itemsC[0] = nullptr;
        }
        if (--itemsC[1]->m_count == 1) {
            delete itemsC[1];
            itemsC[1] = nullptr;
        }
        if (--itemsC[2]->m_count == 1) {
            delete itemsC[2];
            itemsC[2] = nullptr;
        }
        if (--itemsC[3]->m_count == 1) {
            delete itemsC[3];
            itemsC[3] = nullptr;
        }
        if (--itemsC[5]->m_count == 1) {
            delete itemsC[5];
            itemsC[5] = nullptr;
        }
        m_inInventoryHand = item;
        item = nullptr;
        return;
    }

    if (item->m_id >= 191 && item->m_id <= 193) {
        for (int i = 0; i < 9; i++) {
            if (i != 1) {
                itemsC[i]->m_count--;
                if (!itemsC[i]->m_count) {
                    delete itemsC[i];
                    itemsC[i] = nullptr;
                }
            }
        }
        m_inInventoryHand = item;
        item = nullptr;
        return;
    }

    if (item->m_id >= 201 && item->m_id <= 203) {
        if (--itemsC[3]->m_count == 1) {
            delete itemsC[3];
            itemsC[3] = nullptr;
        }
        if (--itemsC[5]->m_count == 1) {
            delete itemsC[5];
            itemsC[5] = nullptr;
        }
        if (--itemsC[6]->m_count == 1) {
            delete itemsC[6];
            itemsC[6] = nullptr;
        }
        if (--itemsC[8]->m_count == 1) {
            delete itemsC[8];
            itemsC[8] = nullptr;
        }
        m_inInventoryHand = item;
        item = nullptr;
        return;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Checks the correctness of the recipe for the workbench.
 */
void CInventory::bigCraftControl () {
    auto & item = m_craftItems;

    for (int i = 0; i < 9; i++) {                   // stick
        uint8_t size = 0;
        if (item[i]) {
            if (item[i]->m_id == 105) {

            } else {
                continue;
            }
        } else {
            continue;
        }
        for (int j = 0; j < 9; j++) {
            if (j != i) {
                if (item[j]) {
                    size++;
                }
            }
        }
        if (!size) {
            if (item[i]->m_count > 24) {
                if (item[9]) {
                    delete item[9];
                }
                item[9] = m_tailMaker->getInventoryTail(113, 96);
                return;
            } else {
                if (item[9]) {
                    delete item[9];
                }
                item[9] = m_tailMaker->getInventoryTail(113, static_cast<uint8_t>(item[i]->m_count * 4));
                return;
            }
        }
    }

    if (item [0] && item [0]->m_id == 105 &&                // workbench
        item [1] && item [1]->m_id == 105 &&
        !item [2] &&
        item [3] && item [3]->m_id == 105 &&
        item [4] && item [4]->m_id == 105 &&
        !item [5] &&
        !item [6] &&
        !item [7] &&
        !item [8]) {

        if (item[9]) {
            if (item[9]->m_id == 112) {
                return;
            } else {
                delete item[9];
                item[9] = m_tailMaker->getInventoryTail(112);
                return;
            }
        } else {
            item[9] = m_tailMaker->getInventoryTail(112);
            return;
        }
    }

    if (item [0] && item [0]->m_id == 105 &&            // chest
        item [1] && item [1]->m_id == 105 &&
        item [2] && item [2]->m_id == 105 &&
        item [3] && item [3]->m_id == 105 &&
        !item [4] &&
        item [5] && item [5]->m_id == 105 &&
        item [6] && item [6]->m_id == 105 &&
        item [7] && item [7]->m_id == 105 &&
        item [8] && item [8]->m_id == 105) {

        if (item[9]) {
            if (item[9]->m_id == 111) {
                return;
            } else {
                delete item[9];
                item[9] = m_tailMaker->getInventoryTail(111);
                return;
            }
        } else {
            item[9] = m_tailMaker->getInventoryTail(111);
            return;
        }
    }

    if (!item [0] &&                                    // blade
        !item [2] &&
        !item [3] &&
        !item [5] &&
        !item [6] &&
        item [7] && item [7]->m_id == 113 &&
        !item [8]) {

        if (item[1] && item[1]->m_id == 105 &&             // wood
            item[4] && item[4]->m_id == 105) {
            if (item[9]) {
                if (item[9]->m_id == 161) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(161);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(161);
                return;
            }
        }

        if (item[1] && item[1]->m_id == 102 &&             // stone
            item[4] && item[4]->m_id == 102) {
            if (item[9]) {
                if (item[9]->m_id == 162) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(162);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(162);
                return;
            }
        }

        if (item[1] && item[1]->m_id == 108 &&             // iron
            item[4] && item[4]->m_id == 108) {
            if (item[9]) {
                if (item[9]->m_id == 163) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(163);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(163);
                return;
            }
        }

        if (item[1] && item[1]->m_id == 109 &&             // gold
            item[4] && item[4]->m_id == 109) {
            if (item[9]) {
                if (item[9]->m_id == 164) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(164);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(164);
                return;
            }
        }

        if (item[1] && item[1]->m_id == 110 &&             // diamond
            item[4] && item[4]->m_id == 110) {
            if (item[9]) {
                if (item[9]->m_id == 165) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(165);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(165);
                return;
            }
        }
    }



    if (!item [3] &&                                //pick
        item [4] && item [4]->m_id == 113 &&
        !item [5] &&
        !item [6] &&
        item [7] && item [7]->m_id == 113 &&
        !item [8]) {

        if (item[0] && item[0]->m_id == 105 &&                // wood
            item[1] && item[1]->m_id == 105 &&
            item[2] && item[2]->m_id == 105) {

            if (item[9]) {
                if (item[9]->m_id == 151) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(151);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(151);
                return;
            }
        }

        if (item[0] && item[0]->m_id == 102 &&                // stone
            item[1] && item[1]->m_id == 102 &&
            item[2] && item[2]->m_id == 102) {

            if (item[9]) {
                if (item[9]->m_id == 152) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(152);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(152);
                return;
            }
        }

        if (item[0] && item[0]->m_id == 108 &&                // iron
            item[1] && item[1]->m_id == 108 &&
            item[2] && item[2]->m_id == 108) {

            if (item[9]) {
                if (item[9]->m_id == 153) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(153);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(153);
                return;
            }
        }

        if (item[0] && item[0]->m_id == 109 &&                // gold
            item[1] && item[1]->m_id == 109 &&
            item[2] && item[2]->m_id == 109) {

            if (item[9]) {
                if (item[9]->m_id == 154) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(154);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(154);
                return;
            }
        }

        if (item[0] && item[0]->m_id == 110 &&                // diamond
            item[1] && item[1]->m_id == 110 &&
            item[2] && item[2]->m_id == 110) {

            if (item[9]) {
                if (item[9]->m_id == 155) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(155);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(155);
                return;
            }
        }
    }

    if (!item [2] &&                                // axe
        item [4] && item [4]->m_id == 113 &&
        !item [5] &&
        !item [6] &&
        item [7] && item [7]->m_id == 113 &&
        !item [8]) {

        if (item [0] && item [0]->m_id == 105 &&        // wood
            item [1] && item [1]->m_id == 105 &&
            item [3] && item [3]->m_id == 105) {

            if (item[9]) {
                if (item[9]->m_id == 171) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(171);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(171);
                return;
            }
        }

        if (item [0] && item [0]->m_id == 102 &&        // stone
            item [1] && item [1]->m_id == 102 &&
            item [3] && item [3]->m_id == 102) {

            if (item[9]) {
                if (item[9]->m_id == 172) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(172);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(172);
                return;
            }
        }

        if (item [0] && item [0]->m_id == 108 &&        // iron
            item [1] && item [1]->m_id == 108 &&
            item [3] && item [3]->m_id == 108) {

            if (item[9]) {
                if (item[9]->m_id == 173) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(173);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(173);
                return;
            }
        }

        if (item [0] && item [0]->m_id == 109 &&        // gold
            item [1] && item [1]->m_id == 109 &&
            item [3] && item [3]->m_id == 109) {

            if (item[9]) {
                if (item[9]->m_id == 174) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(174);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(174);
                return;
            }
        }

        if (item [0] && item [0]->m_id == 110 &&        // diamond
            item [1] && item [1]->m_id == 110 &&
            item [3] && item [3]->m_id == 110) {

            if (item[9]) {
                if (item[9]->m_id == 175) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(175);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(175);
                return;
            }
        }
    }

    if (!item [4] &&                                        // helmet
        !item [6] &&
        !item [7] &&
        !item [8]) {

        if (item [0] && item [0]->m_id == 108 &&               // iron
            item [1] && item [1]->m_id == 108 &&
            item [2] && item [2]->m_id == 108 &&
            item [3] && item [3]->m_id == 108 &&
            item [5] && item [5]->m_id == 108) {

            if (item[9]) {
                if (item[9]->m_id == 181) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(181);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(181);
                return;
            }
        }

        if (item [0] && item [0]->m_id == 109 &&               // gold
            item [1] && item [1]->m_id == 109 &&
            item [2] && item [2]->m_id == 109 &&
            item [3] && item [3]->m_id == 109 &&
            item [5] && item [5]->m_id == 109) {

            if (item[9]) {
                if (item[9]->m_id == 182) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(182);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(182);
                return;
            }
        }

        if (item [0] && item [0]->m_id == 110 &&               // diamond
            item [1] && item [1]->m_id == 110 &&
            item [2] && item [2]->m_id == 110 &&
            item [3] && item [3]->m_id == 110 &&
            item [5] && item [5]->m_id == 110) {

            if (item[9]) {
                if (item[9]->m_id == 183) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(183);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(183);
                return;
            }
        }
    }

    if (!item [0] &&                                 // boots
        !item [1] &&
        !item [2] &&
        !item [4] &&
        !item [7]) {

        if (item [3] && item [3]->m_id == 108 &&        // iron
            item [5] && item [5]->m_id == 108 &&
            item [6] && item [6]->m_id == 108 &&
            item [8] && item [8]->m_id == 108) {

            if (item[9]) {
                if (item[9]->m_id == 201) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(201);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(201);
                return;
            }
        }

        if (item [3] && item [3]->m_id == 109 &&        // gold
            item [5] && item [5]->m_id == 109 &&
            item [6] && item [6]->m_id == 109 &&
            item [8] && item [8]->m_id == 109) {

            if (item[9]) {
                if (item[9]->m_id == 202) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(202);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(202);
                return;
            }
        }

        if (item [3] && item [3]->m_id == 110 &&        // diamond
            item [5] && item [5]->m_id == 110 &&
            item [6] && item [6]->m_id == 110 &&
            item [8] && item [8]->m_id == 110) {

            if (item[9]) {
                if (item[9]->m_id == 203) {
                    return;
                } else {
                    delete item[9];
                    item[9] = m_tailMaker->getInventoryTail(203);
                    return;
                }
            } else {
                item[9] = m_tailMaker->getInventoryTail(203);
                return;
            }
        }
    }

    if (item [0] && item [0]->m_id == 108 &&           // iron armor
        !item [1] &&
        item [2] && item [2]->m_id == 108 &&
        item [3] && item [3]->m_id == 108 &&
        item [4] && item [4]->m_id == 108 &&
        item [5] && item [5]->m_id == 108 &&
        item [6] && item [6]->m_id == 108 &&
        item [7] && item [7]->m_id == 108 &&
        item [8] && item [8]->m_id == 108 ) {

        if (item[9]) {
            if (item[9]->m_id == 191) {
                return;
            } else {
                delete item[9];
                item[9] = m_tailMaker->getInventoryTail(191);
                return;
            }
        } else {
            item[9] = m_tailMaker->getInventoryTail(191);
            return;
        }
    }

    if (item [0] && item [0]->m_id == 109 &&           // gold armor
        !item [1] &&
        item [2] && item [2]->m_id == 109 &&
        item [3] && item [3]->m_id == 109 &&
        item [4] && item [4]->m_id == 109 &&
        item [5] && item [5]->m_id == 109 &&
        item [6] && item [6]->m_id == 109 &&
        item [7] && item [7]->m_id == 109 &&
        item [8] && item [8]->m_id == 109 ) {

        if (item[9]) {
            if (item[9]->m_id == 192) {
                return;
            } else {
                delete item[9];
                item[9] = m_tailMaker->getInventoryTail(192);
                return;
            }
        } else {
            item[9] = m_tailMaker->getInventoryTail(192);
            return;
        }
    }

    if (item [0] && item [0]->m_id == 110 &&           // diamond armor
        !item [1] &&
        item [2] && item [2]->m_id == 110 &&
        item [3] && item [3]->m_id == 110 &&
        item [4] && item [4]->m_id == 110 &&
        item [5] && item [5]->m_id == 110 &&
        item [6] && item [6]->m_id == 110 &&
        item [7] && item [7]->m_id == 110 &&
        item [8] && item [8]->m_id == 110 ) {

        if (item[9]) {
            if (item[9]->m_id == 193) {
                return;
            } else {
                delete item[9];
                item[9] = m_tailMaker->getInventoryTail(193);
                return;
            }
        } else {
            item[9] = m_tailMaker->getInventoryTail(193);
            return;
        }
    }

    if (item[9]) {
        delete item[9];
        item[9] = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * A controller to operate the inventory.
 * @return - EXIT if it was pressed QUIT or CONTINUE.
 */
EAction CInventory::simpleInventory () {
    clear();
    drawBG();
    drawArmor();
    drawInventory();
    draw();
    m_back = false;
    m_inInventoryHand = nullptr;
    SDL_Event event;
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                clearLittleCraft();
                clearHand();
                return EAction::EXIT;
            }
            checkKeyboard(&event);
            if (m_back) {
                clearLittleCraft();
                clearHand();
                updateStats();
                return EAction::CONTINUE;
            }
            checkMouseSI(&event);
            clear();
            drawBG();
            drawInventory();
            drawArmor();
            drawHandItem(&event);
            draw();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Mouse control for inventory menu.
 * @param event - currently event.
 */
void CInventory::checkMouseSI (SDL_Event * event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            int x = event->button.x;
            int y = event->button.y;

            uint8_t button = understandMouseIninventory(x, y);
            if (button) {
                if (button >= 1 && button <= 40) {
                    swapLeft(m_items, static_cast<uint8_t>(button - 1));
                    updateStats();
                    return;
                }
                if (button >= 41 && button <= 44) {
                    swapLeft(m_inventoryCraftItems, static_cast<uint8_t>(button - 41));
                    littleCraftController();
                    return;
                }
                if (button == 45) {
                    getLittleCraftItem();
                    littleCraftController();
                    return;
                }
                if (button == 46) {
                    if (m_inInventoryHand) {
                        delete m_inInventoryHand;
                        m_inInventoryHand = nullptr;
                        return;
                    }
                }
            } else {
                button = understandMouseInArmor(x, y);
                if (button) {
                    swapInArmor(button);
                    updateArmor();
                    return;
                }
            }
        }
        if (event->button.button == SDL_BUTTON_RIGHT) {
            int x = event->button.x;
            int y = event->button.y;

            uint8_t button = understandMouseIninventory(x, y);
            if (button) {
                if (button >= 1 && button <= 40) {
                    swapRight(m_items, static_cast<uint8_t>(button - 1));
                    updateStats();
                }
                if (button >= 41 && button <= 44) {
                    swapRight(m_inventoryCraftItems, static_cast<uint8_t>(button - 41));
                    littleCraftController();
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Keyboard control for inventory menu.
 * @param event - currently event.
 */
void CInventory::checkKeyboard (SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        m_back = (event->key.keysym.sym == SDLK_ESCAPE || event->key.keysym.sym == SDLK_i);
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Replace armor items.
 * @param pos - currently position: 1 - helmet, 2 - armor, 3 - boots.
 */
void CInventory::swapInArmor (uint8_t pos) {
    if (!m_inInventoryHand) {
        m_inInventoryHand = m_armorItems[pos - 1];
        m_armorItems[pos - 1] = nullptr;
        return;
    }
    switch (pos) {
        case 1:
            if (m_inInventoryHand->m_id == 181 || m_inInventoryHand->m_id == 182 || m_inInventoryHand->m_id == 183) {
                CInventoryTails * tmp = m_inInventoryHand;
                m_inInventoryHand = m_armorItems[0];
                m_armorItems[0] = tmp;
            }
            break;
        case 2:
            if (m_inInventoryHand->m_id == 191 || m_inInventoryHand->m_id == 192 || m_inInventoryHand->m_id == 193) {
                CInventoryTails * tmp = m_inInventoryHand;
                m_inInventoryHand = m_armorItems[1];
                m_armorItems[1] = tmp;
            }
            break;
        case 3:
            if (m_inInventoryHand->m_id == 201 || m_inInventoryHand->m_id == 202 || m_inInventoryHand->m_id == 203) {
                CInventoryTails * tmp = m_inInventoryHand;
                m_inInventoryHand = m_armorItems[2];
                m_armorItems[2] = tmp;
            }
            break;
        default:
            break;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Replace half item in vector with item in hand.
 * @param vector - currently vector.
 * @param pos - position in vector.
 */
void CInventory::swapRight (std::vector<CInventoryTails *> & vector, uint8_t pos) {
    if (vector[pos]) {
        if (vector[pos]->m_maxCount == 1) {
            if (m_inInventoryHand) {
                return;
            } else {
                m_inInventoryHand = vector[pos];
                vector[pos] = nullptr;
                return;
            }
        } else {
            if (m_inInventoryHand) {
                if (vector[pos]->m_id == m_inInventoryHand->m_id) {
                    uint8_t size = static_cast<uint8_t>(vector[pos]->m_count / 2 + 1);
                    uint8_t currently = static_cast<uint8_t>(99 - m_inInventoryHand->m_count);
                    if (size > currently) {
                        m_inInventoryHand->m_count += currently;
                        vector[pos]->m_count -= currently;
                        return;
                    } else {
                        m_inInventoryHand->m_count += size;
                        vector[pos]->m_count -= size;
                        if (vector[pos]->m_count == 0) {
                            delete vector[pos];
                            vector[pos] = nullptr;
                        }
                        return;
                    }
                }
                return;
            } else {
                uint8_t size = static_cast<uint8_t>(vector[pos]->m_count / 2 + 1);
                m_inInventoryHand = m_tailMaker->getInventoryTail(vector[pos]->m_id, size);
                vector[pos]->m_count -= size;
                if (vector[pos]->m_count == 0) {
                    delete vector[pos];
                    vector[pos] = nullptr;
                }
                return;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Swaping item in vector with item in hand.
 * @param vector - currently vector.
 * @param pos - position in vector.
 */
void CInventory::swapLeft (std::vector<CInventoryTails *> &vector, uint8_t pos) {
    CInventoryTails * tmp = vector[pos];
    if (tmp && m_inInventoryHand) {
        if (tmp->m_id == m_inInventoryHand->m_id) {
            uint8_t size = m_inInventoryHand->m_maxCount - m_inInventoryHand->m_count;
            if (size) {
                if (tmp->m_count > size) {
                    tmp->m_count -= size;
                    m_inInventoryHand->m_count = m_inInventoryHand->m_maxCount;
                } else {
                    m_inInventoryHand->m_count += tmp->m_count;
                    delete tmp;
                    vector[pos] = nullptr;
                }
                return;
            }
        }
    }
    vector[pos] = m_inInventoryHand;
    m_inInventoryHand = tmp;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Clear little craft table before exit.
 */
void CInventory::clearLittleCraft () {
    for (int i = 0; i < 4; i++) {
        if (m_inventoryCraftItems[i]) {
            insertInCurrentPosition(m_inventoryCraftItems[i]);
            m_inventoryCraftItems[i] = nullptr;
        }
    }
    if (m_inventoryCraftItems[4]) {
        delete m_inventoryCraftItems[4];
        m_inventoryCraftItems[4] = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Remove the item that is in hand in inventory.
 */
void CInventory::clearHand () {
    insertInCurrentPosition(m_inInventoryHand);
    m_inInventoryHand = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Get craft item.
 */
void CInventory::getLittleCraftItem () {
    if (m_inventoryCraftItems[4]){
        if (m_inventoryCraftItems[4]->m_id == 113) {
            if (m_inInventoryHand) {
                if (m_inInventoryHand->m_id == 113) {
                    uint8_t currently = static_cast<uint8_t>(99 - m_inInventoryHand->m_count);
                    if (currently < m_inventoryCraftItems[4]->m_count) {
                        currently = static_cast<uint8_t>((currently / 4) * 4);
                        m_inInventoryHand->m_count += currently;
                        m_inventoryCraftItems[4]->m_count -= currently;
                        for (int i = 0; i < 4; i++) {
                            if (m_inventoryCraftItems[i]) {
                                m_inventoryCraftItems[i]->m_count -= static_cast<uint8_t>(currently / 4);
                                if (m_inventoryCraftItems[i]->m_count == 0) {
                                    delete m_inventoryCraftItems[i];
                                    m_inventoryCraftItems[i] = nullptr;
                                }
                                return;
                            }
                        }
                    } else {
                        m_inInventoryHand->m_count += m_inventoryCraftItems[4]->m_count;
                        delete m_inventoryCraftItems[4];
                        m_inventoryCraftItems[4] = nullptr;
                        for (int i = 0; i < 4; i++) {
                            if (m_inventoryCraftItems[i]) {
                                delete m_inventoryCraftItems[i];
                                m_inventoryCraftItems[i] = nullptr;
                                return;
                            }
                        }
                    }
                }
                return;
            } else {
                m_inInventoryHand = m_inventoryCraftItems[4];
                m_inventoryCraftItems[4] = nullptr;
                for (int i = 0; i < 4; i++) {
                    if (m_inventoryCraftItems[i]) {
                        m_inventoryCraftItems[i]->m_count -= static_cast<uint8_t>(m_inInventoryHand->m_count / 4);
                        if (m_inventoryCraftItems[i]->m_count == 0) {
                            delete m_inventoryCraftItems[i];
                            m_inventoryCraftItems[i] = nullptr;
                        }
                        return;
                    }
                }
            }
        }
        if (m_inventoryCraftItems[4]->m_id == 112) {
            if (!m_inInventoryHand) {
                m_inInventoryHand = m_inventoryCraftItems[4];
                m_inventoryCraftItems[4] = nullptr;
                for (int i = 0; i < 4; i++) {
                    if (m_inventoryCraftItems[i]->m_count - 1 == 0) {
                        delete m_inventoryCraftItems[i];
                        m_inventoryCraftItems[i] = nullptr;
                    } else {
                        m_inventoryCraftItems[i]->m_count--;
                    }
                }
                return;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Insert in currently position for inventory item.
 * @param inventoryTails - inventory item.
 */
void CInventory::insertInCurrentPosition (CInventoryTails *inventoryTails) {
    if (!inventoryTails) {
        return;
    }
    for (int i = 0; i < INVENTORY_MAX_COUNT; i++) {
        if (m_items[i]) {
            if (m_items[i]->m_id == inventoryTails->m_id) {
                uint8_t size = m_items[i]->m_maxCount - m_items[i]->m_count;
                if (inventoryTails->m_count > size) {
                    m_items[i]->m_count += size;
                    inventoryTails->m_count -= size;
                } else {
                    m_items[i]->m_count += inventoryTails->m_count;
                    delete inventoryTails;
                    return;
                }
            }
        }
    }
    for (int i = 0; i < INVENTORY_MAX_COUNT; i++) {
        if (!m_items[i]) {
            m_items[i] = m_tailMaker->getInventoryTail(inventoryTails->m_id, inventoryTails->m_count);
            delete inventoryTails;
            return;
        }
    }
    delete inventoryTails;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Controller for crafting in inventory.
 */
void CInventory::littleCraftController () {
    auto & item = m_inventoryCraftItems;
    if (!item[0] && !item[1] && !item[2] && !item[3] && item[4]) {
        if (item[4]) {
            delete item[4];
            item[4] = nullptr;
        }
        return;
    }
    if (item[0] && item[0]->m_id == 105 && !item[1] && !item[2] && !item[3]) {
        if (item[4]) {
            delete item[4];
            item[4] = nullptr;
        }
        if (item[0]->m_count > 24) {
            item[4] = m_tailMaker->getInventoryTail(113, 96);
        } else {
            item[4] = m_tailMaker->getInventoryTail(113, (uint8_t)4 * item[0]->m_count);
        }
        return;
    }
    if (item[1] && item[1]->m_id == 105 && !item[0] && !item[2] && !item[3]) {
        if (item[4]) {
            delete item[4];
            item[4] = nullptr;
        }
        if (item[1]->m_count > 24) {
            item[4] = m_tailMaker->getInventoryTail(113, 96);
        } else {
            item[4] = m_tailMaker->getInventoryTail(113, (uint8_t)4 * item[1]->m_count);
        }
        return;
    }
    if (item[2] && item[2]->m_id == 105 && !item[1] && !item[0] && !item[3]) {
        if (item[4]) {
            delete item[4];
            item[4] = nullptr;
        }
        if (item[2]->m_count > 24) {
            item[4] = m_tailMaker->getInventoryTail(113, 96);
        } else {
            item[4] = m_tailMaker->getInventoryTail(113, (uint8_t)4 * item[2]->m_count);
        }
        return;
    }
    if (item[3] && item[3]->m_id == 105 && !item[1] && !item[2] && !item[0]) {
        if (item[4]) {
            delete item[4];
            item[4] = nullptr;
        }
        if (item[3]->m_count > 24) {
            item[4] = m_tailMaker->getInventoryTail(113, 96);
        } else {
            item[4] = m_tailMaker->getInventoryTail(113, (uint8_t)4 * item[3]->m_count);
        }
        return;
    }

    if (item[0] && item[0]->m_id == 105 &&
        item[1] && item[1]->m_id == 105 &&
        item[2] && item[2]->m_id == 105 &&
        item[3] && item[3]->m_id == 105) {
        if (item[4]) {
            delete item[4];
            item[4] = nullptr;
        }
        item[4] = m_tailMaker->getInventoryTail(112);
        return;
    }
    if (item[4]) {
        delete item[4];
        item[4] = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Understands where the courses are relative craft.
 *
 * |                        |
 * | | 1| | 2| | 3|         |
 * |                        |
 * | | 4| | 5| | 6|   |10|  |
 * |                        |
 * | | 7| | 8| | 9|         |
 * |                        |
 *
 * @param x - x-coordinate cursor.
 * @param y - y-coordinate cursor.
 * @return - cell number or zero if not found.
 */
uint8_t CInventory::understandMouseCraft (int x, int y) {
    if (y < CRAFT_Y + 38 || y > CRAFT_Y + 158)
        return 0;
    if (x >= CRAFT_X + 5 && x <= CRAFT_X + 213) {
        if (x >= CRAFT_X +   5 && x <= CRAFT_X +  37) {
            if (y >= CRAFT_Y +  38 && y <= CRAFT_Y +  70) {
                return  1;
            }
            if (y >= CRAFT_Y +  82 && y <= CRAFT_Y + 114) {
                return  4;
            }
            if (y >= CRAFT_Y + 126 && y <= CRAFT_Y + 158) {
                return  7;
            }
        }
        if (x >= CRAFT_X +  49 && x <= CRAFT_X +  81) {
            if (y >= CRAFT_Y +  38 && y <= CRAFT_Y +  70) {
                return  2;
            }
            if (y >= CRAFT_Y +  82 && y <= CRAFT_Y + 114) {
                return  5;
            }
            if (y >= CRAFT_Y + 126 && y <= CRAFT_Y + 158) {
                return  8;
            }
        }
        if (x >= CRAFT_X +  93 && x <= CRAFT_X + 125) {
            if (y >= CRAFT_Y +  38 && y <= CRAFT_Y +  70) {
                return  3;
            }
            if (y >= CRAFT_Y +  82 && y <= CRAFT_Y + 114) {
                return  6;
            }
            if (y >= CRAFT_Y + 126 && y <= CRAFT_Y + 158) {
                return  9;
            }
        }
        if (x >= CRAFT_X + 181 && x <= CRAFT_X + 213) {
            if (y >= CRAFT_Y +  82 && y <= CRAFT_Y + 114) {
                return 10;
            }
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Understands where the courses are relative chest.
 *
 * |                                                   |
 * |      |31|                                         |
 * |                                                   |
 * | | 1| | 2| | 3| | 4| | 5| | 6| | 7| | 8| | 9| |10| |
 * |                                                   |
 * | |11| |12| |13| |14| |15| |16| |17| |18| |19| |20| |
 * |                                                   |
 * | |21| |22| |23| |24| |25| |26| |27| |28| |29| |30| |
 * |                                                   |
 *
 * @param x - x-coordinate cursor.
 * @param y - y-coordinate cursor.
 * @return - cell number or zero if not found.
 */
uint8_t CInventory::understandMouseChest (int x, int y) {
    if (y - CHEST_Y < 13 || y - CHEST_Y > 179)
        return 0;
    if (x >= CHEST_X + 5 && x <= CHEST_X + 433) {
        if (y >= CHEST_Y + 12 && y <= CHEST_Y + 44) {
            if (x >= CHEST_X + 49 && x <= CHEST_X + 81) {
                return 31;
            }
            return 0;
        }
        if (y >= CHEST_Y + 59 && y <= CHEST_Y + 91) {
            if (x >= CHEST_X +   5 && x <= CHEST_X +  37) {
                return  1;
            }
            if (x >= CHEST_X +  49 && x <= CHEST_X +  81) {
                return  2;
            }
            if (x >= CHEST_X +  93 && x <= CHEST_X + 125) {
                return  3;
            }
            if (x >= CHEST_X + 137 && x <= CHEST_X + 169) {
                return  4;
            }
            if (x >= CHEST_X + 181 && x <= CHEST_X + 213) {
                return  5;
            }
            if (x >= CHEST_X + 225 && x <= CHEST_X + 257) {
                return  6;
            }
            if (x >= CHEST_X + 269 && x <= CHEST_X + 301) {
                return  7;
            }
            if (x >= CHEST_X + 313 && x <= CHEST_X + 345) {
                return  8;
            }
            if (x >= CHEST_X + 357 && x <= CHEST_X + 389) {
                return  9;
            }
            if (x >= CHEST_X + 401 && x <= CHEST_X + 433) {
                return 10;
            }
            return 0;
        }
        if (y >= CHEST_Y + 103 && y <= CHEST_Y + 135) {
            if (x >= CHEST_X +   5 && x <= CHEST_X +  37) {
                return 11;
            }
            if (x >= CHEST_X +  49 && x <= CHEST_X +  81) {
                return 12;
            }
            if (x >= CHEST_X +  93 && x <= CHEST_X + 125) {
                return 13;
            }
            if (x >= CHEST_X + 137 && x <= CHEST_X + 169) {
                return 14;
            }
            if (x >= CHEST_X + 181 && x <= CHEST_X + 213) {
                return 15;
            }
            if (x >= CHEST_X + 225 && x <= CHEST_X + 257) {
                return 16;
            }
            if (x >= CHEST_X + 269 && x <= CHEST_X + 301) {
                return 17;
            }
            if (x >= CHEST_X + 313 && x <= CHEST_X + 345) {
                return 18;
            }
            if (x >= CHEST_X + 357 && x <= CHEST_X + 389) {
                return 19;
            }
            if (x >= CHEST_X + 401 && x <= CHEST_X + 433) {
                return 20;
            }
            return 0;
        }
        if (y >= CHEST_Y + 147 && y <= CHEST_Y + 179) {
            if (x >= CHEST_X +   5 && x <= CHEST_X +  37) {
                return 21;
            }
            if (x >= CHEST_X +  49 && x <= CHEST_X +  81) {
                return 22;
            }
            if (x >= CHEST_X +  93 && x <= CHEST_X + 125) {
                return 23;
            }
            if (x >= CHEST_X + 137 && x <= CHEST_X + 169) {
                return 24;
            }
            if (x >= CHEST_X + 181 && x <= CHEST_X + 213) {
                return 25;
            }
            if (x >= CHEST_X + 225 && x <= CHEST_X + 257) {
                return 26;
            }
            if (x >= CHEST_X + 269 && x <= CHEST_X + 301) {
                return 27;
            }
            if (x >= CHEST_X + 313 && x <= CHEST_X + 345) {
                return 28;
            }
            if (x >= CHEST_X + 357 && x <= CHEST_X + 389) {
                return 29;
            }
            if (x >= CHEST_X + 401 && x <= CHEST_X + 433) {
                return 30;
            }
            return 0;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Understands where the courses are relative inventory.
 *
 * |                                                                      |
 * |                                         |46|                         |
 * |                                                                      |
 * | |31| |32| |33| |34| |35| |36| |37| |38| |39| |40|                    |
 * |                                                      |41| |42|       |
 * | |21| |22| |23| |24| |25| |26| |27| |28| |29| |30|              |45|  |
 * |                                                      |43| |44|       |
 * | |11| |12| |13| |14| |15| |16| |17| |18| |19| |20|                    |
 * |                                                                      |
 * | | 1| | 2| | 3| | 4| | 5| | 6| | 7| | 8| | 9| |10|                    |
 * |                                                                      |
 *
 * @param x - x-coordinate cursor.
 * @param y - y-coordinate cursor.
 * @return - cell number or zero if not found.
 */
uint8_t CInventory::understandMouseIninventory (int x, int y) {
    if (y < INVENTORY_Y + 13 || y > INVENTORY_Y + 228)
        return 0;
    if (x >= INVENTORY_X + 6 && x <= INVENTORY_X + 434) {
        if (y >= INVENTORY_Y + 13 && y <= INVENTORY_Y + 46) {
            if (x >= INVENTORY_X + 357 && x <= INVENTORY_X + 390) {
                return 46;
            }
            return 0;
        }
        if (y >= INVENTORY_Y + 59 && y <= INVENTORY_Y + 91) {
            if (x >= INVENTORY_X +   5 && x <= INVENTORY_X +  37) {
                return 31;
            }
            if (x >= INVENTORY_X +  49 && x <= INVENTORY_X +  81) {
                return 32;
            }
            if (x >= INVENTORY_X +  93 && x <= INVENTORY_X + 125) {
                return 33;
            }
            if (x >= INVENTORY_X + 137 && x <= INVENTORY_X + 169) {
                return 34;
            }
            if (x >= INVENTORY_X + 181 && x <= INVENTORY_X + 213) {
                return 35;
            }
            if (x >= INVENTORY_X + 225 && x <= INVENTORY_X + 257) {
                return 36;
            }
            if (x >= INVENTORY_X + 269 && x <= INVENTORY_X + 301) {
                return 37;
            }
            if (x >= INVENTORY_X + 313 && x <= INVENTORY_X + 345) {
                return 38;
            }
            if (x >= INVENTORY_X + 357 && x <= INVENTORY_X + 389) {
                return 39;
            }
            if (x >= INVENTORY_X + 401 && x <= INVENTORY_X + 433) {
                return 40;
            }
            return 0;
        }
        if (y >= INVENTORY_Y + 103 && y <= INVENTORY_Y + 135) {
            if (x >= INVENTORY_X +   5 && x <= INVENTORY_X +  37) {
                return 21;
            }
            if (x >= INVENTORY_X +  49 && x <= INVENTORY_X +  81) {
                return 22;
            }
            if (x >= INVENTORY_X +  93 && x <= INVENTORY_X + 125) {
                return 23;
            }
            if (x >= INVENTORY_X + 137 && x <= INVENTORY_X + 169) {
                return 24;
            }
            if (x >= INVENTORY_X + 181 && x <= INVENTORY_X + 213) {
                return 25;
            }
            if (x >= INVENTORY_X + 225 && x <= INVENTORY_X + 257) {
                return 26;
            }
            if (x >= INVENTORY_X + 269 && x <= INVENTORY_X + 301) {
                return 27;
            }
            if (x >= INVENTORY_X + 313 && x <= INVENTORY_X + 345) {
                return 28;
            }
            if (x >= INVENTORY_X + 357 && x <= INVENTORY_X + 389) {
                return 29;
            }
            if (x >= INVENTORY_X + 401 && x <= INVENTORY_X + 433) {
                return 30;
            }
            return 0;
        }
        if (y >= INVENTORY_Y + 147 && y <= INVENTORY_Y + 179) {
            if (x >= INVENTORY_X +   5 && x <= INVENTORY_X +  37) {
                return 11;
            }
            if (x >= INVENTORY_X +  49 && x <= INVENTORY_X +  81) {
                return 12;
            }
            if (x >= INVENTORY_X +  93 && x <= INVENTORY_X + 125) {
                return 13;
            }
            if (x >= INVENTORY_X + 137 && x <= INVENTORY_X + 169) {
                return 14;
            }
            if (x >= INVENTORY_X + 181 && x <= INVENTORY_X + 213) {
                return 15;
            }
            if (x >= INVENTORY_X + 225 && x <= INVENTORY_X + 257) {
                return 16;
            }
            if (x >= INVENTORY_X + 269 && x <= INVENTORY_X + 301) {
                return 17;
            }
            if (x >= INVENTORY_X + 313 && x <= INVENTORY_X + 345) {
                return 18;
            }
            if (x >= INVENTORY_X + 357 && x <= INVENTORY_X + 389) {
                return 19;
            }
            if (x >= INVENTORY_X + 401 && x <= INVENTORY_X + 433) {
                return 20;
            }
            return 0;
        }
        if (y >= INVENTORY_Y + 195 && y <= INVENTORY_Y + 229) {
            if (x >= INVENTORY_X +   5 && x <= INVENTORY_X +  37) {
                return  1;
            }
            if (x >= INVENTORY_X +  49 && x <= INVENTORY_X +  81) {
                return  2;
            }
            if (x >= INVENTORY_X +  93 && x <= INVENTORY_X + 125) {
                return  3;
            }
            if (x >= INVENTORY_X + 137 && x <= INVENTORY_X + 169) {
                return  4;
            }
            if (x >= INVENTORY_X + 181 && x <= INVENTORY_X + 213) {
                return  5;
            }
            if (x >= INVENTORY_X + 225 && x <= INVENTORY_X + 257) {
                return  6;
            }
            if (x >= INVENTORY_X + 269 && x <= INVENTORY_X + 301) {
                return  7;
            }
            if (x >= INVENTORY_X + 313 && x <= INVENTORY_X + 345) {
                return  8;
            }
            if (x >= INVENTORY_X + 357 && x <= INVENTORY_X + 389) {
                return  9;
            }
            if (x >= INVENTORY_X + 401 && x <= INVENTORY_X + 433) {
                return 10;
            }
            return 0;
        }
    }
    if (x >= INVENTORY_X + 466 && x <= INVENTORY_X + 616) {
        if (y >= INVENTORY_Y + 80 && y <= INVENTORY_Y + 112) {
            if (x >= INVENTORY_X + 466 && x <= INVENTORY_X + 498) {
                return 41;
            }
            if (x >= INVENTORY_X + 510 && x <= INVENTORY_X + 542) {
                return 42;
            }
        }
        if (y >= INVENTORY_Y + 124 && y <= INVENTORY_Y + 156) {
            if (x >= INVENTORY_X + 466 && x <= INVENTORY_X + 498) {
                return 43;
            }
            if (x >= INVENTORY_X + 510 && x <= INVENTORY_X + 542) {
                return 44;
            }
        }
        if (y >= INVENTORY_Y + 101 && y <= INVENTORY_Y + 133) {
            if (x >= INVENTORY_X + 584 && x <= INVENTORY_X + 616) {
                return 45;
            }
            return 0;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Understands where the courses are relative armor.
 *
 * |     |
 * | |1| |
 * |     |
 * | |2| |
 * |     |
 * | |3| |
 * |     |
 *
 * @param x - x-coordinate cursor.
 * @param y - y-coordinate cursor.
 * @return - cell number or zero if not found.
 */
uint8_t CInventory::understandMouseInArmor (int x, int y) {
    if (x >= 1866 && x <= 1898) {
        if (y >= 406 && y <= 438)
            return 1;
        if (y >= 444 && y <= 476)
            return 2;
        if (y >= 482 && y <= 514)
            return 3;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Change currently selected item in speed inventory.
 * @param select - 0 - prev item, 11 - next item, 1 - 10 num of item.
 */
void CInventory::changeSpeedSelect (uint8_t select) {
    if (select == 0) {
        (m_selectedInSpeedInventory ? m_selectedInSpeedInventory-- : m_selectedInSpeedInventory = 9);
        updateStats();
        return;
    }
    if (select == 11) {
        (m_selectedInSpeedInventory == 9 ? m_selectedInSpeedInventory = 0 : m_selectedInSpeedInventory++);
        updateStats();
        return;
    }
    m_selectedInSpeedInventory = select - static_cast<uint8_t >(1);
    updateStats();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw mandatory game fields: health, protection, quick inventory.
 */
void CInventory::drawMainInfo () {
    drawSpeedInventory();
    drawSelecter();
    drawHealth();
    drawArmor();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw a quick inventory.
 */
void CInventory::drawSpeedInventory () {
    drawInPosition(m_speedInventory, NULL, &m_r_speedInventoryOut);
    for (int i = 0; i < 10; i++) {
        if (m_items[i]) {
            drawInPosition(m_items[i]->m_texture, NULL, &m_r_speedInventoryItems[i]);
            if (m_items[i]->m_count > 1) {
                drawNum(m_r_speedInventoryItems[i].x + 16, m_r_speedInventoryItems[i].y + 28, m_items[i]->m_count);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw a field inventory for the chest.
 * @param chest - pointer to chest.
 */
void CInventory::drawChest (CBuildChest *chest) {
    drawInPosition(m_chestInventory, NULL, &m_r_chestOut);
    for (int i = 0; i < CHEST_MAX_COUNT; i++) {
        if (chest->m_inventory[i]) {
            drawInPosition(chest->m_inventory[i]->m_texture, NULL, &m_r_chestItems[i]);
            if (chest->m_inventory[i]->m_count > 1) {
                drawNum(m_r_chestItems[i].x + 16, m_r_chestItems[i].y + 28, chest->m_inventory[i]->m_count);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw a field for inventory.
 */
void CInventory::drawInventory () {
    drawInPosition(m_inventory, NULL, &m_r_inventoryOut);
    for (int i = 0; i < INVENTORY_MAX_COUNT; i++) {
        if (m_items[i]) {
            drawInPosition(m_items[i]->m_texture, NULL, &m_r_inventoryItems[i]);
            if (m_items[i]->m_count > 1) {
                drawNum(m_r_inventoryItems[i].x + 16, m_r_inventoryItems[i].y + 28, m_items[i]->m_count);
            }
        }
    }
    for (int i = 0; i < 5; i++) {
        if (m_inventoryCraftItems[i]) {
            drawInPosition(m_inventoryCraftItems[i]->m_texture, NULL, &m_r_craftInventoryItems[i]);
            if (m_inventoryCraftItems[i]->m_count > 1) {
                drawNum(m_r_craftInventoryItems[i].x + 16, m_r_craftInventoryItems[i].y + 28,
                        m_inventoryCraftItems[i]->m_count);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw a field for crafting.
 */
void CInventory::drawCraft () {
    drawInPosition(m_craft, NULL, &m_r_craftOut);
    for (int i = 0; i < 10; i++) {
        if (m_craftItems[i]) {
            drawInPosition(m_craftItems[i]->m_texture, NULL, &m_r_craftItems[i]);
            if (m_craftItems[i]->m_count > 1) {
                drawNum(m_r_craftItems[i].x + 16, m_r_craftItems[i].y + 28, m_craftItems[i]->m_count);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Highlights the currently selected position.
 */
void CInventory::drawSelecter () {
    for (int i = 0; i < 4; i++) {
        m_r_selecterOut[i].x += m_selectedInSpeedInventory * 44;
        drawInPosition(m_selecterTexture, &m_r_selecterIn[i], &m_r_selecterOut[i]);
        m_r_selecterOut[i].x -= m_selectedInSpeedInventory * 44;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Remove from inventory one item that is instantly in hand.
 */
void CInventory::del () {
    if (m_items[m_selectedInSpeedInventory]) {
        if (m_items[m_selectedInSpeedInventory]->m_count == 1) {
            delete m_items[m_selectedInSpeedInventory];
            m_items[m_selectedInSpeedInventory] = nullptr;
            updateStats();
            return;
        }
        m_items[m_selectedInSpeedInventory]->m_count--;
        updateStats();
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Will add to the inventory of the produced item.
 * @param id - ID of subject of the plurality of building objects.
 */
void CInventory::add (uint8_t id) {
    auto item = m_tailMaker->getInventoryTailFromBuild(id);
    for (int i = 0; i < INVENTORY_MAX_COUNT; i++) {
        if (m_items[i]) {
            if (m_items[i]->m_id == item->m_id) {
                if (m_items[i]->m_count != m_items[i]->m_maxCount) {
                    m_items[i]->m_count++;
                    delete item;
                    updateStats();
                    return;
                }
            }
        }
    }
    if (m_inventoryItemsCount == INVENTORY_MAX_COUNT) {
        delete item;
        updateStats();
        return;
    }
    for (int i = 0; i < INVENTORY_MAX_COUNT; i++) {
        if (!m_items[i]) {
            m_items[i] = item;
            updateStats();
            return;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Initial adding of elements.
 * @param id - item ID.
 * @param count - item count.
 */
void CInventory::initialize (uint8_t id, uint8_t count) {
    m_items[m_inventoryItemsCount] = m_tailMaker->getInventoryTail(id, count);
    switch (m_items[m_inventoryItemsCount]->m_id) {
        case 181:
            if (m_armorItems[0]) {
                m_inventoryItemsCount++;
            } else {
                m_armorItems[0] = m_items[m_inventoryItemsCount];
                m_items[m_inventoryItemsCount] = nullptr;
            }
            break;
        case 182:
            if (m_armorItems[0]) {
                m_inventoryItemsCount++;
            } else {
                m_armorItems[0] = m_items[m_inventoryItemsCount];
                m_items[m_inventoryItemsCount] = nullptr;
            }
            break;
        case 183:
            if (m_armorItems[0]) {
                m_inventoryItemsCount++;
            } else {
                m_armorItems[0] = m_items[m_inventoryItemsCount];
                m_items[m_inventoryItemsCount] = nullptr;
            }
            break;
        case 191:
            if (m_armorItems[1]) {
                m_inventoryItemsCount++;
            } else {
                m_armorItems[0] = m_items[m_inventoryItemsCount];
                m_items[m_inventoryItemsCount] = nullptr;
            }
            break;
        case 192:
            if (m_armorItems[1]) {
                m_inventoryItemsCount++;
            } else {
                m_armorItems[0] = m_items[m_inventoryItemsCount];
                m_items[m_inventoryItemsCount] = nullptr;
            }
            break;
        case 193:
            if (m_armorItems[1]) {
                m_inventoryItemsCount++;
            } else {
                m_armorItems[0] = m_items[m_inventoryItemsCount];
                m_items[m_inventoryItemsCount] = nullptr;
            }
            break;
        case 201:
            if (m_armorItems[2]) {
                m_inventoryItemsCount++;
            } else {
                m_armorItems[0] = m_items[m_inventoryItemsCount];
                m_items[m_inventoryItemsCount] = nullptr;
            }
            break;
        case 202:
            if (m_armorItems[2]) {
                m_inventoryItemsCount++;
            } else {
                m_armorItems[0] = m_items[m_inventoryItemsCount];
                m_items[m_inventoryItemsCount] = nullptr;
            }
            break;
        case 203:
            if (m_armorItems[2]) {
                m_inventoryItemsCount++;
            } else {
                m_armorItems[0] = m_items[m_inventoryItemsCount];
                m_items[m_inventoryItemsCount] = nullptr;
            }
            break;
        default:
            m_inventoryItemsCount++;
    }
    updateStats();
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw a number at the position.
 * @param x - x-coordinate left top.
 * @param y - y-coordinate left top.
 * @param num - number > 1.
 */
void CInventory::drawNum (int x, int y, uint8_t num) {
    if (num / 10) {
        SDL_Rect rect = {x - 10, y, 10, 12};
        drawInPosition(m_nums, &m_r_nums[num / 10], &rect);
        rect.x += 10;
        drawInPosition(m_nums, &m_r_nums[num % 10], &rect);
        return;
    }
    SDL_Rect rect1 = {x - 5, y, 10, 12};
    drawInPosition(m_nums, &m_r_nums[num], &rect1);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draws the selected item at the cursor position.
 * @param event
 */
void CInventory::drawHandItem (SDL_Event *event) {
    if (!m_inInventoryHand
        || (event->type != SDL_MOUSEMOTION
        && event->type != SDL_MOUSEBUTTONDOWN
        && event->type != SDL_MOUSEBUTTONUP)) {
        return;
    }
    SDL_Rect pos = {event->button.x, event->button.y, 32, 32};
    drawInPosition(m_inInventoryHand->m_texture, NULL, &pos);
    if (m_inInventoryHand->m_count > 1)
        drawNum(event->button.x + 16, event->button.y + 20, m_inInventoryHand->m_count);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Updates player data: damage.
 */
void CInventory::updateStats () {
    if (m_items[m_selectedInSpeedInventory]) {
        m_inHand = m_items[m_selectedInSpeedInventory];
        *m_damage = m_inHand->m_damage;
        return;
    }
    m_inHand = nullptr;
    *m_damage = PLAYER_HAND_DAMAGE;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Updates player data: armor.
 */
void CInventory::updateArmor () {
    for (int i = 0; i < 3; i++) {
        *m_armor = 0;
        if (m_armorItems[i]) {
            switch (m_armorItems[i]->m_id % 10) {
                case 1:
                    *m_armor += (i == 1 ? 2 : 1);
                    break;
                case 2:
                    *m_armor += (i == 1 ? 4 : 3);
                    break;
                case 3:
                    *m_armor += (i == 1 ? 10 : 6);
                    break;
                default:
                    *m_armor += 0;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Will draw the dressed protection.
 */
void CInventory::drawArmor () {
    drawInPosition(m_armorTable, NULL, &m_r_armorTable);
    for (int i = 0; i < 3; i++) {
        if (m_armorItems[i]) {
            drawInPosition(m_armorItems[i]->m_texture, NULL, &m_r_armorItems[i]);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draws player health.
 */
void CInventory::drawHealth () {
    drawInPosition(m_healthPlayer, &m_r_healthIn, &m_r_healthOut);
    if (*m_health == PLAYER_HEALTH) {
        for (auto &i : m_r_healthHearthOut) {
            drawInPosition(m_healthPlayer, &m_r_healthFullIn, &i);
        }
        return;
    }
    uint8_t count;
    count = static_cast<uint8_t>(*m_health / (PLAYER_HEALTH / 10));
    for (int i = 0; i < count; i++) {
        drawInPosition(m_healthPlayer, &m_r_healthFullIn, &m_r_healthHearthOut[i]);
    }
    drawInPosition(m_healthPlayer, &m_r_healthHalfIn, &m_r_healthHearthOut[count]);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draws an image of a specific \ full size on a renderer at a specific location
 * or starting from the top left if dstRect == NULL.
 * @param texture - where does the image come from.
 * @param srcRect - what is the size of the drawn image.
 * @param dstRect - what size will be drawn.
 */
void CInventory::drawInPosition (SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect) const {
    SDL_RenderCopy(m_renderer, texture, srcRect, dstRect);
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
void CInventory::setRect (SDL_Rect * rect, int x, int y, int w, int h) {
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Clear the renderer inside the window.
 */
void CInventory::clear () {
    SDL_RenderClear(m_renderer);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Refresh the renderer inside the window.
 */
void CInventory::draw () {
    SDL_RenderPresent(m_renderer);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Draw background.
 */
void CInventory::drawBG () {
    clear();
    drawInPosition(m_background, NULL, NULL);
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Write data about all inventory items for the test.
 * @param str - header text.
 */
void CInventory::testInfo (std::string str) const {
    if (str != "") {
        std::cout << str << std::endl;
    }
    for (int i = 30; i >= 0; i -= 10) {
        for (int j = 0; j < 10; j++) {
            std::cout << (m_items[i + j] ? "1 " : "0 ");
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < 5; i++) {
        std::cout << (m_inventoryCraftItems[i] ? "1 " : "0 ");
    }
    std::cout << std::endl;
}
//======================================================================================================================