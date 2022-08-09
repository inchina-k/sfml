#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

using namespace std;

class Animation
{
    sf::RenderWindow &m_window;
    sf::Texture m_texture;
    vector<vector<unique_ptr<sf::Sprite>>> m_frames;
    int m_counter;
    size_t m_frame_index;
    size_t m_anim_index;

    int m_x = 0, m_y = 0;
    float m_w, m_h;

public:
    Animation(sf::RenderWindow &window)
        : m_window(window), m_counter(0), m_frame_index(0), m_anim_index(0)
    {
        if (!m_texture.loadFromFile("data/Wolfpack.png"))
        {
            cout << "file is missing" << endl;
            exit(1);
        }

        m_w = m_texture.getSize().x / 8;
        m_h = m_texture.getSize().y / 8;

        load();
    }

    void draw(sf::Vector2f &coords)
    {
        m_frames[m_anim_index][m_frame_index]->setPosition(coords);
        m_window.draw(*m_frames[m_anim_index][m_frame_index]);

        if (++m_counter == 10)
        {
            if (++m_frame_index == 4)
            {
                m_frame_index = 0;
            }

            m_counter = 0;
        }
    }

    void set_anim_index(int index)
    {
        m_anim_index = index;
    }

private:
    void load()
    {
        m_frames.resize(4);

        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 4; j++)
            {
                m_frames[i].push_back(make_unique<sf::Sprite>(m_texture, sf::IntRect(j * m_w, i * m_h, m_w, m_h)));
                m_frames[i].back()->scale(3, 3);
                m_frames[i].back()->setOrigin(m_frames[i].back()->getLocalBounds().width / 2, m_frames[i].back()->getLocalBounds().height / 2);
            }
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    Animation anim(window);

    sf::Vector2f coords(window.getSize().x / 2, window.getSize().y / 2);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                anim.set_anim_index(0);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                anim.set_anim_index(1);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                anim.set_anim_index(2);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            {
                anim.set_anim_index(3);
            }
        }

        window.clear();
        anim.draw(coords);
        window.display();
    }

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}