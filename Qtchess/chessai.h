#ifndef CHESSAI_H
#define CHESSAI_H
#include <utility>
#include <crabtree.h>
#include <chessboard.h>
class ChessAI {
  std::pair<int, int> cell1, cell2;
  ChessAI();
  ~ChessAI();

 public:
  static ChessAI& getMove(ChessBoardAI* newBoard);
  const std::pair<int, int>& getCell1();
  const std::pair<int, int>& getCell2();
};

#endif  // CHESSAI_H
