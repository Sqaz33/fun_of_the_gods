// #include <iostream>
// #include <exception>

// #include "view.hpp"
// #include "game_model.hpp"
// #include "game_controller.hpp"
// #include "cell.hpp"
// #include "creature.hpp"

// int main() try {
//     using namespace game_field;
//     using namespace game_field_area;
//     using namespace game_model;
//     using namespace game_controller;
//     using namespace user_input;
//     using namespace factory;
//     using namespace view;
//     using namespace player;
//     using namespace game_event;
    
//     auto field = std::make_shared<
//                 GameFieldExcludedCells<cell::Cell<creature::Creature>>>(
//                     50, 50, cell::Cell(creature::Creature()), {}
//                 );

//     std::pair<int, int> ul = {0, 0};
//     std::pair<int, int> lr = {field->width() - 1, 
//                                     field->height() - 1};
        
//     auto modelArea = 
//         std::make_unique<GameFieldExcludedCellsArea>(field, ul, lr);
//     auto model = std::make_shared<GameModel>(std::move(modelArea), 2);

//     auto grid = std::make_shared<DrawableGridCanvas>(
//         1261.f, 2452.f, 50, 50, 5.f);
//     auto frame = std::make_shared<DrawableFrame>(
//         1281.f, 2472.f, 5.f, sf::Color::White);

//     auto nestedL = std::make_shared<DrawableNestedLayout>(10.f, 10.f);
//     nestedL->addComponent(frame, "frame");
//     nestedL->addComponent(grid, "grid_canvas");

    
//     auto text = std::make_shared<DrawableText>(
//         "", 40, "calibri.ttf", sf::Color::Black, sf::Vector2f(10.f, 0.f));

//     auto stackL = std::make_shared<DrawableStackLayout>();
//     stackL->addComponent(nestedL, "nestedL");
//     stackL->addComponent(text, "text");
    
//     std::pair<unsigned, unsigned> windowSz = stackL->size();
//     auto window = std::make_shared<sf::RenderWindow>(
//             sf::VideoMode({windowSz.first, windowSz.second}), "Fun Of The Gods");

//     auto areaFactory = 
//         std::make_unique<GameFieldExcludedCellsAreaFactory>();
    
//     float startX = 10;
//     float startY = text->size().second + 10 + 10;
//     auto input = std::make_unique<UserInput>
//         window, startX, startY, 28.11f, 28.11f);

//     std::unordered_map<int, sf::Color> crColors 
//         { {1, sf::Color::Red}, {2, sf::Color::Blue} };

//     std::vector<
//         std::shared_ptr<Player<Cell<Creature>>>> players(2);

//     auto controller = std::make_shared<GameController>(
//         10, 10, 10,
//         field, std::move(areaFactory),
//         std::move(model),  stackL, 
//         players, std::move(input),
//         window, crColors
//     );

//     field->attach(controller, event_t::GAME_FIELD_UPDATE);
//     input->attach(controller, event_t::USER_ASKED_RESTART);
//     input->attach(controller, event_t::USER_ASKED_CLOSE);
//     for (auto&& p : players) {
//         input->attach(p, event_t::USER_ASKED_SET_CREATURE);
//         p->attach(controller, event_t::PLAYER_KILL_CREATURE);
//     }

//     controller->game();

//     return 0;
// } catch (const std::exception& e) {
//     std::cerr << e.what() << '\n';
//     return 1;
// }