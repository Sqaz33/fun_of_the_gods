#include <gtest/gtest.h>

#include "game_model.hpp"

namespace {
    bool eqFields(std::shared_ptr<game_field::IGameField> a, 
            std::shared_ptr<game_field::IGameField> b)
    {   
        if (a->width() != b->width() || 
            a->height() != b->height()) return false;
        for (int y = 0; y < a->height(); ++y) {
            for (int x = 0; x < a->width(); ++x) {
                if (a->hasCreatureInCell(x, y) != 
                    b->hasCreatureInCell(x, y))
                {
                    return false;
                }
                if (!a->hasCreatureInCell(x, y)) {
                    continue;
                }
                auto&& crA = a->getCreatureByCell(x, y);
                auto&& crB = b->getCreatureByCell(x, y);
                if (crA.player() != crB.player()) {
                    return false;
                }
            }
        }
        return true;
    }
}

// #################################################################################################
// GameModel field handling tests
// #################################################################################################
TEST(GameModelTest, SingleCreature) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;
    
    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"), 
        std::make_shared<player::Player>(2, "player2"),  
    };
    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto actualField 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    
    actualField->setCreatureInCell(1, 1, player[0]);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = std::make_unique<
                GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();

    auto f = 
        std::make_unique<
            factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));
    model.computeEr_();

    figure = 
        std::make_unique<figure::DummyFigure>();
    creatureFactory 
        = std::make_unique<CreatureFactory>();
    cellFactory =
        std::make_unique<CellFactory>();
    auto expectField 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, SingleNeighbor) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    
    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"), 
        std::make_shared<player::Player>(2, "player2"),  
    };
    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto actualField 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    
    actualField->setCreatureInCell(1, 1, player[0]);
    actualField->setCreatureInCell(0, 1, player[0]);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = std::make_unique<
                GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();

    auto f = 
        std::make_unique<
            factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));
    model.computeEr_();

    
    figure = 
        std::make_unique<figure::DummyFigure>();
    creatureFactory 
        = std::make_unique<CreatureFactory>();
    cellFactory =
        std::make_unique<CellFactory>();
    auto expectField 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, TwoNeighbors) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"), 
        std::make_shared<player::Player>(2, "player2"),  
    };
    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto actualField 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    
    actualField->setCreatureInCell(1, 1, player[0]);
    actualField->setCreatureInCell(2, 1, player[0]);
    actualField->setCreatureInCell(1, 2, player[0]);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = std::make_unique<
                GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();

    auto f = 
        std::make_unique<
            factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));
    model.computeEr_();


    figure = 
        std::make_unique<figure::DummyFigure>();
    creatureFactory 
        = std::make_unique<CreatureFactory>();
    cellFactory =
        std::make_unique<CellFactory>();
    auto expectField 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    expectField->setCreatureInCell(1, 1, player[0]);
    expectField->setCreatureInCell(2, 1, player[0]);
    expectField->setCreatureInCell(1, 2, player[0]);
    expectField->setCreatureInCell(2, 2, player[0]);


    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, SingleCellField) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;


    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"), 
        std::make_shared<player::Player>(2, "player2"),  
    };
    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto actualField 
        = std::make_shared<GameFieldWithFigure>(
                1, 1,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    
    actualField->setCreatureInCell(0, 0, player[0]);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {0, 0};
    auto area = std::make_unique<
                GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();
    
    auto f = 
        std::make_unique<
            factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));
    model.computeEr_();


    figure = 
        std::make_unique<figure::DummyFigure>();
    creatureFactory 
        = std::make_unique<CreatureFactory>();
    cellFactory =
        std::make_unique<CellFactory>();
    auto expectField 
        = std::make_shared<GameFieldWithFigure>(
                1, 1,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, Stable) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"), 
        std::make_shared<player::Player>(2, "player2"),  
    };
    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto actualField 
        = std::make_shared<GameFieldWithFigure>(
                4, 4,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    
    actualField->setCreatureInCell(1, 1, player[0]);
    actualField->setCreatureInCell(2, 1, player[0]);
    actualField->setCreatureInCell(1, 2, player[0]);
    actualField->setCreatureInCell(2, 2, player[0]);


    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {3, 3};
    auto area = std::make_unique<
                GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();
    
    auto f = 
        std::make_unique<
            factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));
    model.computeEr_();

    
    figure = 
        std::make_unique<figure::DummyFigure>();
    creatureFactory 
        = std::make_unique<CreatureFactory>();
    cellFactory =
        std::make_unique<CellFactory>();
    auto expectField 
        = std::make_shared<GameFieldWithFigure>(
                4, 4,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    expectField->setCreatureInCell(1, 1, player[0]);
    expectField->setCreatureInCell(2, 1, player[0]);
    expectField->setCreatureInCell(1, 2, player[0]);
    expectField->setCreatureInCell(2, 2, player[0]);

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, Glider) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"), 
        std::make_shared<player::Player>(2, "player2"),  
    };
    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto actualField 
        = std::make_shared<GameFieldWithFigure>(
                5, 5,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    
    actualField->setCreatureInCell(0, 2, player[0]);
    actualField->setCreatureInCell(1, 3, player[0]);
    actualField->setCreatureInCell(2, 1, player[0]);
    actualField->setCreatureInCell(2, 2, player[0]);
    actualField->setCreatureInCell(2, 3, player[0]);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {4, 4};
    auto area = std::make_unique<
                GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();
    
    auto f = 
        std::make_unique<
            factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));
    model.computeEr_();

    
    figure = 
        std::make_unique<figure::DummyFigure>();
    creatureFactory 
        = std::make_unique<CreatureFactory>();
    cellFactory =
        std::make_unique<CellFactory>();
    auto expectField 
        = std::make_shared<GameFieldWithFigure>(
                5, 5,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    expectField->setCreatureInCell(1, 1, player[0]);
    expectField->setCreatureInCell(2, 2, player[0]);
    expectField->setCreatureInCell(3, 2, player[0]);
    expectField->setCreatureInCell(1, 3, player[0]);
    expectField->setCreatureInCell(2, 3, player[0]);

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, Oscillator) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"), 
        std::make_shared<player::Player>(2, "player2"),  
    };
    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto actualField 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    
    actualField->setCreatureInCell(0, 1, player[0]);
    actualField->setCreatureInCell(1, 1, player[0]);
    actualField->setCreatureInCell(2, 1, player[0]);


    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = std::make_unique<
                GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();
    
    auto f = 
        std::make_unique<
            factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));
    model.computeEr_();

    
    figure = 
        std::make_unique<figure::DummyFigure>();
    creatureFactory 
        = std::make_unique<CreatureFactory>();
    cellFactory =
        std::make_unique<CellFactory>();
    auto expectField 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    expectField->setCreatureInCell(1, 0, player[0]);
    expectField->setCreatureInCell(1, 1, player[0]);
    expectField->setCreatureInCell(1, 2, player[0]);

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, Blinker2) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"),
        std::make_shared<player::Player>(2, "player2") 
    };
    
    // Initial setup (vertical blinker)
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto actualField = std::make_shared<GameFieldWithFigure>(
        5, 5,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );
    
    actualField->setCreatureInCell(2, 1, player[0]);
    actualField->setCreatureInCell(2, 2, player[0]);
    actualField->setCreatureInCell(2, 3, player[0]);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {4, 4};
    auto area = std::make_unique<GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();
    
    auto f = std::make_unique<factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy = std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));
    model.computeEr_();

    // Expected result (horizontal blinker)
    figure = std::make_unique<figure::DummyFigure>();
    creatureFactory = std::make_unique<CreatureFactory>();
    cellFactory = std::make_unique<CellFactory>();
    auto expectField = std::make_shared<GameFieldWithFigure>(
        5, 5,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    expectField->setCreatureInCell(1, 2, player[0]);
    expectField->setCreatureInCell(2, 2, player[0]);
    expectField->setCreatureInCell(3, 2, player[0]);

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, TwoPlayerCreaturesSpawnNewCreature) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"),
        std::make_shared<player::Player>(2, "player2") 
    };
    
    // Initial setup (vertical blinker)
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto actualField = std::make_shared<GameFieldWithFigure>(
        5, 5,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );
    
    actualField->setCreatureInCell(0, 0, player[0]);
    actualField->setCreatureInCell(1, 0, player[1]);
    actualField->setCreatureInCell(0, 1, player[1]);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {4, 4};
    auto area = std::make_unique<GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();
    
    auto f = std::make_unique<factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy = std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));
    model.computeEr_();

    // Expected result (horizontal blinker)
    figure = std::make_unique<figure::DummyFigure>();
    creatureFactory = std::make_unique<CreatureFactory>();
    cellFactory = std::make_unique<CellFactory>();
    auto expectField = std::make_shared<GameFieldWithFigure>(
        5, 5,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    expectField->setCreatureInCell(0, 0, player[0]);
    expectField->setCreatureInCell(1, 0, player[1]);
    expectField->setCreatureInCell(0, 1, player[1]);
    expectField->setCreatureInCell(1, 1, player[1]);

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, TwoPlayerCreaturesDisappear) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"),
        std::make_shared<player::Player>(2, "player2") 
    };
    
    // Initial setup (vertical blinker)
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto actualField = std::make_shared<GameFieldWithFigure>(
        5, 5,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );
    
    actualField->setCreatureInCell(0, 0, player[0]);
    actualField->setCreatureInCell(1, 0, player[1]);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {4, 4};
    auto area = std::make_unique<GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();
    
    auto f = std::make_unique<factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy = std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));
    model.computeEr_();

    // Expected result (horizontal blinker)
    figure = std::make_unique<figure::DummyFigure>();
    creatureFactory = std::make_unique<CreatureFactory>();
    cellFactory = std::make_unique<CellFactory>();
    auto expectField = std::make_shared<GameFieldWithFigure>(
        5, 5,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    ASSERT_TRUE(eqFields(actualField, expectField));
}



