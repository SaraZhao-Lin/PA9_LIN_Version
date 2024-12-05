#ifndef CARD_HPP
#define CARD_HPP

#include "GameState.hpp"
#include <string>
#include <cmath>
#include <iostream>

// Abstract base class for cards
class Card {
public:
    Card(const std::string& name) : name(name) {}
    virtual ~Card() {}

    // Pass the shared game state for consistent updates
    virtual void applyEffect(GameState* state) = 0;
    std::string getName() const { return name; }

private:
    std::string name;
};

// Derived class for Lockpicker card
class LockpickerCard : public Card {
public:
    LockpickerCard() : Card("Lockpicker") {}

    void applyEffect(GameState* state) override {
        int increment = static_cast<int>(80 * state->multiplier) + state->nobleTraitorBonus;
        state->gold += increment;
        std::cout << "Lockpicker card increased gold by " << increment 
                  << " (including Noble Traitor bonus: " << state->nobleTraitorBonus << ")." << std::endl;
    }
};


// Derived class for GadgetUpgrade card
class GadgetUpgradeCard : public Card {
public:
    GadgetUpgradeCard() : Card("GadgetUpgrade") {}

    void applyEffect(GameState* state) override {
        state->multiplier *= 1.2f; // Persistently update the shared multiplier
        std::cout << "GadgetUpgrade card increased gold multiplier to " << state->multiplier << std::endl;
    }
};

// Derived class for Guards card
class GuardsCard : public Card {
public:
    GuardsCard() : Card("Guards") {}

    void applyEffect(GameState* state) override {
        int decrement = 50;
        state->gold -= decrement;
        std::cout << "Guards card deducted " << decrement << " gold. Current gold: " << state->gold << std::endl;
    }
};


// Derived class for Thief card
class ThiefCard : public Card {
public:
    ThiefCard() : Card("Thief") {}

    void applyEffect(GameState* state) override {
        int increment = static_cast<int>(100 * state->multiplier) + state->nobleTraitorBonus;
        state->gold += increment;
        std::cout << "Thief card increased gold by " << increment 
                  << " (including Noble Traitor bonus: " << state->nobleTraitorBonus << ")." << std::endl;
    }
};

// Derived class for Noble Traitor card
class NobleTraitorCard : public Card {
public:
    NobleTraitorCard() : Card("Noble Traitor") {}

    void applyEffect(GameState* state) override {
        state->nobleTraitorBonus += 50; // Enable Noble Traitor bonus
        std::cout << "Noble Traitor card activated! Each gold-increasing card will now add an additional 50 gold." << std::endl;
    }
};

// Derived class for Noble Party card
class NoblePartyCard : public Card {
public:
    NoblePartyCard() : Card("Noble Party") {}

    void applyEffect(GameState* state) override {
        int increment = static_cast<int>(120 * state->multiplier) + state->nobleTraitorBonus;
        state->gold += increment;
        std::cout << "Noble Party card increased gold by " << increment 
                  << " (including Noble Traitor bonus: " << state->nobleTraitorBonus << ")." << std::endl;
    }
};

// Derived class for Patrol Guards card
class PatrolGuardsCard : public Card {
public:
    PatrolGuardsCard() : Card("Patrol Guards") {}

    void applyEffect(GameState* state) override {
        int decrement = 50;
        state->gold -= decrement;
        std::cout << "Patrol Guards card deducted " << decrement << " gold. Current gold: " << state->gold << std::endl;
    }
};

// Derived class for Guild Infighting card
class GuildInfightingCard : public Card {
public:
    GuildInfightingCard() : Card("Guild Infighting") {}

    void applyEffect(GameState* state) override {
        int decrement = 30;
        state->gold -= decrement;
        std::cout << "Guild Infighting card deducted " << decrement << " gold. Current gold: " << state->gold << std::endl;
    }
};


#endif
