#include "Manager.h"
#include "Player.h"
#include "Team.h"
#include <iostream>
#include <limits>
#include <cstdlib>

// ------------ Function Declarations ------------
void clearScreen();
void printMenu();
void getInfo();
void aboutDev();
void runProgram();

// Main
int main (int argc, char *argv[])
{
    runProgram();
    return 0;
}

// ------------------------- Function Implementations -------------------------
void printMenu()
{
    std::cout << "\n-<<- Build your dream team! ->>-\n";
    std::cout << "     1) Build your Team." << std::endl;
    std::cout << "     2) Get Info." << std::endl;
    std::cout << "     3) About Dev." << std::endl;
    std::cout << "     4) Quit." << std::endl;
}

void getInfo()
{
    std::cout << "\n----------------------------------------------------------\n";
    std::cout << "This is a small team-management program that lets you\n"
          << "build your dream XI with some control over formation and roles.\n";
    std::cout << "\n=== How the program works ===\n";
    std::cout << "1. Choose \"Build your Team\" from the menu.\n";
    std::cout << "2. Enter your club name and manager info.\n";
    std::cout << "3. Pick one of the supported formations.\n";
    std::cout << "4. Enter each player from LEFT to RIGHT on the pitch:\n";
    std::cout << "   - Goalkeeper\n";
    std::cout << "   - Back line\n";
    std::cout << "   - Midfield\n";
    std::cout << "   - Attacking midfield (if exists)\n";
    std::cout << "   - Forwards\n";
    std::cout << "5. Enjoy the ASCII-art layout of your dream XI!\n";

    std::cout << "\nSupported formations:\n";
    std::cout << "  • 4-4-2   : classic balanced\n";
    std::cout << "  • 4-3-3   : attacking wingers\n";
    std::cout << "  • 3-4-3   : very attacking, 3 at the back\n";
    std::cout << "  • 4-5-1   : compact midfield, lone striker\n";
    std::cout << "  • 5-3-2   : very defensive, wing-backs\n";
    std::cout << "  • 3-5-2   : attacking wing-backs\n";
    std::cout << "  • 4-2-3-1 : modern double pivot + 3 AMs\n";
    std::cout << "  • 4-3-2-1 : \"Christmas tree\" shape\n";
    std::cout << "  • 4-2-2-2 : narrow double 10s + 2 ST\n";
    std::cout << "  • 4-1-4-1 : single holding mid\n\n";

    std::cout << "Tip: positions are free text. You can type things like\n";
    std::cout << "LB, RB, CB, CDM, CM, CAM, LW, RW, ST, etc. They will be\n";
    std::cout << "shown in UPPERCASE in the final layout.\n";
    std::cout << "----------------------------------------------------------\n";
}

void aboutDev()
{
    std::cout << "\n=== About this project ===\n";
    std::cout << "This is a self-developed C++ terminal program built to practice:\n";
    std::cout << "  - classes & inheritance (Manager, Player, Team)\n";
    std::cout << "  - std::vector and std::map for storing players/formations\n";
    std::cout << "  - user input handling and basic validation\n";
    std::cout << "  - formatted ASCII output for fun visualizations.\n\n";

    std::cout << "It lets you mix programming with football tactics by\n";
    std::cout << "building your own fantasy lineup and seeing it drawn as\n";
    std::cout << "a tactical board in the console.\n\n";

    std::cout << "Feel free to customize the code, add more formations,\n";
    std::cout << "positions, or even extra features like saving squads.\n";
    std::cout << "----------------------------------------------------------\n";
}

void runProgram()
{
    int userOption;
    Team myTeam;

    clearScreen();
    do
    {
        printMenu();

        // Robust input loop
        while (true)
        {
            std::cout << "Enter your choice: ";

            if (std::cin >> userOption) {
                // got an int
                if (userOption >= 1 && userOption <= 4) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // eat new line
                    break;  // valid menu choice
                } 
                else {
                    std::cout << "Please enter a number from 1 to 4.\n";
                }
            } 
            else {
                // not an int (e.g., user typed 'a')
                std::cout << "Invalid input. Please enter a number from 1 to 4.\n";
                std::cin.clear(); // clear fail state
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        switch (userOption)
        {
        case 1:
            clearScreen();
            myTeam.createTeam();
            break;

        case 2:
            clearScreen();
            getInfo();
            break;
        
        case 3:
            clearScreen();
            aboutDev();
            break;
            
        case 4:
            std::cout << "\nProgram terminated.\n";
            break;

        default:
            std::cout << "\nInvalid choice. Please enter a number from 1 to 4.\n";
            break;
        }
        
    } while (userOption != 4);
}

void clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}
