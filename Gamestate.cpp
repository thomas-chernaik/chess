//
// Created by thomas on 02/08/23.
//

#include "Gamestate.h"

Gamestate::Gamestate(const Gamestate::boardGrid &board_, bool white) : isWhite(white)
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
        board[7][3].pieceType = "queen";
        board[7][4].isWhite = true;
        board[7][4].pieceType = "king";
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
}


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

Gamestate::boardGrid Gamestate::GetTemporaryMove(move nextMove)
{
    //initialise the memory
    boardGrid tempBoard = Gamestate::boardGrid(new std::shared_ptr<square[]>[8]);
    for (int i = 0; i < 8; i++)
    {
        tempBoard[i] = std::shared_ptr<square[]>(new square[8]);
    }
    //set the board pieces to the current board
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            tempBoard[i][j].pieceType = board[i][j].pieceType;
            tempBoard[i][j].isWhite = board[i][j].isWhite;
        }
    }
    //make the move
    tempBoard[nextMove.newPosition.b][nextMove.newPosition.a].pieceType = tempBoard[nextMove.prevPosition.b][nextMove.prevPosition.a].pieceType;
    tempBoard[nextMove.newPosition.b][nextMove.newPosition.a].isWhite = tempBoard[nextMove.prevPosition.b][nextMove.prevPosition.a].isWhite;
    tempBoard[nextMove.prevPosition.b][nextMove.prevPosition.a].pieceType = "empty";
    return tempBoard;
}

