#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "field.hpp"
#include "renderer.hpp"
#include "messages.hpp"

class Themes
{
    const int m_num_of_themes = 4;

    int m_current_theme_index;

    std::vector<sf::Color> m_background_colors = {sf::Color(3, 19, 43), sf::Color(33, 3, 31), sf::Color(2, 31, 14), sf::Color(20, 11, 1)};

    std::vector<std::pair<sf::Color, sf::Color>> m_field_colors = {{sf::Color(189, 203, 219), sf::Color(225, 237, 252)},
                                                                   {sf::Color(219, 189, 206), sf::Color(252, 225, 243)},
                                                                   {sf::Color(189, 219, 202), sf::Color(225, 252, 230)},
                                                                   {sf::Color(219, 197, 189), sf::Color(252, 233, 225)}};

    std::vector<std::pair<sf::Color, sf::Color>> m_messages_colors = {{sf::Color::Cyan, sf::Color(207, 208, 209)},
                                                                      {sf::Color::Magenta, sf::Color(209, 207, 208)},
                                                                      {sf::Color::Green, sf::Color(207, 209, 208)},
                                                                      {sf::Color::Red, sf::Color(209, 207, 207)}};

    sf::Texture m_texture_winter;
    sf::Texture m_texture_spring;
    sf::Texture m_texture_summer;
    sf::Texture m_texture_fall;

    std::vector<sf::Texture> m_pieces_textures;

    void change_background_color(sf::Color &background);

    void change_field_color(Field &field);

    void change_messages_color(Messages &messages);

    void change_texture(Renderer &renderer);

public:
    Themes();

    void change_theme(int theme_index, sf::Color &background_color, Field &field, Messages &messages, Renderer &renderer);

    int get_number_of_themes() const;

    sf::Color get_default_background_color() const;

    sf::Texture get_default_texture() const;
};