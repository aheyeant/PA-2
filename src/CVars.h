/**
 * @file Contains initializes program constants and variables.
 */
//======================================================================================================================
#pragma once
//======================================================================================================================

//=================Program Constants====================================================================================

//=================Game control Constants===============================================================================
const int         SCREEN_WIDTH                          = 1920;
const int         SCREEN_HEIGHT                         = 992;
const int         CREATE_WIDTH                          = 1000;
const int         CREATE_HEIGHT                         = 500;
const int         LOAD_WIDTH                            = 1060;
const int         LOAD_HEIGHT                           = 540;
const int         MAX_NPCS                              = 10;
const bool        MAX_10_MS                             = true; //true - max update time = 10ms

const int         MAX_LENGTH_GROUND_HILL                = 100;
const int         MAX_MOVE_GROUND_HORIZON               = 5;
const int         CHANCE_CREATE_GROUND_HILL             = 50;   // 1 / $(50)
const int         CHANCE_STEP_WHEN_CREATE_GROUND_HILL   = 2;    // 1 / $(2)

const int         MAX_LENGTH_STONE_HILL                 = 100;
const int         MAX_MOVE_STONE_HORIZON                = 5;
const int         CHANCE_CREATE_STONE_HILL              = 50;   // 1 / $(50)
const int         CHANCE_STEP_WHEN_CREATE_STONE_HILL    = 2;    // 1 / $(2)

const int         NOISE_LEVEL_GROUND                    = 2;    // 1 / ((CREATE_HEIGHT - "y-coordinate") * $(2))
const int         NOISE_LEVEL_STONE                     = 2;    // 1 / ((horizon - "y-coordinate") * $(2))


const int         CHANCE_PLANT_TREE                     = 5;    // 1 / $(5)
const int         STEP_BETWEEN_TREE                     = 5;
const int         CHANCE_SCATTER_MINERALS               = 5;    // 1 / $(5)
const int         STEP_BACK_WHEN_CREATE_CAVE            = 0;
const int         COUNT_TYPES_NPCS                      = 2;
const int         CHANCE_SEED_NPCS                      = 80;   // 1 / $(80)
const int         CHANCE_SPILL_FLUID                    = 50;   // 1 / $(50)

const int         CHANGE_FRAME_SPEED                    = 30;   // 30 frame per second
const int         CHEST_MAX_COUNT                       = 30;
const int         INVENTORY_MAX_COUNT                   = 40;
const float       TAIL_HEALTH_UPPER                     = 1;
const float       STEP_PLAYER                           = 1.4f;

const int         PLAYER_HEALTH                         = 100;

const int         ZOMBIE_HEALTH                         = 50;
const int         WARRIOR_HEALTH                        = 60;
const int         FLUID_ENERGY                          = 10;

const float       PLAYER_HAND_DAMAGE                    = 20;   // per second; recomended - 8
const float       LAVA_DAMAGE                           = 8;    // per second
const float       AIR_DAMGE                             = 20;   // per second
const float       AIR_DAMGE_ACCELERATION                = 0.4;

const int         TIMEOUT                               = 3;    // per millisecond

const int         SPEED_INVENTORY_X                     = 740;
const int         SPEED_INVENTORY_Y                     = 926;
const int         INVENTORY_X                           = 100;
const int         INVENTORY_Y                           = 300;
const int         CHEST_X                               = 940;
const int         CHEST_Y                               = 300;
const int         CRAFT_X                               = 940;
const int         CRAFT_Y                               = 300;
//======================================================================================================================

