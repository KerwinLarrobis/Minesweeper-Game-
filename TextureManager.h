//
// Created by kerwi on 4/14/2025.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H



class TextureManager {
private:
  unordered_map<string, sf::Texture> textures;
public:
  TextureManager() {}

  void loadTexture(const string& name,const string& file) {
    if (textures.find(name) == textures.end()) {
      sf::Texture texture;
      if (texture.loadFromFile(file)) {
        textures[name] = texture;
      } else {
        cout << "THIS THING AINT WORKING BRUH LOOK HERE" << endl;
      }
    }
  }

  sf::Texture& getTexture(const string& name) {
    return textures[name];
  }
};



#endif //TEXTUREMANAGER_H
