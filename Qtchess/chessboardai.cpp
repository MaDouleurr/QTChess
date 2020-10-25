#include "chessboardai.h"
#include <QIcon>

ChessBoardAI::ChessBoardAI() : ChessBoard() {}

ChessBoardAI::ChessBoardAI(const ChessBoard& newBoard) : ChessBoard(newBoard) {}

ChessBoardAI::~ChessBoardAI() {}

void ChessBoardAI::findMoves(const QChar NotMoverColor, CrabTree* tree) {
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (board[i][j][0] != NotMoverColor and board[i][j] != "  ") {
        if (board[i][j][1] == 'P') {
          if (Move(std::pair<int, int>(i, j), std::pair<int, int>(i + Pmode, j),
                   false))
            tree->add(std::pair<int, int>(i, j),
                      std::pair<int, int>(i + Pmode, j));
          if (Move(std::pair<int, int>(i, j),
                   std::pair<int, int>(i + Pmode, j + Pmode), false))
            tree->add(std::pair<int, int>(i, j),
                      std::pair<int, int>(i + Pmode, j + Pmode));
          if (Move(std::pair<int, int>(i, j),
                   std::pair<int, int>(i + Pmode, j - Pmode), false))
            tree->add(std::pair<int, int>(i, j),
                      std::pair<int, int>(i + Pmode, j - Pmode));
        }
        if (board[i][j][1] == 'R') {
          for (int k = 0; k < boardSize; k++) {
            if (Move(std::pair<int, int>(i, j), std::pair<int, int>(k, j),
                     false))
              tree->add(std::pair<int, int>(i, j), std::pair<int, int>(k, j));
            if (Move(std::pair<int, int>(i, j), std::pair<int, int>(i, k),
                     false))
              tree->add(std::pair<int, int>(i, j), std::pair<int, int>(i, k));
          }
        }
        if (board[i][j][1] == 'N') {
          for (int k = -2; k <= 2; k++)
            for (int z = -2; z <= 2; z++)
              if (abs(k) != abs(z) and k != 0 and z != 0 and
                  Move(std::pair<int, int>(i, j),
                       std::pair<int, int>(i + k, j + z), false))
                tree->add(std::pair<int, int>(i, j),
                          std::pair<int, int>(i + k, j + z));
        }
        if (board[i][j][1] == 'B') {
          for (int k = -static_cast<int>(boardSize) + 1;
               k < static_cast<int>(boardSize); k++) {
            if (Move(std::pair<int, int>(i, j),
                     std::pair<int, int>(i + k, j + k), false))
              tree->add(std::pair<int, int>(i, j),
                        std::pair<int, int>(i + k, j + k));
            if (Move(std::pair<int, int>(i, j),
                     std::pair<int, int>(i + k, j - k), false))
              tree->add(std::pair<int, int>(i, j),
                        std::pair<int, int>(i + k, j - k));
          }
        }
        if (board[i][j][1] == 'Q') {
          for (int k = -static_cast<int>(boardSize) + 1;
               k < static_cast<int>(boardSize); k++) {
            if (Move(std::pair<int, int>(i, j),
                     std::pair<int, int>(i + k, j + k), false))
              tree->add(std::pair<int, int>(i, j),
                        std::pair<int, int>(i + k, j + k));
            if (Move(std::pair<int, int>(i, j),
                     std::pair<int, int>(i + k, j - k), false))
              tree->add(std::pair<int, int>(i, j),
                        std::pair<int, int>(i + k, j - k));
            if (Move(std::pair<int, int>(i, j), std::pair<int, int>(k, j),
                     false))
              tree->add(std::pair<int, int>(i, j), std::pair<int, int>(k, j));
            if (Move(std::pair<int, int>(i, j), std::pair<int, int>(i, k),
                     false))
              tree->add(std::pair<int, int>(i, j), std::pair<int, int>(i, k));
          }
        }
        if (board[i][j][1] == 'K')
          for (int k = -1; k <= 1; k++)
            for (int z = -1; z <= 1; z++)
              if (Move(std::pair<int, int>(i, j),
                       std::pair<int, int>(i + k, j + z), false))
                tree->add(std::pair<int, int>(i, j),
                          std::pair<int, int>(i + k, j + z));
      }
    }
  }
}