//=================Images Constants=====================================================================================
const std::string DIR                                   = "src/";
const std::string BACK_GROUND_PATH                      = DIR + "image/back_ground.bmp";
const std::string BACK_GROUND_CHEST_PATH                = DIR + "image/GAME_IMAGES/chest_BG.bmp";
const std::string BACK_GROUND_CREATING_PATH             = DIR + "image/back_ground_creating.bmp";
const std::string BACK_GROUND_LOADING_PATH              = DIR + "image/back_ground_loading.bmp";
const std::string START_PAGE_NEW_GAME_PATH              = DIR + "image/START_PAGE/new_game.bmp";
const std::string START_PAGE_CONTINUE_PATH              = DIR + "image/START_PAGE/continue.bmp";
const std::string START_PAGE_SETTINGS_PATH              = DIR + "image/START_PAGE/settings.bmp";
const std::string START_PAGE_KEYS_PATH                  = DIR + "image/START_PAGE/keys.bmp";
const std::string START_PAGE_EXIT_PATH                  = DIR + "image/START_PAGE/exit.bmp";
const std::string KEYS_PATH                             = DIR + "image/KEY_MAP/key_map.bmp";
const std::string EXIT_PATH                             = DIR + "image/EXIT_PAGE/exit_menu.bmp";
const std::string GAME_MENU_CONTINUE_PATH               = DIR + "image/GAME_MENU/continue.bmp";
const std::string GAME_MENU_SAVE_PATH                   = DIR + "image/GAME_MENU/save.bmp";
const std::string GAME_MENU_KEYS_PATH                   = DIR + "image/GAME_MENU/keys.bmp";
const std::string GAME_MENU_SETTINGS_PATH               = DIR + "image/GAME_MENU/settings.bmp";
const std::string GAME_MENU_EXIT_PATH                   = DIR + "image/GAME_MENU/exit.bmp";
const std::string CONTINUE_MENU_SKIN_PATH               = DIR + "image/CONTINUE_MENU/continue_game.bmp";
const std::string CONTINUE_MENU_NUMS_PATH               = DIR + "image/CONTINUE_MENU/num_for_continue.bmp";
const std::string SETTINGS_MENU_SKIN_PATH               = DIR + "image/SETTINGS_MENU/settings.bmp";
const std::string SETTINGS_MENU_SELECTER_PATH           = DIR + "image/SETTINGS_MENU/selecter.bmp";
const std::string GAME_SELECTER_PATH                    = DIR + "image/game_selecter.bmp";
const std::string GAME_INVENTORY_SKIN_PATH              = DIR + "image/GAME_IMAGES/inventory.bmp";
const std::string GAME_CHEST_INVENTORY_SKIN_PATH        = DIR + "image/GAME_IMAGES/chest_inventory.bmp";
const std::string GAME_CRAFT_SKIN_PATH                  = DIR + "image/GAME_IMAGES/craft.bmp";
const std::string GAME_SPEED_INVENTORY_SKIN_PATH        = DIR + "image/GAME_IMAGES/speed_inventory.bmp";
const std::string GAME_SELECTER_INVENTORY_SKIN_PATH     = DIR + "image/GAME_IMAGES/selecter_inventory.bmp";
const std::string GAME_ARMOR_TABLE_SKIN_PATH            = DIR + "image/GAME_IMAGES/armor_table.bmp";
const std::string GAME_HEALTH_PLAYER_SKIN_PATH          = DIR + "image/GAME_IMAGES/health_player.bmp";
const std::string GAME_NUMS                             = DIR + "image/GAME_IMAGES/nums.bmp";
const std::string RESPAWN_BACK_GROUND                   = DIR + "image/RESPAWN/back_ground.bmp";
const std::string RESPAWN_TIME_1                        = DIR + "image/RESPAWN/time_1.bmp";
const std::string RESPAWN_TIME_2                        = DIR + "image/RESPAWN/time_2.bmp";
const std::string RESPAWN_TIME_3                        = DIR + "image/RESPAWN/time_3.bmp";
const std::string RESPAWN_TIME_4                        = DIR + "image/RESPAWN/time_4.bmp";
const std::string RESPAWN_TIME_5                        = DIR + "image/RESPAWN/time_5.bmp";
//======================================================================================================================

//=================Players Images Constants=============================================================================
const std::string PLAYER_PLAYER_PATH                    = DIR + "image/SKINS/modnyj.bmp";
const std::string PLAYER_ZOMBIE_PATH                    = DIR + "image/SKINS/zombie.bmp";
const std::string PLAYER_WARRIOR_PATH                   = DIR + "image/SKINS/warrior.bmp";
//======================================================================================================================

//=================Build Tail Constants=================================================================================
const std::string BUILD_TAIL_GROUND_PATH                = DIR + "image/MAP_TAILS/ground.bmp";             // id = 1
const std::string BUILD_TAIL_GREEN_GROUND_PATH          = DIR + "image/MAP_TAILS/greenGround.bmp";        // id = 2
const std::string BUILD_TAIL_SAND_PATH                  = DIR + "image/MAP_TAILS/sand.bmp";               // id = 3
const std::string BUILD_TAIL_STONE_PATH                 = DIR + "image/MAP_TAILS/stone.bmp";              // id = 4
const std::string BUILD_TAIL_MOSS_STONE_PATH            = DIR + "image/MAP_TAILS/moss_stone.bmp";         // id = 5
const std::string BUILD_TAIL_WOOD_PATH                  = DIR + "image/MAP_TAILS/wood.bmp";               // id = 6
const std::string BUILD_TAIL_LAVA_STONE_PATH            = DIR + "image/MAP_TAILS/lava_stone.bmp";         // id = 7

const std::string BUILD_TAIL_SKY_1_PATH                 = DIR + "image/MAP_TAILS/sky_2.bmp";              // id = 8
const std::string BUILD_TAIL_SKY_2_PATH                 = DIR + "image/MAP_TAILS/sky_3.bmp";              // id = 8

