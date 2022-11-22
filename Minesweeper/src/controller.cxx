#include "controller.hxx"

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_),
          mouse_pos{0,0}
{ }


void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, mouse_pos);
}

//Custom function ------------------------------------
//on mouse move, have the piece follow the mouse.
void
Controller::on_mouse_move(ge211::Posn<int> position)
{
    //update the position of the mouse
    mouse_pos = position;
}

void
Controller::on_mouse_down(ge211::Mouse_button button, View::Position spos)
{
    //get the board position and create a move var
    Model::Position boardpos = view_.screen_to_board(spos);
    Move const *move;
    //play the move if game is not over, check for right button, and get that
    // move object from model to play it
    if (!model_.is_game_over() and button == ge211::Mouse_button::left and
    (move = model_.find_move(boardpos)))
    {
        model_.play_move(move->first);
    }
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

