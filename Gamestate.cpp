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

void Gamestate::GetNextGameState(move nextMove)
{
    board[nextMove.newPosition.a][nextMove.newPosition.b].pieceType = board[nextMove.prevPosition.a][nextMove.prevPosition.b].pieceType;
    std::cout << board[nextMove.newPosition.a][nextMove.newPosition.b].pieceType << "\n";
    board[nextMove.newPosition.a][nextMove.newPosition.b].isWhite = board[nextMove.prevPosition.a][nextMove.prevPosition.b].isWhite;
    board[nextMove.prevPosition.a][nextMove.prevPosition.b].pieceType = "empty";
    std::cout << board[nextMove.newPosition.a][nextMove.newPosition.b].pieceType << "\n";
}

void Gamestate::DebugGameState()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::cout << board[i][j].isWhite << " " << board[i][j].pieceType << " ";
        }
        std::cout << "\n";
    }
}

//get possible moves and the selected square as an array of int2s
std::shared_ptr<int2[]> Gamestate::GetSquaresToHighlight()
{
    std::shared_ptr<int2[]> highlighted;
    if (selectedSquare.operator==(int2(-1, -1)))
    {
        highlighted = std::shared_ptr<int2[]>(new int2[0]{});
        return highlighted;
    }
    highlighted = std::shared_ptr<int2[]>(new int2[numPossibleMoves+1]);
    highlighted[0] = selectedSquare;
    for(int i=1; i<numPossibleMoves+1; i++)
    {
        highlighted[i] = possibleMoves[i-1].newPosition;
    };
    return highlighted;
}

bool int2::operator==(const int2 &rhs)
{
    return a == rhs.a && b == rhs.b;
}

void Gamestate::SelectSquare(int2 selected)
{
    if(board[selected.b][selected.a].pieceType == "empty" || board[selected.b][selected.a].isWhite != isWhite)
    {
        selectedSquare = int2(-1, -1);
        return;
    }
    selectedSquare = selected;
    possibleMoves = std::shared_ptr<move[]>(new move[1]);
    possibleMoves[0] = move(selectedSquare, int2(0,0));
    numPossibleMoves = 1;
    numHighlighted = numPossibleMoves+1;
}