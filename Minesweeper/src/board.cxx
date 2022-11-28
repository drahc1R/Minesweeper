/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

#include "board.hxx"
#include <algorithm>

using namespace ge211;

Board::Board(Dimensions dims)
        : dims_(dims)
{
    if (dims_.width < 2 || dims_.height < 2) {
        throw Client_logic_error("Board::Board: dims too small");
    }

    if (dims_.width > Position_set::coord_limit ||
        dims_.height > Position_set::coord_limit) {
        throw Client_logic_error("Board::Board: dims too large");
    }
}

Board::Dimensions
Board::dimensions() const
{
    return dims_;
}

bool
Board::good_position(Position pos) const
{
    return 0 <= pos.x && pos.x < dims_.width &&
           0 <= pos.y && pos.y < dims_.height;
}

Type
Board::operator[](Position pos) const
{
    bounds_check_(pos);
    return get_(pos);
}

Board::reference
Board::operator[](Position pos)
{
    bounds_check_(pos);
    return reference(*this, pos);
}

size_t
Board::numTypes(Type type) const
{
    switch (type)
    {
    case Type::bomb:
        return bombs.size();
    case Type::seen:
        return seen.size();
    case Type::flag:
        return flags.size();
    default:
        return dims_.width * dims_.height -
               bombs.size() - seen.size();
    }
}

/// Not needed
Board::Rectangle
Board::center_positions() const
{
    return Rectangle::from_top_left({dims_.width / 2 - 1,
                                     dims_.height / 2 - 1},
                                    {2, 2});
}

static std::vector<Board::Dimensions>
build_directions()
{
    std::vector<Board::Dimensions> result;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx || dy) {
                result.push_back({dx, dy});
            }
        }
    }

    return result;
}

std::vector<Board::Dimensions> const&
Board::all_directions()
{
    static std::vector<Dimensions> result = build_directions();
    return result;
}

Board::Rectangle
Board::all_positions() const
{
    return Rectangle::from_top_left(the_origin, dims_);
}


/// is it necessary? no.
// bool
// operator==(Board const& b1, Board const& b2)
// {
//     return b1.dims_ == b2.dims_ &&
//            b1.light_ == b2.light_ &&
//            b1.dark_ == b2.dark_;
// }


/// edited to return bomb, safe for player board?
/// Might have bug (might not need the else statement)
Type
Board::get_(Position pos) const
{
    if (bombs[pos]) {
        return Type::bomb;
    } else if (safe[pos]) {
        return Type::safe;
    } else {
        return Type::flag;
    }
}

void
Board::set_(Position pos, Type type)
{
    switch (type) {
    case Type::bomb:
        bombs[pos] = true;
        safe[pos] = false;
        break;

    default:
        bombs[pos] = false;
        safe[pos] = true;
    }
}


///Sets all positions to bombs or safe. defaults to everything safe
void
Board::set_all(Position_set pos_set, Type type)
{
    switch (type) {
    case Type::bomb:
        bombs |= pos_set;
        safe &= ~pos_set;
        break;

    case Type::safe:
        safe |= pos_set;
        bombs &= ~pos_set;
        break;

    /// Unknown might not be necessary
    case Type::unknown:
        unknown |= pos_set;
        safe &= ~pos_set;
        bombs &= ~pos_set;

    default:
        unknown |= pos_set;
        safe |= ~pos_set;
        bombs &= ~pos_set;
    }
}

void
Board::bounds_check_(Position pos) const
{
    if (!good_position(pos)) {
        throw Client_logic_error("Board: position out of bounds");
    }
}

bool
operator!=(Board const& b1, Board const& b2)
{
    return !(b1 == b2);
}

std::ostream&
operator<<(std::ostream& os, Board const& board)
{
    Board::Dimensions dims = board.dimensions();

    for (int y = 0; y < dims.height; ++y) {
        for (int x = 0; x < dims.width; ++x) {
            os << board[{x, y}];
        }
        os << "\n";
    }

    return os;
}

Board::reference::reference(Board& board, Position pos) noexcept
        : board_(board),
          pos_(pos)
{ }

Board::reference&
Board::reference::operator=(reference const& that) noexcept
{
    *this = Type(that);
    return *this;
}

Board::reference&
Board::reference::operator=(Type type) noexcept
{
    board_.set_(pos_, type);
    return *this;
}

Board::reference::operator Type() const noexcept
{
    return board_.get_(pos_);
}

Board::multi_reference
Board::at_set(Position_set pos_set)
{
    return multi_reference(*this, pos_set);
}

Board::multi_reference::multi_reference(
        Board& board,
        Position_set pos_set) noexcept
        : board_(board),
          pos_set_(pos_set)
{ }

Board::multi_reference&
Board::multi_reference::operator=(Type type) noexcept
{
    board_.set_all(pos_set_, type);
    return *this;
}