#ifndef INCLUDE_VIEW_HPP
#define INCLUDE_VIEW_HPP

#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "tetromino.hpp"
#include "tetris-game-model.hpp"

using namespace tetris_game_model;

namespace view {

/**
 * @brief 
 * 
 */
class IDrawable {
public:
    virtual void draw(sf::RenderWindow& window, sf::Vector2f start) = 0;
    virtual std::pair<float, float> size() const = 0;
    virtual ~IDrawable() { }
};

/**
 * @brief 
 * 
 */
class IDrawableComposite : public IDrawable {
public:
    void draw(sf::RenderWindow& window, sf::Vector2f start) = 0;
    virtual void addComponent(
        std::shared_ptr<IDrawable> comp, const std::string& name) = 0;

    virtual std::shared_ptr<IDrawable> getComponent(
        const std::string& name) = 0;

    virtual void deleteComponent(const std::string& name) = 0;

private:
    std::map<std::string, 
            std::list<std::shared_ptr<IDrawable>>::iterator
        > componentsMap_;
protected:
    std::list<std::shared_ptr<IDrawable>> components_;
};

/**
 * @brief 
 * 
 */
class DrawableStackLayout final : public IDrawableComposite {    
public:
    void draw(sf::RenderWindow& window, sf::Vector2f start) override;
    std::pair<float, float> size() const override;

public:
    void addComponent(
        std::shared_ptr<IDrawable> comp, const std::string& name) override;

    std::shared_ptr<IDrawable> getComponent(
        const std::string& name) override;

    void deleteComponent(const std::string& name) override;
};

/**
 * @brief 
 * 
 */
class DrawableNestedLayout final : public IDrawableComposite {
public:
    DrawableNestedLayout(float widthOffset, float heightOffset);

public:
    void draw(sf::RenderWindow& window, sf::Vector2f start) override;
    std::pair<float, float> size() const override;

public:
    void addComponent(
        std::shared_ptr<IDrawable> comp, const std::string& name) override;

    std::shared_ptr<IDrawable> getComponent(
        const std::string& name) override;

    void deleteComponent(const std::string& name) override;

public:
    float widthOffset() const;
    float heightOffset() const;
    void setWidthOffset(float offset);
    void setHeightOffset(float offset);

private:
    float widthOffset_;
    float heightOffset_;
};

/**
 * @brief 
 * 
 */
class DrawableFrame final : public IDrawable {
public:
    DrawableFrame(float width, float height, float thickness, sf::Color color);

public:
    void draw(sf::RenderWindow& window, sf::Vector2f start) override;
    std::pair<float, float> size() const override;

    void setThickness(float thickness);
    float thickness() const;
    void setColor(sf::Color color);
    sf::Color color() const;

private:
    float thickness_;
    sf::Color color_;
    sf::RectangleShape externalRect_;
    sf::RectangleShape internalRect_;
};

/**
 * @brief 
 * 
 */
class DrawableGridCanvas final : public IDrawable {
public:
    DrawableGridCanvas(float width,
                       float height,
                       std::size_t widthInCells, 
                       std::size_t heightInCells, 
                       float gridThickness,
                       sf::Color gridColor = sf::Color::Black);

public:
    void draw(sf::RenderWindow& window, sf::Vector2f start) override;
    std::pair<float, float> size() const override;
    
    void paintCell(std::pair<std::size_t, std::size_t> pos, sf::Color color);
    void clear();
    sf::Color gridColor() const;
    void setGridColor(sf::Color color);

private:
    void drawCellAt_(std::size_t cellX, 
                      std::size_t cellY,
                      sf::Vector2f start,
                      sf::RenderWindow& window,
                      sf::Color blockColor);
    void drawGrid_(sf::RenderWindow& window, sf::Vector2f start);

private:
    struct cell_t {
        std::pair<std::size_t, std::size_t> pos;
        sf::Color color;
    };
private:
    std::vector<cell_t> cells_;
    float gridThickness_;
    std::size_t widthInCells_;
    std::size_t heightInCells_;
    float width_;
    float height_;
    float cellWidth_;
    float cellHeight_;
    sf::Color gridColor_;
};

/**
 * @brief 
 * 
 */
class DrawableText final : public IDrawable {
public:
    DrawableText(std::string txt, int characterSize, 
                 std::string font, sf::Color color = sf::Color::Black, 
                 sf::Vector2f startPos = {0, 0});

public:
    void draw(sf::RenderWindow& window, sf::Vector2f start) override;
    std::pair<float, float> size() const override;
    void setText(const std::string& txt);
    std::string text() const;

private:
    sf::Font font_;
    int characterSize_;
    sf::Vector2f startPos_;
    sf::Text sfTxt_;
};

} // namespace view

#endif // INCLUDE_VIEW_HPP