//
// Created by thomas on 02/08/23.
//

#include "Gamestate.h"

Gamestate::Gamestate(Gamestate::boardGrid board_, bool white) : isWhite(white)
{
    if (board_ == nullptr)
    {
        //initialise the memory
        board = Gamestate::boardGrid(new std::shared_ptr<square[]>[8]);
        for (int i = 0; i < 8; i++)
        {
            board[i] = std::shared_ptr<square[]>(new square[8]);
        }
        //set the board pieces to the start chess state
        board[0][0].isWhite = false;
        board[0][0].pieceType = "rook";
        board[0][1].isWhite = false;
        board[0][1].pieceType = "knight";
        board[0][2].isWhite = false;
        board[0][2].pieceType = "bishop";
        board[0][3].isWhite = false;
        board[0][3].pieceType = "queen";
        board[0][4].isWhite = false;
        board[0][4].pieceType = "king";
        board[0][5].isWhite = false;
        board[0][5].pieceType = "bishop";
        board[0][6].isWhite = false;
        board[0][6].pieceType = "knight";
        board[0][7].isWhite = false;
        board[0][7].pieceType = "rook";

        for (int i = 0; i < 8; i++)
        {
            board[1][i].pieceType = "pawn";
            board[1][i].isWhite = false;
            board[6][i].pieceType = "pawn";
            board[6][i].isWhite = true;
        }

        board[7][0].isWhite = true;
        board[7][0].pieceType = "rook";
        board[7][1].isWhite = true;
        board[7][1].pieceType = "knight";
        board[7][2].isWhite = true;
        board[7][2].pieceType = "bishop";
        board[7][3].isWhite = true;
        board[7][3].pieceType = "king";
        board[7][4].isWhite = true;
        board[7][4].pieceType = "queen";
        board[7][5].isWhite = true;
        board[7][5].pieceType = "bishop";
        board[7][6].isWhite = true;
        board[7][6].pieceType = "knight";
        board[7][7].isWhite = true;
        board[7][7].pieceType = "rook";
    } else
    {
        board = board_;
    }
};


Gamestate::boardGrid Gamestate::DisplayState()
{
    return board;
}

std::string *Gamestate::GetNextGameState(move nextMove)
{
    Gamestate *next;
    board[nextMove.newPosition.a][nextMove.newPosition.b].pieceType = board[nextMove.prevPosition.a][nextMove.prevPosition.b].pieceType;
    board[nextMove.newPosition.a][nextMove.newPosition.b].isWhite = board[nextMove.prevPosition.a][nextMove.prevPosition.b].isWhite;
    board[nextMove.prevPosition.a][nextMove.prevPosition.b].pieceType = "empty";
    next = new Gamestate(board, !isWhite);

}