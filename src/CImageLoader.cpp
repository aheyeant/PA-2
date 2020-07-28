/**
 * @file Implementation of class CImageLoader.
 */
//======================================================================================================================
#include "CImageLoader.h"
//======================================================================================================================

//==================struct TWater=======================================================================================
/**
 * @param renderer - current renderer.
 */
TWater::TWater (SDL_Renderer *renderer) {
    t_water_1 = createTexture(BUILD_TAIL_WATER_1_PATH, renderer);
    t_water_2 = createTexture(BUILD_TAIL_WATER_2_PATH, renderer);
    t_water_3 = createTexture(BUILD_TAIL_WATER_3_PATH, renderer);
    t_water_4 = createTexture(BUILD_TAIL_WATER_4_PATH, renderer);

    isGood = (t_water_1 && t_water_2 && t_water_3 && t_water_4);
}

//----------------------------------------------------------------------------------------------------------------------
TWater::~TWater () {
    if (t_water_1)
        SDL_DestroyTexture(t_water_1);
    if (t_water_2)
        SDL_DestroyTexture(t_water_2);
    if (t_water_3)
        SDL_DestroyTexture(t_water_3);
    if (t_water_4)
        SDL_DestroyTexture(t_water_4);
}
//======================================================================================================================

//==================struct TLava========================================================================================
/**
 * @param renderer - current renderer.
 */
TLava::TLava (SDL_Renderer *renderer) {
    t_lava_1 = createTexture(BUILD_TAIL_LAVA_1_PATH, renderer);
    t_lava_2 = createTexture(BUILD_TAIL_LAVA_2_PATH, renderer);
    t_lava_3 = createTexture(BUILD_TAIL_LAVA_3_PATH, renderer);
    t_lava_4 = createTexture(BUILD_TAIL_LAVA_4_PATH, renderer);

    isGood = (t_lava_1 && t_lava_2 && t_lava_3 && t_lava_4);
}

//----------------------------------------------------------------------------------------------------------------------
TLava::~TLava () {
    if (t_lava_1)
        SDL_DestroyTexture(t_lava_1);
    if (t_lava_2)
        SDL_DestroyTexture(t_lava_2);
    if (t_lava_3)
        SDL_DestroyTexture(t_lava_3);
    if (t_lava_4)
        SDL_DestroyTexture(t_lava_4);
}
//======================================================================================================================

//==================struct TMapTail=====================================================================================
/**
 * @param renderer - current renderer.
 */
TMapTail::TMapTail (SDL_Renderer *renderer) {
    t_ground        = createTexture(BUILD_TAIL_GROUND_PATH, renderer);
    t_greenGround   = createTexture(BUILD_TAIL_GREEN_GROUND_PATH, renderer);
    t_stone         = createTexture(BUILD_TAIL_STONE_PATH, renderer);
    t_sand          = createTexture(BUILD_TAIL_SAND_PATH, renderer);
    t_mossStone     = createTexture(BUILD_TAIL_MOSS_STONE_PATH, renderer);
    t_lavaStone     = createTexture(BUILD_TAIL_LAVA_STONE_PATH, renderer);
    t_wood          = createTexture(BUILD_TAIL_WOOD_PATH, renderer);
    t_foliage       = createTexture(BUILD_TAIL_FOLIAGE_PATH, renderer);
    t_coal          = createTexture(BUILD_TAIL_COAL_PATH, renderer);
    t_iron          = createTexture(BUILD_TAIL_IRON_PATH, renderer);
    t_gold          = createTexture(BUILD_TAIL_GOLD_PATH, renderer);
    t_diamond       = createTexture(BUILD_TAIL_DIAMOND_PATH, renderer);
    t_chest         = createTexture(BUILD_TAIL_CHEST_PATH, renderer);
    t_workbench     = createTexture(BUILD_TAIL_WORKBENCH_PATH, renderer);
    t_BG_ground     = createTexture(BUILD_TAIL_BG_GROUND_PATH, renderer);
    t_BG_stone      = createTexture(BUILD_TAIL_BG_STONE_PATH, renderer);
    std::ifstream fin (SKY_SETTINGS_PATH, std::ios_base::binary);
    if (fin.fail()) {
        fin.close();
        t_BG_sky = createTexture(BUILD_TAIL_SKY_1_PATH, renderer);
    } else {
        uint8_t type;
        if ((fin.read((char*) &type, sizeof(uint8_t)).gcount() != sizeof(uint8_t))) {
            fin.close();
            t_BG_sky = createTexture(BUILD_TAIL_SKY_1_PATH, renderer);
        } else {
            fin.close();
            if (type) {
                t_BG_sky = createTexture(BUILD_TAIL_SKY_2_PATH, renderer);
            } else {
                t_BG_sky = createTexture(BUILD_TAIL_SKY_1_PATH, renderer);
            }
        }
    }
    lava            = new TLava (renderer);
    water           = new TWater (renderer);

    isGood = (t_gold       && t_greenGround && t_stone     && t_sand       && t_mossStone  && t_lavaStone && t_ground &&
              t_wood       && t_foliage     && t_coal      && t_iron       && t_gold       && t_diamond   && t_chest  &&
              t_workbench  && t_BG_sky      && t_BG_ground && t_BG_stone   && lava->isGood && water->isGood);
}

