#include <iostream>
#include <exception>
#include <memory>

#include "view.hpp"
#include "game_model.hpp"
#include "game_controller.hpp"
#include "cell.hpp"
#include "creature.hpp"

int main() {
    using namespace game_field;
    using namespace game_field_area;
    using namespace game_model;
    using namespace game_controller;
    using namespace user_input;
    using namespace factory;
    using namespace view;
    using namespace player;
    using namespace game_event;
    
    using GameFieldExcludedCells = 
        GameFieldExcludedCells<cell::Cell<creature::Creature>>;
    using GameFieldExcludedCellsArea =
        GameFieldExcludedCellsArea<cell::Cell<creature::Creature>>;
    using GameFieldExcludedCellsAreaFactory =
        GameFieldExcludedCellsAreaFactory<cell::Cell<creature::Creature>>;
    using Player = Player<creature::Creature, cell::Cell>;
    using GameController = GameController<creature::Creature, cell::Cell>;

    // game config //
    ///////////////////////////
    const int fieldWidth = 50;
    const int fieldHeight = 50;
    const int K = 10, N = 10, T = 10;
    ///////////////////////////

    // view config //
    ///////////////////////////
    const float gridThickness = 2;             
    const float gridCellWidth = 20;
    const float gridCellHieght = 20;
    const float frameThickness = 5;
    ///////////////////////////


    auto field = std::make_shared<GameFieldExcludedCells >(
                    fieldWidth, fieldHeight, 
                    cell::Cell(creature::Creature()), 
                    std::vector<std::pair<int, int>>()
                );

    float gridWidth = gridCellWidth * field->width() 
                      + gridThickness * (field->width() + 1);
    float gridHeight = gridCellHieght * field->height()
                       + gridThickness * (field->height() + 1);
    auto grid = std::make_shared<DrawableGridCanvas>(
        gridWidth, gridHeight, 
        field->width(), field->height(), 
        gridThickness);

    float frameWidth = gridWidth + frameThickness * 2;
    float frameHeight = gridHeight + frameThickness * 2;
    auto frame = std::make_shared<DrawableFrame>(
        frameWidth, frameHeight, 
        frameThickness, sf::Color::White);

    auto nestedL = 
        std::make_shared<DrawableNestedLayout>(frameThickness, frameThickness);
    nestedL->addComponent(frame, "frame");
    nestedL->addComponent(grid, "grid_canvas");

    auto text = std::make_shared<DrawableText>(
        "dummy", 40, "calibri.ttf", sf::Color::Black, sf::Vector2f(250, 0.f));

    auto stackL = std::make_shared<DrawableStackLayout>();
    stackL->addComponent(text, "text");
    stackL->addComponent(nestedL, "nestedL");
    
    std::pair<unsigned, unsigned> windowSz = stackL->size();
    auto window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode({windowSz.first, windowSz.second}), "Fun Of The Gods");
    
    float startX = frameThickness;
    float startY = frameThickness;
    auto input = std::make_shared<UserInput>(
        window, startX, 
        startY, gridCellWidth, 
        gridCellHieght, gridThickness);


    std::pair<int, int> ul = {0, 0};
    std::pair<int, int> lr = {field->width() - 1, 
                                    field->height() - 1};
        
    auto modelArea = 
        std::make_unique<GameFieldExcludedCellsArea>(field, ul, lr);
    modelArea->unlock();
    auto model = std::make_unique<GameModel>(std::move(modelArea), 2);

    std::vector<std::shared_ptr<Player>> players(2);
    for (auto& p : players) {
        p.reset(new Player());
    }

    auto areaFactory = 
        std::make_unique<GameFieldExcludedCellsAreaFactory>();

    std::unordered_map<int, sf::Color> crColors 
        { {0, sf::Color::Red}, {1, sf::Color::Blue} };

    auto controller = std::make_shared<GameController>(
        K, T, N,
        field, std::move(areaFactory),
        std::move(model),  stackL, 
        players, input,
        window, crColors
    );

    // configuring the subject-observer interaction //
    /////////////////////////////////////////////////////////////////
    field->attach(controller, 
        static_cast<int>(event_t::GAME_FIELD_UPDATE));
    input->attach(controller, 
        static_cast<int>(event_t::USER_ASKED_RESTART));
    input->attach(controller, 
        static_cast<int>(event_t::USER_ASKED_CLOSE));
    for (auto&& p : players) {
        input->attach(p, 
            static_cast<int>(event_t::USER_ASKED_SET_CREATURE));
        p->attach(controller, 
            static_cast<int>(event_t::PLAYER_KILL_CREATURE));
    }
    /////////////////////////////////////////////////////////////////

    controller->game();

    window->close();

    return 0;
}
// } catch (const std::exception& e) {
//     std::cerr << e.what() << '\n';
// #ifdef DEBUG
//      throw;
// #endif
//     return 1;
// }