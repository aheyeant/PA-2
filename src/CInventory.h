/**
 * @file Contains declaration of class CInventory.
 */
//======================================================================================================================
#pragma once
//======================================================================================================================

//======================================================================================================================
#include <SDL2/SDL_system.h>
#include <SDL2/SDL_events.h>
#include "CTails.h"
#include "CPages.h"
#include "CVars.h"
//======================================================================================================================

//=================================class CInventory=====================================================================
class CInventory {
public:
    /**
     * Creates an inventory class, fills in textures and forms.
     * @param renderer - currently renderer.
     * @param tailMaker - the class that creates tails.
     * @param health - pointer to player health.
     * @param damage - pointer to player damage.
     * @param armor - pointer to player armor.
     */
    CInventory (SDL_Renderer * renderer,
                CTailMaker   * images,
                float        * health,
                float        * damage,
                float        * armor);

    //------------------------------------------------------------------------------------------------------------------
    ~CInventory ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Destroy all data
     */
    void destroy ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Will add to the inventory of the produced item.
     * @param id - ID of subject of the plurality of building objects.
     */
    void add (uint8_t id);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Remove from inventory one item that is instantly in hand.
     */
    void del ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Change currently selected item in speed inventory.
     * @param select - 0 - prev item, 11 - next item, 1 - 10 num of item.
     */
    void changeSpeedSelect (uint8_t select);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw mandatory game fields: health, protection, quick inventory.
     */
    void drawMainInfo ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * A controller to operate the inventory.
     * @return - EXIT if it was pressed QUIT or CONTINUE.
     */
    EAction simpleInventory ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * A controller to operate the chest.
     * @return - EXIT if it was pressed QUIT or CONTINUE.
     */
    EAction chestInventory (CBuildChest *chest);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * A controller to operate the workbench.
     * @return - EXIT if it was pressed QUIT or CONTINUE.
     */
    EAction workbenchInventory ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Creates a vector of inventory items to be saved later.
     * @return - vector of inventory items.
     */
    std::vector<std::pair<uint8_t, uint8_t>> getItems ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Initial adding of elements.
     * @param id - item ID.
     * @param count - item count.
     */
    void initialize (uint8_t id, uint8_t count);

    //------------------------------------------------------------------------------------------------------------------
    CInventoryTails              * m_inHand;

private:
    CInventoryTails              * m_inInventoryHand;
    SDL_Renderer                 * m_renderer;
    TImages                      * m_images;
    CTailMaker                   * m_tailMaker;

    std::vector<CInventoryTails *> m_items = {nullptr, nullptr, nullptr, nullptr, nullptr,
                                              nullptr, nullptr, nullptr, nullptr, nullptr,
                                              nullptr, nullptr, nullptr, nullptr, nullptr,
                                              nullptr, nullptr, nullptr, nullptr, nullptr,
                                              nullptr, nullptr, nullptr, nullptr, nullptr,
                                              nullptr, nullptr, nullptr, nullptr, nullptr,
                                              nullptr, nullptr, nullptr, nullptr, nullptr,
                                              nullptr, nullptr, nullptr, nullptr, nullptr};

    std::vector<CInventoryTails *> m_inventoryCraftItems  = {nullptr, nullptr, nullptr, nullptr, nullptr};

    std::vector<CInventoryTails *> m_craftItems  = {nullptr, nullptr, nullptr, nullptr, nullptr,
                                                    nullptr, nullptr, nullptr, nullptr, nullptr};

    std::vector<CInventoryTails *> m_armorItems = {nullptr, nullptr, nullptr};

    SDL_Texture                  * m_armorTable;
    SDL_Texture                  * m_chestInventory;
    SDL_Texture                  * m_craft;
    SDL_Texture                  * m_healthPlayer;
    SDL_Texture                  * m_inventory;
    SDL_Texture                  * m_speedInventory;
    SDL_Texture                  * m_selecterTexture;
    SDL_Texture                  * m_nums;
    SDL_Texture                  * m_background;

    uint8_t                      m_selectedInSpeedInventory;
    uint8_t                      m_inventoryItemsCount;

    float                        * m_health;
    float                        * m_damage;
    float                        * m_armor;

