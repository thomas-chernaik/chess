//
// Created by thomas on 02/08/23.
//
#include <iostream>
#include <memory>


#ifndef CHESS_GAMESTATE_H
#define CHESS_GAMESTATE_H

struct float2
{
    float a = 0;
    float b = 0;
};

struct int2
{
    int a = 0;
    int b = 0;

    inline bool operator==(const int2 &rhs);
};

struct move
{
    int2 prevPosition;
    int2 newPosition;
};

//this can be made more efficient if needed later
struct square
{
    bool isWhite = false;
    std::string pieceType = "empty";
};

class Gamestate
{
public:
    using boardGrid = std::shared_ptr<std::shared_ptr<square[]>[]>;

    Gamestate(const boardGrid board_ = nullptr, bool white = true);

    ~Gamestate();

    boardGrid DisplayState();

    std::shared_ptr<move> GetPossibleMoves();

    bool IsCheck();

    bool IsCheckMate();

    void GetNextGameState(move nextMove);

    void DebugGameState();

    void SelectSquare(int2 selected);
    int numHighlighted;

    std::shared_ptr<move[]> possibleMoves;

    std::shared_ptr<int2[]> GetSquaresToHighlight();

protected:
    bool isWhite;
    boardGrid board;
    int numPossibleMoves;
    int2 selectedSquare = int2(-1, -1);

    move *GetPawnMoves(float2 position);

    move *GetRookMoves(float2 position);

    move *GetKnightMoves(float2 position);

    move *GetBishopMoves(float2 position);

    move *GetQueenMoves(float2 position);

    move *GetKingMoves(float2 position);


};


#endif //CHESS_GAMESTATE_H
