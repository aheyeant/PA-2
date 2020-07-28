/**
 * @file Contains declaration of class CImageLoader.
 */
//======================================================================================================================
#pragma once
//======================================================================================================================

//======================================================================================================================
#include <string>
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>

#include "CVars.h"
#include "CExceptions.h"
//======================================================================================================================

//==================struct TWater=======================================================================================
/**
 * Structure TWater contains a water texture.
 */
struct TWater {
    /**
     * @param renderer - current renderer.
     */
    explicit TWater (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TWater ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_water_1;
    SDL_Texture * t_water_2;
    SDL_Texture * t_water_3;
    SDL_Texture * t_water_4;
    bool          isGood;
};
//======================================================================================================================

//==================struct TLava========================================================================================
/**
 * Structure TLava contains a lava texture.
 */
struct TLava {
    /**
     * @param renderer - current renderer.
     */
    explicit TLava (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TLava ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_lava_1;
    SDL_Texture * t_lava_2;
    SDL_Texture * t_lava_3;
    SDL_Texture * t_lava_4;
    bool          isGood;
};
//======================================================================================================================

//==================struct TTails=======================================================================================
/**
 * Structure TTails contains all the textures that can be extracted in the game world.
 */
struct TTails {
    /**
     * @param renderer - current renderer.
     */
    explicit TTails (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TTails ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_ground;
    SDL_Texture * t_stone;
    SDL_Texture * t_sand;
    SDL_Texture * t_mossStone;
    SDL_Texture * t_wood;
    SDL_Texture * t_foliage;
    SDL_Texture * t_coal;
    SDL_Texture * t_iron;
    SDL_Texture * t_gold;
    SDL_Texture * t_diamond;
    SDL_Texture * t_chest;
    SDL_Texture * t_workbench;
    bool          isGood;
};
//======================================================================================================================

//==================struct TPicks=======================================================================================
/**
 * Structure TPicks contains a picks texture.
 */
struct TPicks {
    /**
     * @param renderer - current renderer.
     */
    explicit TPicks (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TPicks ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_woodPick;
    SDL_Texture * t_stonePick;
    SDL_Texture * t_ironPick;
    SDL_Texture * t_goldPick;
    SDL_Texture * t_diamondPick;
    bool          isGood;
};
//======================================================================================================================

//==================struct TBlades======================================================================================
/**
 * Structure TBlades contains a blades texture.
 */
struct TBlades {
    /**
     * @param renderer - current renderer.
     */
    explicit TBlades (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TBlades ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_woodBlade;
    SDL_Texture * t_stoneBlade;
    SDL_Texture * t_ironBlade;
    SDL_Texture * t_goldBlade;
    SDL_Texture * t_diamondBlade;
    bool          isGood;
};
//======================================================================================================================

//==================struct TAxes========================================================================================
/**
 * Structure TAxes contains a axes texture.
 */
struct TAxes {
    /**
    * @param renderer - current renderer.
    */
    explicit TAxes (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TAxes ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_woodAxe;
    SDL_Texture * t_stoneAxe;
    SDL_Texture * t_ironAxe;
    SDL_Texture * t_goldAxe;
    SDL_Texture * t_diamondAxe;
    bool          isGood;
};
//======================================================================================================================

//==================struct THelmets=====================================================================================
/**
 * Structure THelmets contains a helmets texture.
 */
struct THelmets {
    /**
     * @param renderer - current renderer.
     */
    explicit THelmets (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~THelmets ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_ironHelmet;
    SDL_Texture * t_goldHelmet;
    SDL_Texture * t_diamondHelmet;
    bool          isGood;
};
//======================================================================================================================

//==================struct TArmors======================================================================================
/**
 * Structure TArmor contains a armors texture.
 */
struct TArmors {
    /**
     * @param renderer - current renderer.
     */
    explicit TArmors (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TArmors ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_ironArmor;
    SDL_Texture * t_goldArmor;
    SDL_Texture * t_diamondArmor;
    bool          isGood;
};
//======================================================================================================================

//==================struct TGameSkin====================================================================================
/**
 * Structure TGameSkin contains a inventory and chest texture.
 */
struct TGameSkin {
    /**
     * @param renderer - current renderer.
     */
    explicit TGameSkin (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TGameSkin ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_armorTable;
    SDL_Texture * t_chestInventory;
    SDL_Texture * t_craft;
    SDL_Texture * t_healthPlayer;
    SDL_Texture * t_inventory;
    SDL_Texture * t_selecterInventory;
    SDL_Texture * t_speedInventory;
    SDL_Texture * t_nums;
    bool          isGood;
};
//======================================================================================================================

//==================struct TBoots=======================================================================================
/**
 * Structure TBoots contains a boots texture.
 */
struct TBoots{
    /**
     * @param renderer - current renderer.
     */
    explicit TBoots (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TBoots ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_ironBoot;
    SDL_Texture * t_goldBoot;
    SDL_Texture * t_diamondBoot;
    bool          isGood;
};
//======================================================================================================================

//==================struct TInventoryTail===============================================================================
/**
 * Structure TInventoryTail contains all the textures that can be in the inventory.
 */
struct TInventoryTail {
    /**
     * @param renderer - current renderer.
     */
    explicit TInventoryTail (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TInventoryTail ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_stick;
    TTails      * tails;
    TPicks      * picks;
    TBlades     * blades;
    TAxes       * axes;
    THelmets    * helmets;
    TArmors     * armors;
    TBoots      * boots;
    bool          isGood;
};
//======================================================================================================================


//==================struct TMapTail=====================================================================================
/**
 * Structure TMapTail contains all the textures that can be used on the map of the game world.
 */
struct TMapTail {
    /**
     * @param renderer - current renderer.
     */
    explicit TMapTail (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TMapTail ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_ground;
    SDL_Texture * t_greenGround;
    SDL_Texture * t_stone;
    SDL_Texture * t_sand;
    SDL_Texture * t_mossStone;
    SDL_Texture * t_lavaStone;
    SDL_Texture * t_wood;
    SDL_Texture * t_foliage;
    SDL_Texture * t_coal;
    SDL_Texture * t_iron;
    SDL_Texture * t_gold;
    SDL_Texture * t_diamond;
    SDL_Texture * t_chest;
    SDL_Texture * t_workbench;
    SDL_Texture * t_BG_sky;
    SDL_Texture * t_BG_ground;
    SDL_Texture * t_BG_stone;
    TLava       * lava;
    TWater      * water;
    bool          isGood;
};
//======================================================================================================================

//==================struct TPlayerImage=================================================================================
/**
 * Structure TPlayerImage contains all the textures of the players.
 */
struct TPlayerImage {
    /**
     * @param renderer - current renderer.
     */
    explicit TPlayerImage (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TPlayerImage ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture * t_player;
    SDL_Texture * t_zombie;
    SDL_Texture * t_warrior;
    bool          isGood;
};
//======================================================================================================================

//==================struct TImages======================================================================================
/**
 * Structure TImages contains all the textures of the game world.
 */
struct TImages {
    /**
     * @param renderer - current renderer.
     */
    explicit TImages (SDL_Renderer * renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~TImages ();

    //------------------------------------------------------------------------------------------------------------------
    SDL_Texture     * t_selecter;
    SDL_Texture     * t_background;
    TMapTail        * mapTail;
    TInventoryTail  * inventoryTail;
    TPlayerImage    * playerImage;
    TGameSkin       * gameSkin;
    bool              isGood;
};
//======================================================================================================================

//==================class CImageLoader==================================================================================
/**
 * Class CImageLoader load all textures for the game world.
 */
class CImageLoader {
public:
    /**
     * Will create a new structure with all the textures,
     * if there is a load error, set up a m_images == nullptr.
     * @param sdl_renderer - renderer for the association with the texture.
     */
    explicit CImageLoader (SDL_Renderer * sdl_renderer);

    //------------------------------------------------------------------------------------------------------------------
    ~CImageLoader ();

    //------------------------------------------------------------------------------------------------------------------
    /**
     * @return a pointer to a structure of textures or nullptr in case of error.
     */
    TImages * getImages () const;

private:
    TImages * m_images;
};
//======================================================================================================================


//==================createTexture=======================================================================================
/**
 * Creates a texture from a PNG file that is named fileName.
 * @param fileName - file path.
 * @return a pointer to created texture.
 */
SDL_Texture * createTexture (const std::string &fileName, SDL_Renderer * renderer);
//======================================================================================================================