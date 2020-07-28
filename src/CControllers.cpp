/**
 * @file Implementation of classes: CGameController, CPageController.
 */
//======================================================================================================================
#include "CControllers.h"
//======================================================================================================================

//======================class CGameController===========================================================================
/**
 * @param map - the game map
 */
CGameController::CGameController (CMap *map) : m_map(map) {}

//----------------------------------------------------------------------------------------------------------------------
CGameController::~CGameController () {
    delete m_map;
}

//----------------------------------------------------------------------------------------------------------------------
/**
 * Controls the gameplay.
 */
EGameAction CGameController::run () {
    if (!m_map)
        return EGameAction::FINISH;
    int x_coordinate = 0;
    int y_coordinate = 0;
    bool left_buttom_click = false;

    SDL_Event event;

    event.type = SDL_KEYUP;
    uint32_t time = SDL_GetTicks();
    CTimer timer;


    while (true) {
        while (SDL_PollEvent(&event)) {
            timer.setTimer(TIMEOUT);
            if (event.type == SDL_QUIT) {
                return EGameAction::FINISH;
            }

            if (event.type == SDL_MOUSEMOTION) {
                x_coordinate = event.motion.x;
                y_coordinate = event.motion.y;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                left_buttom_click = true;
                x_coordinate = event.button.x;
                y_coordinate = event.button.y;
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                left_buttom_click = false;
                x_coordinate = event.button.x;
                y_coordinate = event.button.y;
            }

            float currentlyTime = (SDL_GetTicks() - time);
            time += currentlyTime;
            if (MAX_10_MS) {
                if (currentlyTime > 10) {
                    currentlyTime = 10;
                }
            }

            SDL_RenderClear(m_map->m_renderer);
            m_map->draw(currentlyTime, x_coordinate, y_coordinate);
            m_map->m_player->inventory->drawMainInfo();
            SDL_RenderPresent(m_map->m_renderer);
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_KP_8) {
                    if (m_map->m_player->m_onGround) {
                        m_map->m_player->dy = -0.35f;
                        m_map->m_player->m_onGround = false;
                    }
                }
                if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_KP_4) {
                    m_map->m_player->dx = -STEP_PLAYER;
                }
                if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_KP_6) {
                    m_map->m_player->dx = STEP_PLAYER;
                }
                if (event.key.keysym.sym == SDLK_i) {
                    m_map->m_player->inventory->simpleInventory();
                    time = SDL_GetTicks();
                    left_buttom_click = false;
                }
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    auto item = new CGameMenuPage(m_map->m_renderer);
                    auto tmp = item->run();
                    delete item;
                    switch (tmp) {
                        case EAction::QUIT :
                            return EGameAction::FINISH;
                        case EAction::CONTINUE :
                            break;
                        case EAction::SAVE :
                            try {
                                m_map->save();
                            } catch (const CException &exception) {
                                std::cout << exception.what();
                                return EGameAction::FINISH;
                            }
                            break;
                        case EAction::EXIT :
                            return EGameAction::FINISH;
                        default:
                            return EGameAction::FINISH;
                    }
                    left_buttom_click = false;
                    time = SDL_GetTicks();
                }
                if (event.key.keysym.sym == SDLK_1) {
                    m_map->m_player->inventory->changeSpeedSelect(1);
                }
                if (event.key.keysym.sym == SDLK_2) {
                    m_map->m_player->inventory->changeSpeedSelect(2);
                }
                if (event.key.keysym.sym == SDLK_3) {
                    m_map->m_player->inventory->changeSpeedSelect(3);
                }
                if (event.key.keysym.sym == SDLK_4) {
                    m_map->m_player->inventory->changeSpeedSelect(4);
                }
                if (event.key.keysym.sym == SDLK_5) {
                    m_map->m_player->inventory->changeSpeedSelect(5);
                }
                if (event.key.keysym.sym == SDLK_6) {
                    m_map->m_player->inventory->changeSpeedSelect(6);
                }
                if (event.key.keysym.sym == SDLK_7) {
                    m_map->m_player->inventory->changeSpeedSelect(7);
                }
                if (event.key.keysym.sym == SDLK_8) {
                    m_map->m_player->inventory->changeSpeedSelect(8);
                }
                if (event.key.keysym.sym == SDLK_9) {
                    m_map->m_player->inventory->changeSpeedSelect(9);
                }
                if (event.key.keysym.sym == SDLK_0) {
                    m_map->m_player->inventory->changeSpeedSelect(10);
                }
            }
            if (left_buttom_click) {
                float damage = m_map->m_player->m_damage;
                if (m_map->m_player->inventory->m_inHand) {
                    damage = m_map->m_player->inventory->m_inHand->m_damage;
                }
                if (m_map->m_select) {
                    damage -= m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first]->m_armor;
                    if (damage > 0) {
                        m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first]->m_health -=
                                damage * currentlyTime / 1000;
                    }
                    if (m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first]->m_health <= 0) {
                        uint8_t oldId = m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first]->m_id;
                        uint8_t bg = m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first]->m_BG_id;
                        delete m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first];
                        m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first] = m_map->m_maker->getBuildTail( bg);
                        m_map->m_player->inventory->add(oldId);
                        if (m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first]->m_id == 8) {
                            if (m_map->m_map[m_map->m_selectedTail.second + 1][m_map->m_selectedTail.first]->m_id ==
                                1) {
                                m_map->m_map[m_map->m_selectedTail.second +
                                             1][m_map->m_selectedTail.first]->setUnderSky(true);
                            }
                        }
                    }
                } else {
                    uint8_t dir;
                    int x = static_cast<int>(m_map->m_player->m_position.x);
                    int y = static_cast<int>(m_map->m_player->m_position.y);
                    if (x_coordinate < x) {
                        if (y_coordinate < y) {
                            dir = 11;
                        } else if (y_coordinate > y + 50) {
                            dir = 7;
                        } else {
                            dir = 9;
                        }
                    } else if (x_coordinate > x + 27) {
                        if (y_coordinate < y) {
                            dir = 1;
                        } else if (y_coordinate > y + 50) {
                            dir = 5;
                        } else {
                            dir = 3;
                        }
                    } else {
                        if (y_coordinate > y + 25) {
                            dir = 6;
                        } else {
                            dir = 0;
                        }
                    }
                    m_map->damageWave(x + 14, y + 25, dir, damage);
                }
            }


            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (m_map->m_selectedTail.first) {
                        auto item = m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first];
                        if (m_map->m_select) {
                            if (item->m_id == 10) {
                                if (dynamic_cast<CBuildChest *>(item)) {
                                    auto tmp = m_map->m_player->inventory->chestInventory(
                                            dynamic_cast<CBuildChest *>(item));
                                    if (tmp == EAction::EXIT) {
                                        return EGameAction::FINISH;
                                    }
                                    time = SDL_GetTicks();
                                    left_buttom_click = false;
                                }
                            }
                            if (item->m_id == 9) {
                                auto tmp = m_map->m_player->inventory->workbenchInventory();
                                if (tmp == EAction::EXIT) {
                                    return EGameAction::FINISH;
                                }
                                time = SDL_GetTicks();
                                left_buttom_click = false;
                            }
                        } else {
                            if (m_map->m_player->inventory->m_inHand) {
                                if (m_map->m_player->inventory->m_inHand->m_forMap) {
                                    uint8_t bg = item->m_BG_id;
                                    delete item;
                                    m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first] =
                                            m_map->m_maker->getBuildTailFromInventory(m_map->m_player->inventory->m_inHand->m_id, bg);
                                    m_map->m_player->inventory->del();
                                    if (m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first]->m_id == 1
                                        && m_map->m_map[m_map->m_selectedTail.second - 1][m_map->m_selectedTail.first]->m_id == 8) {
                                        m_map->m_map[m_map->m_selectedTail.second][m_map->m_selectedTail.first]->setUnderSky(
                                                true);
                                    }
                                    if (m_map->m_map[m_map->m_selectedTail.second + 1][m_map->m_selectedTail.first]->m_id == 1) {
                                        m_map->m_map[m_map->m_selectedTail.second + 1][m_map->m_selectedTail.first]->setUnderSky(
                                                false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (event.type == SDL_MOUSEWHEEL) {
                if (event.wheel.y > 0) {
                    m_map->m_player->inventory->changeSpeedSelect(0);
                }
                if (event.wheel.y < 0) {
                    m_map->m_player->inventory->changeSpeedSelect(11);
                }
            }
            if (!m_map->m_player->m_isAlive) {
                SDL_PushEvent(&event);
                return EGameAction::RESTART;
            }

        }
        SDL_Event event1;
        event1.type = SDL_KEYUP;
        SDL_PushEvent(&event1);
    }
}
//======================================================================================================================

