#include "chessboard.h"
#include <vector>
#include <QString>
#include <QMessageBox>

ChessBoard::ChessBoard()
    : board(boardSize, std::vector<QString>(boardSize, QString("  "))) {
  Initiation();
}

ChessBoard::ChessBoard(const ChessBoard& newBoard)
    : firstCell(newBoard.firstCell),
      secondCell(newBoard.secondCell),
      WhiteKingPosition(newBoard.WhiteKingPosition),
      BlackKingPosition(newBoard.BlackKingPosition) {
  board = newBoard.board;
  Pmode = newBoard.Pmode;
  WhiteKingOrLeftRookMove = newBoard.WhiteKingOrLeftRookMove;
  WhiteKingOrRightRookMove = newBoard.WhiteKingOrRightRookMove;
  BlackKingOrLeftRookMove = newBoard.BlackKingOrLeftRookMove;
  BlackKingOrRightRookMove = newBoard.BlackKingOrRightRookMove;
}

ChessBoard::~ChessBoard() {}

void ChessBoard::Initiation() {
  firstCell.first = boardSize;
  secondCell.first = boardSize;
  WhiteKingOrLeftRookMove = false;
  WhiteKingOrRightRookMove = false;
  BlackKingOrLeftRookMove = false;
  BlackKingOrRightRookMove = false;
  WhiteKingPosition.first = 0;
  WhiteKingPosition.second = 4;
  BlackKingPosition.first = 7;
  BlackKingPosition.second = 4;
}

void ChessBoard::sendMessage(const QString& mes) const {
  QMessageBox msgBox;
  msgBox.setText(mes);
  msgBox.exec();
}

int ChessBoard::CheckMove(const std::pair<int, int>& cell1,
                          const std::pair<int, int>& cell2,
                          bool message) const {
  if (cell2.first < 0 or cell2.first > 7 or cell2.second < 0 or
      cell2.second > 7)
    return false;
  char NotMoverColor = (Pmode == 1) ? 'B' : 'W';
  if (board[cell1.first][cell1.second] == "  ") {
    if (message) sendMessage("Empty cell");
    return 0;
  }
  if (board[cell1.first][cell1.second][0] == NotMoverColor) {
    if (message) sendMessage("Figure of the opponent in selected cell");
    return 0;
  }
  if (cell1.first == cell2.first and cell1.second == cell2.second) {
    if (message) sendMessage("You don`t move");
    return 0;
  }
  if (board[cell1.first][cell1.second][1] == 'K' and
      board[cell2.first][cell2.second][1] == 'R' and
      board[cell2.first][cell2.second][0] != NotMoverColor) {
    return CheckMoveCastling(cell1, cell2, NotMoverColor, message);
  }
  if (board[cell2.first][cell2.second] != "  " and
      board[cell2.first][cell2.second][0] != NotMoverColor) {
    if (message) sendMessage("Your figure in another selcted cell");
    return 0;
  }
  if (board[cell1.first][cell1.second][1] == 'P') {
    return CheckMovePawn(cell1, cell2, NotMoverColor, message);
  }
  if (board[cell1.first][cell1.second][1] == 'R') {
    return CheckMoveRook(cell1, cell2, message);
  }
  if (board[cell1.first][cell1.second][1] == 'N') {
    return CheckMoveKnight(cell1, cell2, message);
  }
  if (board[cell1.first][cell1.second][1] == 'B') {
    return CheckMoveBishop(cell1, cell2, message);
  }
  if (board[cell1.first][cell1.second][1] == 'Q') {
    return CheckMoveQeen(cell1, cell2, message);
  }
  if (board[cell1.first][cell1.second][1] == 'K') {
    return CheckMoveKing(cell1, cell2, message);
  } else
    return 0;
}

int ChessBoard::CheckMovePawn(const std::pair<int, int>& cell1,
                              const std::pair<int, int>& cell2,
                              const QChar NotMoverColor, bool message) const {
  int Step = Pmode * (cell2.first - cell1.first);
  if (Step > 0 and Step < 3) {
    if (cell1.second == cell2.second and
        CheckVerticalMove(cell1.first, cell2.first, cell1.second, Pmode,
                          message) and
        board[cell2.first][cell2.second] == "  ") {
      if (Step == 2) {
        if (cell1.first == static_cast<int>(3.5 - Pmode * 2.5))
          return 1;
        else {
          if (message)
            sendMessage(
                "Can`t make two-cells move, because it`s not the first move "
                "that pawn ");
          return 0;
        }
      }
      return 1;
    }

    else {
      if (Step == 1 and abs(cell1.second - cell2.second) == 1 and
          board[cell2.first][cell2.second][0] == NotMoverColor)
        return 1;
      if (cell1.first == static_cast<int>(3.5 + 0.5 * Pmode) and
          board[cell2.first - Pmode][cell2.second] ==
              ((Pmode == 1) ? "BP" : "WP") and
          cell2.first - Pmode == LastCell2.first and
          cell2.second == LastCell2.second and
          -Pmode * (LastCell2.first - LastCell1.first) == 2)
        return 2;
      if (message) sendMessage("There is no opponent's figure on the diagonal");
      return 0;
    }
  }
  if (message)
    sendMessage("The pawn can`t make that pawn " + QString::number(Step));
  return 0;
}

