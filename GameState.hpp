#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

struct GameState {
    int gold = 500;          // Player's gold
    float multiplier = 1.0f; // Multiplier for gold-increasing effects
    int nobleTraitorBonus = 0; // Additional bonus from Noble Traitor
};

#endif
