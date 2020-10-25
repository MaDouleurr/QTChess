#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <utility>
#include <vector>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include "myqpushbutton.h"
#include "chessboard.h"
#include "chessai.h"

namespace Ui {
class Chess;
}

class Chess : public QMainWindow, public ChessBoard {
  Q_OBJECT

 public:
  explicit Chess(QWidget *parent = nullptr);
  ~Chess() override;

 private:
  Ui::Chess *ui;
  const uint cellSize = 80;
  std::pair<int, int> screenSize;
  std::vector<std::vector<MyQPushButton *>> board_b;
  QPushButton *play_b;
  QPushButton *exit_b;
  QLabel *player1_l;
  QLabel *player2_l;
  std::vector<QComboBox *> player_cb;
  int computerMode;
  ChessAI *chessai;
  std::vector<MyQPushButton *> pawnTo_b;
  QString pawnTo_S;
  int newComputerMode;

  void setPieces();
  void setCellColor(uint, uint);
  void clearBoard();
  void updatePlayer1_cb(const QString &, uint);
  void EnabledCellButtons(const bool en = true);
  void PawnToPiece();
  bool Move(const std::pair<int, int> &cell1, const std::pair<int, int> &cell2,
            bool message = true) override;
  void computerMove();
 private slots:
  void exit();
  void play();
  void cellClick();
  void pawnTo();
};

#endif  // CHESS_H