// #################################################################################################
// game field setup tests
// #################################################################################################
TEST(GameFieldSetupTest, SingleCellSize) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    auto player =
        std::make_shared<player::Player>(1, "player1");

    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto field 
        = std::make_shared<GameFieldWithFigure>(
                1, 1,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    ASSERT_EQ(field->height(), 1);
    ASSERT_EQ(field->width(), 1);
    ASSERT_FALSE(field->hasCreatureInCell(0, 0));

    field->setCreatureInCell(0, 0, player);
    ASSERT_TRUE(field->hasCreatureInCell(0, 0));
}

TEST(GameFieldSetupTest, MultiCellField) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    auto player1 = std::make_shared<player::Player>(1, "player1");
    auto player2 = std::make_shared<player::Player>(2, "player2");

    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto field = std::make_shared<GameFieldWithFigure>(
        3, 3,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    ASSERT_EQ(field->height(), 3);
    ASSERT_EQ(field->width(), 3);

    // Проверяем пустые клетки
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_FALSE(field->hasCreatureInCell(i, j));
        }
    }

    // Заполняем две клетки
    field->setCreatureInCell(0, 0, player1);
    field->setCreatureInCell(2, 2, player2);

    ASSERT_TRUE(field->hasCreatureInCell(0, 0));
    ASSERT_TRUE(field->hasCreatureInCell(2, 2));
    ASSERT_FALSE(field->hasCreatureInCell(1, 1));
}

