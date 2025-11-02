//
// Created by kerwi on 4/9/2025.
//
#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <cctype>
#include <vector>
#include "TextureManager.h"
using namespace std;
#ifndef TILE_H
#define TILE_H



class Tile {
public:
    bool isMine = false;
    bool hasFlag = false;
    bool isRevealed = false;
    int adjacentMines = 0;
    Tile* neighbors[8];
    sf::Sprite sprite;
    TextureManager* tM;

    Tile(TextureManager* textureManager) : tM(textureManager) {
        sprite.setTexture(tM->getTexture("tile_hidden"));
        for (int i = 0; i < 8; i++) {
            neighbors[i] = nullptr;
        }
    }

    void reset() {
        isRevealed = false;
        isMine = false;
        hasFlag = false;
        adjacentMines = 0;
        for (int i = 0; i < 8; i++) {
            neighbors[i] = nullptr;
        }
    }
};



#endif //TILE_H
