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
        for(int i = 0; i < boardSize; i++){
            for(int j = 0; j < boardSize; j++){
                cout << gbDecoder(game_state.get_gameBoard(i,j));
            }
            cout << "\n";
        }
    return;
}

int gbEncoder(char value){
    //does the opposite of the decoder
    switch(value){
        case 'R':
            return R;
        case 'Y':
            return Y;
        default:
            return 0;
    }
}

// The main function
int main() {
  /**********************************************************************************/
  /* Create three initialized game state objects in an array */
  /**********************************************************************************/
  const int numOfRounds = 3;
  GameState game_state[numOfRounds]; //does this call the constructor?
  cout << "Main is called";
  // Read one integer from the user that represents the column
  // the player would like to place their piece (R or Y) in
  // You can assume there will be no formatting errors in the input

  int col;
  int round = 0;
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
        break;
    }
    
    // The coordinates are valid; set the selectedRow and selectedColumn

    //compute row;
    int row;
    for(int i = 0; i < boardSize; i++){
        if(game_state[round].get_gameBoard(i, col) == Empty){
            row = i;
            break;
            game_state[round].set_moveValid(true);
        }
    }

    //some check here to make sure that the row is selected
    if(!game_state[round].get_moveValid()){
        cout << "Invalid column!" << endl;
        break;
    }
    cout << "column chosen: " << col << endl;

    game_state[round].set_selectedColumn(col);
    game_state[round].set_selectedRow(row);



    // members of the game state to the read values
    // Note that the corresponding mutators of GameState must be first
    // implemented before this works
    
    // Call playMove

    playMove(game_state[round]);

    // Print the GameState object, as prescribed in the handout

    printGameboard(game_state[round]);

    // Check if a player won this round and if so handle accordingly

    if(!game_state[round].get_gameOver()){
      break;
    }
    char winner = gbDecoder(game_state[round].get_winner());


    // Check if a player won this match and if so handle accordingly
  }
}
  