bool ChessBoardAI::Move(const std::pair<int, int>& cell1,
                        const std::pair<int, int>& cell2, bool message) {
  int Castling;
  QString FigureCell2 = "  ";
  int move = CheckMove(cell1, cell2, message);
  if (move == 0) return false;
  if (move == 1 or move == 2) {
    FigureCell2 = board[cell2.first][cell2.second];
    board[cell2.first][cell2.second] = board[cell1.first][cell1.second];
    board[cell1.first][cell1.second] = "  ";
    if (move == 2) {
      board[cell2.first - Pmode][cell2.second] = "  ";
    }
    if (board[cell2.first][cell2.second][1] == 'K') {
      if (Pmode == 1) {
        WhiteKingPosition.first = cell2.first;
        WhiteKingPosition.second = cell2.second;
      } else {
        BlackKingPosition.first = cell2.first;
        BlackKingPosition.second = cell2.second;
      }
    }
  }
  bool shah = Shah();
  if (not message or (shah and (move == 1 or move == 2))) {
    if (message) sendMessage("you can`t move this figure, shah!");
    board[cell1.first][cell1.second] = board[cell2.first][cell2.second];
    if (move) {
      board[cell2.first][cell2.second] = FigureCell2;
      if (board[cell1.first][cell1.second][1] == 'K') {
        if (Pmode == 1) {
          WhiteKingPosition.first = cell1.first;
          WhiteKingPosition.second = cell1.second;
        } else {
          BlackKingPosition.first = cell1.first;
          BlackKingPosition.second = cell1.second;
        }
      }
    }
    if (move == 2) {
      board[cell2.first - Pmode][cell2.second] =
          QString(((Pmode == 1) ? "W" : "B")) + QString('P');
    }
    if (shah and (move == 1 or move == 2)) return false;
  }
  if (move == 3) Castling = 1;
  if (move == 4) Castling = -1;
  if (move > 2) {
    board[cell1.first][cell1.second - 1 * Castling] =
        board[cell2.first][cell2.second];
    board[cell1.first][cell1.second - 2 * Castling] =
        board[cell1.first][cell1.second];
    board[cell1.first][cell1.second] = "  ";
    board[cell2.first][cell2.second] = "  ";
    if (Pmode == 1)
      WhiteKingPosition.second = cell1.second - 2 * Castling;
    else
      BlackKingPosition.second = cell1.second - 2 * Castling;
  }
  if (cell2.first == 0 and cell2.second == 0) WhiteKingOrLeftRookMove = true;
  if (cell2.first == 0 and cell2.second == 7) WhiteKingOrRightRookMove = true;
  if (cell2.first == 7 and cell2.second == 0) BlackKingOrLeftRookMove = true;
  if (cell2.first == 7 and cell2.second == 7) BlackKingOrRightRookMove = true;
  if (move != 0 and (board[cell1.first][cell1.second][1] == 'K' or
                     board[cell2.first][cell2.second][1] == 'K')) {
    if (Pmode == 1) {
      WhiteKingOrLeftRookMove = true;
      WhiteKingOrRightRookMove = true;
    } else {
      BlackKingOrLeftRookMove = true;
      BlackKingOrRightRookMove = true;
    }
  }
  if (board[cell1.first][cell1.second][1] == 'R') {
    if (cell1.first == 0 and Pmode == 1) {
      if (cell1.second == 0) WhiteKingOrLeftRookMove = true;
      if (cell1.second == 7) WhiteKingOrRightRookMove = true;
    }
    if (cell1.first == 7 and Pmode == -1) {
      if (cell1.second == 0) BlackKingOrLeftRookMove = true;
      if (cell1.second == 7) BlackKingOrRightRookMove = true;
    }
  }
  if (board[cell1.first][cell1.second][1] == 'K') {
    if (Pmode == 1) {
      WhiteKingOrLeftRookMove = true;
      WhiteKingOrRightRookMove = true;
    } else {
      BlackKingOrLeftRookMove = true;
      BlackKingOrRightRookMove = true;
    }
  }
  if (message) {
    LastCell1 = cell1;
    LastCell2 = cell2;
  }
  return true;
}

int ChessBoardAI::getPmode() { return Pmode; }
