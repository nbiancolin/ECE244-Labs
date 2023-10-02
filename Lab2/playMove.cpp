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

void checkWinner(int switcher, GameState& game_state){
    switch(switcher){
        case 0:
            //diagonal check (since most likely way to win)
            //board is 7 wide and 7 tall

            for(int j = 0; j < boardSize - 3; j++){
                for(int i = 0; i < boardSize - 3; i++){
                    if(game_state.get_gameBoard(j,i) ==
                    game_state.get_gameBoard(j+1,i+1) ==
                    game_state.get_gameBoard(j+2,i+2) ==
                    game_state.get_gameBoard(j+2,i+2)){
                        game_state.set_winner(game_state.get_gameBoard(i, j));
                        game_state.set_gameOver(true);
                        return;
                    }
                }
            }

            break;
        case 1:
            //Horizontal check (next most likely)
            for(int j = 0; j < boardSize; j++) {  //might be more efficient to do this the other way around but idc
                for (int i = 0; i < boardSize - 3; i++) {
                    if (game_state.get_gameBoard(j, i) ==
                    game_state.get_gameBoard(j + 1, i) ==
                    game_state.get_gameBoard(j + 2, i) ==
                    game_state.get_gameBoard(j + 3, i)) {
                        game_state.set_winner(game_state.get_gameBoard(i, j));
                        game_state.set_gameOver(true);
                        return;
                    }
                }
            }
            break;
        case 2:
            //Vertical check (who ever wins from one of these lmao)
            for(int i = 0; i < boardSize - 3; i++){//represents column
                for(int j = 0; j < boardSize; j++){//represents row
                    if(game_state.get_gameBoard(j,i) ==
                    game_state.get_gameBoard(j,i+1) ==
                    game_state.get_gameBoard(j,i+2) ==
                    game_state.get_gameBoard(j,i+3)) {
                        game_state.set_winner(game_state.get_gameBoard(i, j));
                        game_state.set_gameOver(true);
                        return;
                    }
                }
            }
            break;
    }
}

void playMove(GameState& game_state) {

    //function is only called if move is valid
    //goal is solely to update the gamestate object

    /**
     * stuff that needs to get done (not necessarily in this order:
     * turn value flipped
     * moveValid reset
     * gameOver updated if move was a winning move
     *  by extension, update winner if that is the case
     */

    //flip turn value

    bool turn = game_state.get_turn();
    game_state.set_turn(!turn);

    //reset moveValid (?)

    //check for winner

    int switcher = 0;

    while(!game_state.get_gameOver() || switcher <3){ //that way, if/when a winning move is found, just flip the value then and there, and break, and no more checks will run
        checkWinner(switcher, game_state);
        switcher++;
    }
    return;
}