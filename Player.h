#ifndef PLAYER_H
#define PLAYER_H

#include "Manager.h"
#include <string>

// Derived class
class Player: public Manager
{
    private:
        int number = 0;
        std::string position = "Unknown";

    public:
        // Constructors
        Player();   // Default constructor
        Player(const std::string& n, const std::string& natl, int num, const std::string& pos);

        // In-line member functions implementation
        std::string getName() const { return name; } 
        std::string getPosition() const { return position; }
        int getNumber() const { return number; }
};

#endif