//----------------------------------------------------------------------------------------------------------------------
TMapTail::~TMapTail () {
    if (t_ground)
        SDL_DestroyTexture(t_ground);
    if (t_greenGround)
        SDL_DestroyTexture(t_greenGround);
    if (t_stone)
        SDL_DestroyTexture(t_stone);
    if (t_sand)
        SDL_DestroyTexture(t_sand);
    if (t_mossStone)
        SDL_DestroyTexture(t_mossStone);
    if (t_lavaStone)
        SDL_DestroyTexture(t_lavaStone);
    if (t_wood)
        SDL_DestroyTexture(t_wood);
    if (t_foliage)
        SDL_DestroyTexture(t_foliage);
    if (t_coal)
        SDL_DestroyTexture(t_coal);
    if (t_iron)
        SDL_DestroyTexture(t_iron);
    if (t_gold)
        SDL_DestroyTexture(t_gold);
    if (t_diamond)
        SDL_DestroyTexture(t_diamond);
    if (t_chest)
        SDL_DestroyTexture(t_chest);
    if (t_workbench)
        SDL_DestroyTexture(t_workbench);
    if (t_BG_ground)
        SDL_DestroyTexture(t_BG_ground);
    if (t_BG_sky)
        SDL_DestroyTexture(t_BG_sky);
    if (t_BG_stone)
        SDL_DestroyTexture(t_BG_stone);
    delete lava;
    delete water;
}
//======================================================================================================================

//==================struct TTails=======================================================================================
/**
 * @param renderer - current renderer.
 */
TTails::TTails (SDL_Renderer *renderer) {
    t_ground        = createTexture(INVENTORY_TAIL_GROUND_PATH , renderer);
    t_stone         = createTexture(INVENTORY_TAIL_STONE_PATH , renderer);
    t_sand          = createTexture(INVENTORY_TAIL_SAND_PATH , renderer);
    t_mossStone     = createTexture(INVENTORY_TAIL_MOSS_STONE_PATH , renderer);
    t_wood          = createTexture(INVENTORY_TAIL_WOOD_PATH , renderer);
    t_foliage       = createTexture(INVENTORY_TAIL_FOLIAGE_PATH , renderer);
    t_coal          = createTexture(INVENTORY_TAIL_COAL_PATH , renderer);
    t_iron          = createTexture(INVENTORY_TAIL_IRON_PATH , renderer);
    t_gold          = createTexture(INVENTORY_TAIL_GOLD_PATH , renderer);
    t_diamond       = createTexture(INVENTORY_TAIL_DIAMOND_PATH , renderer);
    t_chest         = createTexture(INVENTORY_TAIL_CHEST_PATH , renderer);
    t_workbench     = createTexture(INVENTORY_TAIL_WORKBENCH_PATH , renderer);

    isGood = (t_gold && t_stone && t_sand && t_mossStone && t_wood  && t_foliage && t_ground &&
              t_coal && t_iron  && t_gold && t_diamond   && t_chest && t_workbench);
}

//----------------------------------------------------------------------------------------------------------------------
TTails::~TTails () {
    if (t_ground)
        SDL_DestroyTexture(t_ground);
    if (t_stone)
        SDL_DestroyTexture(t_stone);
    if (t_sand)
        SDL_DestroyTexture(t_sand);
    if (t_mossStone)
        SDL_DestroyTexture(t_mossStone);
    if (t_wood)
        SDL_DestroyTexture(t_wood);
    if (t_foliage)
        SDL_DestroyTexture(t_foliage);
    if (t_coal)
        SDL_DestroyTexture(t_coal);
    if (t_iron)
        SDL_DestroyTexture(t_iron);
    if (t_gold)
        SDL_DestroyTexture(t_gold);
    if (t_diamond)
        SDL_DestroyTexture(t_diamond);
    if (t_chest)
        SDL_DestroyTexture(t_chest);
    if (t_workbench)
        SDL_DestroyTexture(t_workbench);
}
//======================================================================================================================

