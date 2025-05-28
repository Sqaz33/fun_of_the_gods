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
        const std::unordered_map<int, sf::Color>& creatureColors):
    area_(std::move(area))
    , model_(model)
    , view_(view)
    , input_(input)
    , window_(window)
    , creatureColors_(creatureColors)
{}

void GameController::update(
    std::weak_ptr<subject::ISubject> subj, int event_t)
{   
    using evt_t = game_event::event_t;
    auto evt = static_cast<evt_t>(event_t);
    auto lk = subj.lock();
    auto field = 
        std::dynamic_pointer_cast<game_field::IGameField>(lk);
    switch (evt) {
        case evt_t::FIELD_CLEAR: {
            clearGridCanvas_();
            redrawWindowNDisplay_();
            break;
        }
        case evt_t::CELL_CLEAR_IN_FIELD: {
            auto [x, y] = field->lastAffectedCell();
            updateCellInGridCanvasInView_(x, y);
            redrawWindowNDisplay_();
            break;
        }
        case evt_t::CREATURE_KILL_IN_FIELD: {
            auto [x, y] = field->lastAffectedCell();
            updateCellInGridCanvasInView_(x, y);
            if (gameModelSetupPhase_) {
                redrawWindowNDisplay_();
            }
            break;
        }
        case evt_t::CREATURE_REVIVE_IN_FIELD: {
            auto [x, y] = field->lastAffectedCell();
            updateCellInGridCanvasInView_(x, y);
            if (gameModelSetupPhase_) {
                redrawWindowNDisplay_();
            }
            break;
        }
        case evt_t::PLAYER_BETS_CREATURES: {
            auto id = model_->curPlayer();
            auto movesCount = model_->movesRemained();
            notifyAboutPlayerParticipation_(id, movesCount);
            gameModelSetupPhase_ = true;
            break;
        }
        case evt_t::GAME_MOEL_CALCULATED_ER: {
            auto erRem = model_->erRemained();
            notifyAboutModelComputing_(erRem);
            gameModelSetupPhase_ = false;
            break;
        }
        case evt_t::WINNER_DETERMINATE: {
            auto id = model_->winnerPlayer();
            notifyAboutWinner_(id);
            break;
        }
        case evt_t::DRAW_DETERMINATE: {
            noitfyAboutDraw_();
            break;
        }
        case evt_t::USER_INPUT_REQUIRED: {
            input_->readInput();
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
}

void GameController::updateCellInGridCanvasInView_(int xidx, int yidx) {
    auto grid = getCanvasComp_();
    sf::Color color;
    if (!area_->isCellAvailable(xidx, yidx)) {
        color = sf::Color::Black;
    } else {
        auto&& cr = area_->getCreatureByCell(xidx, yidx);
        if (cr.isAlive()) {
            color = creatureColors_.at(cr.id());
        } else {
            color = creatureColors_.at(creature::idDead);
        }
    }
    grid->paintCell({xidx, yidx}, color);
} 

void GameController::notifyAboutWinner_(int player) {
    std::stringstream ss;
    ss << "Won: ";
    ss << player;
    ss << '!';
    setTextOnTextComp_(ss.str());
}

void GameController::noitfyAboutDraw_() {
    setTextOnTextComp_("Draw!");
}

void GameController::notifyAboutPlayerParticipation_(int player, int n) {
    std::stringstream ss;
    ss << "Player: ";
    ss << player;
    ss << ". Remaining creatures: ";
    ss << n;
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

} // namespace game_controller