bool Gamestate::isCheck(move moveToCheck, bool isPieceWhite)
{
    boardGrid boardToCheck = GetTemporaryMove(moveToCheck);
    //find the king of our colour
    int2 kingLocation;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (boardToCheck[j][i].pieceType == "king" && boardToCheck[j][i].isWhite == isPieceWhite)
            {
                kingLocation = int2(i, j);
            }
        }
    }
    short int distance;
    //check for if the king can be taken by a bishop or queen diagonally
    distance = getDistanceInDirection(kingLocation, int2(1, 1), isPieceWhite, boardToCheck);
    if (boardToCheck[kingLocation.b + distance][kingLocation.a + distance].isWhite != isPieceWhite &&
        (boardToCheck[kingLocation.b + distance][kingLocation.a + distance].pieceType == "queen" ||
         boardToCheck[kingLocation.b + distance][kingLocation.a + distance].pieceType == "bishop"))
        return true;
    distance = getDistanceInDirection(kingLocation, int2(-1, 1), isPieceWhite, boardToCheck);
    if (boardToCheck[kingLocation.b + distance][kingLocation.a - distance].isWhite != isPieceWhite &&
        (boardToCheck[kingLocation.b + distance][kingLocation.a - distance].pieceType == "queen" ||
         boardToCheck[kingLocation.b + distance][kingLocation.a - distance].pieceType == "bishop"))
        return true;
    distance = getDistanceInDirection(kingLocation, int2(1, -1), isPieceWhite, boardToCheck);
    if (boardToCheck[kingLocation.b - distance][kingLocation.a + distance].isWhite != isPieceWhite &&
        (boardToCheck[kingLocation.b - distance][kingLocation.a + distance].pieceType == "queen" ||
         boardToCheck[kingLocation.b - distance][kingLocation.a + distance].pieceType == "bishop"))
        return true;
    distance = getDistanceInDirection(kingLocation, int2(-1, -1), isPieceWhite, boardToCheck);
    if (boardToCheck[kingLocation.b - distance][kingLocation.a - distance].isWhite != isPieceWhite &&
        (boardToCheck[kingLocation.b - distance][kingLocation.a - distance].pieceType == "queen" ||
         boardToCheck[kingLocation.b - distance][kingLocation.a - distance].pieceType == "bishop"))
        return true;
    //check for if the king can be taken by a rook or queen vertically
    distance = getDistanceInDirection(kingLocation, int2(0, 1), isPieceWhite, boardToCheck);
    if (boardToCheck[kingLocation.b + distance][kingLocation.a].isWhite != isPieceWhite &&
        (boardToCheck[kingLocation.b + distance][kingLocation.a].pieceType == "queen" ||
         boardToCheck[kingLocation.b + distance][kingLocation.a].pieceType == "rook"))
        return true;
    distance = getDistanceInDirection(kingLocation, int2(0, -1), isPieceWhite, boardToCheck);
    if (boardToCheck[kingLocation.b - distance][kingLocation.a].isWhite != isPieceWhite &&
        (boardToCheck[kingLocation.b - distance][kingLocation.a].pieceType == "queen" ||
         boardToCheck[kingLocation.b - distance][kingLocation.a].pieceType == "rook"))
        return true;
    distance = getDistanceInDirection(kingLocation, int2(1, 0), isPieceWhite, boardToCheck);
    if (boardToCheck[kingLocation.b][kingLocation.a + distance].isWhite != isPieceWhite &&
        (boardToCheck[kingLocation.b][kingLocation.a + distance].pieceType == "queen" ||
         boardToCheck[kingLocation.b][kingLocation.a + distance].pieceType == "rook"))
        return true;
    distance = getDistanceInDirection(kingLocation, int2(-1, 0), isPieceWhite, boardToCheck);
    if (boardToCheck[kingLocation.b][kingLocation.a - distance].isWhite != isPieceWhite &&
        (boardToCheck[kingLocation.b][kingLocation.a - distance].pieceType == "queen" ||
         boardToCheck[kingLocation.b][kingLocation.a - distance].pieceType == "rook"))
        return true;
    //check for if the king can be taken by a king
    int2 moveVectors[8] = {
            int2(1, 1),
            int2(1, 0),
            int2(1, -1),
            int2(0, -1),
            int2(-1, -1),
            int2(-1, 0),
            int2(-1, -1),
            int2(0, 1)
    };
    for (auto moveVector: moveVectors)
    {
        int2 movePos = kingLocation + moveVector;
        if (isOnBoard(movePos))
        {
            if (boardToCheck[movePos.b][movePos.a].isWhite != isPieceWhite &&
                boardToCheck[movePos.b][movePos.a].pieceType == "knight")
            {
                return true;
            }
        }
    }
    //check for if the king can be taken by a knight
    int2 knightMoveVectors[8] = {
            int2(2, 1),
            int2(1, 2),
            int2(-1, 2),
            int2(-2, 1),
            int2(-1, -2),
            int2(-2, -1),
            int2(2, -1),
            int2(1, -2)
    };
    for (auto moveVector: knightMoveVectors)
    {
        int2 movePos = kingLocation + moveVector;
        if (isOnBoard(movePos))
        {
            if (boardToCheck[movePos.b][movePos.a].isWhite != isPieceWhite &&
                boardToCheck[movePos.b][movePos.a].pieceType == "knight")
            {
                return true;
            }
        }
    }
    //check for if the king can be taken by a pawn
    if (isPieceWhite)
    {
        if (!isOnBoard(kingLocation + int2(0, -1)))
            return false;
        //the pawn will be at a lower y than us
        return (!boardToCheck[kingLocation.b - 1][kingLocation.a + 1].isWhite &&
                boardToCheck[kingLocation.b - 1][kingLocation.a + 1].pieceType == "pawn") ||
               (!boardToCheck[kingLocation.b - 1][kingLocation.a - 1].isWhite &&
                boardToCheck[kingLocation.b - 1][kingLocation.a - 1].pieceType == "pawn");
    } else
    {
        if (!isOnBoard(kingLocation + int2(0, 1)))
            return false;
        //the pawn will be at a higher y than us
        return (boardToCheck[kingLocation.b + 1][kingLocation.a + 1].isWhite &&
                boardToCheck[kingLocation.b + 1][kingLocation.a + 1].pieceType == "pawn") ||
               (boardToCheck[kingLocation.b + 1][kingLocation.a - 1].isWhite &&
                boardToCheck[kingLocation.b + 1][kingLocation.a - 1].pieceType == "pawn");
    }
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
    if (selectedSquare == int2(-1, -1))
    {
        highlighted = std::shared_ptr<int2[]>(new int2[0]{});
        return highlighted;
    }
    highlighted = std::shared_ptr<int2[]>(new int2[numPossibleMoves]);
    for (int i = 0; i < numPossibleMoves; i++)
    {
        highlighted[i] = possibleMoves[i].newPosition;
    }
    return highlighted;
}

bool int2::operator==(const int2 &rhs) const
{
    return a == rhs.a && b == rhs.b;
}

int2 int2::operator+(const int2 &rhs) const
{
    return int2(a + rhs.a, b + rhs.b);
}

int2 int2::operator*(const int &rhs) const
{
    return int2(a * rhs, b * rhs);
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
            if (selected == possibleMoves[i].newPosition)
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
    } else if (selectedPieceType == "bishop")
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
    } else
    {
        throw std::domain_error("the piece type is not valid");
    }
}

