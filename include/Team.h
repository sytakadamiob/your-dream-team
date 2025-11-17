#ifndef TEAM_H
#define TEAM_H

#include "Manager.h"
#include "Player.h"
#include <string>
#include <vector>
#include <map>

// Separate class
class Team
{
    private:
        std::string team = "\'Untitled\' FC";
        std::string formation;
        Manager coach;
        std::vector <Player> players;
    
        // ------------------------------ Private functions ------------------------------
        void initializeFormations();
        bool isValidFormation(const std::string& f);
        void clearScreen();
        void showLoading();

        // formation string -> {defCount, midCount, atk_midCount, atkCount}
        std::map<std::string, std::vector<int>> formationMap;

        // Internal helper functions
        std::string upperCase(std::string const s);
        std::string toLower(const std::string& s);

        // Helper functions for nationality decider
        std::string normalizeCountryKey(const std::string& nat);
        std::string countryKeyToFlag(const std::string& key);
        std::string detectMajorityFlag();

        // Generic printing helpers for ANY formation
        void printFormationFancy();                             // Main one
        void printLineWithNumbers(int start, int count,
                                int cellWidth, int baseWidth);  // for one row
        void printConnectors(int upperCount, int lowerCount,
                            int cellWidth, int baseWidth);      // for between rows
        void printGKLine(int cellWidth, int baseWidth);         // for GK at bottom

    public:
        // Constructors
        Team(); // Default constructor
        Team(const std::string& t, const std::vector<Player>& p, const std::string& f);

        void createTeam();
        Player createPlayers();
        void teamManagement();
};

#endif