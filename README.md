# Your Dream Team (C++ Console Program)

A small C++11/17 console team-management program where you can build your dream XI with simple formation and player management features.  
Built as a practice project to learn classes, headers, and basic data structures in C++ along with the idea to play around with soccer players.

## Features

- Add players with:
  - Name  
  - Nationality  
  - Jersey number  
  - Position
- Organize players into a team
- Basic text menu for:
  - Building your team
  - Viewing info about the program
  - Viewing information about the developer and development 

## Files

- `main.cpp` – entry point and menu loop
- `Manager.h / Manager.cpp` – base class that provides shared data/behaviour for players; used by `Player` and `Team`
- `Player.h / Player.cpp` – player data (name, number, position, nationality), derived from `Manager`
- `Team.h / Team.cpp` – team and formation data, uses `Player`/`Manager` to organize the squad overall

## How to Build (macOS / Linux)

```bash
g++ -std=c++17 main.cpp Manager.cpp Player.cpp Team.cpp -o your-dream-team
./your-dream-team
```