//==================struct TPicks=======================================================================================
/**
 * @param renderer - current renderer.
 */
TPicks::TPicks (SDL_Renderer *renderer) {
    t_woodPick      = createTexture(INVENTORY_TAIL_WOOD_PICK_PATH, renderer);
    t_stonePick     = createTexture(INVENTORY_TAIL_STONE_PICK_PATH, renderer);
    t_ironPick      = createTexture(INVENTORY_TAIL_IRON_PICK_PATH, renderer);
    t_goldPick      = createTexture(INVENTORY_TAIL_GOLD_PICK_PATH, renderer);
    t_diamondPick   = createTexture(INVENTORY_TAIL_DIAMOND_PICK_PATH, renderer);

    isGood = (t_woodPick && t_stonePick && t_ironPick && t_goldPick && t_diamondPick);
}

//----------------------------------------------------------------------------------------------------------------------
TPicks::~TPicks () {
    if (t_woodPick)
        SDL_DestroyTexture(t_woodPick);
    if (t_stonePick)
        SDL_DestroyTexture(t_stonePick);
    if (t_ironPick)
        SDL_DestroyTexture(t_ironPick);
    if (t_goldPick)
        SDL_DestroyTexture(t_goldPick);
    if (t_diamondPick)
        SDL_DestroyTexture(t_diamondPick);
}
//======================================================================================================================

//==================struct TBlades======================================================================================
/**
 * @param renderer - current renderer.
 */
TBlades::TBlades (SDL_Renderer *renderer) {
    t_woodBlade     = createTexture(INVENTORY_TAIL_WOOD_BLADE_PATH, renderer);
    t_stoneBlade    = createTexture(INVENTORY_TAIL_STONE_BLADE_PATH, renderer);
    t_ironBlade     = createTexture(INVENTORY_TAIL_IRON_BLADE_PATH, renderer);
    t_goldBlade     = createTexture(INVENTORY_TAIL_GOLD_BLADE_PATH, renderer);
    t_diamondBlade  = createTexture(INVENTORY_TAIL_DIAMOND_BLADE_PATH, renderer);

    isGood = (t_woodBlade && t_stoneBlade && t_ironBlade && t_goldBlade && t_diamondBlade);
}

//----------------------------------------------------------------------------------------------------------------------
TBlades::~TBlades () {
    if (t_woodBlade)
        SDL_DestroyTexture(t_woodBlade);
    if (t_stoneBlade)
        SDL_DestroyTexture(t_stoneBlade);
    if (t_ironBlade)
        SDL_DestroyTexture(t_ironBlade);
    if (t_goldBlade)
        SDL_DestroyTexture(t_goldBlade);
    if (t_diamondBlade)
        SDL_DestroyTexture(t_diamondBlade);
}
//======================================================================================================================

//==================struct TAxes========================================================================================
/**
 * @param renderer - current renderer.
 */
TAxes::TAxes (SDL_Renderer *renderer) {
    t_woodAxe       = createTexture(INVENTORY_TAIL_WOOD_AXE_PATH, renderer);
    t_stoneAxe      = createTexture(INVENTORY_TAIL_STONE_AXE_PATH, renderer);
    t_ironAxe       = createTexture(INVENTORY_TAIL_IRON_AXE_PATH, renderer);
    t_goldAxe       = createTexture(INVENTORY_TAIL_GOLD_AXE_PATH, renderer);
    t_diamondAxe    = createTexture(INVENTORY_TAIL_DIAMOND_AXE_PATH, renderer);

    isGood = (t_woodAxe && t_stoneAxe && t_ironAxe && t_goldAxe && t_diamondAxe);
}

//----------------------------------------------------------------------------------------------------------------------
TAxes::~TAxes () {
    if (t_woodAxe)
        SDL_DestroyTexture(t_woodAxe);
    if (t_stoneAxe)
        SDL_DestroyTexture(t_stoneAxe);
    if (t_ironAxe)
        SDL_DestroyTexture(t_ironAxe);
    if (t_goldAxe)
        SDL_DestroyTexture(t_goldAxe);
    if (t_diamondAxe)
        SDL_DestroyTexture(t_diamondAxe);
}
//======================================================================================================================

//==================struct THelmets=====================================================================================
/**
 * @param renderer - current renderer.
 */