TEST(GameFieldSetupTest, CreatureOverwrite) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    auto player1 = std::make_shared<player::Player>(1, "player1");
    auto player2 = std::make_shared<player::Player>(2, "player2");

    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto field = std::make_shared<GameFieldWithFigure>(
        2, 2,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    field->setCreatureInCell(0, 0, player1);
    ASSERT_TRUE(field->hasCreatureInCell(0, 0));

    // Перезаписываем существо
    field->setCreatureInCell(0, 0, player2);
    ASSERT_TRUE(field->hasCreatureInCell(0, 0));
}

TEST(GameFieldSetupTest, InvalidCellAccess) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    auto player = std::make_shared<player::Player>(1, "player1");
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto field = std::make_shared<GameFieldWithFigure>(
        2, 2,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    // Проверяем выход за границы
    EXPECT_THROW(field->hasCreatureInCell(-1, 0), std::out_of_range);
    EXPECT_THROW(field->hasCreatureInCell(0, -1), std::out_of_range);
    EXPECT_THROW(field->hasCreatureInCell(2, 0), std::out_of_range);
    EXPECT_THROW(field->hasCreatureInCell(0, 2), std::out_of_range);

    EXPECT_THROW(field->setCreatureInCell(-1, 0, player), std::out_of_range);
    EXPECT_THROW(field->setCreatureInCell(0, -1, player), std::out_of_range);
    EXPECT_THROW(field->setCreatureInCell(2, 0, player), std::out_of_range);
    EXPECT_THROW(field->setCreatureInCell(0, 2, player), std::out_of_range);
}

