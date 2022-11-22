#pragma once

#include "model.hxx"

#include <string>

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// View positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// View rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    /// Constructs a view that knows about the given model.
    explicit View(Model const&);

    // TODO: your code here
    // You will probably want to add arguments here so that the
    // controller can communicate UI state (such as a mouse or
    // cursor position):
    // custom edits: position object as parameter---------------------
    void draw(ge211::Sprite_set& set, ge211::Posn<int> mouse_pos);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    // Converts an abstract board position to a concrete screen
    // position.
    Position
    board_to_screen(Model::Position board_pos) const;

    // Converts a concrete screen (pixel) position to an abstract board
    // position.
    Model::Position
    screen_to_board(Position screen_pos) const;

    // TODO: Add any public member functions you need.

private:
    Model const& model_;

    // TODO: Add any additional private members you need, such as helper
    // functions or sprites.

    ge211::Rectangle_sprite tile_;
    ge211::Rectangle_sprite gainTile_;
    ge211::Circle_sprite lightPiece_;
    ge211::Circle_sprite darkPiece_;
    ge211::Circle_sprite loserSprite_;

    // making function that returns the set of possible flips when mouse is
    // hovering over that location.
    Position_set const&
    gains_(Position mousePos) const;

    void
    add_player_sprite_(ge211::Sprite_set&, Player, Position, int z) const;
};
