
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "std_lib_facilities_3.h"

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
	cout<<"programmed in c++ using sfml graphic and audio libraries\n"
	<<"The base pong game logic i borrowed and modified to how i saw fit so writing the pong logic actully didn't take too long\n"
	<<"The pictures used were created by me in photoshop and as a practical prank i borrowed my good friend johns head, thanks john!\n"
	<<"it really took the majority of the time figuring out how to get all the libraries to work correctly and its the first time ive compiled an installer\n"
	<<"mostly i did this for my entertainment and learning, but if you like pong and awesome music enjoy! :)\n\n";
	cout<<"Hope you enjoy my friends\n";
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    // Define some constants
    const float pi = 3.14159f;
    const int gameWidth = 800;
    const int gameHeight = 600;
    sf::Vector2f paddleSize(25, 100);
    float ballRadius = 10.f;
    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "Alan's Pong");
    window.setVerticalSyncEnabled(true);

	//Load background
	sf::Texture background;
	if(!background.loadFromFile("resources/pong_background.jpg"))
		return EXIT_FAILURE;
	sf::Sprite picture;
	picture.setTexture(background);

	//Load ball picture
	sf::Texture john;
	if(!john.loadFromFile("resources/john1.png"))
		return EXIT_FAILURE;
	sf::Sprite ball_pic;
	ball_pic.setTexture(john);
	ball_pic.setOrigin(25,25);
	ball_pic.setPosition(400,300);

	//Load paddle picture
	sf:: Texture pad;
	pad.loadFromFile("resources/paddle.png");
	sf::Sprite right_pic;
	sf::Sprite left_pic;

	right_pic.setTexture(pad);
	right_pic.setOrigin(12.5,50);
	left_pic.setTexture(pad);
	left_pic.setRotation(180);
	left_pic.setOrigin(12.5,50);

	// Load Music used in the game
	sf::Music song;
	if(!song.openFromFile("resources/song.ogg"))
		return EXIT_FAILURE;
	song.setPlayingOffset(sf::seconds(30));
	song.setVolume(20);
	song.setLoop(true);
    // Load the sounds used in the game
    sf::SoundBuffer ballSoundBuffer;
	sf::SoundBuffer winSound;
	sf::SoundBuffer loseSound;

	if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    sf::Sound ballSound(ballSoundBuffer);
	ballSound.setVolume(50);

	if (!loseSound.loadFromFile("resources/lose.wav"))
        return 2;
	sf::Sound lose_sound(loseSound);

	if (!winSound.loadFromFile("resources/win.wav"))
        return 3;
	sf::Sound win_sound(winSound);


    // Create the left paddle
    sf::RectangleShape leftPaddle;
    leftPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    leftPaddle.setOutlineThickness(3);
    leftPaddle.setOutlineColor(sf::Color::Black);
    leftPaddle.setFillColor(sf::Color(100, 100, 200));
    leftPaddle.setOrigin(paddleSize / 2.f);

    // Create the right paddle
    sf::RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    rightPaddle.setOutlineThickness(3);
    rightPaddle.setOutlineColor(sf::Color::Black);
    rightPaddle.setFillColor(sf::Color(200, 100, 100));
    rightPaddle.setOrigin(paddleSize / 2.f);

    // Create the ball
    sf::CircleShape ball;
    ball.setRadius(ballRadius - 3);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(sf::Color::Black);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Initialize the pause message
    sf::Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(40);
    pauseMessage.setPosition(170.f, 150.f);
    pauseMessage.setColor(sf::Color::White);
    pauseMessage.setString("Welcome to alan's pong!\nPress space to start the game");

    // Define the paddles properties
    sf::Clock AITimer;
    const sf::Time AITime   = sf::seconds(0.06f);
    const float paddleSpeed = 400.f;
    float rightPaddleSpeed  = 0.f;
    const float ballSpeed   = 500.f;
    float ballAngle         = 0.f; // to be changed later

    sf::Clock clock;
    bool isPlaying = false;
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) || 
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Space key pressed: play
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
            {
                if (!isPlaying)
                {
                    // (re)start the game
                    isPlaying = true;
                    clock.restart();
					song.play();

                    // Reset the position of the paddles and ball
                    leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                    rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                    ball.setPosition(gameWidth / 2, gameHeight / 2);

                    // Reset the ball angle
                    do
                    {
                        // Make sure the ball initial angle is not too much vertical
                        ballAngle = (std::rand() % 360) * 2 * pi / 360;
                    }
                    while (std::abs(std::cos(ballAngle)) < 0.9f);
                }
            }
        }

        if (isPlaying)
        {
            float deltaTime = clock.restart().asSeconds();

            // Move the player's paddle
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
               (leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
            {
                leftPaddle.move(0.f, -paddleSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
               (leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
            {
                leftPaddle.move(0.f, paddleSpeed * deltaTime);
            }

            // Move the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
            {
                rightPaddle.move(0.f, rightPaddleSpeed * deltaTime);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.getElapsedTime() > AITime)
            {
                AITimer.restart();
                if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 2)
                    rightPaddleSpeed = paddleSpeed/2;
                else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 2)
                    rightPaddleSpeed = -paddleSpeed/2;
                else
                    rightPaddleSpeed = 0.f;
            }

            // Move the ball
            float factor = ballSpeed * deltaTime;
            ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

            // Check collisions between the ball and the screen
            if (ball.getPosition().x - ballRadius < 0.f)
            {
                isPlaying = false;
				lose_sound.play();
                pauseMessage.setString("You lost !\nPress space to restart or\nescape to exit");
            }
            if (ball.getPosition().x + ballRadius > gameWidth)
            {
                isPlaying = false;
				win_sound.play();
                pauseMessage.setString("You won !\nPress space to restart or\nescape to exit");
            }
            if (ball.getPosition().y - ballRadius < 0.f)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
            }
            if (ball.getPosition().y + ballRadius > gameHeight)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
            }

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 && 
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > leftPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right Paddle
            if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
                ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
            }
        }

        // Clear the window
        window.clear();
		window.draw(picture);

        if (isPlaying)
        {
            // Draw the paddles and the ball
			ball_pic.setPosition(ball.getPosition());//set picture position to ball positon
			left_pic.setPosition(leftPaddle.getPosition());
			right_pic.setPosition(rightPaddle.getPosition());
            window.draw(left_pic);
            window.draw(right_pic);
            window.draw(ball_pic);
        }
        else
        {
            // Draw the pause message
            window.draw(pauseMessage);
			window.draw(ball_pic);
        }

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
