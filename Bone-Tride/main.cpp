#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()

#include "Player.h"
#include "Zombie.h"

using namespace std;

const int WIN_WIDTH	    = 1000;
const int WIN_HEIGHT	= 700;

const string gameName		= "Bone Tride";
const string gameVer        = "v0.0.6";
const string auth           = "by Roomy";

int main()
{
    // Game
    int playerScore = 0;
    int spawnInt = 300;

    // Seed the random number generator
    srand(time(NULL));

    // Create the window
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), gameName + " " + gameVer + " " + auth);

    // Set the window's vertical sync enabled
    //window.setVerticalSyncEnabled(true);

    // Set the frame rate limit to 60 frames per second
    window.setFramerateLimit(60);

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
    {
        cout << "Failed to load arial.ttf" << endl;
        return 1;
    }

    // Load the background texture
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/textures/background/background.png"))
    {
        cout << "Failed to load background.png" << endl;
        return 1;
    }

    // Create the background sprite
    sf::Sprite background;
    background.setTexture(backgroundTexture);
    background.setScale(WIN_WIDTH / static_cast<float>(backgroundTexture.getSize().x), 
        WIN_HEIGHT / static_cast<float>(backgroundTexture.getSize().y));

    // Load the player texture
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/textures/entity/player/player.png"))
    {
        cout << "Failed to load player.png" << endl;
        return 1;
    }

    // Load the zombie texture
    sf::Texture zombieTexture;
    if (!zombieTexture.loadFromFile("assets/textures/entity/enemy/zombie.png"))
    {
        cout << "Failed to load zombie.png" << endl;
        return 1;
    }

    // Create a list of enemies to draw
    vector<sf::Sprite> enemies;

    // Load the bullet texture
    sf::Texture bulletTexture;
    if (!bulletTexture.loadFromFile("assets/textures/projectile/bullet.png"))
    {
        cout << "Failed to load bullet.png" << endl;
        return 1;
    }

    // Load the shooting sound
    sf::SoundBuffer shootingBuffer;
    if (!shootingBuffer.loadFromFile("assets/audio/fire.wav"))
    {
        cout << "Failed to load fire.wav" << endl;
        return 1;
    }
    sf::Sound shootingSound;
    shootingSound.setBuffer(shootingBuffer);

    // Load the playerHurt sound
    sf::SoundBuffer playerHitBuffer;
    if (!playerHitBuffer.loadFromFile("assets/audio/playerHit.wav"))
    {
        cout << "Failed to load playerHit.wav" << endl;
        return 1;
    }
    sf::Sound playerHitSound;
    playerHitSound.setBuffer(playerHitBuffer);

    // Load the playerHurt sound
    sf::SoundBuffer enemyHitBuffer;
    if (!enemyHitBuffer.loadFromFile("assets/audio/enemyHit.wav"))
    {
        cout << "Failed to load enemyHit.wav" << endl;
        return 1;
    }
    sf::Sound enemyHitSound;
    enemyHitSound.setBuffer(enemyHitBuffer);

    // Load the upgrade sound
    sf::SoundBuffer upgradeBuffer;
    if (!upgradeBuffer.loadFromFile("assets/audio/upgrade.wav"))
    {
        cout << "Failed to load upgrade.wav" << endl;
        return 1;
    }
    sf::Sound upgradeSound;
    upgradeSound.setBuffer(upgradeBuffer);

    // Create the player sprite
    sf::Sprite player;
    player.setTexture(playerTexture);
    player.setOrigin(playerWidth / 2, playerHeight / 2);
    player.setPosition(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    player.setScale(sf::Vector2f(playerScale, playerScale));

    // Set the player velocity
    sf::Vector2f velocity(0.0f, 0.0f);
    float speed = 200.0f;

    sf::Clock timer;
    sf::Clock shootTimer;
    sf::Clock attackTimer;
    sf::Clock spawnTimer;
    float shootDelay        = .1f; // Delay in seconds
    float attackDelay       = .5f; // Delay in seconds
    float decreaseInterval  = .5f;
    bool canShoot   = true; // Flag to track if the delay has passed
    bool canAttack  = true; // Flag to track if the delay has passed

    // Creating a view (camera)
    sf::View view;

    // Create a list of bullets
    vector<sf::Sprite> bullets;

    // Run the main game loop
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Check score | add buffs
        if (playerScore > 20 && !upgrade1)
        {
            upgradeSound.play();
            playerDamage = 10;
            upgrade1 = true;
            cout << "Upgrade1!" << endl;
        }
        else if (playerScore > 50 && !upgrade2)
        {
            upgradeSound.play();
            playerDamage = 20;
            upgrade2 = true;
            cout << "Upgrade2!" << endl;
        }
        else if (playerScore > 100 && !upgrade3)
        {
            upgradeSound.play();
            playerDamage = 40;
            upgrade3 = true;
            cout << "Upgrade3!" << endl;
        }

        // Get the mouse position in window coordinates
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Convert the mouse position to world coordinates
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        mousePosF = window.mapPixelToCoords(mousePos);

        // Calculate the angle between the player and mouse position
        sf::Vector2f playerPos = player.getPosition();
        sf::Vector2f direction = mousePosF - playerPos;
        float angle = atan2(direction.y, direction.x) * 180.0f / 3.114159;

        // Normalize the direction vector
        direction = direction / sqrt(direction.x * direction.x + direction.y * direction.y);

        // Set the player rotation
        player.setRotation(angle);

        // Set view size
        view.setSize(WIN_WIDTH, WIN_HEIGHT);
        view.setCenter(WIN_WIDTH / 2, WIN_HEIGHT / 2);

        // Set the view to player position
        view.setCenter(playerPos);

        // Update the player velocity
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            velocity.y = -speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            velocity.y = speed;
        }
        else
        {
            velocity.y = 0.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            velocity.x = -speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            velocity.x = speed;
        }
        else
        {
            velocity.x = 0.0f;
        }

        // Update the flag based on the elapsed time
        if (!canShoot && shootTimer.getElapsedTime().asSeconds() > shootDelay)
        {
            canShoot = true;
        }

        // Check if the left click is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            // Check if the delay has passed
            if (canShoot)
            {
                // Set the flag to false
                canShoot = false;

                // Reset the timer
                shootTimer.restart();

                // Create a bullet
                sf::Sprite bullet;
                bullet.setTexture(bulletTexture);
                bullet.setOrigin(1.5f, 0.5f);
                bullet.setPosition(playerPos);
                bullet.setRotation(angle);
                bullet.setScale(sf::Vector2f(2, 2));

                // Add the bullet to the vector
                bullets.push_back(bullet);

                // Play the shooting sound
                shootingSound.play();
            }
        }

        // Update the player position
        sf::Time deltaTime = timer.restart();
        player.move(velocity * deltaTime.asSeconds());

        // Update the bullet positions
        for (auto& bullet : bullets)
        {
            sf::Vector2f bulletVelocity(cos(bullet.getRotation() * 3.14159 / 180.0f), sin(bullet.getRotation() * 3.14159 / 180.0f));
            bulletVelocity *= 1000.0f;
            bullet.move(bulletVelocity * deltaTime.asSeconds());
        }

        if (spawnTimer.getElapsedTime().asSeconds() > decreaseInterval)
        {
            if (spawnInt <= 50)
            {
                spawnInt = 50;
            }
            // Decrease the integer value

            cout << "spawnInt: " << spawnInt << endl;
            cout << "decreaseInterval: " << decreaseInterval << endl;
            spawnInt--;

            // Reset the timer
            spawnTimer.restart();
        }

        // Generate a new random sprite every 200 frames
        static int frameCounter = 0;
        if (frameCounter % spawnInt == 0)
        {
            sf::Sprite sprite(zombieTexture);

            sprite.setScale(sf::Vector2f(3, 3));
            sprite.setOrigin(sf::Vector2f(8 / 2, 8 / 2));

            // Generate a random position on the screen border
            int side = rand() % 4;
            if (side == 0)
                sprite.setPosition(-50, rand() % 600);
            else if (side == 1)
                sprite.setPosition(rand() % WIN_HEIGHT, -50);
            else if (side == 2)
                sprite.setPosition(WIN_WIDTH, rand() % 600);
            else
                sprite.setPosition(rand() % WIN_WIDTH, WIN_HEIGHT);

            enemies.push_back(sprite);
        }
        ++frameCounter;

        // Update the position and rotation of all enemies
        for (sf::Sprite& sprite : enemies)
        {
            // Calculate the direction vector towards the mouse cursor
            sf::Vector2f direction = sf::Vector2f(playerPos) - sprite.getPosition();

            // Normalize the direction vector
            direction /= sqrt(direction.x * direction.x + direction.y * direction.y);

            // Move the sprite towards the mouse cursor
            sprite.move(direction * 1.0f);

            // Rotate the sprite towards the mouse cursor
            sprite.setRotation(atan2(direction.y, direction.x) * 180.0f / 3.14159265f);
        }

        // Update the flag based on the elapsed time
        if (!canAttack && attackTimer.getElapsedTime().asSeconds() > attackDelay)
        {
            canAttack = true;
        }

        // Check for collisions between the player and enemies
        for (int j = 0; j < enemies.size(); j++)
        {
            if (canAttack)
            {
                if (player.getGlobalBounds().intersects(enemies[j].getGlobalBounds()))
                {

                    // The player has been hit: damage the player
                    playerHealth -= zombieDamage;
                    playerHitSound.play();

                    if (playerHealth <= 0)
                    {
                        // The player is dead: game over
                        cout << "Game Over" << endl;
                        //window.close();
                    }

                    // Set the flag to false
                    canAttack = false;

                    // Reset the timer
                    attackTimer.restart();

                    break;
                }
            }
        }

        // Check for collisions between bullets and enemies
        for (int i = 0; i < bullets.size(); i++)
        {
            for (int j = 0; j < enemies.size(); j++)
            {
                if (bullets[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds()))
                {
                    // Play enemyHit.wav
                    enemyHitSound.play();

                    // The bullet has hit the enemy: remove the bullet and damage the enemy
                    bullets.erase(bullets.begin() + i);
                    zombieHealth -= playerDamage;

                    if (zombieHealth <= 0)
                    {
                        // The enemy is dead: remove it
                        enemies.erase(enemies.begin() + j);
                        zombieHealth = 100;
                        playerScore += 1;
                    }

                    break;
                }
            }
        }

        // Set the view
        //window.setView(view);

        // Clear the window to blue
        window.clear();

        // Draw the background
        window.draw(background);

        for (sf::Sprite& zombie : enemies)
        {
            // Create the enemy health bar
            sf::RectangleShape zombieHealthBar(sf::Vector2f(0, 0));
            zombieHealthBar.setPosition(zombie.getPosition().x - zombieWidth / 2, zombie.getPosition().y + playerHeight / 2 + 10);
            zombieHealthBar.setFillColor(sf::Color::Green);

            // Update the health bar and text
            zombieHealthBar.setSize(sf::Vector2f(zombieWidth * (zombieHealth / 100.f), 10));

            if (zombieHealth < 100)
            {
                zombieHealthBar.setFillColor(sf::Color::Yellow);
            }
            if (zombieHealth < 20)
            {
                zombieHealthBar.setFillColor(sf::Color::Red);
            }
            if (zombieHealth < 0)
            {
                zombieHealth = 0;
            }

            //window.draw(zombieHealthBar);

        }

        // Create the player health bar
        sf::RectangleShape playerHealthBar(sf::Vector2f(0, 0));
        playerHealthBar.setPosition(20, 20);
        playerHealthBar.setFillColor(sf::Color::Green);

        // Create the player health text
        sf::Text playerHealthText;
        playerHealthText.setFont(font); // font is a sf::Font that you must load
        playerHealthText.setString(to_string(playerHealth));
        playerHealthText.setCharacterSize(20);
        playerHealthText.setFillColor(sf::Color::White);
        //playerHealthText.setOrigin(playerHealthText.getGlobalBounds().width / 2, playerHealthText.getGlobalBounds().height / 2);
        playerHealthText.setPosition(20, 40);

        // Update the player health bar and text
        playerHealthBar.setSize(sf::Vector2f(100 * (playerHealth / 100.f), 20));
        playerHealthText.setString("Health: " + to_string(playerHealth));

        // Create a player score system
        sf::Text playerScoreText;
        playerScoreText.setFont(font);
        playerScoreText.setString("Score: " + to_string(playerScore));
        playerScoreText.setCharacterSize(10);
        playerScoreText.setFillColor(sf::Color::White);
        playerScoreText.setPosition(20, 60);

        // Buch of stuff about health bar and stuff idk
        if (playerHealth < 100)
        {
            playerHealthBar.setFillColor(sf::Color::Yellow);
        }
        if (playerHealth < 20)
        {
            playerHealthBar.setFillColor(sf::Color::Red);
        }
        if (playerHealth < 0)
        {
            playerHealth = 0;
        }

        // Render the health bar and text
        window.draw(playerHealthBar);
        window.draw(playerHealthText);
        window.draw(playerScoreText);

        // Draw the player
        window.draw(player);

        // Draw the bullets
        for (const auto& bullet : bullets)
        {
            window.draw(bullet);
        }

        // Draw all zombies
        for (const sf::Sprite& sprite : enemies)
        {
            window.draw(sprite);
        }

        // Display the window contents
        window.display();
    }

    return 0;
}