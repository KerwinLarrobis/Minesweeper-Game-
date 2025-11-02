//
// Created by kerwi on 4/9/2025.
//
#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "TextureManager.h"
#include "Tile.h"
#include "leaderboard.h"
using namespace std;
#ifndef BOARD_H
#define BOARD_H

class Board {
public:
    bool paused = false;
    bool gameOver = false;
    bool gameWon = false;
    bool debugged = false;
    bool leaderboardOn = false;
    sf::RenderWindow window;
    int rowCount;
    int colCount;
    int mineCount;
    vector<vector<Tile>> board;
    TextureManager tM;
    int flagCount = 0;
    int minesLeft = 0;
    chrono::time_point<std::chrono::steady_clock> startTime = chrono::steady_clock::now();
    chrono::time_point<std::chrono::steady_clock> pauseStartTime;
    chrono::seconds totalPausedTime = chrono::seconds(0);
    int timer = 0;
    bool replacementDone = false;
    string name = "placeholderidk";
    Board(int rowCount, int colCount, int mineCount, string name) : rowCount(rowCount), colCount(colCount), mineCount(mineCount) , name(name) {
        minesLeft = mineCount - flagCount;
        window.create(sf::VideoMode(colCount * 32, rowCount * 32 + 100), "Game Window");
        tM.loadTexture("debug", "files/images/debug.png");
        tM.loadTexture("digits", "files/images/digits.png");
        tM.loadTexture("face_happy", "files/images/face_happy.png");
        tM.loadTexture("face_lose", "files/images/face_lose.png");
        tM.loadTexture("face_win", "files/images/face_win.png");
        tM.loadTexture("flag", "files/images/flag.png");
        tM.loadTexture("leaderboard", "files/images/leaderboard.png");
        tM.loadTexture("mine", "files/images/mine.png");
        tM.loadTexture("number_1", "files/images/number_1.png");
        tM.loadTexture("number_2", "files/images/number_2.png");
        tM.loadTexture("number_3", "files/images/number_3.png");
        tM.loadTexture("number_4", "files/images/number_4.png");
        tM.loadTexture("number_5", "files/images/number_5.png");
        tM.loadTexture("number_6", "files/images/number_6.png");
        tM.loadTexture("number_7", "files/images/number_7.png");
        tM.loadTexture("number_8", "files/images/number_8.png");
        tM.loadTexture("pause", "files/images/pause.png");
        tM.loadTexture("play", "files/images/play.png");
        tM.loadTexture("tile_hidden", "files/images/tile_hidden.png");
        tM.loadTexture("tile_revealed", "files/images/tile_revealed.png");

        board.resize(rowCount, vector<Tile>(colCount, Tile(&tM)));
        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                board[i][j] = Tile(&tM);
                board[i][j].sprite.setPosition(j * 32, i * 32);
            }
        }

        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                board[i][j].sprite.setPosition(j * 32, i * 32);
            }
        }
        assignMines();
        findAdjacent();


    }

    void revealAdjacent(Tile* tile) {
        if (tile-> hasFlag == false) {
            tile->isRevealed = true;
        }
        if (tile->adjacentMines == 0 && !tile->isMine) {
            for (int i = 0; i < 8; i++) {
                Tile* neighbor = tile->neighbors[i];
                if (neighbor && !neighbor->isRevealed) {
                    revealAdjacent(neighbor);
                }
            }
        }
    }

    void drawDigits(int number, sf::RenderWindow& window, bool sec, bool min) {
        vector<sf::Sprite> digits;
        int x = 33;
        int y = 32 * (static_cast<float>(rowCount) + 0.5f) + 16;
        if (sec == true) {
            x = colCount * 32 - 54;
        }
        if (min == true) {
            x = colCount * 32 - 97;
        }
        if (number < 0 && (sec == false || min == false)) {
            sf::Sprite minus;
            minus.setTexture(tM.getTexture("digits"));
            minus.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
            minus.setPosition(x, y);
            digits.push_back(minus);
            x += 21;
            number = -number;
        } else if (sec == false && min == false) {
            sf::Sprite zero;
            zero.setTexture(tM.getTexture("digits"));
            zero.setTextureRect(sf::IntRect(0, 0, 21, 32));
            zero.setPosition(x, y);
            digits.push_back(zero);
            x += 21;
        }

        string numberString = to_string(number);
        if (number < 10) {
            sf::Sprite zero;
            zero.setTexture(tM.getTexture("digits"));
            zero.setTextureRect(sf::IntRect(0, 0, 21, 32));
            zero.setPosition(x, y);
            digits.push_back(zero);
            x += 21;
        }
        for (int i = 0; i < numberString.length(); i++) {
            sf::Sprite digit;
            int digitNumber = numberString[i] - '0';
            digit.setTexture(tM.getTexture("digits"));
            digit.setTextureRect(sf::IntRect(digitNumber * 21, 0, 21, 32));
            digit.setPosition(x, y);
            digits.push_back(digit);
            x += 21;
        }
        for (auto& sprite : digits) {
            window.draw(sprite);
        }
    }

    void draw(sf::RenderWindow& window) {
        for (int i = 0; i < rowCount; ++i) {
            for (int j = 0; j < colCount; ++j) {
                Tile& tile = board[i][j];

                if (tile.isRevealed) {
                    tile.sprite.setTexture(tM.getTexture("tile_revealed"));

                } else {
                    tile.sprite.setTexture(tM.getTexture("tile_hidden"));
                }
                window.draw(tile.sprite);

                if (tile.hasFlag && !tile.isRevealed) {
                    sf::Sprite flagSprite;
                    flagSprite.setTexture(tM.getTexture("flag"));
                    flagSprite.setPosition(tile.sprite.getPosition());
                    window.draw(flagSprite);
                }
                if (tile.isRevealed && tile.isMine) {
                    sf::Sprite mineSprite;
                    mineSprite.setTexture(tM.getTexture("mine"));
                    mineSprite.setPosition(tile.sprite.getPosition());
                    window.draw(mineSprite);
                }
                if (tile.isRevealed && !tile.isMine && tile.adjacentMines > 0) {
                    sf::Sprite numberSprite;
                    string numKey = "number_" + to_string(tile.adjacentMines);
                    numberSprite.setTexture(tM.getTexture(numKey));
                    numberSprite.setPosition(tile.sprite.getPosition());
                    window.draw(numberSprite);
                }
            }
        }
        sf::RectangleShape menuBackground(sf::Vector2f(colCount * 32, 100));
        menuBackground.setFillColor(sf::Color::White);
        menuBackground.setPosition(sf::Vector2f(0, rowCount * 32));
        window.draw(menuBackground);
        sf::Sprite faceButton;
        faceButton.setTexture(tM.getTexture("face_happy"));
        faceButton.setPosition((static_cast<float>(colCount) * 32) / 2.0f - 32, (static_cast<float>(rowCount) + 0.5f) * 32);
        sf::Sprite debugButton;
        debugButton.setTexture(tM.getTexture("debug"));
        debugButton.setPosition(colCount * 32 - 304, (static_cast<float>(rowCount) + 0.5f) * 32);
        sf::Sprite pauseButton;
        pauseButton.setTexture(tM.getTexture("pause"));
        pauseButton.setPosition(colCount * 32 - 240, (static_cast<float>(rowCount) + 0.5f) * 32);
        sf::Sprite leaderboardButton;
        leaderboardButton.setTexture(tM.getTexture("leaderboard"));
        leaderboardButton.setPosition(colCount * 32 - 176, (static_cast<float>(rowCount) + 0.5f) * 32);
        minesLeft = mineCount - flagCount;
        drawDigits(minesLeft, window, false, false);
        timer = 0;
        if (!paused) {
            auto currentTime = std::chrono::steady_clock::now();
            timer = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime - totalPausedTime).count();
        } else {
            timer = std::chrono::duration_cast<std::chrono::seconds>(pauseStartTime - startTime - totalPausedTime).count();
        }
        timer = min(timer, 999);
        int min = timer / 60;
        int sec = timer % 60;
        drawDigits(min, window, false, true);
        drawDigits(sec, window, true, false);
        if (paused == true || leaderboardOn == true) {
            pauseButton.setTexture(tM.getTexture("play"));
            for (int i = 0; i < rowCount; ++i) {
                for (int j = 0; j < colCount; ++j) {
                    Tile& tile = board[i][j];

                    tile.sprite.setTexture(tM.getTexture("tile_revealed"));
                    window.draw(tile.sprite);
                }
            }
        }
        if (gameOver == true) {
            faceButton.setTexture(tM.getTexture("face_lose"));
            for (int i = 0; i < rowCount; ++i) {
                for (int j = 0; j < colCount; ++j) {
                    Tile& tile = board[i][j];

                    if (tile.isMine) {
                        tile.sprite.setTexture(tM.getTexture("mine"));

                    }
                    window.draw(tile.sprite);
                }
            }
        }
        if (gameWon == true) {
            faceButton.setTexture(tM.getTexture("face_win"));
            ifstream leaderboard("files/leaderboard.txt");
            string line;
            vector<string> newTimes;
            if (replacementDone == false) {
                while (getline(leaderboard, line)) {
                    stringstream ss(line);
                    if (newTimes.size() >= 5) {
                        break;
                    }
                    size_t colon = line.find(':');
                    size_t space = line.find(' ');
                    string minute = line.substr(0, colon);
                    string second = line.substr(colon + 1, space - colon - 1);
                    if (minute[0] == '0') {
                        minute = minute.substr(1);
                    }
                    if (second[0] == '0') {
                        second = second.substr(1);
                    }
                    int totalTime = stoi(minute) * 60 + stoi(second);
                    if (totalTime > timer && replacementDone == false) {
                        string minString = to_string(min);
                        string secondString = to_string(sec);
                        if (min < 10) {
                            minString = "0" + minString;
                        }
                        if (sec < 10) {
                            secondString = "0" + secondString;
                        }
                        newTimes.push_back(minString + ":" + secondString + ", " + name + "*");
                        replacementDone = true;
                    }
                    if (newTimes.size() < 5) {
                        newTimes.push_back(line);
                    }
                }
                leaderboard.close();
                if (newTimes.size() > 5) {
                    newTimes.resize(5);
                }
                ofstream outFile("files/leaderboard.txt", ios::trunc); // overwrite file
                for (const auto& newLine : newTimes) {
                    outFile << newLine << endl;
                }
                outFile.close();
            }
            startTime = chrono::steady_clock::now();
            timer = 0;
            totalPausedTime = chrono::seconds(0);
            for (int i = 0; i < rowCount; ++i) {
                for (int j = 0; j < colCount; ++j) {
                    Tile& tile = board[i][j];

                    if (tile.isRevealed) {
                        tile.sprite.setTexture(tM.getTexture("tile_revealed"));

                    } else {
                        tile.sprite.setTexture(tM.getTexture("flag"));
                    }
                    window.draw(tile.sprite);
                }
            }
        }
        if (debugged == true && paused == false && leaderboardOn == false) {
            for (int i = 0; i < rowCount; ++i) {
                for (int j = 0; j < colCount; ++j) {
                    Tile& tile = board[i][j];
                    if (tile.isMine) {
                        sf::Sprite mineSprite;
                        mineSprite.setTexture(tM.getTexture("mine"));
                        mineSprite.setPosition(tile.sprite.getPosition());
                        window.draw(mineSprite);
                    }
                }
            }
        }
        window.draw(faceButton);
        window.draw(debugButton);
        window.draw(pauseButton);
        window.draw(leaderboardButton);
    }
    void assignMines() {
        for (int i = 0; i < rowCount; ++i) {
            for (int j = 0; j < colCount; ++j) {
                board[i][j].isMine = false;
            }
        }
        srand(time(NULL));
        int i = 0;
        while (i < mineCount) {
            int row = rand() % rowCount;
            int col = rand() % colCount;
            if (!board[row][col].isMine) {
                board[row][col].isMine = true;
                i++;
            }
        }

    }
    void findAdjacent() {
        int dx[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
        int dy[8] = {-1, -1, -1,  0, 0,  1, 1, 1};

        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < colCount; j++) {
                Tile& tile = board[i][j];
                tile.adjacentMines = 0;

                for (int n = 0; n < 8; ++n) {
                    int ni = i + dy[n];
                    int nj = j + dx[n];

                    if (ni >= 0 && ni < rowCount && nj >= 0 && nj < colCount) {
                        tile.neighbors[n] = &board[ni][nj];
                        if (board[ni][nj].isMine)
                            tile.adjacentMines++;
                    } else {
                        tile.neighbors[n] = nullptr;
                    }
                }
            }
        }
    }
    bool checkWin() {
        for (int i = 0; i < rowCount; ++i) {
            for (int j = 0; j < colCount; ++j) {
                Tile& tile = board[i][j];
                if (!tile.isMine && !tile.isRevealed)
                    return false;
            }
        }
        cout << "you WIN YES YEAH YEAH YES" << endl;
        
        return true;
    }
    void reset() {
        for (int i = 0; i < rowCount; ++i) {
            for (int j = 0; j < colCount; ++j) {
                Tile& tile = board[i][j];
                board[i][j].reset();
                tile.sprite.setColor(sf::Color::White);
            }
        }
        paused = false;
        gameOver = false;
        gameWon = false;
        gameWon = false;
        leaderboardOn = false;
        flagCount = 0;
        minesLeft = mineCount;
        assignMines();
        findAdjacent();
        startTime = chrono::steady_clock::now();
        timer = 0;
        totalPausedTime = chrono::seconds(0);
        replacementDone = false;
    }
    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed && leaderboardOn == false) {
                    window.close();
                }
                if (event.type == sf::Event::MouseButtonPressed && leaderboardOn == false) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (event.mouseButton.x >= (static_cast<float>(colCount) * 32) / 2.0f - 32 && event.mouseButton.x < (static_cast<float>(colCount) * 32) / 2.0f + 32 && event.mouseButton.y >= (static_cast<float>(rowCount) + 0.5f) * 32 && event.mouseButton.y < (static_cast<float>(rowCount) + 0.5f) * 32 + 64) {
                            reset();
                        }
                        if (event.mouseButton.x >= colCount * 32 - 304 && event.mouseButton.x < colCount * 32 - 240 && event.mouseButton.y >= (static_cast<float>(rowCount) + 0.5f) * 32 && event.mouseButton.y < (static_cast<float>(rowCount) + 0.5f) * 32 + 64 && gameWon == false && gameOver == false && !paused) {
                            if (debugged == true) {
                                debugged = false;
                                cout << "THIS IS WORKING BRUH!!!!" << endl;
                            } else {
                                debugged = true;
                            }
                        }
                        if (event.mouseButton.x >= colCount * 32 - 240 && event.mouseButton.x < colCount * 32 - 176 && event.mouseButton.y >= (static_cast<float>(rowCount) + 0.5f) * 32 && event.mouseButton.y < (static_cast<float>(rowCount) + 0.5f) * 32 + 64 && gameWon == false && gameOver == false) {
                            if (paused == true) {
                                paused = false;
                                totalPausedTime += chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - pauseStartTime);
                            } else {
                                paused = true;
                                pauseStartTime = chrono::steady_clock::now();
                            }
                        }
                        if (event.mouseButton.x >= colCount * 32 - 176 && event.mouseButton.x < colCount * 32 - 112 && event.mouseButton.y >= (static_cast<float>(rowCount) + 0.5f) * 32 && event.mouseButton.y < (static_cast<float>(rowCount) + 0.5f) * 32 + 64) {
                            leaderboardOn = true;
                        }
                    }

                    int x = event.mouseButton.x / 32;
                    int y = (event.mouseButton.y)/ 32;

                    if (x >= 0 && x < colCount && y >= 0 && y < rowCount && gameWon == false && gameOver == false) {
                        Tile& tile = board[y][x];

                        if (event.mouseButton.button == sf::Mouse::Left) {


                            if (!tile.hasFlag && !tile.isRevealed && !gameOver && !paused) {
                                tile.isRevealed = true;

                                if (tile.isMine) {
                                    gameOver = true;
                                    tile.sprite.setColor(sf::Color::Red);
                                } else if (tile.adjacentMines == 0) {
                                    cout << "OVER HERE!!!!" << endl;
                                    revealAdjacent(&tile);
                                }

                                if (checkWin()) {
                                    gameWon = true;
                                }
                            }
                        } else if (event.mouseButton.button == sf::Mouse::Right) {
                            if (!tile.isRevealed) {
                                if (tile.hasFlag == false) {
                                    tile.hasFlag = true;
                                    flagCount++;
                                } else {
                                    tile.hasFlag = false;
                                    flagCount--;
                                }
                            }
                        }
                    }

                }
                if (leaderboardOn == true) {
                    window.clear();
                    draw(window);
                    window.display();
                    paused = true;
                    pauseStartTime = chrono::steady_clock::now();
                    leaderboard leaderboardWindow(rowCount, colCount);
                    leaderboardWindow.run();
                    while (window.pollEvent(event)) {

                    }
                    leaderboardOn = false;
                    paused = false;
                    totalPausedTime += chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - pauseStartTime);
                }
            }

            window.clear();
            draw(window);
            window.display();
        }
    }

};


#endif //BOARD_H
