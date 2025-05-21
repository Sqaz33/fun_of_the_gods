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
                auto&& crA = a->getCreatureByCell(x, y);
                auto&& crB = b->getCreatureByCell(x, y);
                if (crB.isAlive() != crB.isAlive() || 
                    crA.id() != crB.id()) return false;
            }
        }
        return true;
    }
}

TEST(GameModelTest, SingleCreature) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;

    auto creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto actualField 
        = std::make_shared<GameFieldExcludedCells>(
                3, 3,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );
    
    actualField->reviveCreatureInCell(1, 1, 1);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = std::make_unique<
                GameFieldExcludedCellsArea>(actualField, lu, rl);
    area->unlock();
    GameModel model(std::move(area), 1);
    auto [con, id] = model.compute();
    ASSERT_EQ(con, false);
    
    creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto expectField 
        = std::make_shared<GameFieldExcludedCells>(
                3, 3,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, SingleNeighbor) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;

    auto creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto actualField 
        = std::make_shared<GameFieldExcludedCells>(
                3, 3,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );
    
    actualField->reviveCreatureInCell(1, 1, 1);
    actualField->reviveCreatureInCell(0, 1, 1);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = std::make_unique<
                GameFieldExcludedCellsArea>(actualField, lu, rl);
    area->unlock();
    GameModel model(std::move(area), 1);
    auto [con, id] = model.compute();
    ASSERT_EQ(con, false);
    
    creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto expectField 
        = std::make_shared<GameFieldExcludedCells>(
                3, 3,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, TwoNeighbors) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;

    auto creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto actualField 
        = std::make_shared<GameFieldExcludedCells>(
                3, 3,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );
    
    actualField->reviveCreatureInCell(1, 1, 1);
    actualField->reviveCreatureInCell(2, 1, 1);
    actualField->reviveCreatureInCell(1, 2, 1);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = std::make_unique<
                GameFieldExcludedCellsArea>(actualField, lu, rl);
    area->unlock();
    GameModel model(std::move(area), 1);
    auto [con, id] = model.compute();
    ASSERT_EQ(con, false);

    creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto expectField 
        = std::make_shared<GameFieldExcludedCells>(
                3, 3,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );

    expectField->reviveCreatureInCell(1, 1, 1);
    expectField->reviveCreatureInCell(2, 1, 1);
    expectField->reviveCreatureInCell(1, 2, 1);
    expectField->reviveCreatureInCell(2, 2, 1);


    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, SingleCellField) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;

    auto creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto actualField 
        = std::make_shared<GameFieldExcludedCells>(
                1, 1,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );
    
    actualField->reviveCreatureInCell(0, 0, 1);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {0, 0};
    auto area = std::make_unique<
                GameFieldExcludedCellsArea>(actualField, lu, rl);
    area->unlock();
    GameModel model(std::move(area), 1);
    auto [con, id] = model.compute();
    ASSERT_EQ(con, false);
    
    creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto expectField 
        = std::make_shared<GameFieldExcludedCells>(
                1, 1,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, Stable) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;

    auto creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto actualField 
        = std::make_shared<GameFieldExcludedCells>(
                4, 4,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );
    
    actualField->reviveCreatureInCell(1, 1, 1);
    actualField->reviveCreatureInCell(2, 1, 1);
    actualField->reviveCreatureInCell(1, 2, 1);
    actualField->reviveCreatureInCell(2, 2, 1);


    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {3, 3};
    auto area = std::make_unique<
                GameFieldExcludedCellsArea>(actualField, lu, rl);
    area->unlock();
    GameModel model(std::move(area), 1);
    auto [con, id] = model.compute();
    ASSERT_EQ(con, false);
    
    creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto expectField 
        = std::make_shared<GameFieldExcludedCells>(
                4, 4,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );

    expectField->reviveCreatureInCell(1, 1, 1);
    expectField->reviveCreatureInCell(2, 1, 1);
    expectField->reviveCreatureInCell(1, 2, 1);
    expectField->reviveCreatureInCell(2, 2, 1);

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, Glider) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;

    auto creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto actualField 
        = std::make_shared<GameFieldExcludedCells>(
                5, 5,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );
    
    actualField->reviveCreatureInCell(0, 2, 1);
    actualField->reviveCreatureInCell(1, 3, 1);
    actualField->reviveCreatureInCell(2, 1, 1);
    actualField->reviveCreatureInCell(2, 2, 1);
    actualField->reviveCreatureInCell(2, 3, 1);

    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {4, 4};
    auto area = std::make_unique<
                GameFieldExcludedCellsArea>(actualField, lu, rl);
    area->unlock();
    GameModel model(std::move(area), 1);
    auto [con, id] = model.compute();
    ASSERT_EQ(con, false);
    
    creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto expectField 
        = std::make_shared<GameFieldExcludedCells>(
                5, 5,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );

    expectField->reviveCreatureInCell(1, 1, 1);
    expectField->reviveCreatureInCell(2, 2, 1);
    expectField->reviveCreatureInCell(3, 2, 1);
    expectField->reviveCreatureInCell(1, 3, 1);
    expectField->reviveCreatureInCell(2, 3, 1);

    ASSERT_TRUE(eqFields(actualField, expectField));
}

TEST(GameModelTest, Oscillator) {
    using namespace game_field;
    using namespace game_field_area;
    using namespace factory;
    using namespace game_model;

    auto creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto actualField 
        = std::make_shared<GameFieldExcludedCells>(
                3, 3,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );
    
    actualField->reviveCreatureInCell(0, 1, 1);
    actualField->reviveCreatureInCell(1, 1, 1);
    actualField->reviveCreatureInCell(2, 1, 1);


    std::pair<int, int> lu {0, 0};
    std::pair<int, int> rl {2, 2};
    auto area = std::make_unique<
                GameFieldExcludedCellsArea>(actualField, lu, rl);
    area->unlock();

    GameModel model(std::move(area), 1);
    auto [con, id] = model.compute();
    ASSERT_EQ(con, false);
    
    creatureFabric 
        = std::make_unique<CreatureFactory>();
    auto expectField 
        = std::make_shared<GameFieldExcludedCells>(
                3, 3,
                std::vector<std::pair<int, int>>(),
                std::move(creatureFabric)
            );

    expectField->reviveCreatureInCell(1, 0, 1);
    expectField->reviveCreatureInCell(1, 1, 1);
    expectField->reviveCreatureInCell(1, 2, 1);

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