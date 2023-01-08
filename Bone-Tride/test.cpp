//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <cstdlib> // for rand() and srand()
//#include <ctime>   // for time()
//
//using namespace std;
//
//int main()
//{
//    // Seed the random number generator
//    srand(time(NULL));
//
//    // Create the window
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Random Blocks");
//
//    // Set the window's vertical sync enabled
//    window.setVerticalSyncEnabled(true);
//
//    // Load the texture
//    sf::Texture texture;
//    if (!texture.loadFromFile("assets/textures/entity/enemy/zombie.png"))
//        return 1;
//
//    // Create a list of sprites to draw
//    std::vector<sf::Sprite> sprites;
//
//    // Run the program as long as the window is open
//    while (window.isOpen())
//    {
//        // Check for events
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            // Close the window if the "close" button is clicked
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        // Clear the window with a white color
//        window.clear(sf::Color::White);
//
//        // Generate a new random sprite every 10 frames
//        static int frameCounter = 0;
//        if (frameCounter % 200 == 0)
//        {
//            sf::Sprite sprite(texture);
//
//            sprite.setScale(sf::Vector2f(3, 3));
//            sprite.setOrigin(sf::Vector2f(8 / 2, 8 / 2));
//
//            // Generate a random position on the screen border
//            int side = rand() % 4;
//            if (side == 0)
//                sprite.setPosition(-50, rand() % 600);
//            else if (side == 1)
//                sprite.setPosition(rand() % 800, -50);
//            else if (side == 2)
//                sprite.setPosition(850, rand() % 600);
//            else
//                sprite.setPosition(rand() % 800, 650);
//
//            sprites.push_back(sprite);
//        }
//        ++frameCounter;
//
//        // Get the mouse position in window coordinates
//        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
//
//        // Update the position and rotation of all sprites
//        for (sf::Sprite& sprite : sprites)
//        {
//            // Calculate the direction vector towards the mouse cursor
//            sf::Vector2f direction = sf::Vector2f(mousePosition) - sprite.getPosition();
//
//            // Normalize the direction vector
//            direction /= std::sqrt(direction.x * direction.x + direction.y * direction.y);
//
//            // Move the sprite towards the mouse cursor
//            sprite.move(direction * 2.0f);
//
//            // Rotate the sprite towards the mouse cursor
//            sprite.setRotation(std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f);
//        }
//
//        // Draw all sprites
//        for (const sf::Sprite& sprite : sprites)
//            window.draw(sprite);
//
//        // Display the window
//        window.display();
//    }
//
//    return 0;
//}