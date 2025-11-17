#include "Team.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <thread>
#include <chrono>
#include <cstdlib>

void Team::initializeFormations()
{
    // Format: {formation : {defCount, midCount, atk_midCount, atkCount}}
    formationMap["4-4-2"] = {4, 4, 0, 2};
    formationMap["4-3-3"] = {4, 3, 0, 3};
    formationMap["3-4-3"] = {3, 4, 0, 3};
    formationMap["4-5-1"] = {4, 5, 0, 1};
    formationMap["5-3-2"] = {5, 3, 0, 2};
    formationMap["3-5-2"] = {3, 5, 0, 2};
    formationMap["4-2-3-1"] = {4, 2, 3, 1};
    formationMap["4-3-2-1"] = {4, 3, 2, 1};
    formationMap["4-2-2-2"] = {4, 2, 2, 2};
    formationMap["4-1-4-1"] = {4, 1, 4, 1};
}

bool Team::isValidFormation(const std::string& f)
{
    return formationMap.find(f) != formationMap.end();
}

// Constructors Implementation
Team::Team()
{
    initializeFormations(); // Set up the map when Team is created
}
Team::Team(const std::string& t, const std::vector<Player>& p, const std::string& f)
    : team(t), players(p), formation(f)
{
    initializeFormations();
}

// --------------------- Main Process [createTeam(), createPlayers(), teamManagement()] ---------------------
void Team::createTeam() // Trigger
{
    std::string coachName, coachNatl;
    players.clear();            // Fresh start
    initializeFormations();     // Ensure map is ready

    std::cout << "\nEnter your team name: ";
    getline(std::cin, team);

    // Manager(Coach) Setup 
    std::cout << "\nCreate your team manager(coach):" << std::endl;
    std::cout << "Name: ";
    getline(std::cin, coachName);
    std::cout << "Nationality: ";
    getline(std::cin, coachNatl);

    coach = Manager(coachName, coachNatl); // Store the manager

    // ----------------------- Team Management & Player Creations -----------------------
    std::cout << "\nEnter your team's formation (n-n-n) or (n-n-n-n):" << std::endl;

    while (true)
    {
        getline(std::cin, formation);
        
        if (isValidFormation(formation))
        {
            break; // Valid formation, continue
        }
        else
        {
            std::cout << "Invalid formation! Please try again: ";
        }
    }

    // Get counts from map
    std::vector<int> counts = formationMap[formation];
    int defCount = counts[0];
    int midCount = counts[1];
    int atk_midCount = counts[2];
    int atkCount = counts[3];
    
    // GK 
    std::cout << "\nEnter info for GOALKEEPER (GK):\n";
    players.push_back(createPlayers());

    // Backline (left to right)
    std::cout << "\n--- BACKLINE (Left to Right) ---" << std::endl;
    for (int i = 0; i < defCount; ++i) {
        std::cout << "\nEnter info for back player #" << (i+1) << ":\n";
        players.push_back(createPlayers());
    }

    // Midfield (left to right)
    std::cout << "\n--- MIDFIELD (Left to Right) ---" << std::endl;
    for (int i = 0; i < midCount; ++i) {
        std::cout << "\nEnter info for mid player #" << (i+1) << ":\n";
        players.push_back(createPlayers());
    }

    // Attacking Midfield (if exists)
    if (atk_midCount != 0)
    {
        std::cout << "\n--- ATTACKING MIDFIELD (Left to Right) ---" << std::endl;
        for (int i = 0; i < atk_midCount; ++i) {
            std::cout << "\nEnter info for atk-mid player #" << (i+1) << ":\n";
            players.push_back(createPlayers());
        }
    }

    // Forwards (left to right)
    std::cout << "\n--- FORWARDS (Left to Right) ---" << std::endl;
    for (int i = 0; i < atkCount; ++i) {
        std::cout << "\nEnter info for fwd player #" << (i+1) << ":\n";
        players.push_back(createPlayers());
    }

    showLoading();      // Loading animation
    teamManagement();   // Display the team

}

