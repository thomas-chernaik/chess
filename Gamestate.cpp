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
    std::cout << nextMove.prevPosition.a << " " << nextMove.newPosition.a;
    //make the move
    board[nextMove.newPosition.b][nextMove.newPosition.a].pieceType = board[nextMove.prevPosition.b][nextMove.prevPosition.a].pieceType;
    std::cout << board[nextMove.newPosition.a][nextMove.newPosition.b].pieceType << "\n";
    board[nextMove.newPosition.b][nextMove.newPosition.a].isWhite = board[nextMove.prevPosition.b][nextMove.prevPosition.a].isWhite;
    board[nextMove.prevPosition.b][nextMove.prevPosition.a].pieceType = "empty";
    std::cout << board[nextMove.newPosition.b][nextMove.newPosition.a].pieceType << "\n";
    //switch the iswhite
    isWhite = !isWhite;
    //reset the selected piece
    numPossibleMoves = 0;
    numHighlighted = 0;
    selectedSquare =  int2(-1,-1);

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
    highlighted = std::shared_ptr<int2[]>(new int2[numPossibleMoves + 1]);
    highlighted[0] = selectedSquare;
    for (int i = 1; i < numPossibleMoves + 1; i++)
    {
        highlighted[i] = possibleMoves[i - 1].newPosition;
    };
    return highlighted;
}

bool int2::operator==(const int2 &rhs)
{
    return a == rhs.a && b == rhs.b;
}

void Gamestate::SelectSquare(int2 selected)
{
    //if we have a piece selected, we want to see if this is selecting a different piece or attempting to make a move
    if (board[selected.b][selected.a].pieceType == "empty" || board[selected.b][selected.a].isWhite != isWhite)
    {
        //now we need to see if the square that has been selected is a possible move
        int moveIndex = -1;
        for (int i = 0; i < numPossibleMoves; i++)
        {
            if (selected.operator==(possibleMoves[i].newPosition))
            {
                moveIndex = i;
                break;
            }
        }
        //if the move isn't valid, simply deselect the selected square
        if (moveIndex == -1)
        {
            selectedSquare = int2(-1, -1);
            numHighlighted = 0;
        } else
        {
            //make the move
            GetNextGameState(possibleMoves[moveIndex]);
        }
    } else
    {
        //selecting a new square
        selectedSquare = selected;
        possibleMoves = std::shared_ptr<move[]>(new move[1]);
        possibleMoves[0] = move(selected, int2(0, 0));
        numPossibleMoves = 1;
        numHighlighted = numPossibleMoves + 1;
    }

}