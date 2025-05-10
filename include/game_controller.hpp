#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include <sstream>

#include "game_field_area_factory.hpp"
#include "player.hpp"
#include "user_input.hpp"
#include "view.hpp"
#include "game_model.hpp"

namespace game_controller {

template <class creature_t, template<class> class cell_t>
class GameController : 
    observer::IObserver<
        GameController<cell_t<creature_t>>> 
{
    using IGameField = game_field::IGameField<cell_t<creature_t>>;
    using IGameModel = game_model::IGameModel<cell_t<creature_t>>;
    using IUserInput = user_input::IUserInput;
    using IDrawableComposite = view::IDrawableComposite;
    using DrawableText = view::DrawableText;
    using IGameFieldAreaFactory = 
        factory::IGameFieldAreaFactory<cell_t<creature_t>>
    using Player = player::Player<cell_t<creature_t>>;

public:
    GameController(
            int K,
            int T,
            int N,
            std::shared_ptr<IGameField> field,  
            std::unique_ptr<IGameFieldAreaFactory> areaFactory,
            std::unique_ptr<IGameModel> model,
            std::shared_ptr<IDrawableComposite> view,
            const std::vector<Player>& players,
            std::shared_ptr<IUserInput> input):
        K_(K)
        , T_(T)
        , N_(N)
        , field_(std::move(field))
        , areaFactory_(std::move(areaFactory))
        , model_(std::move(model))
        , view_(view)
        , players_(players)
        , input_(input)
    {}

    void game() {
        while (!askedStop_) {
            setupFieldFirstTime_(K_);
            while ((!askedRestart_) && (!winnerDeterminate_)) {    
                computeModel_(T_);
                setupFieldFirstTime_(N_);
            }
            winnerDeterminate_ = false;
            while ((!askedRestart_) && (!askedStop_)) {
                input_->readInput();
            }
        } 
    }

private:
    void setupFieldFirstTime_(int player) {
        setupPhase = true;
        auto p = players_[player];
        auto&& area = p->fieldArea();
        curK_ = K_;
        area.unlock();
        while (curK_) {
            notifyAboutPlayerParticipation_(player, curK_);
            input_->readInput();
        } 
        area.lock();
        setupPhase = false;
    }

    void setupField_(int player) {
        setupPhase = true;
        auto p = players_[player];
        auto&& area = p->fieldArea();
        curN_ = N_;
        area.unlock();
        while (curN_) {
            notifyAboutPlayerParticipation_(player, curN_);
            input_->readInput();
        } 
        area.lock();
        setupPhase = false;
    }

    void computeModel_(int T) {
        while (T--) { 
            auto [suc, crId] = model_->compute() 
            if (!suc) {
                if (crId == -1) {
                    noitfyAboutDraw_();
                } else {
                    for (int i = 0; i < players_.size(); ++i) {
                        auto&& p = players_[i];
                        if (p.creature().id() == crId) {
                            notifyAboutWinner_(i);
                        }
                    }
                }  
                winnerDeterminate_ = true;
            }   
        }
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

    void setTextOnTextComp_(const std::string& text) {
        auto comp = view_->getComponent("text");
        if (!comp) {
            throw std::logic_error(
                "The view is missing the text component");
        }
        if (auto text = 
            std::dynamic_pointer_cast<DrawableText>())
        {   
            text->setText(text);
        } else {
            throw std::logic_error(
                "The view is missing the DrawableText component");
        }
    }

private:
    std::shared_ptr<IGameField> field_;
    std::unique_ptr<IGameFieldAreaFactory> areaFactory_;
    std::unique_ptr<IGameModel> model_;
    std::shared_ptr<IDrawableComposite> view_;
    std::vector<Player> players_;
    std::shared_ptr<IUserInput> input_;
    int K_;
    int T_;
    int N_;
    int curK_;
    int curN_;
    bool winnerDeterminate_ = false;
    bool askedRestart_ = false;
    bool askedStop_ = false;
    bool setupPhase = false;
};

}

#endif // GAME_CONTROLLER_HPP