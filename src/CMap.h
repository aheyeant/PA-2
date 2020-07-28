/**
 * @file Contains declaration of classes: CMap, CMapLoader, CMapCreater, CTestMapCreater.
 */
//======================================================================================================================
#pragma once
//======================================================================================================================

//======================================================================================================================
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <tuple>
#include <iostream>
#include <queue>

#include "CTails.h"
#include "CPlayer.h"
#include "CExceptions.h"
//======================================================================================================================

//==================class CMap==========================================================================================
class CMap {
public:
    /**
     * Creates a new map.
     * @param number - number of currently map.
     */
    explicit CMap (uint8_t number);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * No deeper deletions are required, the destructor itself
     * frees all map elements before exiting.
     */
    ~CMap ();

    //------------------------------------------------------------------------------------------------------------------
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
    void save ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draw one frame based on the current position of the player.
     * @param time - time interval.
     */
    void draw (float time, int x_sel, int y_sel);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Set currently renderer.
     * @param renderer - currently renderer.
     */
    void setRenderer (SDL_Renderer *renderer);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draws the currently selected tail on the map.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     */
    void drawSelectedTail (int x, int y);

    //------------------------------------------------------------------------------------------------------------------
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
    void damageWave (int x, int y, uint8_t dir, uint8_t damage);



    CBuildTailInterface                 *** m_map;
    std::vector<CPlayerInterface *>         m_NPCs;
    CPlayer                               * m_player;
    CTailMaker                            * m_maker; // don't need inside destroy;
    SDL_Renderer                          * m_renderer;
    std::pair<int, int>                     m_selectedTail;
    bool                                    m_select;
    bool                                    m_inicialize;
private:
    bool                                    m_currentlyDamage;
    uint32_t                                m_updateTime;
    uint8_t                                 m_damage;
    std::vector<std::pair<int, int>>        m_damageVector;
    uint8_t                                 m_number;
    SDL_Rect                                m_out[3];
    SDL_Rect                                m_in[3];
    SDL_Rect                                m_selIn[4];
    SDL_Rect                                m_selOut[4];

    //------------------------------------------------------------------------------------------------------------------
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
    void setOutRect (int x, int y);

    //------------------------------------------------------------------------------------------------------------------
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
    void setSelectRect (int x, int y);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Draws the texture of the player on the map.
     * @param playerInterface - current player.
     */
    void drawPlayer (CPlayerInterface * playerInterface);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Check whether damage to nip.
     * @param playerInterface - current player.
     */
    void damageControl (CPlayerInterface * playerInterface);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Provide liquid transfusion.
     * @param x - x-coordinate.
     * @param y - y-coordinate.
     */
    void updateFluidFunction (int x, int y);
};
//======================================================================================================================

//==================class CMapLoader====================================================================================
class CMapLoader {
public:
    /**
     * @param renderer - current renderer.
     */
    explicit CMapLoader (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~CMapLoader ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Initializes the map.
     * @return - map created.
     * @throw CErrorMapFileException
     * @throw CExistFileException.
     */
    CMap * returnMap ();

private:
    SDL_Renderer                      * m_renderer;
    CPlayer                           * m_player;
    CTailMaker                        * m_maker;
    CMap                              * m_returnMap;
    CBuildTailInterface             *** m_map;
    std::vector<CPlayerInterface *>     m_NPCs;
    uint8_t                             m_selectMap;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Allocate memory for all objects in the class.
     * @throw CBMPLoadExcepion.
     */
    void allocateData ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Emergency exit if there is an exception.
     */
    void emergencyEscape ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Reads the selected map from a file.
     */
    void readSelect ();

    //------------------------------------------------------------------------------------------------------------------
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
    void readMap ();

};
//======================================================================================================================

//==================class CMapCreater===================================================================================
class CMapCreater {
public:
    /**
     * Generates a new map and writes it to a new file named "map (number)"
     * where the number is the first free card number starting from one.
     * @throw CMapCreateException.
     */
    CMapCreater ();
private:
    uint8_t                                                      ** m_map;
    uint8_t                                                     *** m_chestInventory;
    std::vector<std::pair<uint8_t, std::pair<uint16_t, uint16_t>>>  m_NPCs;
    std::pair<uint8_t, std::pair<uint16_t, uint16_t>>               m_player;
    uint8_t                                                         m_mapNumber;
    uint8_t                                                         m_chestCount;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Will create map size of CREATE_WIDTH x CREATE_HEIGHT.
     */
    void allocateMap ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Destroy all allocated data.
     */
    void destroyData ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Creates a new map.
     */
    void createMap ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * The function returns the inventory for a new chest,
     * if the flag typeFlag(default 0) type is set to 0
     * in the chest will be a few rare things, if 1 rare
     * things will be more.
     * Used for chests at different depths.
     * @param type - 0-ordinary things, 1-rare things;
     * @return - pointer to possible inventory.
     */
    uint8_t** getChestContent (bool typeFlag = false);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Reads from a file the number of existing maps if the file
     * does not exist creates a new file and writes 1 if it fails
     * to write issues exception: CWriteFileException.
     * If there were no errors, writes the increment of the read
     * number to the file if unable to write issues exception:
     *
     * @throw CWriteFileException.
     */
    void readCount ();