// #################################################################################################
// game model setup tests
// #################################################################################################
TEST(GameModelTest, PlayersAreaSetupCheck) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"),
        std::make_shared<player::Player>(2, "player2") 
    };
    
    // Initial setup (vertical blinker)
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto field = std::make_shared<GameFieldWithFigure>(
        4, 4,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {3, 3};
    auto area = std::make_unique<GameFieldWithFigureArea>(field, lu, rl);
    area->unlock();
    
    auto f = std::make_unique<factory::GameFieldWithFigureAreaCurryFactory>(field);
    auto creatStrategy = std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), player, std::move(creatStrategy));

    std::pair<int, int> actualPLayer1LU = {0, 0};
    std::pair<int, int> actualPLayer1RL = {1, 3};
    auto&& player1Area = player[0]->fieldArea();
    ASSERT_EQ(player1Area.upperLeftCorner(), actualPLayer1LU);
    ASSERT_EQ(player1Area.lowerRightCorner(), actualPLayer1RL);

    std::pair<int, int> actualPLayer2LU = {2, 0};
    std::pair<int, int> actualPLayer2RL = {3, 3};
    auto&& player2Area = player[1]->fieldArea();
    ASSERT_EQ(player2Area.upperLeftCorner(), actualPLayer2LU);
    ASSERT_EQ(player2Area.lowerRightCorner(), actualPLayer2RL);
}

TEST(GameModelTest, PlayersInitialOwnershipCheck) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> players { 
        std::make_shared<player::Player>(1, "player1"),
        std::make_shared<player::Player>(2, "player2") 
    };
    
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto field = std::make_shared<GameFieldWithFigure>(
        4, 4,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {3, 3};
    auto area = std::make_unique<GameFieldWithFigureArea>(field, lu, rl);
    area->unlock();
    
    auto f = std::make_unique<factory::GameFieldWithFigureAreaCurryFactory>(field);
    auto creatStrategy = std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), players, std::move(creatStrategy));

    // Проверяем, что игрокам правильно назначены их области
    ASSERT_EQ(players[0]->id(), 1);
    ASSERT_EQ(players[1]->id(), 2);
    ASSERT_EQ(players[0]->name(), "player1");
    ASSERT_EQ(players[1]->name(), "player2");
}

TEST(GameModelTest, PlayersAreaSizeCheck) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> players { 
        std::make_shared<player::Player>(1, "player1"),
        std::make_shared<player::Player>(2, "player2") 
    };
    
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto field = std::make_shared<GameFieldWithFigure>(
        6, 8,  // Увеличим размер поля для проверки
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {5, 7};
    auto area = std::make_unique<GameFieldWithFigureArea>(field, lu, rl);
    area->unlock();
    
    auto f = std::make_unique<factory::GameFieldWithFigureAreaCurryFactory>(field);
    auto creatStrategy = std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), players, std::move(creatStrategy));

    // Проверяем размеры областей игроков
    auto&& player1Area = players[0]->fieldArea();
    auto&& player2Area = players[1]->fieldArea();
    
    // Ожидаемые координаты для поля 6x8
    ASSERT_EQ(player1Area.upperLeftCorner(), std::make_pair(0, 0));
    ASSERT_EQ(player1Area.lowerRightCorner(), std::make_pair(2, 7));
    
    ASSERT_EQ(player2Area.upperLeftCorner(), std::make_pair(3, 0));
    ASSERT_EQ(player2Area.lowerRightCorner(), std::make_pair(5, 7));
}

TEST(GameModelTest, PlayersFieldAccessibility) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    std::vector<std::shared_ptr<player::Player>> players { 
        std::make_shared<player::Player>(1, "player1"),
        std::make_shared<player::Player>(2, "player2") 
    };
    
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto field = std::make_shared<GameFieldWithFigure>(
        4, 4,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {3, 3};
    auto area = std::make_unique<GameFieldWithFigureArea>(field, lu, rl);
    area->unlock();
    
    auto f = std::make_unique<factory::GameFieldWithFigureAreaCurryFactory>(field);
    auto creatStrategy = std::make_unique<ConwayCreatureStrategy>();
    GameModel model(0, 0, 0, std::move(area), std::move(f), players, std::move(creatStrategy));

    ASSERT_FALSE(players[0]->fieldArea().isCellAvailable(0, 0));
    ASSERT_FALSE(players[1]->fieldArea().isCellAvailable(0, 0));
    
    ASSERT_NE(&players[0]->fieldArea(), &players[1]->fieldArea());
}

// #################################################################################################
// player interaction tests
// #################################################################################################
TEST(PlayerInteractionsTest, SingleCreatureSet) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;
    using namespace player;

    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto field 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = 
        std::make_unique<GameFieldWithFigureArea>(field, lu, rl);
    auto player = std::make_shared<Player>(1, "p1");
    player->setFieldArea(std::move(area));
    player->fieldArea().unlock();
    player->tapOnCreature(1, 1);

    ASSERT_TRUE(field->hasCreatureInCell(1, 1));
    auto&& creat = field->getCreatureByCell(1, 1);
    ASSERT_EQ(creat.player(), player);
}

