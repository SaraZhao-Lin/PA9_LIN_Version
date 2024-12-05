#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <random>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float WHEEL_RADIUS = 200.0f; // Radius of the wheel
const int NUM_CARDS = 8;          // Number of card slots on the wheel

// Load a texture from a file
sf::Texture loadTexture(const std::string& path) {
    sf::Texture texture;
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load: " << path << ". Ensure the file is in the correct directory." << std::endl;
        system("pause"); // Prevent immediate exit
        exit(-1);
    }
    return texture;
}

// Initialize the wheel sprite
sf::Sprite initializeWheel(sf::Texture& wheelTexture) {
    sf::Sprite wheelSprite(wheelTexture);
    wheelSprite.setOrigin(wheelTexture.getSize().x / 2.0f, wheelTexture.getSize().y / 2.0f);
    wheelSprite.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
    wheelSprite.setScale(0.4f, 0.4f);
    return wheelSprite;
}

// Initialize the arrow sprite
sf::Sprite initializeArrow(sf::Texture& arrowTexture) {
    sf::Sprite arrowSprite(arrowTexture);
    arrowSprite.setOrigin(arrowTexture.getSize().x, arrowTexture.getSize().y / 2.0f);
    arrowSprite.setPosition(WINDOW_WIDTH / 2.0f - WHEEL_RADIUS - 30.0f, WINDOW_HEIGHT / 2.0f);
    arrowSprite.setScale(0.3f, 0.3f);
    return arrowSprite;
}

// Initialize the card sprites and assign random cards
void initializeCards(
    std::vector<sf::Sprite>& cardSprites,
    std::vector<int>& assignedCards,
    std::vector<bool>& revealedCards,
    sf::Texture& cardBackTexture,
    std::vector<sf::Texture>& cardTextures
) {
    for (int i = 0; i < NUM_CARDS; ++i) {
        cardSprites[i].setTexture(cardBackTexture);
        cardSprites[i].setOrigin(cardBackTexture.getSize().x / 2.0f, cardBackTexture.getSize().y / 2.0f);
        cardSprites[i].setScale(0.2f, 0.2f);
        assignedCards[i] = i;
    }

    // Shuffle the card assignments
    std::shuffle(assignedCards.begin(), assignedCards.end(), std::mt19937{std::random_device{}()});

    // Initialize the revealed cards tracker
    revealedCards.assign(NUM_CARDS, false);
}

// Update the card positions to rotate with the wheel
void updateCardPositions(std::vector<sf::Sprite>& cardSprites, sf::Sprite& wheelSprite) {
    for (int i = 0; i < NUM_CARDS; ++i) {
        float angle = (i * (360.0f / NUM_CARDS) - wheelSprite.getRotation()) * (M_PI / 180.0f);
        float x = WINDOW_WIDTH / 2.0f + WHEEL_RADIUS * std::cos(angle);
        float y = WINDOW_HEIGHT / 2.0f + WHEEL_RADIUS * std::sin(angle);
        cardSprites[i].setPosition(x, y);
        cardSprites[i].setRotation(0.0f); // Cards stay upright
    }
}

// Reveal the selected card
void revealSelectedCard(
    int selectedSlot,
    std::vector<sf::Sprite>& cardSprites,
    std::vector<int>& assignedCards,
    std::vector<bool>& revealedCards,
    std::vector<sf::Texture>& cardTextures
) {
    if (!revealedCards[selectedSlot]) {
        cardSprites[selectedSlot].setTexture(cardTextures[assignedCards[selectedSlot]]);
        revealedCards[selectedSlot] = true;
        std::cout << "Revealed card in slot " << selectedSlot << ": card" << assignedCards[selectedSlot] + 1 << ".png" << std::endl;
    }
}

// Handle wheel rotation and return the selected slot
int handleWheelRotation(sf::Sprite& wheelSprite, float& rotationSpeed, float deceleration) {
    wheelSprite.rotate(rotationSpeed);
    rotationSpeed -= deceleration;

    if (rotationSpeed <= 0.0f) {
        rotationSpeed = 0.0f;
        float rotation = fmod(wheelSprite.getRotation(), 360.0f);
        int selectedSlot = static_cast<int>((180.0f + 360.0f + rotation + (360.0f / NUM_CARDS) / 2.0f) / (360.0f / NUM_CARDS)) % NUM_CARDS;
        return selectedSlot;
    }

    return -1; // Indicate that the wheel is still spinning
}

// Main game loop
void gameLoop(
    sf::RenderWindow& window,
    sf::Sprite& wheelSprite,
    sf::Sprite& arrowSprite,
    std::vector<sf::Sprite>& cardSprites,
    std::vector<int>& assignedCards,
    std::vector<bool>& revealedCards,
    std::vector<sf::Texture>& cardTextures,
    sf::RectangleShape& startButton,
    sf::Text& startText
) {
    float rotationSpeed = 0.0f;
    float deceleration = 0.3f;
    bool isSpinning = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (startButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        isSpinning = true;
                        rotationSpeed = 15.0f;
                    }
                }
            }
        }

        if (isSpinning) {
            int selectedSlot = handleWheelRotation(wheelSprite, rotationSpeed, deceleration);
            if (selectedSlot != -1) {
                isSpinning = false;
                revealSelectedCard(selectedSlot, cardSprites, assignedCards, revealedCards, cardTextures);
            }
        }

        updateCardPositions(cardSprites, wheelSprite);

        window.clear();
        window.draw(wheelSprite);
        for (auto& sprite : cardSprites) {
            window.draw(sprite);
        }
        window.draw(arrowSprite);
        window.draw(startButton);
        window.draw(startText);
        window.display();
    }
}
