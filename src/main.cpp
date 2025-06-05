#ifndef TEST
#include <iostream>
#include <exception>
#include <memory>
#include <format>

#include "view.hpp"
#include "game_model.hpp"
#include "game_controller.hpp"
#include "cell.hpp"
#include "creature.hpp"
#include "player.hpp"

int main() try {
    using namespace game_field;
    using namespace game_field_area;
    using namespace game_model;
    using namespace game_controller;
    using namespace user_input;
    using namespace factory;
    using namespace view;
    using namespace player;
    using namespace game_event;
    using namespace figure;
    using namespace creature_strategy;
    
    constexpr float k = 0.8f;

    // game config //
    ///////////////////////////
    const int fieldWidth = 50;
    const int fieldHeight = 50;
    const int K = 10, N = 10, T = 10;
    ///////////////////////////

    // view config //
    ///////////////////////////
    const float gridThickness = 2 * k;             
    const float gridCellWidth = 20 * k;
    const float gridCellHieght = 20 * k;
    const float frameThickness = 5 * k;
    ///////////////////////////


    // create field
    // ###########################################################################
    auto creatFactory = 
        std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    
    float w = fieldWidth;
    float h = fieldHeight;
    
    auto figure = 
#if 1
        std::make_unique<Romb>(h / 2 - 0.5, w / 2 - 0.5);
#else
        std::make_unique<DummyFigure>();
#endif

    auto field = std::make_shared<GameFieldWithFigure>(
                    fieldWidth, fieldHeight, 
                    std::move(creatFactory),
                    std::move(cellFactory),
                    std::move(figure)
                );
    // ###########################################################################


    // create and link view
    // ###########################################################################
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
        "dummy", 40 * k, "calibri.ttf", sf::Color::Black, sf::Vector2f(250 * k, 0.f));

    auto stackL = std::make_shared<DrawableStackLayout>();
    stackL->addComponent(text, "text");
    stackL->addComponent(nestedL, "nestedL");
    
    std::pair<unsigned, unsigned> windowSz = stackL->size();
    auto window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode({windowSz.first, windowSz.second}), "Fun Of The Gods");
    // ###########################################################################


    // create input
    // ###########################################################################
    float startX = frameThickness;
    float startY = frameThickness;
    auto input = std::make_shared<UserInput>(
        window, startX, 
        startY, gridCellWidth, 
        gridCellHieght, gridThickness);
    // ###########################################################################


    // create controller
    // ###########################################################################
    std::pair<int, int> ul = {0, 0};
    std::pair<int, int> lr = {field->width() - 1, 
                                    field->height() - 1};
        
    std::vector<std::shared_ptr<Player>> players(2);
    int id = 0;
    for (auto& p : players) {
        p.reset(new Player(id, std::format("Player {}", id)));
        ++id;
    }

    auto modelArea = 
        std::make_unique<GameFieldWithFigureArea>(field, ul, lr);
    modelArea->unlock();
    auto areaFactory = 
        std::make_unique<
            GameFieldWithFigureAreaCurryFactory>(field);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    auto model = std::make_shared<GameModel>(
        K, N, T, 
        std::move(modelArea), 
        std::move(areaFactory), 
        players,
        std::move(creatStrategy));

    std::unordered_map<int, sf::Color> crColors { 
        {-1, sf::Color::White}, 
        {0, sf::Color::Red}, 
        {1, sf::Color::Blue} 
    };

    auto controllerArea = 
        std::make_unique<GameFieldWithFigureArea>(field, ul, lr);
    controllerArea->unlock();
    auto controller = std::make_shared<GameController>(
        std::move(controllerArea), 
        model,  stackL, 
        input, window, 
        crColors,
        field
    );
    // ###########################################################################

    
    // configuring the subject-observer interaction //
    /////////////////////////////////////////////////////////////////

    // controller
    field->attach(controller, 
        static_cast<int>(event_t::FIELD_CLEAR));
    field->attach(controller, 
        static_cast<int>(event_t::CREATURE_REMOVE_IN_FIELD));
    field->attach(controller, 
        static_cast<int>(event_t::CREATURE_SET_IN_FIELD));
    model->attach(controller, 
        static_cast<int>(event_t::PLAYER_BETS_CREATURES));
    model->attach(controller,
        static_cast<int>(event_t::GAME_MODEL_CALCULATED_ER));
    model->attach(controller,
        static_cast<int>(event_t::WINNER_DETERMINATE));
    model->attach(controller,
        static_cast<int>(event_t::DRAW_DETERMINATE));
    model->attach(controller,
        static_cast<int>(event_t::USER_INPUT_REQUIRED));
    input->attach(controller, 
        static_cast<int>(event_t::USER_ASKED_SET_CREATURE));

    // model
    input->attach(model, 
        static_cast<int>(event_t::USER_ASKED_CLOSE));
    input->attach(model, 
        static_cast<int>(event_t::USER_ASKED_RESTART));
    field->attach(model, 
        static_cast<int>(event_t::CREATURE_REMOVE_IN_FIELD));
    field->attach(model, 
        static_cast<int>(event_t::CREATURE_SET_IN_FIELD));
    /////////////////////////////////////////////////////////////////

    controller->game();
    window->close();
    
    return 0;
} catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
}
#endif // TEST