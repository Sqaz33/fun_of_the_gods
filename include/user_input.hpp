#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include <utility>
#include <tuple>
#include <memory>

#include <SFML/Window.hpp>

#include "subject.hpp"

namespace user_input {
struct IUserInput : subject::ISubject { 
    virtual void readInput() = 0; 
    virtual std::tuple<bool, int, int> lastCoordInput() noexcept = 0;
    virtual ~IUserInput() {}
};

class UserInput : 
    public IUserInput 
    ,public std::enable_shared_from_this<UserInput>
{
public:
    UserInput(
        std::shared_ptr<sf::Window> window, 
        int startX, 
        int startY,
        int cellWidth,
        int cellHeight,
        int gridThickness);

public:
    void readInput() override;
    std::tuple<bool, int, int> lastCoordInput() noexcept override; 

public:
    std::shared_ptr<UserInput> slf();

public:
    void attach(
        std::shared_ptr<observer::IObserver> obs, int event_t) override
    { subject::ISubject::attach(obs, event_t); }

    void detach(
        std::weak_ptr<observer::IObserver> obs, int event_t) override
    { subject::ISubject::detach(obs, event_t); }

protected:
    void notify(int event_t, std::weak_ptr<ISubject> slf) override {
        subject::ISubject::notify(event_t, slf);
    }

private:
    void fireUserAskedClose_(); 
    void fireUserAskedSetCreature_(); 
    void fireUserAskedRestart_(); 
    void computeCoord_(int x, int y);

private:
    std::shared_ptr<sf::Window> window_;
    int startX_;
    int startY_;
    int cellWidth_;
    int cellHeight_;
    int gridThickness_;
    std::tuple<bool, int, int>  lastCoordInput_ = {false, 0, 0};
}; 

}

#endif // USER_INPUT_HPP