int ChessBoard::CheckMoveRook(const std::pair<int, int>& cell1,
                              const std::pair<int, int>& cell2,
                              bool message) const {
  if (cell1.first != cell2.first and cell1.second != cell2.second) {
    if (message) sendMessage("The rook can only move perpendicular");
    return 0;
  }
  if (cell1.first != cell2.first)
    return CheckVerticalMove(
        cell1.first, cell2.first, cell1.second,
        (cell2.first - cell1.first) / abs(cell2.first - cell1.first), message);
  else
    return CheckGorizontalMove(cell1.second, cell2.second, cell1.first,
                               message);
}

int ChessBoard::CheckMoveKnight(const std::pair<int, int>& cell1,
                                const std::pair<int, int>& cell2,
                                bool message) const {
  if ((abs(cell1.first - cell2.first) == 2 and
       abs(cell1.second - cell2.second) == 1) or
      (abs(cell1.first - cell2.first) == 1 and
       abs(cell1.second - cell2.second) == 2))
    return 1;
  if (message) sendMessage("Knight can`t move that way");
  return 0;
}

int ChessBoard::CheckMoveBishop(const std::pair<int, int>& cell1,
                                const std::pair<int, int>& cell2,
                                bool message) const {
  if (abs(cell1.first - cell2.first) != abs(cell1.second - cell2.second)) {
    if (message) sendMessage("Wrong move for bishop");
    return 0;
  }
  return CheckDiagonalMove(cell1, cell2, message);
}

int ChessBoard::CheckMoveQeen(const std::pair<int, int>& cell1,
                              const std::pair<int, int>& cell2,
                              bool message) const {
  if (cell1.first == cell2.first and cell1.second != cell2.second)
    return CheckGorizontalMove(cell1.second, cell2.second, cell1.first,
                               message);
  if (cell1.first != cell2.first and cell1.second == cell2.second)
    return CheckVerticalMove(
        cell1.first, cell2.first, cell1.second,
        (cell2.first - cell1.first) / abs(cell2.first - cell1.first), message);
  if (abs(cell1.first - cell2.first) == abs(cell1.second - cell2.second)) {
    return CheckDiagonalMove(cell1, cell2, message);
  }
  if (message) sendMessage("Wrong move for Qeen");
  return 0;
}

int ChessBoard::CheckMoveKing(const std::pair<int, int>& cell1,
                              const std::pair<int, int>& cell2,
                              bool message) const {
  if (abs(cell1.first - cell2.first) > 1 or
      abs(cell1.second - cell2.second) > 1) {
    sendMessage("The king can only move one cell ");
    return 0;
  }
  if (cell1.first == cell2.first and cell1.second != cell2.second)
    return CheckGorizontalMove(cell1.second, cell2.second, cell1.first,
                               message);
  if (cell1.first != cell2.first and cell1.second == cell2.second)
    return CheckVerticalMove(
        cell1.first, cell2.first, cell1.second,
        (cell2.first - cell1.first) / abs(cell2.first - cell1.first), message);
  if (abs(cell1.first - cell2.first) == abs(cell1.second - cell2.second)) {
    return CheckDiagonalMove(cell1, cell2, message);
  }
  if (message) sendMessage("Wrong move for King");
  return 0;
}

int ChessBoard::CheckMoveCastling(const std::pair<int, int>& cell1,
                                  const std::pair<int, int>& cell2,
                                  const QChar NotMoverColor,
                                  bool message) const {
  if (CheckGorizontalMove(cell1.second, cell2.second, cell1.first, false))
    if (Pmode == 1) {
      if (cell2.first == 0 and cell2.second == 0 and
          not WhiteKingOrLeftRookMove and CheckCellsForCastling(cell1, cell2)) {
        return 3;
      }
      if (cell2.first == 0 and cell2.second == 7 and
          not WhiteKingOrRightRookMove and
          CheckCellsForCastling(cell1, cell2)) {
        return 4;
      }
    } else {
      if (cell2.first == 7 and cell2.second == 0 and
          not BlackKingOrLeftRookMove and CheckCellsForCastling(cell1, cell2)) {
        return 3;
      }
      if (cell2.first == 7 and cell2.second == 7 and
          not BlackKingOrRightRookMove and
          CheckCellsForCastling(cell1, cell2)) {
        return 4;
      }
    }

  if (message) sendMessage("Castling is not possible");
  return 0;
}