bool Gamestate::isOnBoard(int2 pos)
{
    return pos.a >= 0 && pos.a < 8 && pos.b >= 0 && pos.b < 8;
}

std::shared_ptr<move[]> Gamestate::GetKnightMoves(int2 position)
{
    //there are 8 possible knight moves
    bool isPieceWhite = board[position.b][position.a].isWhite;
    bool knightMoves[8] = {false, false, false, false, false, false, false, false};
    int2 moveVectors[8] = {
            int2(2, 1),
            int2(1, 2),
            int2(-1, 2),
            int2(-2, 1),
            int2(-1, -2),
            int2(-2, -1),
            int2(2, -1),
            int2(1, -2)
    };
    numPossibleMoves = 0;
    for (int i = 0; i < 8; i++)
    {
        int2 movePos = position + moveVectors[i];
        if (isOnBoard(movePos))
        {
            if (board[movePos.b][movePos.a].isWhite != isPieceWhite || board[movePos.b][movePos.a].pieceType == "empty")
            {
                knightMoves[i] = true;
                numPossibleMoves++;
            }
        }
    }
    if (numPossibleMoves == 0)
        return nullptr;
    std::shared_ptr<move[]> moves = std::shared_ptr<move[]>(new move[numPossibleMoves]);
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        if (knightMoves[i])
        {
            moves[count].newPosition = moveVectors[i] + position;
            moves[count].prevPosition = position;
            if (isCheck(moves[count], isPieceWhite))
            {
                numPossibleMoves--;
            } else
            {
                count++;
            }
        }
    }

    return moves;
}

short int
Gamestate::getDistanceInDirection(int2 position, int2 moveVector, bool isPieceWhite, Gamestate::boardGrid board_)
{
    if (!board_)
        board_ = board;
    short int count = 0;
    for (int i = 1; i < 8; i++)
    {
        if (isOnBoard(position + moveVector * i))
        {
            //check if we can move up to an empty piece
            if (board_[position.b + (i * moveVector.b)][position.a + (i * moveVector.a)].pieceType == "empty")
                count++;
                //check if we can move up to take a piece
            else
            {
                if (board_[position.b + (i * moveVector.b)][position.a + (i * moveVector.a)].isWhite != isPieceWhite)
                {
                    count++;
                }
                break;

            }
        } else
        {
            break;
        }
    }
    return count;
}


std::shared_ptr<move[]> Gamestate::GetMovesFromVectors(int2 position, int2 *moveVectors, int numMoveVectors)
{
    bool isPieceWhite = board[position.b][position.a].isWhite;
    std::unique_ptr<short int[]> moveAmounts(new short int[numMoveVectors]);
    numPossibleMoves = 0;
    for (int i = 0; i < numMoveVectors; i++)
    {
        moveAmounts[i] = getDistanceInDirection(position, moveVectors[i], isPieceWhite);
        numPossibleMoves += moveAmounts[i];
    }
    if (numPossibleMoves == 0)
    {
        return nullptr;
    }
    std::shared_ptr<move[]> moves = std::shared_ptr<move[]>(new move[numPossibleMoves]);
    int previouslyAdded = 0;
    addingRoutesOffset = 0;
    for (int i = 0; i < numMoveVectors; i++)
    {
        addRouteToArray(moves, previouslyAdded, moveAmounts[i], moveVectors[i], position);
        previouslyAdded += moveAmounts[i];
    }
    return moves;
}

std::shared_ptr<move[]> Gamestate::GetBishopMoves(int2 position)
{
    int2 moveVectors[] = {
            int2(1, 1),
            int2(1, -1),
            int2(-1, -1),
            int2(-1, 1)
    };
    return GetMovesFromVectors(position, moveVectors, 4);
}

std::shared_ptr<move[]> Gamestate::GetQueenMoves(int2 position)
{
    int2 moveVectors[] = {
            int2(1, 1),
            int2(1, -1),
            int2(-1, -1),
            int2(-1, 1),
            int2(0, 1),
            int2(0, -1),
            int2(-1, 0),
            int2(1, 0)
    };
    return GetMovesFromVectors(position, moveVectors, 8);
}

