//
//  GameState.cpp
//  Connect Four Game
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Modified by Salma Emara and Ethan Hugh on 2023-09-06.
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//

#include "GameState.h"
#include "globals.h"

// ECE244 Student: add you code below

GameState::GameState() { //constructor (I didn't see one defined anywhere, so better safe than sorry)
    selectedRow = 0;
    selectedColumn = 0;
    moveValid = true;
    gameOver = false;
    turn = true;
    winner = Empty;
    //gameBoard = int[boardSize][boardSize];
}



int GameState::get_selectedRow() {
    //return GameState->selectedRow; //which is it? this or the other
    return selectedRow;
}

int GameState::get_selectedColumn() {
    return selectedColumn;
    //return GameState->selectedColumn;
}

void GameState::set_selectedRow(int value) {
    //GameState->selectedRow = value;
    selectedRow = value;
    return;
}

void GameState::set_selectedColumn(int value) {
    //GameState->selectedColumn = value;
    selectedColumn = value;
    return;
}

bool GameState::get_moveValid() {
    //return GameState->moveValid;
    return moveValid;
}

void GameState::set_moveValid(bool value){
    //GameState->moveValid = value
    moveValid = value;
}

// Get the gameOver value
bool GameState::get_gameOver(){
    //return GameState->gameOver;
    return gameOver;
}

// Set the gameOver variable to value
void GameState::set_gameOver(bool value){
    //GameState->gameOver = value;
    //return;
    gameOver = value;
}

// Get the value of turn
bool GameState::get_turn(){
    return turn;
    //return GameState->turn;
}

// Set the value of turn
void GameState::set_turn(bool value){
    //GameState->turn = value;
    turn = value;
    return;
}

// Get the value of winner
int GameState::get_winner(){
    //return GameState->winner;
    return winner;
}

// Set the value of winner;
void GameState::set_winner(int value){
    //GameState->winner = value;
    winner = value;
    //return;
}

// Get the game board value at the board location at row and col
// This method checks that row, col and value are in range/valid
// and if not it returns Empty
int GameState::get_gameBoard(int row, int col){
    //check if row and col exist
    if(row < boardSize && col < boardSize){  //n.b. I thought it was good practice to define constants with all caps? (so shouldn't this be BOARD_SIZE) ?
        GameState::set_selectedColumn(col); //do this? or not?
        GameState::set_selectedRow(row);
        if(GameState::get_moveValid()){
            //return GameState->gameBoard[row][col]; //what to return??
            return gameBoard[row][col];
        }
    }
    return Empty;
}

// Set the game board value at the board location at row and col to value
// This method checks that row, col and value are in range/valid and if not it
// just returns
void GameState::set_gameBoard(int row, int col, int value){
    //check if row and col exist
    if(row < boardSize && col < boardSize) {
        GameState::set_selectedColumn(col);
        GameState::set_selectedRow(row);
        if(value){
            //GameState->gameBoad[row][col] = value;
            gameBoard[row][col] = value;
            return;
        } else{
            //cerr << "Invalid value assigned for set_gameBoard"
            //apparently we're snot
            return;
        }
    }
    return;
}