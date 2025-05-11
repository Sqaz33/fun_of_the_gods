#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <cassert>

#include "view.hpp"

#include <SFML/System/Angle.hpp>

namespace {

void drawVerticalLine(sf::RenderWindow& window, 
                           sf::Vector2f start, 
                           float lenght, 
                           float thickness,
                           sf::Color color) 
{
    sf::RectangleShape line({lenght, thickness});
    line.setPosition({start.x + thickness, start.y});
    line.setFillColor(color);
    line.rotate(sf::degrees(90));
    window.draw(line);
}

void drawHorizontalLine(sf::RenderWindow& window, 
                             sf::Vector2f start, 
                             float lenght, 
                             float thickness,
                             sf::Color color) 
{
    sf::RectangleShape line({lenght, thickness});
    line.setPosition(start);
    line.setFillColor(color);
    window.draw(line);
}


} // namespace 

namespace view {

// ##################################################
// IDrawableComposite
void IDrawableComposite::addComponent(
    std::shared_ptr<IDrawable> comp, const std::string& name) {
    auto it = components_.insert(components_.end(), comp);
    componentsMap_.emplace(name, it);
}

std::shared_ptr<IDrawable> IDrawableComposite::getComponent(
    const std::string& name) {
    if (componentsMap_.contains(name)) {
        return *componentsMap_[name];
    }
    for (auto p : components_) {
        auto drawable 
            = std::dynamic_pointer_cast<IDrawableComposite>(p);
        if (drawable) {
            auto res = drawable->getComponent(name);
            if (res) {
                return res;
            }
        }     
    }
    return nullptr;
}

void IDrawableComposite::deleteComponent(const std::string& name) {
    if (componentsMap_.contains(name)) {
        auto it = componentsMap_.find(name);
        components_.erase(it->second);
        componentsMap_.erase(it);
        return;
    }
    for (auto p : components_) {
        auto drawable 
            = std::dynamic_pointer_cast<IDrawableComposite>(p);
        if (drawable) {
            drawable->deleteComponent(name);
        }     
    }
}

// ##################################################
// DrawableStackLayout

void DrawableStackLayout::addComponent(
    std::shared_ptr<IDrawable> comp, const std::string& name) {
    IDrawableComposite::addComponent(comp, name);
}

std::shared_ptr<IDrawable> DrawableStackLayout::getComponent(
    const std::string& name) {
    return IDrawableComposite::getComponent(name);
}

void DrawableStackLayout::deleteComponent(const std::string& name) {
    IDrawableComposite::deleteComponent(name);
}

void DrawableStackLayout::draw(sf::RenderWindow& window, sf::Vector2f start) {
    auto curStart = start;
    auto it = components_.rbegin(); 
    auto end = components_.rend();
    for (auto comp = *it; it != end; ++it) {
        comp = *it;
        comp->draw(window, curStart);
        auto compSz = comp->size();
        curStart = {
            curStart.x,
            curStart.y + compSz.second
        };
    }
}

std::pair<float, float> DrawableStackLayout::size() const {
    if (components_.empty()) return {0., 0.};

    float width = 0;
    float height = 0;
    for (const auto& comp : components_) {
        width = std::max(width, comp->size().first);
        height += comp->size().second;
    }

    return {width, height};
}  

// ##################################################
// DrawableNestedLayout
DrawableNestedLayout::DrawableNestedLayout(float widthOffset, float heightOffset) :
    widthOffset_(widthOffset)
    , heightOffset_(heightOffset)
{}

void DrawableNestedLayout::draw(sf::RenderWindow& window, sf::Vector2f start) {
    auto curStart = start;
    for (auto comp : components_) {
        comp->draw(window, curStart);
        curStart = {
            curStart.x + widthOffset_,
            curStart.y + heightOffset_
        };
    }
}

std::pair<float, float> DrawableNestedLayout::size() const {
    if (components_.empty()) return {0., 0.};
    return components_.front()->size();
}

void DrawableNestedLayout::addComponent(
    std::shared_ptr<IDrawable> comp, const std::string& name) {
    auto compSz = comp->size();
    if (!components_.empty()) {
        auto backSz = components_.back()->size();
        if (compSz.first >= backSz.first || compSz.second >= backSz.second) {
            assert(false);
        }
    }
    IDrawableComposite::addComponent(comp, name);
}

std::shared_ptr<IDrawable> DrawableNestedLayout::getComponent(
    const std::string& name) {
    return IDrawableComposite::getComponent(name);
}

void DrawableNestedLayout::deleteComponent(const std::string& name) {
    IDrawableComposite::deleteComponent(name);
}

float DrawableNestedLayout::widthOffset() const {
    return widthOffset_;
}

float DrawableNestedLayout::heightOffset() const {
    return heightOffset_;
}

void DrawableNestedLayout::setWidthOffset(float offset) {
    widthOffset_ = offset;
}

void DrawableNestedLayout::setHeightOffset(float offset) {
    heightOffset_ = offset;
}

// ##################################################
// DrawableFrame
DrawableFrame::DrawableFrame(float width, float height, float thickness, sf::Color color) :
        thickness_(thickness)
        , externalRect_({width, height})
        , color_(color)
{
    if (thickness > std::min(width, height)) {
        throw std::logic_error("thickness > min(width, height)");
    }
        
    auto interWidth = width - thickness * 2;
    auto interHeigh = height - thickness * 2;
    internalRect_ = sf::RectangleShape({interWidth, interHeigh});

    externalRect_.setFillColor(color_);
}

void DrawableFrame::draw(sf::RenderWindow& window, sf::Vector2f start) {
    sf::Vector2f interPos(
        start.x + thickness_,
        start.y + thickness_  
    );
    externalRect_.setPosition(start);
    internalRect_.setPosition(interPos);
    window.draw(externalRect_);
    window.draw(internalRect_);
}

std::pair<float, float> DrawableFrame::size() const {
    auto sz = externalRect_.getSize();
    return {sz.x, sz.y};
}

void DrawableFrame::setThickness(float thickness) {
    thickness_ = thickness;
}

float DrawableFrame::thickness() const { return thickness_; }

void DrawableFrame::setColor(sf::Color color) {
    externalRect_.setFillColor(color);
    internalRect_.setFillColor(color);
    color_ = color;
}

sf::Color DrawableFrame::color() const {
    return color_;
} 

// ##################################################
// DrawableGridCanvas
DrawableGridCanvas::DrawableGridCanvas(float width,
                                      float height,
                                      std::size_t widthInCells, 
                                      std::size_t heightInCells, 
                                      float gridThickness,
                                      sf::Color gridColor) : 
    gridThickness_(gridThickness)
    , width_(width)
    , height_(height)
    , widthInCells_(widthInCells)
    , heightInCells_(heightInCells)
    , gridColor_(gridColor)
{   
    cellWidth_ = ((width - gridThickness_) - (gridThickness_ * widthInCells)) 
                 / widthInCells;
    cellHeight_ = ((height - gridThickness_) - (gridThickness_ * heightInCells)) 
                 / heightInCells;
}

void DrawableGridCanvas::draw(sf::RenderWindow& window, sf::Vector2f start) {
    drawGrid_(window, start);
    for (const auto& cell : cells_) {
        auto x = cell.pos.first;
        auto y = cell.pos.second;
        drawCellAt_(x, y, start, window, cell.color);
    }
}

std::pair<float, float> DrawableGridCanvas::size() const {
    return {width_, height_};
}

void DrawableGridCanvas::paintCell(
    std::pair<std::size_t, std::size_t> pos, sf::Color color) {
    cells_.emplace_back(pos, color);
}

void DrawableGridCanvas::clear() {
    cells_.clear();
}


float DrawableGridCanvas::gridThickness() const noexcept {
    return gridThickness_;
}

float DrawableGridCanvas::cellWidth() const noexcept {
    return cellWidth_;
}

float DrawableGridCanvas::cellHeight() const noexcept {
    return cellHeight_;
}

void DrawableGridCanvas::drawCellAt_(std::size_t cellX, 
                                     std::size_t cellY,
                                     sf::Vector2f start,
                                     sf::RenderWindow& window,
                                     sf::Color blockColor) 
{   
    float x = start.x 
              + cellX * cellWidth_ 
              + gridThickness_ * (cellX + 1);
    float y = start.y
            + cellY * cellHeight_ 
            + gridThickness_ * (cellY + 1);
    sf::Vector2f pos {x , y};

    sf::RectangleShape block({cellWidth_, cellHeight_});
    block.setPosition(pos);

    block.setFillColor(blockColor);

    window.draw(block);
}


void DrawableGridCanvas::drawGrid_(sf::RenderWindow& window, sf::Vector2f start) {
    float limX = start.x + width_;
    int c = 0;
    for (float x = start.x; x < limX; x += cellWidth_ + gridThickness_) {
        drawVerticalLine(window, {x, start.y}, height_, gridThickness_, gridColor_);
    }

    float limY = start.y + height_;
    for (float y = start.y; y < limY; y += cellHeight_ + gridThickness_) {
        drawHorizontalLine(window, {start.x, y}, width_, gridThickness_, gridColor_);
    }
}

// ##################################################
// DrawableText
DrawableText::DrawableText(std::string txt, int characterSize, 
                           std::string font, sf::Color color, 
                           sf::Vector2f startPos) :
    font_(font)
    , characterSize_(characterSize)
    , startPos_(startPos)
    , sfTxt_(font_) 
{
    sfTxt_.setString(txt);
    sfTxt_.setFillColor(color);
    sfTxt_.setCharacterSize(characterSize_);
}

void DrawableText::draw(sf::RenderWindow& window, sf::Vector2f start) {
    sf::Vector2f newStart = {
        start.x + startPos_.x,
        start.y + startPos_.y
    };
    sfTxt_.setPosition(newStart);
    window.draw(sfTxt_);
}

std::pair<float, float> DrawableText::size() const {
    auto boundsSz = sfTxt_.getLocalBounds().size;
    return {boundsSz.x, boundsSz.y + characterSize_ * 2};
}

void DrawableText::setText(const std::string& txt) {
    sfTxt_.setString(txt);
}

std::string DrawableText::text() const {
    return sfTxt_.getString();
}

} // namespace view