constexpr double M_PI = 3.1415926535;

#include "DisplayGame.hpp"
#include "Card.hpp"
#include "GameState.hpp"

int main() {
    // Initialize the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Card Wheel Game");

    // Load textures
    sf::Texture wheelTexture = loadTexture("wheel.png");
    sf::Texture arrowTexture = loadTexture("arrow.png");
    sf::Texture cardBackTexture = loadTexture("card_back.png");

    // Load all card textures
    std::vector<sf::Texture> cardTextures(NUM_CARDS);
    for (int i = 0; i < NUM_CARDS; ++i) {
        std::string cardPath = "card" + std::to_string(i + 1) + ".png";
        cardTextures[i] = loadTexture(cardPath);
    }

    // Texture for specific cards
    sf::Texture lockpickerTexture = loadTexture("card1.png");
    sf::Texture gadgetUpgradeTexture = loadTexture("card2.png");
    sf::Texture guardsTexture = loadTexture("card3.png");
    sf::Texture thiefTexture = loadTexture("card4.png");
    sf::Texture nobleTraitorTexture = loadTexture("card5.png");
    sf::Texture noblePartyTexture = loadTexture("card6.png");
    sf::Texture patrolGuardsTexture = loadTexture("card7.png");
    sf::Texture guildInfightingTexture = loadTexture("card8.png");

    // Initialize wheel and arrow
    sf::Sprite wheelSprite = initializeWheel(wheelTexture);
    sf::Sprite arrowSprite = initializeArrow(arrowTexture);

    // Initialize cards
    std::vector<sf::Sprite> cardSprites(NUM_CARDS);
    std::vector<int> assignedCards(NUM_CARDS);
    std::vector<bool> revealedCards(NUM_CARDS);
    initializeCards(cardSprites, assignedCards, revealedCards, cardBackTexture, cardTextures);

    // Initialize card logic
    std::vector<Card*> cardObjects(NUM_CARDS);
    for (int i = 0; i < NUM_CARDS; ++i) {
        if (assignedCards[i] == 0) { // card1.png corresponds to Lockpicker
            cardObjects[i] = new LockpickerCard();
        } else if (assignedCards[i] == 1) { // card2.png corresponds to GadgetUpgrade
            cardObjects[i] = new GadgetUpgradeCard();
        } else if (assignedCards[i] == 2) { // card3.png corresponds to Guards
            cardObjects[i] = new GuardsCard();
        } else if (assignedCards[i] == 3) { // card4.png corresponds to Thief
            cardObjects[i] = new ThiefCard();
        } else if (assignedCards[i] == 4) { // card5.png corresponds to Noble Traitor
            cardObjects[i] = new NobleTraitorCard();
        } else if (assignedCards[i] == 5) { // card6.png corresponds to Noble Party
            cardObjects[i] = new NoblePartyCard();
        } else if (assignedCards[i] == 6) { // card7.png corresponds to Patrol Guards
            cardObjects[i] = new PatrolGuardsCard();
        } else if (assignedCards[i] == 7) { // card8.png corresponds to Guild Infighting
            cardObjects[i] = new GuildInfightingCard();
        } else {
            cardObjects[i] = nullptr; // Placeholder for unimplemented cards
        }
    }

    // Create shared game state
    GameState gameState;

    // Initialize gold display
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font file!" << std::endl;
        system("pause");
        return -1;
    }
    sf::Text goldText("Gold: 0", font, 24);
    goldText.setFillColor(sf::Color::White);
    goldText.setPosition(WINDOW_WIDTH - 150, 20);

    // Initialize start button
    sf::RectangleShape startButton(sf::Vector2f(120, 50));
    startButton.setPosition(WINDOW_WIDTH / 2.0f - 60, WINDOW_HEIGHT - 100);
    startButton.setFillColor(sf::Color::Red);
    sf::Text startText("Spin", font, 24);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(WINDOW_WIDTH / 2.0f - 30, WINDOW_HEIGHT - 90);

    // Game loop
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
            std::cout << "Selected slot: " << selectedSlot << std::endl;
            if (selectedSlot != -1) {
                isSpinning = false;

                // Reveal card and apply its effect
                if (!revealedCards[selectedSlot]) {
                    cardSprites[selectedSlot].setTexture(cardTextures[assignedCards[selectedSlot]]);
                    revealedCards[selectedSlot] = true;

                    if (cardObjects[selectedSlot]) {
                        std::cout << "Selected card: " << cardObjects[selectedSlot]->getName() << std::endl;
                        cardObjects[selectedSlot]->applyEffect(&gameState); // Pass shared state
                        std::cout << "Gold: " << gameState.gold << std::endl;
                        std::cout << "Multiplier: " << gameState.multiplier << std::endl;
                        goldText.setString("Gold: " + std::to_string(gameState.gold)); // Update gold display
                    }
                }
            }
        }

        updateCardPositions(cardSprites, wheelSprite);

        // Render
        window.clear();
        window.draw(wheelSprite);
        for (auto& sprite : cardSprites) {
            window.draw(sprite);
        }
        window.draw(arrowSprite);
        window.draw(startButton);
        window.draw(startText);
        window.draw(goldText); // Draw gold display
        window.display();
    }

    // Cleanup
    for (Card* card : cardObjects) {
        delete card;
    }

    return 0;
}
