#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

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
    using IGameFieldAreaCurryFactory = factory::IGameFieldAreaCurryFactory;
    using DrawableGridCanvas = view::DrawableGridCanvas;

public:
    GameController(
        std::unique_ptr<IGameFieldArea> area,
        std::shared_ptr<IGameModel> model,
        std::shared_ptr<view::IDrawableComposite> view,
        std::shared_ptr<IUserInput> input,
        std::shared_ptr<sf::RenderWindow> window,
        const std::unordered_map<int, sf::Color>& playersCreatureColors,
        const std::shared_ptr<IGameField> field);

    void update(int event_t) override;
    void game();

private:
    void redrawWindowNDisplay_();
    void updateCellInGridCanvasInView_(int xidx, int yidx);
    std::shared_ptr<DrawableGridCanvas> getCanvasComp_();
    void clearGridCanvas_();
    void restartController_();
    void notifyAboutWinner_(const std::string& name);
    void notifyAboutDraw_();
    void notifyAboutPlayerParticipation_(
        const std::string& name, int moveRemained);
    void notifyAboutModelComputing_(int erRemained);
    void setTextOnTextComp_(const std::string& txt);
    void drawCanvasBackground_();

private:
    std::unique_ptr<IGameFieldArea> area_;          
    std::shared_ptr<IGameModel> model_;             
    std::shared_ptr<view::IDrawableComposite> view_;
    std::shared_ptr<IUserInput> input_;             
    std::shared_ptr<sf::RenderWindow> window_;      
    const std::shared_ptr<IGameField> field_;
    std::unordered_map<int, sf::Color> playersCreatureColors_; 
    bool gameModelSetupPhase_ = false; 
};

} // namespace game_controller

#endif // GAME_CONTROLLER_HPP