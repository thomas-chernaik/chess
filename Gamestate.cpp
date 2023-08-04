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
    selectedSquare = int2(-1, -1);

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
    if (possibleMoves == nullptr)
        return nullptr;
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
            numPossibleMoves = 0;
        } else
        {
            //make the move
            GetNextGameState(possibleMoves[moveIndex]);
        }
    } else
    {
        //selecting a new square
        selectedSquare = selected;
        GeneratePossibleMoves();
    }

}

void Gamestate::GeneratePossibleMoves()
{
    //we need to call the correct move generator for the selected piece type
    std::string selectedPieceType = board[selectedSquare.b][selectedSquare.a].pieceType;
    if (selectedPieceType == "pawn")
    {
        possibleMoves = GetPawnMoves(selectedSquare);
        if (possibleMoves == nullptr)
        {
            numPossibleMoves = 0;
        }
    } else if (selectedPieceType == "rook")
    {
        possibleMoves = GetRookMoves(selectedSquare);
        if (possibleMoves == nullptr)
        {
            numPossibleMoves = 0;
        }
    } /*else if (selectedPieceType == "bishop")
    {
        possibleMoves = GetBishopMoves(selectedSquare);
    } else if (selectedPieceType == "knight")
    {
        possibleMoves = GetKnightMoves(selectedSquare);
    } else if (selectedPieceType == "king")
    {
        possibleMoves = GetKingMoves(selectedSquare);
    } else if (selectedPieceType == "queen")
    {
        possibleMoves = GetQueenMoves(selectedSquare);
    }
    else
    {
        throw std::domain_error("the piece type is not valid");
    }*/
}

std::shared_ptr<move[]> Gamestate::GetRookMoves(int2 position)
{
    //a rook can move horizontally or vertically until it reaches the edge, or a piece
    //get the colour of the piece
    bool isPieceWhite = board[position.b][position.a].isWhite;
    short int up = 0;
    short int down = 0;
    short int left = 0;
    short int right = 0;
    //get up distance
    for (int i = 1; i < 8; i++)
    {
        if (position.b + i < 8)
        {
            //check if we can move up to an empty piece
            if (board[position.b + i][position.a].pieceType == "empty")
                up += 1;
                //check if we can move up to take a piece
            else
            {
                if (board[position.b + i][position.a].isWhite != isPieceWhite)
                {
                    up += 1;
                }
                break;

            }
        } else
        {
            break;
        }
    }
    //get down distance
    for (int i = 1; i < 8; i++)
    {
        if (position.b - i >= 0)
        {
            //check if we can move down to an empty piece
            if (board[position.b - i][position.a].pieceType == "empty")
                down += 1;
                //check if we can move down to take a piece
            else
            {
                if (board[position.b - i][position.a].isWhite != isPieceWhite)
                {
                    down += 1;
                }
                break;
            }
        } else
        {
            break;
        }
    }
    //get right distance
    for (int i = 1; i < 8; i++)
    {
        if (position.a + i < 8)
        {
            //check if we can move right to an empty piece
            if (board[position.b][position.a+i].pieceType == "empty")
                right += 1;
                //check if we can move right to take a piece
            else
            {
                if (board[position.b][position.a+i].isWhite != isPieceWhite)
                {
                    right += 1;
                }
                break;

            }
        } else
        {
            break;
        }
    }
    //get left distance
    for (int i = 1; i < 8; i++)
    {
        if (position.a - i >= 0)
        {
            //check if we can move left to an empty piece
            if (board[position.b][position.a-i].pieceType == "empty")
                left += 1;
                //check if we can move left to take a piece
            else
            {
                if (board[position.b][position.a-i].isWhite != isPieceWhite)
                {
                    left += 1;
                }
                break;

            }
        } else
        {
            break;
        }
    }
    numPossibleMoves = up+down+left+right;
    if(numPossibleMoves == 0)
        return nullptr;
    std::shared_ptr<move[]> moves = std::shared_ptr<move[]>(new move[numPossibleMoves]);
    for(int i=1; i<=up; i++)
    {
        moves[i-1].prevPosition = position;
        moves[i-1].newPosition = int2(position.a, position.b+i);
    }
    for(int i=1; i<=down; i++)
    {
        moves[i+up-1].prevPosition = position;
        moves[i+up-1].newPosition = int2(position.a, position.b-i);
    }
    for(int i=1; i<=right; i++)
    {
        moves[i+up+down-1].prevPosition = position;
        moves[i+up+down-1].newPosition = int2(position.a+i, position.b);
    }
    for(int i=1; i<=left; i++)
    {
        moves[i+up+down+right-1].prevPosition = position;
        moves[i+up+down+right-1].newPosition = int2(position.a-i, position.b);
    }
    return moves;
}

