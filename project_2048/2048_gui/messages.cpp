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
    sf::Color text_outline(207, 208, 209);
    int outline_thickness = 1;

    m_message_title.set_properties(m_text_size, text_style, text_fill, text_outline, outline_thickness);
    m_message_goal.set_properties(m_text_size, text_style, text_fill, text_outline, outline_thickness);
    m_message_curr_score.set_properties(m_text_size, text_style, text_fill, text_outline, outline_thickness);
    m_message_best_score.set_properties(m_text_size, text_style, text_fill, text_outline, outline_thickness);
}

void Messages::place_messages(sf::RenderWindow &window, double x, double y, int cell_size)
{
    m_message_title.set_pos(window.getSize().x / 2, y / 2);

    m_message_goal.set_pos(x / 2, window.getSize().y / 2);

    m_message_curr_score.set_pos(window.getSize().x - x / 2, window.getSize().y / 2 - cell_size);

    m_message_best_score.set_pos(window.getSize().x - x / 2, window.getSize().y / 2 + cell_size);
}

void Messages::set_title_default()
{
    m_message_title.set_str(m_text_default);
}

void Messages::set_title_game_won()
{
    m_message_title.set_str(m_text_won);
}

void Messages::set_title_game_lost()
{
    m_message_title.set_str(m_text_lost);
}

void Messages::change_message_goal()
{
    m_goal = m_text_goal + to_string(m_game.get_goal());
    m_message_goal.set_str(m_goal);
}

void Messages::change_message_curr_score()
{
    m_curr_score = m_text_curr_score + to_string(m_game.get_curr_score());
    m_message_curr_score.set_str(m_curr_score);
}

void Messages::change_message_best_score()
{
    m_best_score = m_text_best_score + to_string(m_game.get_best_score(m_game.get_goal()));
    m_message_best_score.set_str(m_best_score);
}

void Messages::set_messages_color(std::pair<sf::Color, sf::Color> &messages_color)
{
    m_message_title.set_color(messages_color);

    m_message_goal.set_color(messages_color);

    m_message_curr_score.set_color(messages_color);

    m_message_best_score.set_color(messages_color);
}

void Messages::show_messages(sf::RenderWindow &window)
{
    m_message_title.show_message(window);

    m_message_goal.show_message(window);

    m_message_curr_score.show_message(window);

    m_message_best_score.show_message(window);
}