    //------------------------------------------------------------------------------------------------------------------
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
    void writeMap ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Creates a ground mountainous terrain.
     * @param skyHorizon - the boundary between heaven and earth.
     */
    void makeGroundRelief (int skyHorizon);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Creates a stone mountainous terrain.
     * @param groundHorizon - the level of the horizon between the earth and the stones.
     */
    void makeStoneRelief (int groundHorizon);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Add stones on map above the level groundHorizon.
     * @param groundHorizon - the level of the horizon between the earth and the stones.
     */
    void addStones (int groundHorizon);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Add stones on map under the level groundHorizon.
     * @param groundHorizon - the level of the horizon between the earth and the stones.
     */
    void addGround (int groundHorizon);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Add trees to the map.
     */
    void plantTree ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Create caves on the map.
     */
    void makeCave ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Add minerals to the map.
     * @param skyHorizon - start layer.
     */
    void scatterMinerals (int skyHorizon);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Add randomly generated minerals.
     * @param x - x-coordinate left top corner.
     * @param y - y-coordinate left top corner.
     * @param type - if it's not 0, can add diamonds.
     */
    void setMinerals (int x, int y, int type);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Add water or lava to a map starting in position x,y.
     * @param startX - x-coordinate.
     * @param startY - y-coordinate.
     * @param type - 1 - lava, 0 - water;
     */
    void spillFluidInPosition (int startX, int startY, uint8_t type);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Add water or lava to a map.
     * @param skyHorizon - the initial level section of the earth and sky.
     */
    void spillFluid (int skyHorizon);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Adds new NPCs or any of the specified types in any place of the map.
     * @param count - NPSs count.
     * @param type - 0 - any type, > 0 - type of NPC.
     */
    void addNPCs (int count, uint8_t type = 0);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Add one nip to or near a specific position if there is a conflict.
     * @param type - type 0f NPC.
     * @param x - x-coordinate left top.
     * @param y - y-coordinate left top.
     */
    int addNPCInPosition (int type, int x, int y);

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Add a player in the middle of the map.
     */
    void addPlayer ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Will make the earth nearest to the sky green.
     */
    void makeGreenGround ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Fix imperfections in the map.
     */
    void fixBugs ();
};
//======================================================================================================================

//==================class CTestMapCreater===============================================================================
class CTestMapCreater {
public:
    /**
     * Generates a new map and writes it to a new file named "map (number)"
     * where the number is the first free card number starting from one.
     * @throw CMapCreateException.
     */
    CTestMapCreater ();

private:
    std::vector <std::pair<uint8_t, uint8_t>> * m_chests;
    uint8_t                                  ** m_item;
    uint8_t                                     m_type;
    uint8_t                                     m_mapNumber;
    uint8_t                                     m_chestCount;
    uint16_t                                    m_x_coordinate;
    uint16_t                                    m_y_coordinate;

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Destroy all allocated data.
     */
    void destroyData ();

    //------------------------------------------------------------------------------------------------------------------
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
    void writeMap ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Reads from a file the number of existing maps if the file
     * does not exist creates a new file and writes 1 if it fails
     * to write issues exception: CWriteFileException.
     * If there were no errors, writes the increment of the read
     * number to the file if unable to write issues exception:
     *
     * @throw CWriteFileException.
     */
    void readCount ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Creates a new map.
     */
    void createMap ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * Fix imperfections in the map.
     */
    void fixBugs ();
};
//======================================================================================================================