//======================class CPageController===========================================================================
CPageController::CPageController () = default;

//----------------------------------------------------------------------------------------------------------------------
CPageController::~CPageController () = default;

//----------------------------------------------------------------------------------------------------------------------
/**
 * Controls the menu and start the game.
 */
void CPageController::run () {
    CSDLInitializer *cSDLInit = new CSDLInitializer();
    if (!cSDLInit->isGood()) {
        cSDLInit->destroy();
        delete cSDLInit;
        throw CSDLInitException("");
    }
    EAction action;
    CMap *map = nullptr;
    EAction prevAction = EAction::MAIN_PAGE;
    CPageInterface *page = new CMainPage(cSDLInit->getRenderer());
    bool exit = false;
    while (!exit) {
        action = page->run();
        delete page;
        switch (action) {
            case EAction::MAIN_PAGE : {
                prevAction = action;
                page = new CMainPage(cSDLInit->getRenderer());
                break;
            }
            case EAction::NEW_GAME_PAGE : {
                prevAction = action;
                page = new CNewGamePage(cSDLInit->getRenderer());
                break;
            }
            case EAction::LOADING_GAME_PAGE : {
                prevAction = action;
                page = new CLoadingGamePages(cSDLInit->getRenderer(), &map);
                break;
            }
            case EAction::CONTINUE_PAGE : {
                prevAction = action;
                try {
                    page = new CContinuePage(cSDLInit->getRenderer());
                } catch (const CException &exception) {
                    std::cout << exception.what();
                    delete page;
                    page = new CMainPage(cSDLInit->getRenderer());
                }
                break;
            }
            case EAction::SETTINGS_PAGE : {
                prevAction = action;
                page = new CSettingsPage(cSDLInit->getRenderer());
                break;
            }
            case EAction::KEYS_PAGE : {
                prevAction = action;
                page = new CKeysPage(cSDLInit->getRenderer());
                break;
            }
            case EAction::EXIT : {
                page = new CExitPage(cSDLInit->getRenderer());
                break;
            }
            case EAction::QUIT : {
                exit = true;
                break;
            }
            case EAction::CONTINUE : {
                switch (prevAction) {
                    case EAction::SETTINGS_PAGE : {
                        page = new CSettingsPage(cSDLInit->getRenderer());
                        break;
                    }
                    case EAction::KEYS_PAGE : {
                        prevAction = action;
                        page = new CKeysPage(cSDLInit->getRenderer());
                        break;
                    }
                    case EAction::CONTINUE_PAGE : {
                        try {
                            page = new CContinuePage(cSDLInit->getRenderer());
                        } catch (const CException &exception) {
                            std::cout << exception.what();
                            delete page;
                            page = new CMainPage(cSDLInit->getRenderer());
                        }
                        break;
                    }
                    default: {
                        page = new CMainPage(cSDLInit->getRenderer());
                        break;
                    }
                }
                break;
            }
            case EAction::START_GAME : {
                if (map) {
                    CGameController *controller = new CGameController(map);
                    auto gameAction = controller->run();
                    delete controller;
                    map = nullptr;
                    switch (gameAction) {
                        case EGameAction::RESTART :
                            page = new CRespawnPage (cSDLInit->getRenderer());
                            break;
                        case EGameAction::FINISH :
                            page = new CMainPage(cSDLInit->getRenderer());
                            break;
                    }
                } else {
                    page = new CMainPage(cSDLInit->getRenderer());
                }
                break;
            }
            default: {
                prevAction = action;
                exit = true;
            }
        }
    }

    cSDLInit->destroy();
    delete cSDLInit;
    if (prevAction == EAction::ERROR)
        throw CPageException("");
}
//======================================================================================================================