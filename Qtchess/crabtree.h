#ifndef CRABTREE_H
#define CRABTREE_H
#include <vector>
#include <QChar>

class ChessBoardAI;

class CrabTree {
 public:
  std::vector<CrabTree*> nodes;
  ChessBoardAI* board;
  const int Pmode;
  std::pair<int, int> cell1, cell2;
  int value;
  CrabTree(ChessBoardAI* newBoard, const int Pmode,
           const std::pair<int, int>& cell1 = std::pair<int, int>(8, 8),
           const std::pair<int, int>& cell2 = std::pair<int, int>(8, 8));
  void add(const std::pair<int, int>& cell1, const std::pair<int, int>& cell2);
  ~CrabTree();
  void countValue();
  int costPiece(const QChar piece) const;
  void findMoves();
  int sumValues();
};

#endif  // CRABTREE_H
