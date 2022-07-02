#include "messages.hpp"

using namespace std;

Messages::Messages(Game &game, sf::Font &font, int text_size)
    : m_game(game), m_text_size(text_size),
      m_message_title(m_text_default, font),
      m_message_goal(m_goal, font),
      m_message_curr_score(m_curr_score, font),
      m_message_best_score(m_best_score, font)
{
}

void Messages::load_messages()
{
    sf::Text::Style text_style(sf::Text::Style::Bold);
    sf::Color text_fill(sf::Color::Cyan);
    sf::Color text_outline(209, 207, 207);
    int outline_thickness = 1;

    m_message_title.set_properties(m_text_size, text_style, text_fill, text_outline, outline_thickness);
    m_message_goal.set_properties(m_text_size, text_style, text_fill, text_outline, outline_thickness);
    m_message_curr_score.set_properties(m_text_size, text_style, text_fill, text_outline, outline_thickness);
    m_message_best_score.set_properties(m_text_size, text_style, text_fill, text_outline, outline_thickness);
}

void Messages::place_messages(sf::RenderWindow &window, int cell_size)
{
    double x = window.getSize().x / 2 - cell_size * 2;
    double y = window.getSize().y / 2 - cell_size * 2;

    double coord_x = window.getSize().x / 2;
    double coord_y = y / 2;

    m_message_title.set_pos(coord_x, coord_y);

    m_message_goal.set_pos(x / 2, window.getSize().y / 2);

    m_curr_score = m_text_curr_score + to_string(m_game.get_curr_score());
    m_message_curr_score.set_str(m_curr_score);
    m_message_curr_score.set_pos(window.getSize().x - x / 2, window.getSize().y / 2 - cell_size);

    m_best_score = m_text_best_score + to_string(m_game.get_best_score(m_game.get_goal()));
    m_message_best_score.set_str(m_best_score);
    m_message_best_score.set_pos(window.getSize().x - x / 2, window.getSize().y / 2 + cell_size);
}

void Messages::change_messages(bool won, bool lost)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        m_message_title.set_str(m_text_default);
    }
    else if (!won && !(lost) && !m_game.get_curr_score())
    {
        string goal = m_text_goal + to_string(m_game.get_goal());
        m_message_goal.set_str(goal);
    }
    else if (won)
    {
        m_message_title.set_str(m_text_won);
        string goal = m_text_goal + to_string(m_game.get_goal());
        m_message_goal.set_str(goal);
    }
    else
    {
        m_message_title.set_str(m_text_lost);
        string goal = m_text_goal + to_string(m_game.get_goal());
        m_message_goal.set_str(goal);
    }
}

void Messages::show_messages(sf::RenderWindow &window)
{
    m_message_title.show_message(window);

    m_message_goal.show_message(window);

    m_message_curr_score.show_message(window);

    m_message_best_score.show_message(window);
}