const std::string BUILD_TAIL_WORKBENCH_PATH             = DIR + "image/MAP_TAILS/workbench.bmp";          // id = 9
const std::string BUILD_TAIL_CHEST_PATH                 = DIR + "image/MAP_TAILS/chest.bmp";              // id = 10
const std::string BUILD_TAIL_IRON_PATH                  = DIR + "image/MAP_TAILS/iron.bmp";               // id = 11
const std::string BUILD_TAIL_GOLD_PATH                  = DIR + "image/MAP_TAILS/gold.bmp";               // id = 12
const std::string BUILD_TAIL_DIAMOND_PATH               = DIR + "image/MAP_TAILS/diamond.bmp";            // id = 13
const std::string BUILD_TAIL_COAL_PATH                  = DIR + "image/MAP_TAILS/coal.bmp";               // id = 14
const std::string BUILD_TAIL_FOLIAGE_PATH               = DIR + "image/MAP_TAILS/foliage.bmp";            // id = 15

const std::string BUILD_TAIL_BG_GROUND_PATH             = DIR + "image/MAP_TAILS/BG_ground.bmp";          // id = 16
const std::string BUILD_TAIL_BG_STONE_PATH              = DIR + "image/MAP_TAILS/BG_stone.bmp";           // id = 17

const std::string BUILD_TAIL_WATER_1_PATH               = DIR + "image/MAP_TAILS/water_1.bmp";            // id = 18
const std::string BUILD_TAIL_WATER_2_PATH               = DIR + "image/MAP_TAILS/water_2.bmp";            // id = 18
const std::string BUILD_TAIL_WATER_3_PATH               = DIR + "image/MAP_TAILS/water_3.bmp";            // id = 18
const std::string BUILD_TAIL_WATER_4_PATH               = DIR + "image/MAP_TAILS/water_4.bmp";            // id = 18

const std::string BUILD_TAIL_LAVA_1_PATH                = DIR + "image/MAP_TAILS/lava_1.bmp";             // id = 19
const std::string BUILD_TAIL_LAVA_2_PATH                = DIR + "image/MAP_TAILS/lava_2.bmp";             // id = 19
const std::string BUILD_TAIL_LAVA_3_PATH                = DIR + "image/MAP_TAILS/lava_3.bmp";             // id = 19
const std::string BUILD_TAIL_LAVA_4_PATH                = DIR + "image/MAP_TAILS/lava_4.bmp";             // id = 19
//======================================================================================================================

//=================Inventory Tail Constants=============================================================================
const std::string INVENTORY_TAIL_GROUND_PATH            = DIR + "image/INVENTORY_TAILS/ground.bmp";       // id = 101
const std::string INVENTORY_TAIL_STONE_PATH             = DIR + "image/INVENTORY_TAILS/stone.bmp";        // id = 102
const std::string INVENTORY_TAIL_SAND_PATH              = DIR + "image/INVENTORY_TAILS/sand.bmp";         // id = 103
const std::string INVENTORY_TAIL_MOSS_STONE_PATH        = DIR + "image/INVENTORY_TAILS/moss_stone.bmp";   // id = 104
const std::string INVENTORY_TAIL_WOOD_PATH              = DIR + "image/INVENTORY_TAILS/wood.bmp";         // id = 105
const std::string INVENTORY_TAIL_FOLIAGE_PATH           = DIR + "image/INVENTORY_TAILS/foliage.bmp";      // id = 106
const std::string INVENTORY_TAIL_COAL_PATH              = DIR + "image/INVENTORY_TAILS/coal.bmp";         // id = 107
const std::string INVENTORY_TAIL_IRON_PATH              = DIR + "image/INVENTORY_TAILS/iron.bmp";         // id = 108
const std::string INVENTORY_TAIL_GOLD_PATH              = DIR + "image/INVENTORY_TAILS/gold.bmp";         // id = 109
const std::string INVENTORY_TAIL_DIAMOND_PATH           = DIR + "image/INVENTORY_TAILS/diamond.bmp";      // id = 110
const std::string INVENTORY_TAIL_CHEST_PATH             = DIR + "image/INVENTORY_TAILS/chest.bmp";        // id = 111
const std::string INVENTORY_TAIL_WORKBENCH_PATH         = DIR + "image/INVENTORY_TAILS/workbench.bmp";    // id = 112
const std::string INVENTORY_TAIL_STICK_PATH             = DIR + "image/INVENTORY_TAILS/stick.bmp";        // id = 113
//======================================================================================================================

