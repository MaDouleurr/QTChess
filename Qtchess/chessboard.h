#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <utility>
#include <QChar>
#include <vector>
class ChessBoard {
 protected:
  int Pmode;
  const uint boardSize = 8;
  std::pair<uint, uint> firstCell, secondCell;
  bool WhiteKingOrLeftRookMove;
  bool WhiteKingOrRightRookMove;
  bool BlackKingOrLeftRookMove;
  bool BlackKingOrRightRookMove;
  std::pair<int, int> WhiteKingPosition;
  std::pair<int, int> BlackKingPosition;
  std::pair<int, int> LastCell1;
  std::pair<int, int> LastCell2;
  std::vector<std::vector<QString>> board;

  virtual bool Move(const std::pair<int, int>& cell1,
                    const std::pair<int, int>& cell2, bool message = true) = 0;
  int CheckMove(const std::pair<int, int>& cell1,
                const std::pair<int, int>& cell2, bool message = true) const;
  int CheckMovePawn(const std::pair<int, int>& cell1,
                    const std::pair<int, int>& cell2, const QChar NotMoverColor,
                    bool message = true) const;
  int CheckMoveRook(const std::pair<int, int>& cell1,
                    const std::pair<int, int>& cell2,
                    bool message = true) const;
  int CheckMoveKnight(const std::pair<int, int>& cell1,
                      const std::pair<int, int>& cell2,
                      bool message = true) const;
  int CheckMoveBishop(const std::pair<int, int>& cell1,
                      const std::pair<int, int>& cell2,
                      bool message = true) const;
  int CheckMoveQeen(const std::pair<int, int>& cell1,
                    const std::pair<int, int>& cell2,
                    bool message = true) const;
  int CheckMoveKing(const std::pair<int, int>& cell1,
                    const std::pair<int, int>& cell2,
                    bool message = true) const;
  int CheckMoveCastling(const std::pair<int, int>& cell1,
                        const std::pair<int, int>& cell2,
                        const QChar NotMoverColor, bool message = true) const;
  bool CheckVerticalMove(const int v1, const int v2, const int g,
                         const int mode, bool message = true) const;
  bool CheckGorizontalMove(const int g1, const int g2, const int v,
                           bool message = true) const;
  bool CheckDiagonalMove(const std::pair<int, int>& cell1,
                         const std::pair<int, int>& cell2,
                         bool message = true) const;
  bool CheckDiagonalLine(const std::pair<int, int>& cell,
                         const QChar NotMoverColor) const;
  bool CheckVerticalLine(const std::pair<int, int>& cell,
                         const QChar NotMoverColor) const;
  bool CheckGorizontalLine(const std::pair<int, int>& cell,
                           const QChar NotMoverColor) const;
  bool CheckKnight(const std::pair<int, int>& cell,
                   const QChar NotMoverColor) const;
  bool CheckCell(const std::pair<int, int>& cell,
                 const QChar NotMoverColor) const;
  bool CheckCellsForCastling(const std::pair<int, int>& cell1,
                             const std::pair<int, int>& cell2) const;
  bool Shah() const;
  bool ExistenceMove(const QChar NotMoverColor);
  void Initiation();
  void sendMessage(const QString& mes) const;

 public:
  ChessBoard();
  ChessBoard(const ChessBoard& newBoard);
  virtual ~ChessBoard();
};

#endif  // CHESSBOARD_H
