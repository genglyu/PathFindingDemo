#include <SFML/Graphics.hpp> // Looks like this is only a display issue in VSCode. The code intellisense is working fine.
#include <stdio.h>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "CMake SFML Project");
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}