TEST(PlayerInteractionsTest, SingleCreatureSetWithLockArea) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;
    using namespace player;

    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto field 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = 
        std::make_unique<GameFieldWithFigureArea>(field, lu, rl);
    auto player = std::make_shared<Player>(1, "p1");
    player->setFieldArea(std::move(area));
    player->tapOnCreature(1, 1);

    ASSERT_TRUE(!field->hasCreatureInCell(1, 1));
}

TEST(PlayerInteractionsTest, SingleCreatureSetAndRemove) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;
    using namespace player;

    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto field 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = 
        std::make_unique<GameFieldWithFigureArea>(field, lu, rl);
    auto player = std::make_shared<Player>(1, "p1");
    player->setFieldArea(std::move(area));
    player->fieldArea().unlock();
    player->tapOnCreature(1, 1);

    ASSERT_TRUE(field->hasCreatureInCell(1, 1));
    auto&& creat = field->getCreatureByCell(1, 1);
    ASSERT_EQ(creat.player(), player);

    player->tapOnCreature(1, 1);
    ASSERT_FALSE(field->hasCreatureInCell(1, 1));
}

TEST(PlayerInteractionsTest, SingleCreatureSetAndRemoveLockArea) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;
    using namespace player;

    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto field 
        = std::make_shared<GameFieldWithFigure>(
                3, 3,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = 
        std::make_unique<GameFieldWithFigureArea>(field, lu, rl);
    auto player = std::make_shared<Player>(1, "p1");
    player->setFieldArea(std::move(area));
    player->tapOnCreature(1, 1);
    ASSERT_FALSE(field->hasCreatureInCell(1, 1));

    player->tapOnCreature(1, 1);
    ASSERT_FALSE(field->hasCreatureInCell(1, 1));
}

TEST(PlayerInteractionsTest, SinglePlayerSetCreatureOnOwnArea) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;
    using namespace player;

    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto field 
        = std::make_shared<GameFieldWithFigure>(
                4, 4,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {1, 3};
    auto area = 
        std::make_unique<GameFieldWithFigureArea>(field, lu, rl);
    auto player = std::make_shared<Player>(1, "p1");
    player->setFieldArea(std::move(area));
    player->fieldArea().unlock();
    player->tapOnCreature(1, 1);

    ASSERT_TRUE(field->hasCreatureInCell(1, 1));
    auto&& creat = field->getCreatureByCell(1, 1);
    ASSERT_EQ(creat.player(), player);
}

TEST(PlayerInteractionsTest, SinglePlayerSetCreatureOnSomeOneElseArea) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;
    using namespace player;

    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto field 
        = std::make_shared<GameFieldWithFigure>(
                4, 4,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );
    std::pair<int, int> lu {2, 0};
    std::pair<int, int> rl {3, 3};
    auto area = 
        std::make_unique<GameFieldWithFigureArea>(field, lu, rl);
    auto player = std::make_shared<Player>(1, "p1");
    player->setFieldArea(std::move(area));
    player->fieldArea().unlock();
    
    ASSERT_FALSE(field->hasCreatureInCell(1, 1));
}

TEST(PlayerInteractionsTest, TwoPlayersSetCreatureOnOwnArea) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;
    using namespace player;

    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto field 
        = std::make_shared<GameFieldWithFigure>(
                4, 4,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    std::pair<int, int> lu1 {0, 0};
    std::pair<int, int> rl1 {1, 3};
    auto area1 = 
        std::make_unique<GameFieldWithFigureArea>(field, lu1, rl1);

    std::pair<int, int> lu2 {2, 0};
    std::pair<int, int> rl2 {3, 3};
    auto area2 = 
        std::make_unique<GameFieldWithFigureArea>(field, lu2, rl2);

    
    std::vector<std::shared_ptr<Player>> players {
        std::make_shared<Player>(1, "p1"),
        std::make_shared<Player>(2, "p2")
    };

    players[0]->setFieldArea(std::move(area1));
    players[1]->setFieldArea(std::move(area2));
    players[0]->fieldArea().unlock();
    players[1]->fieldArea().unlock();

    players[0]->tapOnCreature(0, 0);
    players[1]->tapOnCreature(3, 3);
    
    ASSERT_TRUE(field->hasCreatureInCell(0, 0));
    ASSERT_TRUE(field->hasCreatureInCell(3, 3));

    auto&& creat1 = field->getCreatureByCell(0, 0);
    ASSERT_EQ(creat1.player(), players[0]);

    auto&& creat2 = field->getCreatureByCell(3, 3);
    ASSERT_EQ(creat2.player(), players[1]);
}