    SDL_Rect                     m_r_armorTable;
    SDL_Rect                     m_r_armorItems[3];
    SDL_Rect                     m_r_nums[10];
    SDL_Rect                     m_r_healthIn;
    SDL_Rect                     m_r_healthFullIn;
    SDL_Rect                     m_r_healthHalfIn;
    SDL_Rect                     m_r_healthOut;
    SDL_Rect                     m_r_healthHearthOut[10];
    SDL_Rect                     m_r_speedInventoryOut;
    SDL_Rect                     m_r_inventoryOut;
    SDL_Rect                     m_r_chestOut;
    SDL_Rect                     m_r_craftOut;
    SDL_Rect                     m_r_speedInventoryItems[10];
    SDL_Rect                     m_r_inventoryItems[40];
    SDL_Rect                     m_r_craftInventoryItems[5];
    SDL_Rect                     m_r_chestItems[30];
    SDL_Rect                     m_r_craftItems[10];
    SDL_Rect                     m_r_selecterIn[4];
    SDL_Rect                     m_r_selecterOut[4];

    bool                         m_back;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Mouse control for workbench inventory menu.
     * @param event - currently event.
     */
    void checkMouseWI (SDL_Event * event);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Mouse control for chest inventory menu.
     * @param event - currently event.
     */
    void checkMouseCI (SDL_Event * event, CBuildChest *chest);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Mouse control for inventory menu.
     * @param event - currently event.
     */
    void checkMouseSI (SDL_Event * event);

    //------------------------------------------------------------------------------------------------------------------
    /**
    * Keyboard control for inventory menu.
    * @param event - currently event.
    */
    void checkKeyboard (SDL_Event *event);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Replace half item in vector with item in hand.
     * @param vector - currently vector.
     * @param pos - position in vector.
     */
    void swapRight (std::vector<CInventoryTails *> & vector, uint8_t pos);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Replace armor items.
     * @param pos - currently position: 1 - helmet, 2 - armor, 3 - boots.
     */
    void swapInArmor (uint8_t pos);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Replace item in vector with item in hand.
     * @param vector - currently vector.
     * @param pos - position in vector.
     */
    void swapLeft (std::vector<CInventoryTails *> &vector, uint8_t pos);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Controller for crafting in inventory.
     */
    void littleCraftController ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Get craft item.
     */
    void getLittleCraftItem ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Remove the item that is in hand in inventory.
     */
    void clearHand ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Clear little craft table before exit.
     */
    void clearLittleCraft ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Checks the correctness of the recipe for the workbench.
     */
    void bigCraftControl ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * To obtain a new item.
     */
    void getBigCraftItem ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Clear the craft field.
     */
    void clearBigCraft ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Insert in currently position for inventory item.
     * @param inventoryTails - inventory item.
     */
    void insertInCurrentPosition (CInventoryTails *inventoryTails);

    //------------------------------------------------------------------------------------------------------------------
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
    uint8_t understandMouseInArmor (int x, int y);

    //------------------------------------------------------------------------------------------------------------------
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
    uint8_t understandMouseIninventory (int x, int y);

    //------------------------------------------------------------------------------------------------------------------
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
    uint8_t understandMouseChest (int x, int y);

    //------------------------------------------------------------------------------------------------------------------
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
    uint8_t understandMouseCraft (int x, int y);
    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw a quick inventory.
     */
    void drawSpeedInventory ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Updates player data: damage.
     */
    void updateStats ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Updates player data: armor.
     */
    void updateArmor ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw a field for crafting.
     */
    void drawCraft ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw a field inventory for the chest.
     * @param chest - pointer to chest.
     */
    void drawChest (CBuildChest * chest);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw a field for inventory.
     */
    void drawInventory ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Highlights the currently selected position.
     */
    void drawSelecter ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw a number at the position.
     * @param x - x-coordinate left top.
     * @param y - y-coordinate left top.
     * @param num - number > 1.
     */
    void drawNum (int x, int y, uint8_t num);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draws the selected item at the cursor position.
     * @param event
     */
    void drawHandItem (SDL_Event * event);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Will draw the dressed protection.
     */
    void drawArmor ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draws player health.
     */
    void drawHealth ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draws an image of a specific \ full size on a renderer at a specific location
     * or starting from the top left if dstRect == NULL.
     * @param texture - where does the image come from.
     * @param srcRect - what is the size of the drawn image.
     * @param dstRect - what size will be drawn.
     */
    void drawInPosition (SDL_Texture *texture, SDL_Rect *srcRect, SDL_Rect *dstRect) const;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Sets the rectangle parameters to the specified x, y, w(width), h(height).
     * @param src - rectangle that is changing.
     * @param x - coordinate of the point x of the upper left corner.
     * @param y - coordinate of the point y of the upper left corner.
     * @param w - width.
     * @param h - height.
     */
    void setRect (SDL_Rect * rect, int x, int y, int w = 32, int h = 32);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Clear the renderer inside the window.
     */
    void clear ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Refresh the renderer inside the window.
     */
    void draw ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw background.
     */
    void drawBG ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Write data about all inventory items for the test.
     * @param str - header text.
     */
    void testInfo (std::string str = "") const;
};
//======================================================================================================================