bool ChessBoard::CheckVerticalMove(const int v1, const int v2, const int g,
                                   const int mode, bool message) const {
  for (int i = v1 + mode; i != v2; i += mode) {
    if (board[i][g] != "  ") {
      if (message) sendMessage("This figure can`t jump over");
      return false;
    }
  }
  return true;
}

bool ChessBoard::CheckGorizontalMove(const int g1, const int g2, const int v,
                                     bool message) const {
  if (g1 == g2) return false;
  int mode = (g2 - g1) / abs(g2 - g1);
  for (int i = g1 + mode; i != g2; i += mode) {
    if (board[v][i] != "  ") {
      if (message) sendMessage("This figure can`t jump over");
      return false;
    }
  }
  return true;
}

bool ChessBoard::CheckDiagonalMove(const std::pair<int, int>& cell1,
                                   const std::pair<int, int>& cell2,
                                   bool message) const {
  int Pmode1 = (cell2.first - cell1.first) / abs(cell2.first - cell1.first);
  int Pmode2 = (cell2.second - cell1.second) / abs(cell2.second - cell1.second);
  for (int i = cell1.first + Pmode1, j = cell1.second + Pmode2;
       i != cell2.first; i += Pmode1, j += Pmode2)
    if (board[i][j] != "  ") {
      if (message) sendMessage("This figure can`t jump over");
      return 0;
    }
  return 1;
}

bool ChessBoard::CheckDiagonalLine(const std::pair<int, int>& cell,
                                   const QChar NotMoverColor) const {
  for (int i = cell.first + 1, j = cell.second + 1;
       i < boardSize and j < boardSize; i++, j++) {
    if (board[i][j][0] == NotMoverColor and
        (board[i][j][1] == 'Q' or board[i][j][1] == 'B'))
      return false;
    if (board[i][j] != "  ") break;
  }
  for (int i = cell.first - 1, j = cell.second + 1; i > -1 and j < boardSize;
       i--, j++) {
    if (board[i][j][0] == NotMoverColor and
        (board[i][j][1] == 'Q' or board[i][j][1] == 'B'))
      return false;
    if (board[i][j] != "  ") break;
  }
  for (int i = cell.first + 1, j = cell.second - 1; i < boardSize and j > -1;
       i++, j--) {
    if (board[i][j][0] == NotMoverColor and
        (board[i][j][1] == 'Q' or board[i][j][1] == 'B'))
      return false;
    if (board[i][j] != "  ") break;
  }
  for (int i = cell.first - 1, j = cell.second - 1; i > -1 and j > -1;
       i--, j--) {
    if (board[i][j][0] == NotMoverColor and
        (board[i][j][1] == 'Q' or board[i][j][1] == 'B'))
      return false;
    if (board[i][j] != "  ") break;
  }
  return true;
}

bool ChessBoard::CheckVerticalLine(const std::pair<int, int>& cell,
                                   const QChar NotMoverColor) const {
  for (int i = cell.first + 1; i < boardSize; i++) {
    if (board[i][cell.second][0] == NotMoverColor and
        (board[i][cell.second][1] == 'Q' or board[i][cell.second][1] == 'R'))
      return false;
    if (board[i][cell.second] != "  ") break;
  }
  for (int i = cell.first - 1; i > -1; i--) {
    if (board[i][cell.second][0] == NotMoverColor and
        (board[i][cell.second][1] == 'Q' or board[i][cell.second][1] == 'R'))
      return false;
    if (board[i][cell.second] != "  ") break;
  }
  return true;
}

bool ChessBoard::CheckGorizontalLine(const std::pair<int, int>& cell,
                                     const QChar NotMoverColor) const {
  for (int i = cell.second + 1; i < boardSize; i++) {
    if (board[cell.first][i][0] == NotMoverColor and
        (board[cell.first][i][1] == 'Q' or board[cell.first][i][1] == 'R'))
      return false;
    if (board[cell.first][i] != "  ") break;
  }
  for (int i = cell.second - 1; i > -1; i--) {
    if (board[cell.first][i][0] == NotMoverColor and
        (board[cell.first][i][1] == 'Q' or board[cell.first][i][1] == 'R'))
      return false;
    if (board[cell.first][i] != "  ") break;
  }
  return true;
}

bool ChessBoard::CheckKnight(const std::pair<int, int>& cell,
                             const QChar NotMoverColor) const {
  QString knight = "" + QString(NotMoverColor) + QString('N');
  for (int i = -2; i <= 2; i++)
    for (int j = -2; j <= 2; j++)
      if (abs(i) != abs(j) and i != 0 and j != 0 and cell.first + i > -1 and
          cell.first + i < boardSize and cell.second + j > -1 and
          cell.second + j < boardSize and
          board[cell.first + i][cell.second + j] == knight)
        return false;
  return true;
}

