#include "game_controller.hpp"

#include <sstream>

namespace {
    using IGameField = game_field::IGameField;
    using IGameFieldArea = game_field_area::IGameFieldArea;
    using IGameModel = game_model::IGameModel;
    using IUserInput = user_input::IUserInput;
    using IGameFieldAreaCurryFactory = 
        factory::IGameFieldAreaCurryFactory;
    using IGameFieldArea = game_field_area::IGameFieldArea;
    using DrawableGridCanvas = view::DrawableGridCanvas;

} // namespace

namespace game_controller {

GameController::GameController(
        std::unique_ptr<IGameFieldArea> area,
        std::shared_ptr<IGameModel> model,
        std::shared_ptr<view::IDrawableComposite> view,
        std::shared_ptr<IUserInput> input,
        std::shared_ptr<sf::RenderWindow> window,
        const std::unordered_map<int, sf::Color>& playersCreatureColors,
        const std::shared_ptr<IGameField> field):
    area_(std::move(area))
    , model_(model)
    , view_(view)
    , input_(input)
    , window_(window)
    , playersCreatureColors_(playersCreatureColors)
    , field_(field)
{ drawCanvasBackground_(); }

void GameController::update(int event_t)
{   
    using evt_t = game_event::event_t;
    auto evt = static_cast<evt_t>(event_t);
    switch (evt) {
        case evt_t::FIELD_CLEAR: {
            clearGridCanvas_();
            redrawWindowNDisplay_();
            break;
        }
        case evt_t::CELL_CLEAR_IN_FIELD: {
            auto [x, y] = field_->lastAffectedCell();
            updateCellInGridCanvasInView_(x, y);
            redrawWindowNDisplay_();
            break;
        }
        case evt_t::CREATURE_KILL_IN_FIELD: {
            auto [x, y] = field_->lastAffectedCell();
            updateCellInGridCanvasInView_(x, y);
            if (gameModelSetupPhase_) {
                redrawWindowNDisplay_();
            }
            break;
        }
        case evt_t::CREATURE_REVIVE_IN_FIELD: {
            auto [x, y] = field_->lastAffectedCell();
            updateCellInGridCanvasInView_(x, y);
            if (gameModelSetupPhase_) {
                redrawWindowNDisplay_();
            }
            break;
        }
        case evt_t::PLAYER_BETS_CREATURES: {
            auto id = model_->curPlayer();
            auto movesCount = model_->movesRemained();
            auto p = model_->curPlayer();
            notifyAboutPlayerParticipation_(p->name(), movesCount);
            gameModelSetupPhase_ = true;
            break;
        }
        case evt_t::GAME_MODEL_CALCULATED_ER: {
            auto erRem = model_->erRemained();
            notifyAboutModelComputing_(erRem);
            gameModelSetupPhase_ = false;
            break;
        }
        case evt_t::WINNER_DETERMINATE: {
            auto p = model_->winnerPlayer();
            notifyAboutWinner_(p->name());
            break;
        }
        case evt_t::DRAW_DETERMINATE: {
            notifyAboutDraw_();
            break;
        }
        case evt_t::USER_INPUT_REQUIRED: {
            input_->readInput();
            break;
        }
        case evt_t::USER_ASKED_SET_CREATURE: {
            auto [suc, x, y] = input_->lastCoordInput();
            if (suc) {
                auto p = model_->curPlayer();
                p->setCreature(x, y);
            }
        }
    }
}

void GameController::game() {
    model_->game();
}       

void GameController::redrawWindowNDisplay_() {
    std::pair<unsigned, unsigned> windowSz = view_->size();
    window_->setSize({windowSz.first, windowSz.second});
    window_->clear(sf::Color::White);
    view_->draw(*window_, {0.f, 0.f});
    window_->display();
}

std::shared_ptr<DrawableGridCanvas> 
GameController::getCanvasComp_() 
{
    auto comp = view_->getComponent("grid_canvas");
    if (!comp) {
        throw std::logic_error(
            "The view is missing the grid component");
    }
    if (auto grid = 
        std::dynamic_pointer_cast<view::DrawableGridCanvas>(comp))
    {   
        return grid;
    } else {
        throw std::logic_error(
            "The view is missing the DrawableGridCanvas component");
    }
}

void GameController::clearGridCanvas_() {
    getCanvasComp_()->clear();
    drawCanvasBackground_();
}

void GameController::updateCellInGridCanvasInView_(int xidx, int yidx) {
    auto grid = getCanvasComp_();
    sf::Color color;
    if (!area_->isCellAvailable(xidx, yidx)) {
        color = sf::Color::Black;
    } else {
        if (area_->hasCreatureInCell(xidx, yidx)) {
            auto&& cr = area_->getCreatureByCell(xidx, yidx);
            color = playersCreatureColors_.at(cr.player()->id());
        } else {
            color = sf::Color::White;
        }
    }
    grid->paintCell({xidx, yidx}, color);
} 

void GameController::notifyAboutWinner_(const std::string& name) {
    std::stringstream ss;
    ss << "Won: ";
    ss << name;
    ss << '!';
    setTextOnTextComp_(ss.str());
}

void GameController::notifyAboutDraw_() {
    setTextOnTextComp_("Draw!");
}

void GameController::notifyAboutPlayerParticipation_(
    const std::string& name, int moveRemained) {
    std::stringstream ss;
    ss << "Player: ";
    ss << name;
    ss << ". Remaining creatures: ";
    ss << moveRemained;
    ss << '.';
    setTextOnTextComp_(ss.str());
}

void GameController::notifyAboutModelComputing_(int n) {
    std::stringstream ss;
    ss << "Remaining er: ";
    ss << n;
    ss << '.';
    setTextOnTextComp_(ss.str());
}

void GameController::setTextOnTextComp_(const std::string& txt) {
    auto comp = view_->getComponent("text");
    if (!comp) {
        throw std::logic_error(
            "The view is missing the text component");
    }
    if (auto text = 
        std::dynamic_pointer_cast<view::DrawableText>(comp))
    {   
        text->setText(txt);
        redrawWindowNDisplay_();
    } else {
        throw std::logic_error(
            "The view is missing the DrawableText component");
    }
}

void GameController::drawCanvasBackground_() {
    auto grid = getCanvasComp_();
    for (int y = 0; y < area_->height(); ++y) {
        for (int x = 0; x < area_->width(); ++x) {
            sf::Color color;
            if (area_->isCellAvailable(x, y)) {
                color = sf::Color::White;
            } else {
                color = sf::Color::Black;
            }
            grid->paintCell({x, y}, color);
        }
    }
}


} // namespace game_controller
