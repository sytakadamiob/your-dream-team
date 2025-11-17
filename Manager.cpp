#include "Manager.h"
#include <iostream>

using namespace std;

Manager::Manager()
{}

Manager::Manager(const string& n, const string& natl)
    : name(n), nationality(natl)
{}   // Empty body