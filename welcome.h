//
// Created by kerwi on 4/9/2025.
//
#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <cctype>
using namespace std;
#ifndef WELCOME_H
#define WELCOME_H

class welcome {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text welcomeText;
    sf::Text promptText;
    string inputTextString;
    sf::Text inputText;
    sf::RectangleShape rect;
    sf::Color color;
    sf::Vector2f size;
    sf::Keyboard keyboard;
    int rowCount;
    int colCount;
    int height;
    int width;

public:
    bool started = false;
    string name;
    void setText(sf::Text &text, float x, float y){
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

    welcome(int row, int col) {
        rowCount = row;
        colCount = col;
        height = rowCount * 32 + 100;
        width = colCount * 32;
        window.create(sf::VideoMode(width, height), "Welcome Window");
        font.loadFromFile("files/font.ttf");
        welcomeText.setFont(font);
        welcomeText.setString("WELCOME TO MINESWEEPER!");
        welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
        welcomeText.setCharacterSize(24);
        welcomeText.setFillColor(sf::Color::White);
        setText(welcomeText, width / 2, height / 2 - 150);

        promptText.setFont(font);
        promptText.setString("Enter your name:");
        promptText.setStyle(sf::Text::Bold);
        promptText.setCharacterSize(20);
        promptText.setFillColor(sf::Color::White);
        setText(promptText, width / 2, height / 2 - 75);

        inputText.setFont(font);
        inputText.setString("|");
        inputText.setStyle(sf::Text::Bold);
        inputText.setCharacterSize(18);
        inputText.setFillColor(sf::Color::Yellow);
        setText(inputText, width / 2, height / 2 - 45);
    }

    void run() {
        while(window.isOpen()) {
            sf::Event event;
            while(window.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    window.close();
                }

                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == 8) {
                        if (!inputTextString.empty()) {
                            inputTextString.pop_back();
                            inputText.setString(inputTextString + "|");
                            setText(inputText, width / 2, height / 2 - 45);
                        }
                        break;
                    }
                    if (inputTextString.length() >= 10) {
                        break;
                    }
                    char letter = static_cast<char>(event.text.unicode);
                    if (isalpha(letter)) {
                        if (inputTextString.length() == 0) {
                            letter = toupper(letter);
                        } else {
                            letter = tolower(letter);
                        }
                        inputTextString.push_back(letter);
                        inputText.setString(inputTextString + "|");
                        setText(inputText, width / 2, height / 2 - 45);
                    }
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    if (inputTextString.length() < 1) {
                        break;
                    } else {
                        name = inputTextString;
                        started = true;
                        window.close();
                        return;
                    }
                }
            }
            window.clear(sf::Color::Blue);
            window.draw(welcomeText);
            window.draw(promptText);
            window.draw(inputText);
            window.display();
        }
    }
};



#endif //WELCOME_H
