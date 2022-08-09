#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

using namespace std;

class Animation
{
    sf::RenderWindow &m_window;
    vector<unique_ptr<sf::Texture>> m_textures;
    vector<unique_ptr<sf::Sprite>> m_frames;
    int m_counter;
    size_t m_index;

public:
    Animation(sf::RenderWindow &window, const string &path)
        : m_window(window), m_counter(0), m_index(0)
    {
        for (int i = 1; i < 7; i++)
        {
            string name = path + to_string(i) + ".png";

            m_textures.push_back(make_unique<sf::Texture>());
            m_textures.back()->loadFromFile(name);
        }

        for (auto &t : m_textures)
        {
            m_frames.push_back(make_unique<sf::Sprite>());
            m_frames.back()->setTexture(*t);

            m_frames.back()->scale(m_frames.back()->getScale().x / 5, m_frames.back()->getScale().y / 5);
            m_frames.back()->setOrigin(m_frames.back()->getLocalBounds().width / 2, m_frames.back()->getLocalBounds().height / 2);
        }
    }

    void draw(sf::Vector2f &coords)
    {
        m_frames[m_index]->setPosition(coords);
        m_window.draw(*m_frames[m_index]);

        if (++m_counter == 3)
        {
            if (++m_index == 6)
            {
                m_index = 0;
            }

            m_counter = 0;
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    string path = "data/";
    Animation anim(window, path);

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
        }

        window.clear();
        anim.draw(coords);
        window.display();
    }

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}