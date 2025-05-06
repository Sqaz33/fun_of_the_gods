#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <compare>
#include <string>

namespace creature {

struct ICreature {
    virtual void kill() noexcept = 0;
    virtual void revive() noexcept = 0;
    virtual bool isAlive() const noexcept = 0;
    virtual int id() const noexcept = 0;
    virtual void setId(int id) noexcept = 0;
    virtual const std::string& color() const noexcept = 0;
    virtual void setColor(const std::string& color) = 0;

    virtual ~ICreature() {}
};

class Creature : public ICreature; 
Creature nextCreature();

class Creature : public ICreature {
public:
    Creature(int id); 
    Creature(int id, const std::string& color);

public:
    void kill() noexcept override;
    void revive() noexcept override;
    bool isAlive() const noexcept override;
    int id() const noexcept override;
    void setId(int id) noexcept override;
    const std::string& color() const noexcept;
    void setColor(const std::string& color) override;

    bool operator<(const Creature& cr);

private:
    int id_;
    bool isAlive_ = false;
    std::string color_;
};

} // namespace creature

#endif // CREATURE_HPP