THelmets::THelmets (SDL_Renderer *renderer) {
    t_ironHelmet    = createTexture(INVENTORY_TAIL_IRON_HELMET_PATH, renderer);
    t_goldHelmet    = createTexture(INVENTORY_TAIL_GOLD_HELMET_PATH, renderer);
    t_diamondHelmet = createTexture(INVENTORY_TAIL_DIAMOND_HELMET_PATH, renderer);

    isGood = (t_ironHelmet && t_goldHelmet && t_diamondHelmet);
}

//----------------------------------------------------------------------------------------------------------------------
THelmets::~THelmets () {
    if (t_ironHelmet)
        SDL_DestroyTexture(t_ironHelmet);
    if (t_goldHelmet)
        SDL_DestroyTexture(t_goldHelmet);
    if (t_diamondHelmet)
        SDL_DestroyTexture(t_diamondHelmet);
}
//======================================================================================================================

//==================struct TArmors======================================================================================
TArmors::TArmors (SDL_Renderer *renderer) {
    t_ironArmor     = createTexture(INVENTORY_TAIL_IRON_ARMOR_PATH, renderer);
    t_goldArmor     = createTexture(INVENTORY_TAIL_GOLD_ARMOR_PATH, renderer);
    t_diamondArmor  = createTexture(INVENTORY_TAIL_DIAMOND_ARMOR_PATH, renderer);

    isGood = (t_ironArmor && t_goldArmor && t_diamondArmor);
}

//----------------------------------------------------------------------------------------------------------------------
TArmors::~TArmors () {
    if (t_ironArmor)
        SDL_DestroyTexture(t_ironArmor);
    if (t_goldArmor)
        SDL_DestroyTexture(t_goldArmor);
    if (t_diamondArmor)
        SDL_DestroyTexture(t_diamondArmor);
}
//======================================================================================================================

//==================struct TGameSkin====================================================================================
/**
 * @param renderer - current renderer.
 */
TGameSkin::TGameSkin (SDL_Renderer *renderer) {
    t_armorTable        = createTexture(GAME_ARMOR_TABLE_SKIN_PATH, renderer);
    t_chestInventory    = createTexture(GAME_CHEST_INVENTORY_SKIN_PATH, renderer);
    t_craft             = createTexture(GAME_CRAFT_SKIN_PATH, renderer);
    t_healthPlayer      = createTexture(GAME_HEALTH_PLAYER_SKIN_PATH, renderer);
    t_inventory         = createTexture(GAME_INVENTORY_SKIN_PATH, renderer);
    t_selecterInventory = createTexture(GAME_SELECTER_INVENTORY_SKIN_PATH, renderer);
    t_speedInventory    = createTexture(GAME_SPEED_INVENTORY_SKIN_PATH, renderer);
    t_nums              = createTexture(GAME_NUMS, renderer);

    isGood = (t_armorTable && t_chestInventory    && t_craft          && t_healthPlayer &&
              t_inventory  && t_selecterInventory && t_speedInventory && t_nums);
}

//----------------------------------------------------------------------------------------------------------------------
TGameSkin::~TGameSkin () {
    if (t_armorTable)
        SDL_DestroyTexture(t_armorTable);
    if (t_chestInventory)
        SDL_DestroyTexture(t_chestInventory);
    if (t_craft)
        SDL_DestroyTexture(t_craft);
    if (t_healthPlayer)
        SDL_DestroyTexture(t_healthPlayer);
    if (t_inventory)
        SDL_DestroyTexture(t_inventory);
    if (t_selecterInventory)
        SDL_DestroyTexture(t_selecterInventory);
    if (t_speedInventory)
        SDL_DestroyTexture(t_speedInventory);
    if (t_nums)
        SDL_DestroyTexture(t_nums);
}
//======================================================================================================================

//==================struct TBoots=======================================================================================
/**
 * @param renderer - current renderer.
 */
TBoots::TBoots (SDL_Renderer *renderer) {
    t_ironBoot      = createTexture(INVENTORY_TAIL_IRON_BOOT_PATH, renderer);
    t_goldBoot      = createTexture(INVENTORY_TAIL_GOLD_BOOT_PATH, renderer);
    t_diamondBoot   = createTexture(INVENTORY_TAIL_DIAMOND_BOOT_PATH, renderer);

    isGood = (t_ironBoot && t_goldBoot && t_diamondBoot);
}

