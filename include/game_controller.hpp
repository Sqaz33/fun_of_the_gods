#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <chrono>
#include <thread>
#include <sstream>
#include <unordered_map>

#include "game_field_area_factory.hpp"
#include "player.hpp"
#include "user_input.hpp"
#include "view.hpp"
#include "game_model.hpp"

namespace game_controller {

class GameController : public observer::IObserver {
    using IGameField = game_field::IGameField;
    using IGameFieldArea = game_field_area::IGameFieldArea;
    using IGameModel = game_model::IGameModel;
    using IUserInput = user_input::IUserInput;
    using IGameFieldAreaFactory = 
        factory::IGameFieldAreaFactory;
    using IGameFieldArea = game_field_area::IGameFieldArea;

public:
    GameController(
            int K,
            int T,
            int N,
            std::unique_ptr<IGameFieldArea> area,
            std::unique_ptr<IGameFieldAreaFactory> areaFactory,
            std::unique_ptr<IGameModel> model,
            std::shared_ptr<view::IDrawableComposite> view,
            const std::vector<std::shared_ptr<player::Player>>& players,
            std::shared_ptr<IUserInput> input,
            std::shared_ptr<sf::RenderWindow> window,
            const std::unordered_map<int, sf::Color>& creatureColors);

public:
    void update(
        std::weak_ptr<subject::ISubject> subj, int event_t) override;

public:
    void game();

private:
    void redrawWindowNDisplay_();
    void updateGridCanvasInView_();
    void giveAreasForTwoPlayers_();
    void giveCreaturePlayers_();
    void setupField_(int N);
    void setupFieldForPlayer_(int N, int player);
    void computeModel_(int T);
    void restartController_();
    void notifyAboutWinner_(int player);
    void noitfyAboutDraw_();
    void notifyAboutPlayerParticipation_(int player, int n);
    void notifyAboutModelComputing_(int n);
    void setTextOnTextComp_(const std::string& txt);

private:
    std::unique_ptr<IGameFieldArea> area_;
    std::unique_ptr<IGameFieldAreaFactory> areaFactory_;
    std::unique_ptr<IGameModel> model_;
    std::shared_ptr<view::IDrawableComposite> view_;
    std::vector<std::shared_ptr<player::Player>> players_;
    std::shared_ptr<IUserInput> input_;
    std::shared_ptr<sf::RenderWindow> window_;
    std::unordered_map<int, sf::Color> creatureColors_;
    int K_;
    int T_;
    int N_;
    int curN_;
    bool winnerDeterminate_ = false;
    bool askedRestart_ = false;
    bool askedClose_ = false;
    bool setupPhase_ = false;
};

} // namespace game_controller

#endif // GAME_CONTROLLER_HPP