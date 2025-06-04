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
    ASSERT_TRUE(creat.player() == player);
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
    ASSERT_TRUE(creat.player() == player);

    player->tapOnCreature(1, 1);
    ASSERT_TRUE(!field->hasCreatureInCell(1, 1));
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
    ASSERT_TRUE(!field->hasCreatureInCell(1, 1));

    player->tapOnCreature(1, 1);
    ASSERT_TRUE(!field->hasCreatureInCell(1, 1));
}






int main(int argc, char* argv[]) {
    try {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }

}