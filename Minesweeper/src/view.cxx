#include "view.hxx"


// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 35;

// Colors are red-green-blue(-alpha), each component
// from 0 to 255.

//create colors for sprites
static Color const tileColor = Color::medium_green();
static Color const gainColor = Color::medium_red();
static Color const lightColor = Color::white();
static Color const darkColor = Color::black();
static Color const loserColor {100, 100, 100};

//make a size for each board unit.
static ge211::Dims<int> bdim {33,33};

//z vals: 0 is standard tile, 1 = winning tile, 2 = player piece, 3+ to
// replace 'eaten' pieces

//edited with new objects.
View::View(Model const& model)
        : model_(model),
          tile_(bdim, tileColor),
          gainTile_(bdim, gainColor),
          lightPiece_(15, lightColor),
          darkPiece_(15, darkColor),
          loserSprite_(15, loserColor)
        // You may want to add sprite initialization here
{
    // //initialize the sprite set here
    // for (int i )
}

//called every 1/60th of a second
//takes in the set to draw and mouse position
//arms length collab with Evan Chen. We walked through the
// general idea of how to go about drawing the board and what helper
// functions we should probably use (AKA function to get the possible gains
// when hovering over a tile.)
void View::draw(Sprite_set& set, ge211::Posn<int> mouse_pos)
{
    // TODO, PROBABLY



    //get set of positions gained on pos where mouse is hovering over
    Position_set const& flips = gains_(screen_to_board(mouse_pos));

    //need to loop through every position in board to draw board
    for (Position bpos : model_.board()) {
        //get screen position from that board position
        Position screenPos = board_to_screen(bpos);
        //adjust tile position by moving it right and down to center
        Position tilePos = screenPos + Dimensions {2,2};
        //get piece position setting it to the tilePos and then moving it right
        // and down to center
        Position piecePos = tilePos + Dimensions {1,2};//Dimensions

        if (flips[bpos]) {
            set.add_sprite(gainTile_, tilePos, 0);
        } else {
            set.add_sprite(tile_, tilePos, 0);
        }

        add_player_sprite_(set, model_[bpos], piecePos, 1);
    }
    add_player_sprite_(set, model_.turn(), mouse_pos, 2);
}
View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.board().dimensions();
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}

//takes a board position ({1,1} and converts this position to physical pixel
// position)
View::Position
View::board_to_screen(Model::Position pos) const
{
    return {grid_size * pos.x, grid_size * pos.y};
}

//takes pixel position and converts to board position
Model::Position
View::screen_to_board(View::Position pos) const
{
    return {pos.x / grid_size, pos.y / grid_size};
}



//arms length collab with evan chen. High level discussion of how to check
// whether a move is real or not and if there is a "gains" set that we can
// return it
Position_set const&
View::gains_(View::Position mouse_pos) const
{
    static Position_set emptySet;
    //returns the current flips that position being hovered will gain.
    //get the move object where the player is hovering over
    {
        Move const *movep = model_.find_move(mouse_pos);
        //if there's a move, then return all the possible moves, or else
        // return empty position set
        if (movep) {
            return movep->second;
        } else {
            return emptySet;
        }
    }
}


// Arms-length collaboration with Evan Chen on how to add each
// sprite to the sprite set. (What to check for first and why, and why
// checking "isgameover" late causes the bug of winning pieces disappearing.
void
View::add_player_sprite_(
        Sprite_set& set,
        Player player,
        Position pos,
        int z) const
{
    {
        if (player == Player::neither)
        {
            return;
        }
        //case for if the game is over, need to replace all loser pieces with
        // the loser sprite
        if (model_.is_game_over() and player != model_.winner())
        {
            set.add_sprite(loserSprite_, pos, z);
        }
        //for every position need to check if its light or dark to add to
        // sprite set
        else if (player == Player::light)
        {
            set.add_sprite(lightPiece_, pos, z);
        }
        else if (player == Player::dark)
        {
            set.add_sprite(darkPiece_, pos, z);
        }


    }
}