#ifndef CHESSKBOARDAI_H
#define CHESSKBOARDAI_H
#include "chessboard.h"
#include "crabtree.h"

class ChessBoardAI:public ChessBoard
{
    void findMoves(const QChar NotMoverColor,CrabTree * tree);
    bool Move(const std::pair<int, int>& cell1, const std::pair<int, int>&cell2, bool message = true)override;
    friend class CrabTree;
public:
    ChessBoardAI();
    ~ChessBoardAI();
    int getPmode();

};

#endif // CHESSKBOARDAI_H
