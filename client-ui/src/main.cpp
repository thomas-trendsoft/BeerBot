#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    // basic assets
    sf::Color bgColor(173,128,24);
    sf::Font font;
    if (!font.loadFromFile("fonts/OpenSans-Regular.ttf"))
    {
      std::cout << "failed to load config files" << std::endl;
      return -1;
    }

    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // Titel Test
    sf::Text text;
    text.setPosition(20.0,20.0);
    text.setFont(font);
    text.setString("BeerBot Control");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Yellow);


    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(bgColor);

        // draw everything here...
        window.draw(text);

        // end the current frame
        window.display();
    }

    return 0;
}
