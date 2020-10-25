#ifndef CHESSBOARDAI_H
#define CHESSBOARDAI_H
#include "chessboard.h"
#include "crabtree.h"

class ChessBoardAI : public ChessBoard {
  void findMoves(const QChar NotMoverColor, CrabTree* tree);
  bool Move(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2,
            bool message = true) override;

 public:
  ChessBoardAI();
  ChessBoardAI(const ChessBoard& newBoard);
  ~ChessBoardAI() override;
  int getPmode();
};

#endif  // CHESSBOARDAI_H
