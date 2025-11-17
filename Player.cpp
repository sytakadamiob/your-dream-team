#include "Player.h"

Player::Player()
{}

Player::Player(const std::string& n, const std::string& natl, int num, const std::string& pos)
    : Manager(n, natl), 
    number(num), position(pos)
{}   // Empty body