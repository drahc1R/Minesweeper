#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

// Model::Model(int width, int height)
//         : board_({width, height})
// {
//     // TODO: initialize `next_moves_` to `turn_`'s available moves
//     // sets next_moves_ keys to all available positions and gains as their
//     // values
//     compute_next_moves_();
// }

Model::Rectangle
Model::board() const
{
    return board_.all_positions();
}

Type
Model::operator[](Position pos) const
{
    return board_[pos];
}

/// MINESWEEPER FUNCTIONS START HERE

/// Goes one step into direction 'dir' to check if there is a bomb.
/// Returns a position_set. Empty if no bomb present in that direction.
Position_set
Model::dir_bombs_(Position start, Dimensions dir) const
{
        // initialise position set
        Position_set res;

        // check for a bomb in direction of 'dir'
        start += dir;

        // if going off the board or the adjacent tile is safe, return empty set
        if (!board_.good_position(start) or
            board_[start] == Type::safe)
        {
            return {};
        }

        // if that direction is still within bounds and there's a bomb. add to
        // position set
        if (board_.good_position(start) and
        board_[start] == Type::bomb)
        {
            res[start] = true;
            return res;
        }
}

/// Returns a union set of positions using dir_bombs_ as a helper
/// function for checking in each direction. Returns a set of positions
/// that contains all nearby bombs for that one position. We can iterate
/// thru this set to count bombs
Position_set
Model::all_adjacent_bombs_(Position pos) const
{
    // creates a position set for the result
    Position_set pset = Position_set();

    // iterate through all eight directions
    for (auto d: board_.all_directions())
    {
        // call helper function to check if that direction has a bomb
        Position_set curr = dir_bombs_(pos, d);

        // if the resulting position set isn't empty (meaning there's a
        // bomb), add to final position set
        if (!curr.empty())
        {
            for (Position p: curr)
            {
                {
                    pset[p] = true;
                }
            }
        }
    }
    return pset;
}

/// Iterates through each position in all_adjacent_bombs's returned position
/// set. Returns an integer. This integer will be used to mark each
/// position with adjacent bombs
// have to iterate thru this pset to make the adjacent tiles for const
// solution board
int
Model::count_bombs_(Position pos) const
{
    int count = 0;
    Position_set pset = all_adjacent_bombs_(pos);
    for (Position p : pset)
    {
        count++;
    }
    return count;
}

/// Returns position set for positions without any bombs adjacent to it.
// have to iterate thru this pset to make the const solution board
Position_set
Model::no_bombs_() const
{
    Position_set pset;
    for (Position p : board_.all_positions())
    {
        if (count_bombs_(p) == 0)
        {
            pset[p] = true;
        }
    }
    return pset;
}

/// function returns a randomised position_set of bomb locations.
//Have to iterate thru this pset to make the const solution board
Position_set
Model::bomb_positions_() const
{
    //use random_source() to generate random x and y coords
    //generated random positions must be within good position and not in pset
    // yet
    //while true until global number of positions in set reaches desired
    // number of bombs
    // return pset.

    //create a pset
    Position_set res;

    //creating randint object (has to parametrized to difficulty, but let's
    // not do that)
    Random_source<int> cord(0, 8);

    //counter to know when to stop adding bombs
    int count = 0;

    while (true)
    {
        if (count == 10)
        {
            return res;
        }
        //get randints
        int xcord = cord.next();
        int ycord = cord.next();

        //creating a position with those randints
        Position p(xcord, ycord);

        //check if that position is already in the set
        if (!res[p])
        {
            res[p] = true;
            count++;
        }
    }
}


/// iterate through the returned set of bomb_positions to set
// Bombs[{Position}] = true;
void
Model::setup_bombs_() const
{
    // generate bomb positions
    Position_set bset = bomb_positions_();

    for (Position pos : bset)
    {
        //set this as a public variable in board so that i could update bomb
        // locations here...
        board_.bombs[pos] = true;
    }
}


/// gets a random position from tiles with no bombs
/// returns this position to start game safely.
Posn<int>
Model::start_pos_() const
{
    Position_set safe = no_bombs_();
    int totalSafePos = 0;
    for (Position p: safe) {
        totalSafePos++;
    }
    Random_source<int> cord(0, totalSafePos);

    int curr = 0;
    int rand = cord.next();
    for (Position p : safe)
    {
        curr++;
        if (curr == rand)
        {
            return p;
        }
    }
}

/// should start game with one of the positions in pset that no_bombs_ returns

/// need two more functions to add to state of board: set up safe
/// tiles psets
/// for adjacent tiles, just need to add the number on top. that should be on
/// view




/// need to fix the board and tileType classes to fit our needs better.



/// Sets game over if selected position is a bomb tile
void
Model::set_game_over_();

///No need
// /// Returns a position_set for all adjacent clear areas that dont have an
// /// integer (for how many bombs are adjancent) associated with it
// Position_set
// Model::first_reveal_(Position) const
// {
//
// }
































