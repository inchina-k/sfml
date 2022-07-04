#include "themes.hpp"

Themes::Themes()
    : m_current_theme_index(0)
{
    m_texture_winter.loadFromFile("data/images/2048 pixel_pieces winter.png");
    m_texture_spring.loadFromFile("data/images/2048 pixel_pieces spring.png");
    m_texture_summer.loadFromFile("data/images/2048 pixel_pieces summer.png");
    m_texture_fall.loadFromFile("data/images/2048 pixel_pieces fall.png");

    m_pieces_textures.push_back(m_texture_winter);
    m_pieces_textures.push_back(m_texture_spring);
    m_pieces_textures.push_back(m_texture_summer);
    m_pieces_textures.push_back(m_texture_fall);
}

void Themes::change_theme(int theme_index, sf::Color &background_color, Field &field, Messages &messages, Renderer &renderer)
{
    m_current_theme_index = theme_index;

    change_background_color(background_color);

    change_field_color(field);

    change_messages_color(messages);

    change_texture(renderer);
}

void Themes::change_background_color(sf::Color &background_color)
{
    background_color = m_background_colors[m_current_theme_index];
}

void Themes::change_field_color(Field &field)
{
    field.set_field_color(m_field_colors[m_current_theme_index]);
}

void Themes::change_messages_color(Messages &messages)
{
    messages.set_messages_color(m_messages_colors[m_current_theme_index]);
}

void Themes::change_texture(Renderer &renderer)
{
    renderer.set_texture(m_pieces_textures[m_current_theme_index]);
}

int Themes::get_number_of_themes() const
{
    return m_num_of_themes;
}

sf::Color Themes::get_default_background_color() const
{
    return m_background_colors.front();
}

sf::Texture Themes::get_default_texture() const
{
    return m_texture_winter;
}