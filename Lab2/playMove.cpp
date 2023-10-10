//
//  playMove.cpp
//  Connect Four Game
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Modified by Salma Emara and Ethan Hugh on 2023-09-06
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: write your code for playMove in this file

#include "GameState.h"
#include "globals.h"

bool winnerFound(GameState& gs){
    //vertical check;
    for(int i = 0; i < boardSize; i++){ //row
        for(int j = 0; j < boardSize -3; j++){ // col
            if(gs.get_gameBoard(i,j) != Empty &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i,j+1) &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i,j+2) &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i,j+3)){
                gs.set_winner(gs.get_gameBoard(i, j));
                gs.set_gameOver(true);
                return true;
            }
        }
    }

    for(int i = 0; i < boardSize -3; i++){
        for(int j = 0; j < boardSize; j++){
            if(gs.get_gameBoard(i,j) != Empty &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i+1,j) &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i+2,j) &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i+3,j)){
                gs.set_winner(gs.get_gameBoard(i, j));
                gs.set_gameOver(true);
                return true;
            }
        }
    }

    for(int i = 0; i < boardSize -3; i++){
        for(int j = 0; j < boardSize -3; j++){
            if(gs.get_gameBoard(i,j) != Empty &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i+1,j+1) &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i+2,j+2) &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i+3,j+3)){
                gs.set_winner(gs.get_gameBoard(i, j));
                gs.set_gameOver(true);
                return true;
            }
        }
    }

    for(int i = boardSize -1; i >= 3; i--){
        for(int j = 0; j < boardSize; j++){
            if(gs.get_gameBoard(i,j) != Empty &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i-1,j+1) &&
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i-2,j+2) && 
            gs.get_gameBoard(i,j) == gs.get_gameBoard(i-3,j+3)){
                gs.set_winner(gs.get_gameBoard(i, j));
                gs.set_gameOver(true);
                return true;
            }
        }
    }

    return false;
}


void playMove(GameState& game_state) {

    //function is only called if move is valid
    //goal is solely to update the gamestate object

    //flip turn value

    bool turn = game_state.get_turn();
    game_state.set_turn(!turn);

    //reset moveValid (?)

    game_state.set_moveValid(false);

    //check for winner
    bool winner = winnerFound(game_state);
    return;
}