//----------------------------------------------------------------------------------------------------------------------
TBoots::~TBoots () {
    if (t_ironBoot)
        SDL_DestroyTexture(t_ironBoot);
    if (t_goldBoot)
        SDL_DestroyTexture(t_goldBoot);
    if (t_diamondBoot)
        SDL_DestroyTexture(t_diamondBoot);
}
//======================================================================================================================

//==================struct TPlayerImage=================================================================================
/**
 * @param renderer - current renderer.
 */
TPlayerImage::TPlayerImage (SDL_Renderer *renderer) {
    t_player = createTexture(PLAYER_PLAYER_PATH, renderer);
    t_zombie = createTexture(PLAYER_ZOMBIE_PATH, renderer);
    t_warrior = createTexture(PLAYER_WARRIOR_PATH, renderer);

    isGood = (t_player && t_zombie && t_warrior);
}

//----------------------------------------------------------------------------------------------------------------------
TPlayerImage::~TPlayerImage () {
    if (t_player)
        SDL_DestroyTexture(t_player);
    if (t_zombie)
        SDL_DestroyTexture(t_zombie);
    if (t_warrior)
        SDL_DestroyTexture(t_warrior);
}
//======================================================================================================================

//==================struct TInventoryTail===============================================================================
/**
 * @param renderer - current renderer.
 */
TInventoryTail::TInventoryTail (SDL_Renderer *renderer) {
    t_stick         = createTexture(INVENTORY_TAIL_STICK_PATH, renderer);
    tails           = new TTails(renderer);
    picks           = new TPicks(renderer);
    blades          = new TBlades(renderer);
    axes            = new TAxes(renderer);
    helmets         = new THelmets (renderer);
    armors          = new TArmors (renderer);
    boots           = new TBoots (renderer);

    isGood = (t_stick      && tails->isGood   && picks->isGood && blades->isGood &&
              axes->isGood && helmets->isGood && armors->isGood && boots->isGood);
}

//----------------------------------------------------------------------------------------------------------------------
TInventoryTail::~TInventoryTail () {
    if (t_stick)
        SDL_DestroyTexture(t_stick);
    delete tails;
    delete picks;
    delete blades;
    delete axes;
    delete helmets;
    delete armors;
    delete boots;
}
//======================================================================================================================

//==================struct TImages======================================================================================
/**
 * @param renderer - current renderer.
 */
TImages::TImages (SDL_Renderer *renderer) {
    t_selecter      = createTexture(GAME_SELECTER_PATH, renderer);
    t_background    = createTexture(BACK_GROUND_CHEST_PATH, renderer);
    mapTail         = new TMapTail (renderer);
    inventoryTail   = new TInventoryTail (renderer);
    playerImage     = new TPlayerImage (renderer);
    gameSkin        = new TGameSkin (renderer);

    isGood = (t_selecter          && mapTail->isGood  && inventoryTail->isGood &&
              playerImage->isGood && gameSkin->isGood && t_background);
}

//----------------------------------------------------------------------------------------------------------------------
TImages::~TImages () {
    if (t_selecter)
        SDL_DestroyTexture(t_selecter);
    if (t_background)
        SDL_DestroyTexture(t_background);
    delete mapTail;
    delete inventoryTail;
    delete playerImage;
    delete gameSkin;
}
//======================================================================================================================

//==================class CImageLoader==================================================================================
/**
* Will create a new structure with all the textures,
* if there is a load error, set up a m_images == nullptr.
* @param sdl_renderer - renderer for the association with the texture.
*/
CImageLoader::CImageLoader (SDL_Renderer *sdl_renderer) {
    m_images = new TImages (sdl_renderer);
    if (!m_images->isGood) {
        delete m_images;
        m_images = nullptr;
    }
}

//----------------------------------------------------------------------------------------------------------------------
CImageLoader::~CImageLoader () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
* @return a pointer to a structure of textures or nullptr in case of error.
*/
TImages* CImageLoader::getImages () const {
    return m_images;
}
//======================================================================================================================

//==================createTexture=======================================================================================
/**
 * Creates a texture from a PNG file that is named fileName.
 * @param fileName - file path.
 * @return a pointer to created texture.
 */
SDL_Texture * createTexture (const std::string &fileName, SDL_Renderer * renderer) {
    SDL_Surface *dst = SDL_LoadBMP(fileName.c_str());
    if (!dst) {
        std::cout << "ERROR of loading BMP file \"" << fileName << "\": " << SDL_GetError() << std::endl;
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, dst);
    SDL_FreeSurface(dst);
    if (!texture) {
        std::cout << "ERROR create texture: " << SDL_GetError() << std::endl;
    }
    return texture;
}
//======================================================================================================================