TEST(PlayerInteractionsTest, TwoPlayersSetCreatureOnSomeOneElseArea) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;
    using namespace player;

    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto field 
        = std::make_shared<GameFieldWithFigure>(
                4, 4,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    std::pair<int, int> lu1 {0, 0};
    std::pair<int, int> rl1 {1, 3};
    auto area1 = 
        std::make_unique<GameFieldWithFigureArea>(field, lu1, rl1);

    std::pair<int, int> lu2 {2, 0};
    std::pair<int, int> rl2 {3, 3};
    auto area2 = 
        std::make_unique<GameFieldWithFigureArea>(field, lu2, rl2);

    
    std::vector<std::shared_ptr<Player>> players {
        std::make_shared<Player>(1, "p1"),
        std::make_shared<Player>(2, "p2")
    };

    players[0]->setFieldArea(std::move(area1));
    players[1]->setFieldArea(std::move(area2));
    players[0]->fieldArea().unlock();
    players[1]->fieldArea().unlock();

    players[0]->tapOnCreature(3, 3);
    players[1]->tapOnCreature(0, 0);
    
    ASSERT_FALSE(field->hasCreatureInCell(0, 0));
    ASSERT_FALSE(field->hasCreatureInCell(3, 3));
}

TEST(PlayerInteractionsTest, TwoPlayersSetCreatureOnOwnAreaAndSomeOneElseArea) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;
    using namespace player;

    auto figure = 
        std::make_unique<figure::DummyFigure>();
    auto creatureFactory 
        = std::make_unique<CreatureFactory>();
    auto cellFactory =
        std::make_unique<CellFactory>();
    auto field 
        = std::make_shared<GameFieldWithFigure>(
                4, 4,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    std::pair<int, int> lu1 {0, 0};
    std::pair<int, int> rl1 {1, 3};
    auto area1 = 
        std::make_unique<GameFieldWithFigureArea>(field, lu1, rl1);

    std::pair<int, int> lu2 {2, 0};
    std::pair<int, int> rl2 {3, 3};
    auto area2 = 
        std::make_unique<GameFieldWithFigureArea>(field, lu2, rl2);

    
    std::vector<std::shared_ptr<Player>> players {
        std::make_shared<Player>(1, "p1"),
        std::make_shared<Player>(2, "p2")
    };

    players[0]->setFieldArea(std::move(area1));
    players[1]->setFieldArea(std::move(area2));
    players[0]->fieldArea().unlock();
    players[1]->fieldArea().unlock();

    players[0]->tapOnCreature(0, 0);
    players[0]->tapOnCreature(2, 0);
    players[1]->tapOnCreature(3, 3);
    players[1]->tapOnCreature(1, 0);
    
    ASSERT_TRUE(field->hasCreatureInCell(0, 0));
    ASSERT_TRUE(field->hasCreatureInCell(3, 3));
    ASSERT_FALSE(field->hasCreatureInCell(2, 0));
    ASSERT_FALSE(field->hasCreatureInCell(1, 0));

    auto&& creat1 = field->getCreatureByCell(0, 0);
    ASSERT_EQ(creat1.player(), players[0]);

    auto&& creat2 = field->getCreatureByCell(3, 3);
    ASSERT_EQ(creat2.player(), players[1]);
}