std::shared_ptr<move[]> Gamestate::GetPawnMoves(int2 position)
{
    //a pawn can either move one step forward, two steps forward if at the second or 7th row, and diagonally to take a piece
    //en passant may be added later
    //get the colour of the piece
    bool isPieceWhite = board[position.b][position.a].isWhite;
    short int forward = 0;
    short int takeRight = false;
    short int takeLeft = false;
    if (!isPieceWhite)
    {
        //check if we are at the end, and promote if so
        if (position.b == 7)
        {
            board[position.b][position.a].pieceType = "queen";
            return nullptr;
        }

        //check if we can move one forward
        if (board[position.b + 1][position.a].pieceType == "empty")
        {
            std::cout << "hey";
            forward = 1;
        }
        //check if we can move two forward
        if (position.b == 1 && forward == 1 && board[position.b + 2][position.a].pieceType == "empty")
        {
            forward = 2;
        }
        //check if we can take left
        if (position.a > 0)
        {
            if (board[position.b + 1][position.a - 1].pieceType != "empty" &&
                board[position.b + 1][position.a - 1].isWhite)
            {
                takeLeft = 1;
            }
        }
        //check if we can take right
        if (position.a < 7)
        {
            if (board[position.b + 1][position.a + 1].pieceType != "empty" &&
                board[position.b + 1][position.a + 1].isWhite)
            {
                takeRight = 1;
            }
        }
        //stuff all the moves into an array
        std::shared_ptr<move[]> moves = std::shared_ptr<move[]>(new move[forward + takeRight + takeLeft]);
        if (forward >= 1)
        {
            moves[0].prevPosition = position;
            moves[0].newPosition = int2(position.a, position.b + 1);
        }
        if (forward == 2)
        {
            moves[1].prevPosition = position;
            moves[1].newPosition = int2(position.a, position.b + 2);
        }
        if (takeRight)
        {
            moves[forward].prevPosition = position;
            moves[forward].newPosition = int2(position.a + 1, position.b + 1);
        }
        if (takeLeft)
        {
            moves[forward + takeRight].prevPosition = position;
            moves[forward + takeRight].newPosition = int2(position.a - 1, position.b + 1);
        }
        numPossibleMoves = forward + takeRight + takeLeft;
        return moves;
    } else
    {
        //check if we are at the end, and promote if so
        if (position.b == 0)
        {
            board[position.b][position.a].pieceType = "queen";
            return nullptr;
        }
        //check if we can move one forward
        if (board[position.b - 1][position.a].pieceType == "empty")
        {
            std::cout << "hey\n";
            forward = 1;
        }
        //check if we can move two forward
        if (position.b == 6 && forward == 1 && board[position.b - 2][position.a].pieceType == "empty")
        {
            forward = 2;
        }
        //check if we can take left
        if (position.a > 0)
        {
            if (board[position.b - 1][position.a - 1].pieceType != "empty" &&
                !board[position.b - 1][position.a - 1].isWhite)
            {
                takeLeft = 1;
            }
        }
        //check if we can take right
        if (position.a < 7)
        {
            if (board[position.b - 1][position.a + 1].pieceType != "empty" &&
                !board[position.b - 1][position.a + 1].isWhite)
            {
                takeRight = 1;
            }
        }
        //stuff all the moves into an array
        std::shared_ptr<move[]> moves = std::shared_ptr<move[]>(new move[forward + takeRight + takeLeft]);
        if (forward >= 1)
        {
            moves[0].prevPosition = position;
            moves[0].newPosition = int2(position.a, position.b - 1);
        }
        if (forward == 2)
        {
            moves[1].prevPosition = position;
            moves[1].newPosition = int2(position.a, position.b - 2);
        }
        if (takeRight)
        {
            moves[forward].prevPosition = position;
            moves[forward].newPosition = int2(position.a + 1, position.b - 1);
        }
        if (takeLeft)
        {
            moves[forward + takeRight].prevPosition = position;
            moves[forward + takeRight].newPosition = int2(position.a - 1, position.b - 1);
        }
        numPossibleMoves = forward + takeRight + takeLeft;
        std::cout << numPossibleMoves;
        return moves;
    }
}