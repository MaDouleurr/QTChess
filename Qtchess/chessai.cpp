#include "chessai.h"
#include "chessboard.h"
#include "chessboardai.h"
#include <random>
#include <ctime>

ChessAI::ChessAI() : cell1(8, 8), cell2(8, 8) {}

ChessAI::~ChessAI() {}

ChessAI& ChessAI::getMove(
    ChessBoardAI* newBoard) {  // Can be optimized and improved
  static ChessAI chessai;
  CrabTree* tree = new CrabTree(newBoard, newBoard->getPmode());
  tree->findMoves();
  int min = 1000, max = -1000, maxi = -1;
  for (uint i = 0; i < tree->nodes.size(); i++) {
    tree->nodes[i]->findMoves();
    int sum = tree->nodes[i]->sumValues();
    if (sum > max) {
      max = sum;
      maxi = i;
    }
    if (sum < min) min = sum;
  }

  if (min == max) {
    std::mt19937 gen;
    gen.seed(time(NULL));
    std::uniform_int_distribution<int> uid(0, tree->nodes.size() - 1);
    maxi = uid(gen);
  }
  chessai.cell1 = tree->nodes[maxi]->cell1;
  chessai.cell2 = tree->nodes[maxi]->cell2;
  delete tree;
  return chessai;
}

const std::pair<int, int>& ChessAI::getCell1() { return cell1; }
const std::pair<int, int>& ChessAI::getCell2() { return cell2; }