bool ChessBoard::CheckCellsForCastling(const std::pair<int, int>& cell1,
                                       const std::pair<int, int>& cell2) const {
  int Step = (cell2.second - cell1.second) / abs(cell2.second - cell1.second);
  for (int i = cell1.second; i != cell2.second - Step; i += Step)
    if (!CheckCell(std::pair<int, int>(cell1.first, i),
                   (Pmode == 1) ? 'B' : 'W'))
      return false;
  return true;
}

bool ChessBoard::CheckCell(const std::pair<int, int>& cell,
                           const QChar NotMoverColor) const {
  QString NotMoverPawn;
  QString NotMoverKing;
  if (Pmode == 1) {
    NotMoverPawn = "BP";
    NotMoverKing = "BK";
  } else {
    NotMoverPawn = "WP";
    NotMoverKing = "WK";
  }
  if (cell.first + Pmode < boardSize and cell.first + Pmode >= 0 and
      (board[cell.first + Pmode][cell.second] == NotMoverKing or
       (cell.second - 1 >= 0 and
        (board[cell.first + Pmode][cell.second - 1] == NotMoverPawn or
         board[cell.first + Pmode][cell.second - 1] == NotMoverKing)) or
       (cell.second + 1 < boardSize and
        (board[cell.first + Pmode][cell.second + 1] == NotMoverPawn or
         board[cell.first + Pmode][cell.second + 1] == NotMoverKing))))
    return false;
  return CheckDiagonalLine(cell, NotMoverColor) and
         CheckGorizontalLine(cell, NotMoverColor) and
         CheckVerticalLine(cell, NotMoverColor) and
         CheckKnight(cell, NotMoverColor);
}

bool ChessBoard::Shah() const {
  std::pair<int, int> a = WhiteKingPosition;
  return not CheckCell(((Pmode == 1) ? WhiteKingPosition : BlackKingPosition),
                       (Pmode == 1) ? 'B' : 'W');
}

bool ChessBoard::ExistenceMove(const QChar NotMoverColor) {
  for (int i = 0; i < boardSize; i++)
    for (int j = 0; j < boardSize; j++) {
      if (board[i][j][0] != NotMoverColor and board[i][j] != "  ") {
        if (board[i][j][1] == 'P') {
          if (Move(std::pair<int, int>(i, j), std::pair<int, int>(i + Pmode, j),
                   false))
            return true;
          if (Move(std::pair<int, int>(i, j),
                   std::pair<int, int>(i + Pmode, j + Pmode), false))
            return true;
          if (Move(std::pair<int, int>(i, j),
                   std::pair<int, int>(i + Pmode, j - Pmode), false))
            return true;
        }
        if (board[i][j][1] == 'R') {
          for (int k = 0; k < boardSize; k++) {
            if (Move(std::pair<int, int>(i, j), std::pair<int, int>(k, j),
                     false))
              return true;
            if (Move(std::pair<int, int>(i, j), std::pair<int, int>(i, k),
                     false))
              return true;
          }
        }
        if (board[i][j][1] == 'N') {
          for (int k = -2; k <= 2; k++)
            for (int z = -2; z <= 2; z++)
              if (abs(k) != abs(z) and k != 0 and z != 0 and
                  Move(std::pair<int, int>(i, j),
                       std::pair<int, int>(i + k, j + z), false))
                return true;
        }
        if (board[i][j][1] == 'B') {
          for (int k = -boardSize + 1; k < boardSize; k++) {
            if (Move(std::pair<int, int>(i, j),
                     std::pair<int, int>(i + k, j + k), false))
              return true;
            if (Move(std::pair<int, int>(i, j),
                     std::pair<int, int>(i + k, j - k), false))
              return true;
          }
        }
        if (board[i][j][1] == 'Q') {
          for (int k = -boardSize + 1; k < boardSize; k++) {
            if (Move(std::pair<int, int>(i, j),
                     std::pair<int, int>(i + k, j + k), false))
              return true;
            if (Move(std::pair<int, int>(i, j),
                     std::pair<int, int>(i + k, j - k), false))
              return true;
            if (Move(std::pair<int, int>(i, j), std::pair<int, int>(k, j),
                     false))
              return true;
            if (Move(std::pair<int, int>(i, j), std::pair<int, int>(i, k),
                     false))
              return true;
          }
        }
        if (board[i][j][1] == 'K')
          for (int k = -1; k <= 1; k++)
            for (int z = -1; z <= 1; z++)
              if (Move(std::pair<int, int>(i, j),
                       std::pair<int, int>(i + k, j + z), false))
                return true;
      }
    }
  return false;
}