// #################################################################################################
// game session tests
// #################################################################################################
TEST(GameModelTest, UpdateOnDrawThenCloseGame) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    using evt_t = game_event::event_t;

    struct SessionObserver : observer::IObserver {   
        SessionObserver(std::shared_ptr<GameModel> model):
            model_(model)
        {}

        void update(int event_t) override {
            evt_.push_back(static_cast<evt_t>(event_t));
            
            switch (evt_.back()) {
                case evt_t::DRAW_DETERMINATE:
                    update_ = true;
                    break;
                case evt_t::CREATURE_REMOVE_IN_FIELD:
                    ++creatRemoveCount_;
                    break;
                default: 
                    unexpected_ = true;
            }

            model_->update(static_cast<int>(evt_t::USER_ASKED_CLOSE));
        }

        std::shared_ptr<GameModel> model_;
        std::vector<evt_t> evt_;
        bool update_ = false;
        bool unexpected_ = false;
        int creatRemoveCount_ = 0;
    };

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"),
        std::make_shared<player::Player>(2, "player2") 
    };
    
    // Initial setup (vertical blinker)
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto actualField = std::make_shared<GameFieldWithFigure>(
        4, 4,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {3, 3};
    auto area = std::make_unique<GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();
    auto f = 
        std::make_unique<
            factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    auto model= std::make_shared<GameModel>(
        0, 0, 1, std::move(area), std::move(f), player, std::move(creatStrategy));

    player[0]->fieldArea().unlock();
    player[0]->tapOnCreature(0, 0);
    player[0]->fieldArea().lock();
    
    player[1]->fieldArea().unlock();
    player[1]->tapOnCreature(2, 0);
    player[1]->fieldArea().lock();

    auto obs = std::make_shared<SessionObserver>(model);

    actualField->attach(obs, 
        static_cast<int>(evt_t::CREATURE_REMOVE_IN_FIELD));
    model->attach(obs, 
        static_cast<int>(evt_t::DRAW_DETERMINATE));

    model->game();

    ASSERT_FALSE(obs->unexpected_);
    ASSERT_TRUE(obs->update_);    
    std::vector<evt_t> expectEvents {
        evt_t::CREATURE_REMOVE_IN_FIELD,
        evt_t::CREATURE_REMOVE_IN_FIELD,
        evt_t::DRAW_DETERMINATE
    };
    ASSERT_EQ(obs->evt_, expectEvents);


    figure = 
        std::make_unique<figure::DummyFigure>();
    creatureFactory 
        = std::make_unique<CreatureFactory>();
    cellFactory =
        std::make_unique<CellFactory>();
    auto expectField 
        = std::make_shared<GameFieldWithFigure>(
                4, 4,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, UpdateOnWinFirstPlayerThenCloseGame) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    using evt_t = game_event::event_t;

    struct SessionObserver : observer::IObserver {   
        SessionObserver(std::shared_ptr<GameModel> model):
            model_(model)
        {}

        void update(int event_t) override {
            evt_.push_back(static_cast<evt_t>(event_t));
            
            switch (evt_.back()) {
                case evt_t::WINNER_DETERMINATE:
                    update_ = true;
                    break;
                case evt_t::CREATURE_REMOVE_IN_FIELD:
                    ++creatRemoveCount_;
                    break;
                default: 
                    unexpected_ = true;
            }

            model_->update(static_cast<int>(evt_t::USER_ASKED_CLOSE));
        }

        std::shared_ptr<GameModel> model_;
        std::vector<evt_t> evt_;
        bool update_ = false;
        bool unexpected_ = false;
        int creatRemoveCount_ = 0;
    };

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"),
        std::make_shared<player::Player>(2, "player2") 
    };
    
    // Initial setup (vertical blinker)
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto actualField = std::make_shared<GameFieldWithFigure>(
        4, 4,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {3, 3};
    auto area = std::make_unique<GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();
    auto f = 
        std::make_unique<
            factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    auto model= std::make_shared<GameModel>(
        0, 0, 1, std::move(area), std::move(f), player, std::move(creatStrategy));

    player[0]->fieldArea().unlock();
    player[0]->tapOnCreature(0, 0);
    player[0]->tapOnCreature(1, 0);
    player[0]->tapOnCreature(0, 1);
    player[0]->tapOnCreature(1, 1);
    player[0]->fieldArea().lock();
    
    player[1]->fieldArea().unlock();
    player[1]->tapOnCreature(3, 3);
    player[1]->fieldArea().lock();

    auto obs = std::make_shared<SessionObserver>(model);

    actualField->attach(obs, 
        static_cast<int>(evt_t::CREATURE_REMOVE_IN_FIELD));
    model->attach(obs, 
        static_cast<int>(evt_t::DRAW_DETERMINATE));
    model->attach(obs, 
        static_cast<int>(evt_t::WINNER_DETERMINATE));

    model->game();

    ASSERT_FALSE(obs->unexpected_);
    ASSERT_TRUE(obs->update_);    
    std::vector<evt_t> expectEvents {
        evt_t::CREATURE_REMOVE_IN_FIELD,
        evt_t::WINNER_DETERMINATE
    };
    ASSERT_EQ(obs->evt_, expectEvents);
    ASSERT_EQ(model->winnerPlayer(), player[0]);


    figure = 
        std::make_unique<figure::DummyFigure>();
    creatureFactory 
        = std::make_unique<CreatureFactory>();
    cellFactory =
        std::make_unique<CellFactory>();
    auto expectField 
        = std::make_shared<GameFieldWithFigure>(
                4, 4,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    expectField->setCreatureInCell(0, 0, player[0]);
    expectField->setCreatureInCell(1, 0, player[0]);
    expectField->setCreatureInCell(0, 1, player[0]);
    expectField->setCreatureInCell(1, 1, player[0]);

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, UpdateOnWinSecondPlayerThenCloseGame) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;
    using namespace creature_strategy;

    using evt_t = game_event::event_t;

    struct SessionObserver : observer::IObserver {   
        SessionObserver(std::shared_ptr<GameModel> model):
            model_(model)
        {}

        void update(int event_t) override {
            evt_.push_back(static_cast<evt_t>(event_t));
            
            switch (evt_.back()) {
                case evt_t::WINNER_DETERMINATE:
                    update_ = true;
                    break;
                case evt_t::CREATURE_REMOVE_IN_FIELD:
                    ++creatRemoveCount_;
                    break;
                default: 
                    unexpected_ = true;
            }

            model_->update(static_cast<int>(evt_t::USER_ASKED_CLOSE));
        }

        std::shared_ptr<GameModel> model_;
        std::vector<evt_t> evt_;
        bool update_ = false;
        bool unexpected_ = false;
        int creatRemoveCount_ = 0;
    };

    std::vector<std::shared_ptr<player::Player>> player { 
        std::make_shared<player::Player>(1, "player1"),
        std::make_shared<player::Player>(2, "player2") 
    };
    
    // Initial setup (vertical blinker)
    auto figure = std::make_unique<figure::DummyFigure>();
    auto creatureFactory = std::make_unique<CreatureFactory>();
    auto cellFactory = std::make_unique<CellFactory>();
    auto actualField = std::make_shared<GameFieldWithFigure>(
        4, 4,
        std::move(creatureFactory),
        std::move(cellFactory),
        std::move(figure)
    );

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {3, 3};
    auto area = std::make_unique<GameFieldWithFigureArea>(actualField, lu, rl);
    area->unlock();
    auto f = 
        std::make_unique<
            factory::GameFieldWithFigureAreaCurryFactory>(actualField);
    auto creatStrategy =
        std::make_unique<ConwayCreatureStrategy>();
    auto model= std::make_shared<GameModel>(
        0, 0, 1, std::move(area), std::move(f), player, std::move(creatStrategy));


    
    player[0]->fieldArea().unlock();
    player[0]->tapOnCreature(0, 0);
    player[0]->fieldArea().lock();

    player[1]->fieldArea().unlock();
    player[1]->tapOnCreature(2, 2);
    player[1]->tapOnCreature(3, 2);
    player[1]->tapOnCreature(2, 3);
    player[1]->tapOnCreature(3, 3);
    player[1]->fieldArea().lock();

    auto obs = std::make_shared<SessionObserver>(model);

    actualField->attach(obs, 
        static_cast<int>(evt_t::CREATURE_REMOVE_IN_FIELD));
    model->attach(obs, 
        static_cast<int>(evt_t::DRAW_DETERMINATE));
    model->attach(obs, 
        static_cast<int>(evt_t::WINNER_DETERMINATE));

    model->game();

    ASSERT_FALSE(obs->unexpected_);
    ASSERT_TRUE(obs->update_);    
    std::vector<evt_t> expectEvents {
        evt_t::CREATURE_REMOVE_IN_FIELD,
        evt_t::WINNER_DETERMINATE
    };
    ASSERT_EQ(obs->evt_, expectEvents);
    ASSERT_EQ(model->winnerPlayer(), player[1]);


    figure = 
        std::make_unique<figure::DummyFigure>();
    creatureFactory 
        = std::make_unique<CreatureFactory>();
    cellFactory =
        std::make_unique<CellFactory>();
    auto expectField 
        = std::make_shared<GameFieldWithFigure>(
                4, 4,
                std::move(creatureFactory),
                std::move(cellFactory),
                std::move(figure)
            );

    expectField->setCreatureInCell(2, 2, player[1]);
    expectField->setCreatureInCell(3, 2, player[1]);
    expectField->setCreatureInCell(2, 3, player[1]);
    expectField->setCreatureInCell(3, 3, player[1]);

    ASSERT_TRUE(eqFields(actualField, expectField));
}

int main(int argc, char* argv[]) {
    try {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }

}