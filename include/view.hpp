/**
 * @file view.hpp
 * @brief Header file for the view components of the game.
 * 
 * Defines interfaces and implementations for renderable UI components
 * using SFML, including layouts, grids, frames, and text elements.
 */

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

namespace view {

/**
 * @interface IDrawable
 * @brief Base interface for all renderable components.
 * 
 * Provides basic drawing capabilities and size information
 * for all UI components in the view system.
 */
class IDrawable {
public:
    /**
     * @brief Draws the component to the specified window.
     * @param window The SFML render window to draw to
     * @param start The starting position for drawing (top-left corner)
     */
    virtual void draw(sf::RenderWindow& window, sf::Vector2f start) = 0;
    
    /**
     * @brief Gets the size of the drawable component.
     * @return Pair containing width and height of the component
     */
    virtual std::pair<float, float> size() const = 0;
    
    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~IDrawable() = default;
};

/**
 * @interface IDrawableComposite
 * @brief Interface for composite drawable components.
 * 
 * Extends IDrawable to support composite structures that can contain
 * and manage multiple child components.
 */
class IDrawableComposite : public IDrawable {
public:
    /**
     * @brief Adds a component to the composite.
     * @param comp The component to add
     * @param name Identifier for the component
     */
    virtual void addComponent(
        std::shared_ptr<IDrawable> comp, const std::string& name) = 0;

    /**
     * @brief Gets a component by name.
     * @param name Identifier of the component to retrieve
     * @return Shared pointer to the component or nullptr if not found
     */
    virtual std::shared_ptr<IDrawable> getComponent(
        const std::string& name) = 0;

    /**
     * @brief Removes a component by name.
     * @param name Identifier of the component to remove
     */
    virtual void deleteComponent(const std::string& name) = 0;

private:
    std::map<std::string, 
            std::list<std::shared_ptr<IDrawable>>::iterator
        > componentsMap_;  ///< Map for quick component lookup
protected:
    std::list<std::shared_ptr<IDrawable>> components_;  ///< List of child components
};

/**
 * @class DrawableStackLayout
 * @brief A composite that stacks components vertically.
 * 
 * Arranges child components in a vertical stack, with each component
 * positioned below the previous one.
 */
class DrawableStackLayout final : public IDrawableComposite {    
public:
    void draw(sf::RenderWindow& window, sf::Vector2f start) override;
    std::pair<float, float> size() const override;

    void addComponent(
        std::shared_ptr<IDrawable> comp, const std::string& name) override;
    std::shared_ptr<IDrawable> getComponent(
        const std::string& name) override;
    void deleteComponent(const std::string& name) override;
};

/**
 * @class DrawableNestedLayout
 * @brief A composite that positions components with offsets.
 * 
 * Positions child components relative to the parent position with
 * configurable horizontal and vertical offsets.
 */
class DrawableNestedLayout final : public IDrawableComposite {
public:
    /**
     * @brief Constructs a nested layout with offsets.
     * @param widthOffset Horizontal offset for child components
     * @param heightOffset Vertical offset for child components
     */
    DrawableNestedLayout(float widthOffset, float heightOffset);

    void draw(sf::RenderWindow& window, sf::Vector2f start) override;
    std::pair<float, float> size() const override;

    void addComponent(
        std::shared_ptr<IDrawable> comp, const std::string& name) override;
    std::shared_ptr<IDrawable> getComponent(
        const std::string& name) override;
    void deleteComponent(const std::string& name) override;

    // Offset management
    float widthOffset() const;
    float heightOffset() const;
    void setWidthOffset(float offset);
    void setHeightOffset(float offset);

private:
    float widthOffset_;   ///< Horizontal offset for children
    float heightOffset_;  ///< Vertical offset for children
};

/**
 * @class DrawableFrame
 * @brief A rectangular frame drawable component.
 * 
 * Renders a rectangular frame with configurable size, thickness, and color.
 */
class DrawableFrame final : public IDrawable {
public:
    /**
     * @brief Constructs a frame with specified properties.
     * @param width Total width of the frame
     * @param height Total height of the frame
     * @param thickness Thickness of the frame borders
     * @param color Color of the frame
     */
    DrawableFrame(float width, float height, float thickness, sf::Color color);