std::shared_ptr<move[]> Gamestate::GetKingMoves(int2 position)
{
    bool isPieceWhite = board[position.b][position.a].isWhite;
    int2 moveVectors[] = {int2(0, 1),
                          int2(1, 0),
                          int2(1, 1),
                          int2(0, -1),
                          int2(-1, 0),
                          int2(-1, -1),
                          int2(-1, 1),
                          int2(1, -1)};
    bool kingMoves[] = {false, false, false, false, false, false, false, false};
    numPossibleMoves = 0;
    for (int i = 0; i < 8; i++)
    {
        int2 movePos = moveVectors[i] + position;
        if (isOnBoard(movePos))
        {
            if (board[movePos.b][movePos.a].isWhite != isPieceWhite || board[movePos.b][movePos.a].pieceType == "empty")
            {
                kingMoves[i] = true;
                numPossibleMoves++;
            }
        }
    }
    if (numPossibleMoves == 0)
        return nullptr;
    std::shared_ptr<move[]> moves = std::shared_ptr<move[]>(new move[numPossibleMoves]);
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        if (kingMoves[i])
        {
            moves[count].prevPosition = position;
            moves[count].newPosition = position + moveVectors[i];
            if (isCheck(moves[count], isPieceWhite))
            {
                numPossibleMoves--;
            } else
            {
                count++;
            }

        }
    }
    return moves;
}

void Gamestate::addRouteToArray(const std::shared_ptr<move[]> &moveArray, int startIndex, int numToAdd, int2 moveVector,
                                int2 position)
{

    for (int i = 0; i < numToAdd; i++)
    {
        moveArray[startIndex + i + addingRoutesOffset].prevPosition = position;
        moveArray[startIndex + i + addingRoutesOffset].newPosition = position + moveVector * (i + 1);
        if (isCheck(moveArray[startIndex + i + addingRoutesOffset], board[position.b][position.a].isWhite))
        {
            numPossibleMoves--;
            addingRoutesOffset--;
        }
    }
}


std::shared_ptr<move[]> Gamestate::GetRookMoves(int2 position)
{
    int2 moveVectors[] = {
            int2(0, 1),
            int2(0, -1),
            int2(-1, 0),
            int2(1, 0)
    };
    return GetMovesFromVectors(position, moveVectors, 4);
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
        int count = 0;
        if (forward >= 1)
        {
            moves[count].prevPosition = position;
            moves[count].newPosition = int2(position.a, position.b + 1);
            if (isCheck(moves[count], isPieceWhite))
            {
                numPossibleMoves--;
            } else
            {
                count++;
            }
        }
        if (forward == 2)
        {
            moves[count].prevPosition = position;
            moves[count].newPosition = int2(position.a, position.b + 2);
            if (isCheck(moves[count], isPieceWhite))
            {
                numPossibleMoves--;
            } else
            {
                count++;
            }
        }
        if (takeRight)
        {
            moves[count].prevPosition = position;
            moves[count].newPosition = int2(position.a + 1, position.b + 1);
            if (isCheck(moves[count], isPieceWhite))
            {
                numPossibleMoves--;
            } else
            {
                count++;
            }
        }
        if (takeLeft)
        {
            moves[count].prevPosition = position;
            moves[count].newPosition = int2(position.a - 1, position.b + 1);
            if (isCheck(moves[count], isPieceWhite))
            {
                numPossibleMoves--;
            } else
            {
                count++;
            }
        }
        numPossibleMoves = count;
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
        int count = 0;
        if (forward >= 1)
        {
            moves[count].prevPosition = position;
            moves[count].newPosition = int2(position.a, position.b - 1);
            if (isCheck(moves[count], isPieceWhite))
            {
                numPossibleMoves--;
            } else
            {
                count++;
            }
        }
        if (forward == 2)
        {
            moves[count].prevPosition = position;
            moves[count].newPosition = int2(position.a, position.b - 2);
            if (isCheck(moves[count], isPieceWhite))
            {
                numPossibleMoves--;
            } else
            {
                count++;
            }
        }
        if (takeRight)
        {
            moves[count].prevPosition = position;
            moves[count].newPosition = int2(position.a + 1, position.b - 1);
            if (isCheck(moves[count], isPieceWhite))
            {
                numPossibleMoves--;
            } else
            {
                count++;
            }
        }
        if (takeLeft)
        {
            moves[count].prevPosition = position;
            moves[count].newPosition = int2(position.a - 1, position.b - 1);
            if (isCheck(moves[count], isPieceWhite))
            {
                numPossibleMoves--;
            } else
            {
                count++;
            }
        }
        numPossibleMoves = count;
        return moves;
    }
}