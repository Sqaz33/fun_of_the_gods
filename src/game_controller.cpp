#include "game_controller.hpp"

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
        int K,
        int T,
        int N,
        std::unique_ptr<IGameFieldArea> area,
        std::unique_ptr<IGameFieldAreaCurryFactory> areaFactory,
        std::unique_ptr<IGameModel> model,
        std::shared_ptr<view::IDrawableComposite> view,
        const std::vector<std::shared_ptr<player::Player>>& players,
        std::shared_ptr<IUserInput> input,
        std::shared_ptr<sf::RenderWindow> window,
        const std::unordered_map<int, sf::Color>& creatureColors):
    K_(K)
    , T_(T)
    , N_(N)
    , area_(std::move(area))
    , areaFactory_(std::move(areaFactory))
    , model_(std::move(model))
    , view_(view)
    , players_(players)
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
        case evt_t::USER_ASKED_CLOSE: {
            askedClose_ = true;
            break;
        } 
        case evt_t::USER_ASKED_RESTART: {
            askedRestart_ = true;
            restartController_();
            break;
        }
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
            if (setupPhase_) {
                ++curN_;
            }   
            break;
        } 
        case evt_t::CREATURE_REVIVE_IN_FIELD: {   
            auto [x, y] = field->lastAffectedCell(); 
            updateCellInGridCanvasInView_(x, y);
            if (setupPhase_) {
                --curN_;
            }
            break;
        }
    }
}

void GameController::game() {
    giveAreasForTwoPlayers_();
    giveCreatureIdPlayers_();
    while (!askedClose_) {
        askedRestart_ = false;
        setupField_(K_);
        while ((!askedRestart_) && (!winnerDeterminate_) && (!askedClose_)) {    
            computeModel_(T_);
            if (!winnerDeterminate_) setupField_(N_);
        }
        winnerDeterminate_ = false;
        while ((!askedRestart_) && (!askedClose_)) {
            input_->readInput();
        }
    } 
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

void GameController::clearGridCanvas_() {
    getCanvasComp_()->clear();
}

void GameController::giveAreasForTwoPlayers_() {
    std::pair<int, int> ul1 = {0, 0};
    std::pair<int, int> lr1 = {area_->width() / 2 - 1, 
                                    area_->height() - 1};
    auto area1 = areaFactory_->createArea(ul1, lr1);
    players_[0]->setFieldArea(std::move(area1));
    
    std::pair<int, int> ul2 = {area_->width() / 2, 0};
    std::pair<int, int> lr2 = {area_->width() - 1, 
                                    area_->height() - 1};
    auto area2 = areaFactory_->createArea(ul2, lr2);
    players_[1]->setFieldArea(std::move(area2));

}

void GameController::giveCreatureIdPlayers_() {
    for (auto&& p : players_) {
        auto id = creature::nextCreatureId();
        p->setCreatId(id);
    }
}

void GameController::setupField_(int N) {
    for (int i = 0; i < players_.size(); ++i) {
        setupFieldForPlayer_(N, i);
    }
}

void GameController::setupFieldForPlayer_(int N, int player) {
    setupPhase_ = true;
    auto p = players_[player];
    auto&& area = p->fieldArea();
    auto a = typeid(area).name();
    curN_ = N;
    area.unlock();
    while (curN_ && !askedClose_ && !askedRestart_) {
        notifyAboutPlayerParticipation_(player, curN_);
        input_->readInput();
    } 
    area.lock();
    setupPhase_ = false;
}

void GameController::computeModel_(int T) {
    while (T-- && !askedClose_ && !askedRestart_) { 
        notifyAboutModelComputing_(T);
        auto [suc, crId] = model_->compute();
        if (!suc) {
            if (crId == -1) {
                noitfyAboutDraw_();
            } else {
                for (int i = 0; i < players_.size(); ++i) {
                    auto&& p = players_[i];
                    if (p->creatId() == crId) {
                        notifyAboutWinner_(i);
                    }
                }
            }  
            winnerDeterminate_ = true;
            break;
        }   
        redrawWindowNDisplay_();
        std::this_thread::sleep_for(
            std::chrono::milliseconds(250));
    }
}

void GameController::restartController_() {
    area_->clear();
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
