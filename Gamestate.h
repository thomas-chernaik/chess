//
// Created by thomas on 02/08/23.
//
#include <iostream>
#include <memory>


#ifndef CHESS_GAMESTATE_H
#define CHESS_GAMESTATE_H


struct int2
{
    int a = 0;
    int b = 0;

    inline bool operator==(const int2 &rhs) const;

    inline int2 operator+(const int2 &rhs) const;

    inline int2 operator*(const int &rhs) const;
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

    explicit Gamestate(const boardGrid &board_ = nullptr, bool white = true);


    boardGrid DisplayState();

    void GeneratePossibleMoves();

    void GetNextGameState(move nextMove);

    void DebugGameState();

    void SelectSquare(int2 selected);

    int numPossibleMoves = 0;

    std::shared_ptr<move[]> possibleMoves;

    std::shared_ptr<int2[]> GetSquaresToHighlight();

    void LoadGame(const std::string& filename);

    void SaveGame(const std::string& filename);

protected:
    bool isWhite;
    boardGrid board;
    int2 selectedSquare = int2(-1, -1);

    std::shared_ptr<move[]> GetPawnMoves(int2 position);

    std::shared_ptr<move[]> GetRookMoves(int2 position);

    std::shared_ptr<move[]> GetKnightMoves(int2 position);

    std::shared_ptr<move[]> GetBishopMoves(int2 position);

    std::shared_ptr<move[]> GetQueenMoves(int2 position);

    std::shared_ptr<move[]> GetKingMoves(int2 position);

    std::shared_ptr<move[]> GetMovesFromVectors(int2 position, int2 moveVectors[], int numMoveVectors);

    short int getDistanceInDirection(int2 position, int2 moveVector, bool isPieceWhite, boardGrid board_ = nullptr);

    static bool isOnBoard(int2 pos);

    void addRouteToArray(const std::shared_ptr<move[]> &moveArray, int startIndex, int numToAdd, int2 moveVector,
                         int2 position);

    boardGrid GetTemporaryMove(move nextMove);

    bool isCheck(move moveToCheck, bool isPieceWhite);


    int addingRoutesOffset = 0;

};


#endif //CHESS_GAMESTATE_H