//=================Inventory Pick Constants=============================================================================
const std::string INVENTORY_TAIL_WOOD_PICK_PATH         = DIR + "image/INVENTORY_TAILS/picks/wood_pick.bmp";      // id = 151
const std::string INVENTORY_TAIL_STONE_PICK_PATH        = DIR + "image/INVENTORY_TAILS/picks/stone_pick.bmp";     // id = 152
const std::string INVENTORY_TAIL_IRON_PICK_PATH         = DIR + "image/INVENTORY_TAILS/picks/iron_pick.bmp";      // id = 153
const std::string INVENTORY_TAIL_GOLD_PICK_PATH         = DIR + "image/INVENTORY_TAILS/picks/gold_pick.bmp";      // id = 154
const std::string INVENTORY_TAIL_DIAMOND_PICK_PATH      = DIR + "image/INVENTORY_TAILS/picks/diamond_pick.bmp";   // id = 155
//======================================================================================================================

//=================Inventory Blade Constants============================================================================
const std::string INVENTORY_TAIL_WOOD_BLADE_PATH        = DIR + "image/INVENTORY_TAILS/blades/wood_blade.bmp";    // id = 161
const std::string INVENTORY_TAIL_STONE_BLADE_PATH       = DIR + "image/INVENTORY_TAILS/blades/stone_blade.bmp";   // id = 162
const std::string INVENTORY_TAIL_IRON_BLADE_PATH        = DIR + "image/INVENTORY_TAILS/blades/iron_blade.bmp";    // id = 163
const std::string INVENTORY_TAIL_GOLD_BLADE_PATH        = DIR + "image/INVENTORY_TAILS/blades/gold_blade.bmp";    // id = 164
const std::string INVENTORY_TAIL_DIAMOND_BLADE_PATH     = DIR + "image/INVENTORY_TAILS/blades/diamond_blade.bmp"; // id = 165
//======================================================================================================================

//=================Inventory Axe Constants=============================================================================
const std::string INVENTORY_TAIL_WOOD_AXE_PATH          = DIR + "image/INVENTORY_TAILS/axes/wood_axe.bmp";        // id = 171
const std::string INVENTORY_TAIL_STONE_AXE_PATH         = DIR + "image/INVENTORY_TAILS/axes/stone_axe.bmp";       // id = 172
const std::string INVENTORY_TAIL_IRON_AXE_PATH          = DIR + "image/INVENTORY_TAILS/axes/iron_axe.bmp";        // id = 173
const std::string INVENTORY_TAIL_GOLD_AXE_PATH          = DIR + "image/INVENTORY_TAILS/axes/gold_axe.bmp";        // id = 174
const std::string INVENTORY_TAIL_DIAMOND_AXE_PATH       = DIR + "image/INVENTORY_TAILS/axes/diamond_axe.bmp";     // id = 175
//======================================================================================================================

//=================Inventory Helmet Constants===========================================================================
const std::string INVENTORY_TAIL_IRON_HELMET_PATH       = DIR + "image/INVENTORY_TAILS/helmets/iron_helmet.bmp";    // id = 181
const std::string INVENTORY_TAIL_GOLD_HELMET_PATH       = DIR + "image/INVENTORY_TAILS/helmets/gold_helmet.bmp";    // id = 182
const std::string INVENTORY_TAIL_DIAMOND_HELMET_PATH    = DIR + "image/INVENTORY_TAILS/helmets/diamond_helmet.bmp"; // id = 182
//======================================================================================================================

//=================Inventory Armor Constants============================================================================
const std::string INVENTORY_TAIL_IRON_ARMOR_PATH        = DIR + "image/INVENTORY_TAILS/armors/iron_armor.bmp";    // id = 191
const std::string INVENTORY_TAIL_GOLD_ARMOR_PATH        = DIR + "image/INVENTORY_TAILS/armors/gold_armor.bmp";    // id = 192
const std::string INVENTORY_TAIL_DIAMOND_ARMOR_PATH     = DIR + "image/INVENTORY_TAILS/armors/diamond_armor.bmp"; // id = 193
//======================================================================================================================

//=================Inventory Boot Constants=============================================================================
const std::string INVENTORY_TAIL_IRON_BOOT_PATH         = DIR + "image/INVENTORY_TAILS/boots/iron_boot.bmp";      // id = 201
const std::string INVENTORY_TAIL_GOLD_BOOT_PATH         = DIR + "image/INVENTORY_TAILS/boots/gold_boot.bmp";      // id = 202
const std::string INVENTORY_TAIL_DIAMOND_BOOT_PATH      = DIR + "image/INVENTORY_TAILS/boots/diamond_boot.bmp";   // id = 203
//======================================================================================================================

//=================Text Constants=======================================================================================
const std::string SKY_SETTINGS_PATH                     = DIR + "maps/sky_type.info";
const std::string MAP_SELECT_PATH                       = DIR + "maps/select_map.info";
const std::string MAP_COUNT_PATH                        = DIR + "maps/count_map.info";
const std::string MAP_NAME_PATH_BEGIN                   = DIR + "maps/map";
const std::string MAP_NAME_PATH_END                     = ".map";
//======================================================================================================================