/// No need
// /// const Move*
// Model::find_move(Position pos) const
// {
//     auto i = next_moves_.find(pos);
//
//     if (i == next_moves_.end()) {
//         // Nothing was found, so return NULL (nullptr in C++)
//         return nullptr;
//     } else {
//         // Dereferences the iterator to get the value then returns a pointer
//         // to that value. This is safe as the pointer is to a value living
//         // within the `next_moves_` structure.
//         return &(*i);
//     }
// }

/// NO need play move
// void
// Model::play_move(Position pos)
// {
//     if (is_game_over()) {
//         throw Client_logic_error("Model::play_move: game over");
//     }
//
//     const Move* movep = find_move(pos);
//     if (movep == nullptr) { // check if there was no such move
//         throw Client_logic_error("Model::play_move: no such move");
//     }
//
//     // TODO: actually execute the move, advance the turn, refill
//     // `next_moves_`, etc.
//
//     really_play_move_(*movep);
// }

//
// BELOW ARE HELPER FUNCTIONS
// Our tests will run on the public functions, not the helper functions
// However, you are required to implement the following functions to help the
// public functions
//

// Position_set
// Model::find_flips_(Position current, Dimensions dir) const
// {
//     // TODO: HELPER FUNCTION
//     // gets current position as start. then go into given direction checking
//     // to see if there is opposing pieces across. then stop if there a
//     // friendly piece on other side.
//     // returns empty if there isnt a friendly piece at end after opposing
//     // piece, or if there arent any opposing pieces.
//
//     // while loop to check if going that direction results in opposing
//     // player piece.
//     //CREATE a set
//
//     Position_set res;
//
//     while(true)
//     {
//         current += dir;
//
//         if (!board_.good_position(current) or
//             board_[current] == Player::neither)
//         {
//             return {};
//         }
//
//         if (board_[current] == turn_)
//         {
//             return res;
//         }
//         res[current] = true;
//     }
// }

// Position_set
// Model::evaluate_position_(Position pos) const
// {
//     // TODO: HELPER FUNCTION
//
//     // creates a position set for the result
//     Position_set pset = Position_set();
//
//     // iterate through all eight directions union each
//     // result of find_flips into empty position set
//     if (board_[pos] == Player::neither) {
//         for (auto d: board_.all_directions()) {
//             pset.operator|=(find_flips_(pos, d));
//         }
//         if (pset.empty()) {
//             pset.clear();
//         } else {
//             pset[pos] = true;
//         }
//     }
//     return pset;
// }

//     // TODO: HELPER FUNCTION

//
// void
// Model::compute_next_moves_()
// {
//     // TODO: HELPER FUNCTION
//     //clear out all moves from movemap
//     next_moves_.clear();
//     //need to return all current moves available
//
//     //need to check if any of the 4 center positions
//     for (auto pos : board_.center_positions())
//     {
//         if (board_[pos] == Player::neither)
//         {
//             next_moves_[pos] = {pos};
//         }
//     }
//     //if next_moves is not empty, then still in opening phase and return
//     // these moves
//     if (!next_moves_.empty())
//     {
//         return;
//     }
//     //iterate through each position of the board if not in opening
//     else
//     {
//         for (Position pos : board_.all_positions())
//         {
//             // get the all the gains from that position
//             Position_set currPos = evaluate_position_(pos);
//
//             //if there are gains, that means its a valid move
//             if (!currPos.empty())
//             {
//                 //add that pos to the next_moves_ map as the key
//                 //set the gains of that pos as the value
//                 next_moves_[pos] = currPos;
//             }
//         }
//     }
// }
//
// bool
// Model::advance_turn_()
// {
//     // TODO: HELPER FUNCTION
//     //change the current turn to the other player
//     turn_ = other_player(turn_);
//     //compute next moves
//     compute_next_moves_();
//     //if next_moves_ is empty.  there arent any valid moves, so return false
//     // to whether current player has a valid move
//     if (next_moves_.empty())
//     {
//         return false;
//     }
//
//     //if next_moves_ is not empty, return true that there are valid moves for
//     // the next player
//     return true;
// }
//
// void
// Model::set_game_over_()
// {
//     // TODO: HELPER FUNCTION
//     turn_ = Player::neither;
//     int darkPoints = board_.count_player(Player::dark);
//     int lightPoints = board_.count_player(Player::light);
//     if (darkPoints > lightPoints)
//     {
//         winner_ = Player::dark;
//     }
//     else
//     {
//         winner_ = Player::light;
//     }
// }
//
// void
// Model::really_play_move_(Move move)
// {
//     // TODO: HELPER FUNCTION
//     //move = (position, position_set of gains)
//     //set position on board
//     //advance turn either player can still play (HAVE AVAILABLE MOVES)
//     //advance turn twice if next player cant play
//
//     //get the move and then use that to update board. set that position to turn
//     board_.set_all(move.second, turn_);
//     //check if next player can play
//     bool check = advance_turn_();
//     if (!check)
//     {
//         //if cant play, then advance turn one more time and check if he can play
//         check = advance_turn_();
//         if (!check)
//         {
//             //if not, both players cant play and its gg
//             set_game_over_();
//         }
//     }
// }