    void draw(sf::RenderWindow& window, sf::Vector2f start) override;
    std::pair<float, float> size() const override;

    // Frame properties
    void setThickness(float thickness);
    float thickness() const;
    void setColor(sf::Color color);
    sf::Color color() const;

private:
    float thickness_;          ///< Border thickness
    sf::Color color_;          ///< Frame color
    sf::RectangleShape externalRect_;  ///< Outer rectangle
    sf::RectangleShape internalRect_;  ///< Inner rectangle (for border effect)
};

/**
 * @class DrawableGridCanvas
 * @brief A grid-based canvas for cell-based rendering.
 * 
 * Provides a grid structure where individual cells can be colored,
 * typically used for game board rendering.
 */
class DrawableGridCanvas final : public IDrawable {
public:
    /**
     * @brief Constructs a grid canvas.
     * @param width Total width in pixels
     * @param height Total height in pixels
     * @param widthInCells Number of horizontal cells
     * @param heightInCells Number of vertical cells
     * @param gridThickness Thickness of grid lines
     * @param gridColor Color of grid lines (default black)
     */
    DrawableGridCanvas(float width,
                      float height,
                      std::size_t widthInCells, 
                      std::size_t heightInCells, 
                      float gridThickness,
                      sf::Color gridColor = sf::Color::Black);

    void draw(sf::RenderWindow& window, sf::Vector2f start) override;
    std::pair<float, float> size() const override;
    
    /**
     * @brief Colors a specific cell in the grid.
     * @param pos Cell coordinates (x,y)
     * @param color Color to apply to the cell
     */
    void paintCell(std::pair<std::size_t, std::size_t> pos, sf::Color color);
    
    /**
     * @brief Clears all cell colors.
     */
    void clear();
    
    // Grid properties
    float gridThickness() const noexcept;
    float cellWidth() const noexcept;
    float cellHeight() const noexcept;

private:
    void drawCellAt_(std::size_t cellX, 
                    std::size_t cellY,
                    sf::Vector2f start,
                    sf::RenderWindow& window,
                    sf::Color blockColor);
    void drawGrid_(sf::RenderWindow& window, sf::Vector2f start);

private:
    std::map<std::pair<std::size_t, std::size_t>, sf::Color> cells_;  ///< Colored cells
    float gridThickness_;      ///< Grid line thickness
    std::size_t widthInCells_; ///< Number of horizontal cells
    std::size_t heightInCells_; ///< Number of vertical cells
    float width_;              ///< Total width in pixels
    float height_;             ///< Total height in pixels
    float cellWidth_;          ///< Calculated cell width
    float cellHeight_;         ///< Calculated cell height
    sf::Color gridColor_;      ///< Color of grid lines
};

/**
 * @class DrawableText
 * @brief A text rendering component.
 * 
 * Renders text with configurable font, size, color, and position.
 */
class DrawableText final : public IDrawable {
public:
    /**
     * @brief Constructs a text component.
     * @param txt The text to display
     * @param characterSize Font size in pixels
     * @param font Path to font file
     * @param color Text color (default black)
     * @param startPos Starting position offset (default 0,0)
     */
    DrawableText(std::string txt, int characterSize, 
                std::string font, sf::Color color = sf::Color::Black, 
                sf::Vector2f startPos = {0, 0});

    void draw(sf::RenderWindow& window, sf::Vector2f start) override;
    std::pair<float, float> size() const override;
    
    /**
     * @brief Sets the text content.
     * @param txt The new text to display
     */
    void setText(const std::string& txt);
    
    /**
     * @brief Gets the current text content.
     * @return The current text
     */
    std::string text() const;

private:
    sf::Font font_;          ///< Font used for rendering
    int characterSize_;      ///< Font size in pixels
    sf::Vector2f startPos_;  ///< Position offset
    sf::Text sfTxt_;         ///< SFML text object
};

} // namespace view

#endif // INCLUDE_VIEW_HPP