Player Team::createPlayers()
{
    std::string name, nationality, position;
    int number;

    std::cout << "Name: ";
    std::getline(std::cin, name);

    std::cout << "Nationality: ";
    std::getline(std::cin, nationality);

    // Input validation for jersey number
    while (true)
    {
        std::cout << "Number (1-99): ";
        if (std::cin >> number && number >= 1 && number <= 99) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // eat newline
            break;
        } else {
            std::cout << "Invalid number. Please enter an integer from 1 to 99.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cout << "Position: ";
    getline(std::cin, position);

    Player player(name, nationality, number, position);

    return player;
}

void Team::teamManagement()
{
    std::string flag = detectMajorityFlag();
    std::cout << "\n----------------------";
    if (!flag.empty())
        std::cout << " " << flag;
    std::cout << " " << team << " FC (" << formation << ") ----------------------\n" 
        << std::endl;

    printFormationFancy();
}

// --------------------------- Generic printing helper functions ---------------------------
void Team::printFormationFancy()
{
    auto it = formationMap.find(formation);
    if (it == formationMap.end()) {
        std::cout << "Cannot display formation: unsupported '" << formation << "'.\n";
        return;
    }

    const std::vector<int>& counts = it->second;
    if (counts.size() != 4) {
        std::cout << "Internal error: formation config invalid.\n";
        return;
    }

    int defCount    = counts[0];
    int midCount    = counts[1];
    int atkMidCount = counts[2];
    int atkCount    = counts[3];

    const int CELL_WIDTH = 18;   // tweak to taste

    // widest row in “slots”
    int baseSlots = defCount;
    baseSlots = std::max(baseSlots, midCount);
    baseSlots = std::max(baseSlots, atkMidCount);
    baseSlots = std::max(baseSlots, atkCount);
    int baseWidth = baseSlots * CELL_WIDTH;

    // indices into players[]
    int gkIndex     = 0;
    int defStart    = 1;
    int midStart    = defStart + defCount;
    int atkMidStart = midStart + midCount;
    int atkStart    = atkMidStart + atkMidCount;

    // ------- attackers (top row) -------
    if (atkCount > 0) {
        printLineWithNumbers(atkStart, atkCount, CELL_WIDTH, baseWidth);

        // connectors to next row
        if (atkMidCount > 0)
            printConnectors(atkCount, atkMidCount, CELL_WIDTH, baseWidth);
        else if (midCount > 0)
            printConnectors(atkCount, midCount, CELL_WIDTH, baseWidth);
    }

    // ------- attacking mids -------
    if (atkMidCount > 0) {
        printLineWithNumbers(atkMidStart, atkMidCount, CELL_WIDTH, baseWidth);
        if (midCount > 0)
            printConnectors(atkMidCount, midCount, CELL_WIDTH, baseWidth);
    }

    // ------- mids -------
    if (midCount > 0) {
        printLineWithNumbers(midStart, midCount, CELL_WIDTH, baseWidth);
        printConnectors(midCount, defCount, CELL_WIDTH, baseWidth);
    }

    // ------- back line -------
    printLineWithNumbers(defStart, defCount, CELL_WIDTH, baseWidth);
    printConnectors(defCount, 1, CELL_WIDTH, baseWidth);

    // ------- GK centered under everything -------
    printGKLine(CELL_WIDTH, baseWidth);
}

void Team::printLineWithNumbers(int start, int count,
                                int cellWidth, int baseWidth)
{
    using std::setw;
    using std::left;

    if (count <= 0) return;

    int rowWidth = count * cellWidth;
    int leftPad  = (baseWidth - rowWidth) / 2;
    if (leftPad < 0) leftPad = 0;

    // Row 1: names + positions, centered
    std::cout << std::string(leftPad, ' ');
    for (int i = 0; i < count; ++i) {
        const Player& p = players[start + i];
        std::string label = p.getName() + " (" + upperCase(p.getPosition()) + ")";

        // horizontal connector to the right, except for last player
        if (i + 1 < count) {
            label += " ----";
        }

        if ((int)label.size() > cellWidth - 1) {
            label = label.substr(0, cellWidth - 1);
        }
        std::cout << setw(cellWidth) << left << label;
    }
    std::cout << "\n";

    // Row 2: numbers under each player (same leftPad)
    std::cout << std::string(leftPad, ' ');
    for (int i = 0; i < count; ++i) {
        const Player& p = players[start + i];
        std::string numLabel = "#" + std::to_string(p.getNumber());
        std::cout << setw(cellWidth) << left << numLabel;
    }
    std::cout << "\n\n";
}

void Team::printConnectors(int upperCount, int lowerCount,
                           int cellWidth, int baseWidth)
{
    using std::setw;
    using std::left;

    int slots = std::max(upperCount, lowerCount);
    if (slots <= 0) return;

    int rowWidth = slots * cellWidth;
    int leftPad  = (baseWidth - rowWidth) / 2;
    if (leftPad < 0) leftPad = 0;

    // Row of slashes (rough diagonals)
    std::cout << std::string(leftPad, ' ');
    for (int i = 0; i < slots; ++i) {
        std::string sym = (i % 2 == 0) ? "|" : "|";
        std::cout << setw(cellWidth) << left << sym;
    }
    std::cout << "\n";

    // Row of vertical bars
    std::cout << std::string(leftPad, ' ');
    for (int i = 0; i < slots; ++i) {
        std::cout << setw(cellWidth) << left << "|";
    }
    std::cout << "\n\n";
}

void Team::printGKLine(int cellWidth, int baseWidth)
{
    if (players.empty()) return;

    const Player& gk = players[0];
    std::string label    = gk.getName() + " (" + upperCase(gk.getPosition()) + ")";
    std::string numLabel = "#" + std::to_string(gk.getNumber());

    // center a single cell of width cellWidth inside baseWidth
    int leftPad = (baseWidth - cellWidth) / 2;
    if (leftPad < 0) leftPad = 0;

    using std::setw;
    using std::left;

    // Name row
    std::cout << std::string(leftPad, ' ')
              << setw(cellWidth) << left << label << "\n";

    // Number row
    std::cout << std::string(leftPad, ' ')
              << setw(cellWidth) << left << numLabel << "\n" << std::endl;
}

// Map raw nationality text
std::string Team::normalizeCountryKey(const std::string& nat)
{
    std::string n = toLower(nat);

    auto has = [&](const char* sub) {
        return n.find(sub) != std::string::npos;
    };

    if (has("germ") || n == "de" || n == "ger" || n == "germany" || n == "german")
        return "german";

    if (has("span") || n == "es" || n == "esp" || n == "spain" || n == "spainish")
        return "spanish";

    if (has("fran") || n == "fr" || n == "france" || n == "french")
        return "french";

    if (has("eng") || has("brit") || n == "uk" || n == "england" || 
        n == "english" || n == "british")
        return "english";

    if (has("portu") || n == "pt" || n == "portugal" || n == "portuguese")
        return "portuguese";

    if (has("brazi") || n == "br" || n == "brazil" || n == "brazilian")
        return "brazilian";

    if (has("arg") || n == "ar" || n == "argentina" || n == "argentine" || n == "argentinian")
        return "argentinian";

    if (has("ital") || n == "it" || n == "italy" || n == "italian")
        return "italian";

    if (has("belgi") || n == "be" || n == "bel" || n == "belgium" || n == "belgian")
        return "belgian";

    // American / USA
    if (has("americ") || n == "usa" || n == "us" || has("u.s.") || n == "american")
        return "american";

    // Croatian
    if (has("croat") || n == "hr" || n == "croatia" || n == "croatian")
        return "croatian";

    // Moroccan
    if (has("morocc") || n == "ma" || has("maroc") || n == "morocco" || n == "moroccan")
        return "moroccan";

    // Mexican
    if (has("mexic") || n == "mx" || n == "mexico" || n == "mexican")
        return "mexican";

    // Canadian
    if (has("canad") || n == "ca" || n == "canada" || n == "canadian")
        return "canadian";

    // Japanese
    if (has("jpn") || n == "jp" || n == "japan" || n == "japanese")
        return "japanese";

    // Korean (we’ll treat as South Korea)
    if (has("korea") || has("korean") || n == "kr" || n == "sk" || n == "korean")
        return "korean";

    // Uruguayan
    if (has("urugua") || n == "uy" || n == "uruguay" || n == "uruguayan" || n == "orientales")
        return "uruguayan";

    // Swiss
    if (has("swiss") || has("switzer") || n == "ch" || n == "switzerland")
        return "swiss";

    // Dutch / Netherlands
    if (has("dutch") || has("nether") || has("holland") || n == "nl" || 
        n == "netherlands" || n == "nederlander")
        return "dutch";

    // Colombian
    if (has("colom") || n == "co" || n == "colombia" || n == "colombian")
        return "colombian";

    // Turkish
    if (has("turk") || n == "tr" || n == "turkey" || n == "turkish")
        return "turkish";

    return ""; // unknown, not handled
}

std::string Team::countryKeyToFlag(const std::string& key)
{
    // Existing
    if (key == "german")        return "🇩🇪";
    if (key == "spanish")       return "🇪🇸";
    if (key == "french")        return "🇫🇷";
    if (key == "english")       return "🇬🇧";
    if (key == "portuguese")    return "🇵🇹";
    if (key == "brazilian")     return "🇧🇷";
    if (key == "argentinian")   return "🇦🇷";
    if (key == "italian")       return "🇮🇹";
    if (key == "belgian")       return "🇧🇪";
    if (key == "american")      return "🇺🇸";
    if (key == "croatian")      return "🇭🇷";
    if (key == "moroccan")      return "🇲🇦";
    if (key == "mexican")       return "🇲🇽";
    if (key == "canadian")      return "🇨🇦";
    if (key == "japanese")      return "🇯🇵";
    if (key == "korean")        return "🇰🇷";
    if (key == "uruguayan")     return "🇺🇾";
    if (key == "swiss")         return "🇨🇭";
    if (key == "dutch")         return "🇳🇱";
    if (key == "colombian")     return "🇨🇴";
    if (key == "turkish")       return "🇹🇷";

    return "";
}

std::string Team::detectMajorityFlag()
{
    std::map<std::string, int> counts;

    // Count coach
    std::string coachKey = normalizeCountryKey(coach.getNationality());
    if (!coachKey.empty())
        counts[coachKey]++;

    // Count players
    for (const Player& p : players) {
        std::string key = normalizeCountryKey(p.getNationality());
        if (!key.empty())
            counts[key]++;
    }

    if (counts.empty())
        return "";

    // Find country with max count
    std::string bestKey;
    int bestCount = -1;
    for (const auto& entry : counts) {
        if (entry.second > bestCount) {
            bestCount = entry.second;
            bestKey = entry.first;
        }
    }

    return countryKeyToFlag(bestKey);
}

// ----------------- Loading animation -----------------
void Team::clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void Team::showLoading()
{
    using namespace std::chrono_literals;

    for (int i = 0; i < 3; ++i) {
        clearScreen();
        std::cout << "\n\n   Building your dream team";
        for (int j = 0; j <= i; ++j) {
            std::cout << ".";
        }
        std::cout << std::flush;
        std::this_thread::sleep_for(1030ms);
    }

    clearScreen();
}


// -------------------- Helper functions implementation --------------------
//here maybe


std::string Team::toLower(const std::string& s)
{
    std::string out = s;
    for (char& c : out) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return out;
}

std::string Team::upperCase(std::string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        s[i] = toupper(s[i]);
    }
    return s;
}