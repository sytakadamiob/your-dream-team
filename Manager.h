#ifndef MANAGER_H
#define MANAGER_H

#include <string>

// Base class
class Manager
{
    protected:
        std::string name = "Unknown";
        std::string nationality = "Unknown";

    public:
    // Constructors
        Manager();  // Default constructor
        Manager(const std::string& n, const std::string& natl);

        // In-line member functions implementation
        const std::string& getName() const { return name; }
        const std::string& getNationality() const { return nationality; }
};

#endif