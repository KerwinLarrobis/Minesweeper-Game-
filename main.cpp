#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include "Tile.h"
#include "welcome.h"
#include "board.h"
#include "leaderboard.h"
using namespace std;
int main() {
    int rowCount;
    int colCount;
    int mineCount;
    ifstream settings("files/config.cfg");
    settings >> colCount >> rowCount >> mineCount;
    welcome welcomeWindow(rowCount, colCount);
    welcomeWindow.run();
    if (welcomeWindow.started == true) {
        Board board(rowCount, colCount, mineCount, welcomeWindow.name);
        board.run();
    }
    return 0;
}