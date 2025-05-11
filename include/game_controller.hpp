#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <chrono>
#include <thread>
#include <sstream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "game_field_area_factory.hpp"
#include "player.hpp"
#include "user_input.hpp"
#include "view.hpp"
#include "game_model.hpp"

namespace game_controller {

template <class creature_t, template<class> class cell_t>
class GameController : public observer::IObserver {
    using IGameField = game_field::IGameField<cell_t<creature_t>>;
    using IGameFieldArea = game_field_area::IGameFieldArea<cell_t<creature_t>>;
    using IGameModel = game_model::IGameModel;
    using IUserInput = user_input::IUserInput;
    using IGameFieldAreaFactory = 
        factory::IGameFieldAreaFactory<cell_t<creature_t>>;
    using Player = player::Player<creature_t, cell_t>;

public:
    GameController(
            int K,
            int T,
            int N,
            std::shared_ptr<IGameField> field,  
            std::unique_ptr<IGameFieldAreaFactory> areaFactory,
            std::unique_ptr<IGameModel> model,
            std::shared_ptr<view::IDrawableComposite> view,
            const std::vector<std::shared_ptr<Player>>& players,
            std::shared_ptr<IUserInput> input,
            std::shared_ptr<sf::RenderWindow> window,
            const std::unordered_map<int, sf::Color>& creatureColors):
        K_(K)
        , T_(T)
        , N_(N)
        , field_(field)
        , areaFactory_(std::move(areaFactory))
        , model_(std::move(model))
        , view_(view)
        , players_(players)
        , input_(input)
        , window_(window)
        , creatureColors_(creatureColors)
    { 
        std::pair<int, int> ul = {0, 0};
        std::pair<int, int> lr = {field_->width() - 1, 
                                    field_->height() - 1};
        
        area_ = std::move(areaFactory_->createArea(field_, ul, lr));
        area_->unlock();
    }

public:
    void update(
        std::weak_ptr<subject::ISubject> subj, int event_t) override 
    {   
        using evt_t = game_event::event_t;
        auto evt = static_cast<evt_t>(event_t);
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
            case evt_t::GAME_FIELD_UPDATE: {
                updateGridCanvasInView_();
                if (setupPhase_) {
                    --curN_;
                }
                break;
            } 
            case evt_t::PLAYER_KILL_CREATURE: {
                updateGridCanvasInView_();
                if (setupPhase_) {
                    ++curN_;
                }
                break;
            }
        }
    }

public:
    void game() {
        giveAreasForTwoPlayers_();
        giveCreaturePlayers_();
        while (!askedClose_) {
            askedRestart_ = false;
            setupField_(K_);
            while ((!askedRestart_) && (!winnerDeterminate_) && (!askedClose_)) {    
                setupField_(N_);
                computeModel_(T_);
            }
            winnerDeterminate_ = false;
            while ((!askedRestart_) && (!askedClose_)) {
                input_->readInput();
            }
        } 
    }       

private:
    void redrawWindowNDisplay_() {
        window_->clear(sf::Color::White);
        std::pair<unsigned, unsigned> windowSz = view_->size();
        window_->setSize({windowSz.first, windowSz.second});
        view_->draw(*window_, {0.f, 0.f});
        window_->display();
    }

    void updateGridCanvasInView_() {
        auto comp = view_->getComponent("grid_canvas");
        if (!comp) {
            throw std::logic_error(
                "The view is missing the grid component");
        }
        if (auto grid = 
            std::dynamic_pointer_cast<view::DrawableGridCanvas>(comp))
        {   
            grid->clear();
            for (int y = 0; y < field_->height(); ++y) {
                for (int x = 0; x < field_->width(); ++x) {
                    sf::Color color;
                    if (!area_->isCellAvailable(x, y)) {
                        color = sf::Color::Black;
                    } else {
                        auto&& cell = area_->getCell(x, y);
                        auto&& cr = cell.creature();
                        if (cr.isAlive()) {
                            color = creatureColors_.at(cr.id());
                        } else {
                            color = sf::Color::White;
                        }
                    }
                    grid->paintCell({x, y}, color);
                }
            }
            redrawWindowNDisplay_();
        } else {
            throw std::logic_error(
                "The view is missing the DrawableGridCanvas component");
        }
    } 

    void giveAreasForTwoPlayers_() {
        std::pair<int, int> ul1 = {0, 0};
        std::pair<int, int> lr1 = {field_->width() / 2 - 1, 
                                        field_->height() - 1};
        auto area1 = areaFactory_->createArea(field_, ul1, lr1);
        players_[0]->setFieldArea(std::move(area1));
        
        std::pair<int, int> ul2 = {field_->width() / 2, 0};
        std::pair<int, int> lr2 = {field_->width() - 1, 
                                        field_->height() - 1};
        auto area2 = areaFactory_->createArea(field_, ul2, lr2);
        players_[1]->setFieldArea(std::move(area2));

    }

    void giveCreaturePlayers_() {
        for (auto&& p : players_) {
            auto cr = creature::nextCreature<creature_t>();
            p->setCreature(cr);
        }
    }

    void setupField_(int N) {
        for (int i = 0; i < players_.size(); ++i) {
            setupFieldForPlayer_(N, i);
        }
    }

    void setupFieldForPlayer_(int N, int player) {
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

    void computeModel_(int T) {
        while (T-- && !askedClose_ && !askedRestart_) { 
            auto [suc, crId] = model_->compute();
            if (!suc) {
                if (crId == -1) {
                    noitfyAboutDraw_();
                } else {
                    for (int i = 0; i < players_.size(); ++i) {
                        auto&& p = players_[i];
                        if (p->creature().id() == crId) {
                            notifyAboutWinner_(i);
                        }
                    }
                }  
                winnerDeterminate_ = true;
                break;
            }   
            std::this_thread::sleep_for(
                std::chrono::milliseconds(250));
        }
    }

    void restartController_() {
        field_->clear();
        updateGridCanvasInView_();
    }

    void notifyAboutWinner_(int player) {
        std::stringstream ss;
        ss << "Won: ";
        ss << player;
        ss << '!';
        setTextOnTextComp_(ss.str());
    }

    void noitfyAboutDraw_() {
        setTextOnTextComp_("Draw!");
    }

    void notifyAboutPlayerParticipation_(int player, int n) {
        std::stringstream ss;
        ss << "Player: ";
        ss << player;
        ss << ". Remaining creatures: ";
        ss << n;
        ss << '.';
        setTextOnTextComp_(ss.str());
    }

    void notifyAboutModelComputing_(int n) {
        std::stringstream ss;
        ss << "Remaining er: ";
        ss << n;
        ss << '.';
        setTextOnTextComp_(ss.str());
    }

    void setTextOnTextComp_(const std::string& txt) {
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

private:
    std::unique_ptr<IGameFieldArea> area_;
    std::shared_ptr<IGameField> field_;
    std::unique_ptr<IGameFieldAreaFactory> areaFactory_;
    std::unique_ptr<IGameModel> model_;
    std::shared_ptr<view::IDrawableComposite> view_;
    std::vector<std::shared_ptr<Player>> players_;
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

}

#endif // GAME_CONTROLLER_HPP