//
//  connectfour.cpp
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
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <iostream>
#include <string>
using namespace std;

#include "GameState.h"
#include "globals.h"

// Function prototypes

char gbDecoder(int value){
    //Game Board Decoder - takes the numerical value in the gameboard array and returns it as the character value for printing
    switch(value){
        case R:
            return 'R';
        case Y:
            return 'Y';
        case Empty:
            return '_';
        default:
            return '0';
    }
}

void printGameboard(GameState& game_state){ //go on, take a guess at what this says
        for(int i = boardSize -1; i >= 0; i--){
            for(int j = 0; j < boardSize; j++){
                cout << gbDecoder(game_state.get_gameBoard(i,j));
            }
            cout << "\n";
        }
    return;
}

int gbEncoder(char value){
    //does the opposite of the decoder (not used but its good practice to make this anyways)
    switch(value){
        case 'R':
            return R;
        case 'Y':
            return Y;
        default:
            return 0;
    }
}

void markBoard(GameState& game_state){
    int row = game_state.get_selectedRow();
    int col = game_state.get_selectedColumn();
    switch(game_state.get_turn()){
        case true:
            game_state.set_gameBoard(row, col, R);
            //cout << "Board is Marked" << endl;
            break;
        case false:
            game_state.set_gameBoard(row, col, Y);
            //cout << "Board is Marked" << endl;
            break;
    }
    //cout << "Board is Marked" << endl;
    
}

// The main function
int main() {
  /**********************************************************************************/
  /* Create three initialized game state objects in an array */
  /**********************************************************************************/
  const int numOfRounds = 3;
  GameState game_state[numOfRounds]; //does this call the constructor?

  // Read one integer from the user that represents the column
  // the player would like to place their piece (R or Y) in
  // You can assume there will be no formatting errors in the input

  //cout << "Connect 4";

  int col;
  int round = 0;
  int winners[3];
  
  for(int round = 0; round < numOfRounds; round++){
    cout << "Game " << round+1 << endl;
  
    while (!game_state[round].get_gameOver()) {
        cout << "Enter column to place piece: ";
        cin >> col;
    
        if( cin.eof() ) {
        cerr << endl << "Game ended by user." << endl ;
        exit( 0 ) ;
        }
        if( cin.fail() ) {
        cin.clear() ;
        cin.ignore( 1000, '\n' ) ;
        col = -1 ; // giving col invalid value so it will be handled as invalid input below
        }
    
        // Check validity of input and if not valid, handle accordingly

        if(col >= boardSize || col < 0){
            cout << "Invalid column!" << endl;
            continue;
        }
        
        // The coordinates are valid; set the selectedRow and selectedColumn

        //compute row;
        int row;
        for(int i = 0; i < boardSize; i++){
            if(game_state[round].get_gameBoard(i, col) == Empty){
                row = i;
                game_state[round].set_moveValid(true);
                break;
            }
        }

        //some check here to make sure that the row is selected
        if(!game_state[round].get_moveValid()){
            cout << "Invalid column!" << endl;
            continue;
        }
        cout << "column chosen: " << col << endl;

        game_state[round].set_selectedColumn(col);
        game_state[round].set_selectedRow(row);
        markBoard(game_state[round]);





        // members of the game state to the read values
        // Note that the corresponding mutators of GameState must be first
        // implemented before this works
        
        // Call playMove

        playMove(game_state[round]);

        // Print the GameState object, as prescribed in the handout

        printGameboard(game_state[round]);

        // Check if a player won this round and if so handle accordingly

        if(!game_state[round].get_gameOver()){
        continue;
        }

        //should only be run if there is a winner
        char winner = gbDecoder(game_state[round].get_winner());
        cout << winner << " won this round!" << endl;
        winners[round] = game_state[round].get_winner();
        // Check if a player won this match and if so handle accordingly
        }
        if( winners[0] + winners[1] + winners[2] >1 || winners[0] + winners[1] + winners[2] < -1){
            break;
        }
    }

    cout << ((winners[0] + winners[1] + winners[2] > 0) ? "R" : "Y") << " won the match!" << endl;

}
  
