#include "crabtree.h"
#include "chesskboardai.h"
#include <QString>
CrabTree::CrabTree(ChessBoardAI* newBoard, const int Pmode,
                   const std::pair<int, int>& cell1,
                   const std::pair<int, int>& cell2)
    : Pmode(Pmode), cell1(cell1), cell2(cell2) {
  value = 0;
  board = newBoard;
  countValue();
}

CrabTree::~CrabTree() {
  for (uint i = 0; i < nodes.size(); i++) delete nodes[i];
}

void CrabTree::add(const std::pair<int, int>& cell1,
                   const std::pair<int, int>& cell2) {
  ChessBoardAI* newBoard = new ChessBoardAI(*board);
  newBoard->Move(cell1, cell2);
  newBoard->Pmode = -newBoard->Pmode;
  nodes.push_back(new CrabTree(newBoard, Pmode, cell1, cell2));
}

void CrabTree::countValue() {
  QChar notMoverColor = (Pmode == 1) ? 'B' : 'W';
  for (uint i = 0; i < board->boardSize; i++) {
    for (uint j = 0; j < board->boardSize; j++) {
      int mode = (board->board[i][j][0] == notMoverColor) ? -1 : 1;
      value += mode * costPiece(board->board[i][j][1]);
    }
  }
}

int CrabTree::costPiece(const QChar piece) const {
  if (piece == 'P') return 10;
  if (piece == 'N') return 30;
  if (piece == 'B') return 30;
  if (piece == 'R') return 50;
  if (piece == 'Q') return 90;
  if (piece == 'K') return 900;
  return 0;
}

void CrabTree::findMoves() {
  board->findMoves((board->Pmode == 1) ? 'B' : 'W', this);
}

int CrabTree::sumValues() {
  int sum = 0;
  for (uint i = 0; i < nodes.size(); i++) sum += nodes[i]->value;
  return sum;
}
