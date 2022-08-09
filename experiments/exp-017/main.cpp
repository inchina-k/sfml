#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

using namespace std;

class Animation
{
public:
    enum class State
    {
        GoDown,
        GoUp,
        GoLeft,
        GoRight,
        StandDown,
        StandUp,
        StandLeft,
        StandRight
    };

private:
    sf::RenderWindow &m_window;
    sf::Texture m_texture;
    vector<vector<unique_ptr<sf::Sprite>>> m_frames;
    int m_counter;
    size_t m_frame_index;
    size_t m_anim_index;
    State m_curr_state = State::StandDown;

    int m_x = 0,
        m_y = 0;

    float m_w, m_h;

public:
    Animation(sf::RenderWindow &window)
        : m_window(window), m_counter(0), m_frame_index(0), m_anim_index(0)
    {
        if (!m_texture.loadFromFile("data/character.png"))
        {
            cout << "file is missing" << endl;
            exit(1);
        }

        m_w = m_texture.getSize().x / 3;
        m_h = m_texture.getSize().y / 4;

        load();
    }

    void draw(sf::Vector2f &coords)
    {
        if (m_curr_state == State::StandDown || m_curr_state == State::StandUp || m_curr_state == State::StandLeft || m_curr_state == State::StandRight)
        {
            m_frame_index = 0;
        }

        m_frames[m_anim_index][m_frame_index]->setPosition(coords);
        m_window.draw(*m_frames[m_anim_index][m_frame_index]);

        if (++m_counter == 10)
        {
            if (++m_frame_index == 3)
            {
                m_frame_index = 0;
            }

            m_counter = 0;
        }
    }

    void set_state(State state)
    {
        m_curr_state = state;

        if (m_curr_state == State::GoDown || m_curr_state == State::GoUp || m_curr_state == State::GoLeft || m_curr_state == State::GoRight)
        {
            m_anim_index = static_cast<int>(m_curr_state);
        }
    }

    State get_curr_state() const
    {
        return m_curr_state;
    }

private:
    void load()
    {
        m_frames.resize(4);

        for (size_t i = 0; i < 4; i++)
        {
            for (size_t j = 0; j < 3; j++)
            {
                m_frames[i].push_back(make_unique<sf::Sprite>(m_texture, sf::IntRect(j * m_w, i * m_h, m_w, m_h)));
                m_frames[i].back()->scale(5, 5);
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
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                anim.set_state(Animation::State::GoDown);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            {
                anim.set_state(Animation::State::GoUp);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                anim.set_state(Animation::State::GoLeft);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                anim.set_state(Animation::State::GoRight);
            }
            else if (anim.get_curr_state() == Animation::State::GoDown)
            {
                anim.set_state(Animation::State::StandDown);
            }
            else if (anim.get_curr_state() == Animation::State::GoUp)
            {
                anim.set_state(Animation::State::StandUp);
            }
            else if (anim.get_curr_state() == Animation::State::GoLeft)
            {
                anim.set_state(Animation::State::StandLeft);
            }
            else if (anim.get_curr_state() == Animation::State::GoRight)
            {
                anim.set_state(Animation::State::StandRight);
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