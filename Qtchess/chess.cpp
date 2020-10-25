#include "chess.h"
#include "ui_chess.h"
#include "chessboardai.h"
#include <chessboard.h>
#include <vector>
#include <QMessageBox>
#include <myqpushbutton.h>

Chess::Chess(QWidget* parent)
    : QMainWindow(parent),
      ChessBoard(),
      ui(new Ui::Chess),
      screenSize(1000, 800),
      board_b(boardSize, std::vector<MyQPushButton*>(boardSize)),
      player_cb(2),
      pawnTo_b(4) {
  ui->setupUi(this);
  this->setFixedSize(QSize(screenSize.first, screenSize.second));
  this->setWindowIcon(QPixmap(":/img/WKing.png"));
  for (uint i = 0; i < boardSize; i++) {
    for (uint j = 0; j < boardSize; j++) {
      board_b[i][j] = new MyQPushButton(this);
      board_b[i][j]->i = i;
      board_b[i][j]->j = j;
      setCellColor(i, j);
      board_b[i][j]->setGeometry(
          QRect(QPoint(50 + static_cast<int>(j * cellSize),
                       50 + static_cast<int>(i * cellSize)),
                QSize(static_cast<int>(cellSize), static_cast<int>(cellSize))));

      board_b[i][j]->setEnabled(false);
      connect(board_b[i][j], SIGNAL(released()), this, SLOT(cellClick()));
    }
  }
  setPieces();
  player1_l = new QLabel("Player 1", this);
  player2_l = new QLabel("Player 2", this);
  player1_l->setGeometry(
      QRect(QPoint(screenSize.second / 2 - 100 / 2, 0), QSize(100, 30)));
  player2_l->setGeometry(
      QRect(QPoint(screenSize.second / 2 - 100 / 2,
                   static_cast<int>(boardSize * cellSize) + 70),
            QSize(100, 30)));
  player1_l->setStyleSheet("background : yellow;"+QString("font-size: %1px").arg(26));
  player2_l->setStyleSheet(QString("font-size: %1px").arg(26));
  play_b = new QPushButton("Play!", this);
  play_b->setGeometry(QRect(
      QPoint(static_cast<int>(boardSize * cellSize) + 70, 50),
      QSize(screenSize.first - static_cast<int>(boardSize * cellSize) - 90,
            100)));
  play_b->setStyleSheet(QString("font-size: %1px").arg(40));
  player_cb[0] = new QComboBox(this);
  player_cb[0]->addItem("Player 1");
  player_cb[0]->addItem("Computer");
  player_cb[0]->setStyleSheet(QString("font-size: %1px").arg(20));
  player_cb[0]->setGeometry(
      QRect(QPoint(play_b->x(), play_b->y() + play_b->height() + 50),
            QSize(play_b->width() / 2, 30)));
  player_cb[1] = new QComboBox(this);
  player_cb[1]->addItem("Player 2");
  player_cb[1]->addItem("Computer");
  player_cb[1]->setStyleSheet(QString("font-size: %1px").arg(20));
  player_cb[1]->setGeometry(QRect(QPoint(play_b->x() + play_b->width() / 2,
                                         play_b->y() + play_b->height() + 50),
                                  QSize(play_b->width() / 2, 30)));

  exit_b = new QPushButton("Exit", this);
  exit_b->setGeometry(
      QRect(QPoint(player_cb[0]->x(),
                   player_cb[0]->y() + player_cb[0]->height() + 50),
            QSize(play_b->size())));
  exit_b->setStyleSheet(QString("font-size: %1px").arg(40));
  connect(play_b, SIGNAL(released()), this, SLOT(play()));
  connect(exit_b, SIGNAL(released()), this, SLOT(exit()));
  for (uint i = 0; i < 2; i++) {
    connect(
        player_cb[i], &QComboBox::currentTextChanged,
        [i, this](const QString& text) { this->updatePlayer1_cb(text, i); });
  }
  computerMode = 0;
  newComputerMode = 0;
  chessai = nullptr;
  QString pawnTo_S = "";
}

Chess::~Chess() {
  delete ui;
  for (uint i = 0; i < boardSize; i++) {
    for (uint j = 0; j < boardSize; j++) {
      delete board_b[i][j];
    }
  }
  delete player1_l;
  delete player2_l;
  delete play_b;
  delete player_cb[0];
  delete player_cb[1];
  delete exit_b;
}

void Chess::exit() { this->destroy(); }

void Chess::play() {
  if (play_b->text() == "Play!") {
    play_b->setText("New game");
  }
  if (play_b->text() == "New game") {
    clearBoard();
    setPieces();
    Pmode = 1;
    if(Pmode == -1){
      player1_l->setStyleSheet(QString("font-size: %1px").arg(26));
      player2_l->setStyleSheet("background : yellow;"+QString("font-size: %1px").arg(26));
    }
    else{
        player2_l->setStyleSheet(QString("font-size: %1px").arg(26));
        player1_l->setStyleSheet("background : yellow;"+QString("font-size: %1px").arg(26));
    }
    if (firstCell.first != boardSize) {
      setCellColor(firstCell.first, firstCell.second);
    }
    Initiation();
  }
  EnabledCellButtons();
  computerMode = newComputerMode;
  if (computerMode == Pmode and Pmode == 1) {
    computerMove();
  }
}

void Chess::EnabledCellButtons(const bool en) {
  for (uint i = 0; i < boardSize; i++)
    for (uint j = 0; j < boardSize; j++) {
      board_b[i][j]->setEnabled(en);
    }
}

void Chess::updatePlayer1_cb(const QString& text, uint i) {
  if (text == "Computer") {
    if (i == 1) {
      player_cb[0]->setEnabled(false);
      newComputerMode = -1;
    } else {
      player_cb[1]->setEnabled(false);
      newComputerMode = 1;
    }
  } else {
    if (i == 1) {
      player_cb[0]->setEnabled(true);
      newComputerMode = 0;
    } else {
      player_cb[1]->setEnabled(true);
      newComputerMode = 0;
    }
  }
}

void Chess::clearBoard() {
  for (uint i = 0; i < boardSize; i++)
    for (uint j = 0; j < boardSize; j++)
      if (board[i][j] != "  ") {
        board[i][j] = "  ";
        board_b[i][j]->setIcon(QPixmap());
      }
}

void Chess::setPieces() {
  board_b[0][0]->setIcon(QPixmap(":/img/WRook.png"));
  board_b[0][1]->setIcon(QPixmap(":/img/WKnight.png"));
  board_b[0][2]->setIcon(QPixmap(":/img/WBishop.png"));
  board_b[0][3]->setIcon(QPixmap(":/img/WQueen.png"));
  board_b[0][4]->setIcon(QPixmap(":/img/WKing.png"));
  board_b[0][5]->setIcon(QPixmap(":/img/WBishop.png"));
  board_b[0][6]->setIcon(QPixmap(":/img/WKnight.png"));
  board_b[0][7]->setIcon(QPixmap(":/img/WRook.png"));
  board[0][0] = "WR";
  board[0][1] = "WN";
  board[0][2] = "WB";
  board[0][3] = "WQ";
  board[0][4] = "WK";
  board[0][5] = "WB";
  board[0][6] = "WN";
  board[0][7] = "WR";

  board_b[7][0]->setIcon(QPixmap(":/img/BRook.png"));
  board_b[7][1]->setIcon(QPixmap(":/img/BKnight.png"));
  board_b[7][2]->setIcon(QPixmap(":/img/BBishop.png"));
  board_b[7][3]->setIcon(QPixmap(":/img/BKing.png"));
  board_b[7][4]->setIcon(QPixmap(":/img/BQueen.png"));
  board_b[7][5]->setIcon(QPixmap(":/img/BBishop.png"));
  board_b[7][6]->setIcon(QPixmap(":/img/BKnight.png"));
  board_b[7][7]->setIcon(QPixmap(":/img/BRook.png"));
  board[7][0] = "BR";
  board[7][1] = "BN";
  board[7][2] = "BB";
  board[7][3] = "BQ";
  board[7][4] = "BK";
  board[7][5] = "BB";
  board[7][6] = "BN";
  board[7][7] = "BR";
  for (uint i = 0; i < boardSize; i++) {
    board_b[1][i]->setIcon(QPixmap(":/img/WPawn.png"));
    board[1][i] = "WP";
  }
  for (uint i = 0; i < boardSize; i++) {
    board_b[6][i]->setIcon(QPixmap(":/img/BPawn.png"));
    board[6][i] = "BP";
  }
  for (uint i = 0; i < boardSize; i++) {
    if (i < 2 or i > 5)
      for (uint j = 0; j < boardSize; j++) {
        board_b[i][j]->setIconSize(
            QSize(static_cast<int>(cellSize), static_cast<int>(cellSize)));
      }
  }
}

void Chess::setCellColor(uint i, uint j) {
  if ((i + j) % 2 == 0)
    board_b[i][j]->setStyleSheet("background: #663300");
  else {
    board_b[i][j]->setStyleSheet("background: #CCCCCC");
  }
}

void Chess::computerMove() {
  QString NotMoverColor = (Pmode == 1) ? "B" : "W";
  QString moverColor = (Pmode == 1) ? "W" : "B";
  chessai = &ChessAI::getMove(new ChessBoardAI(*this));
  Move(chessai->getCell1(), chessai->getCell2());
  if (board[chessai->getCell1().first][chessai->getCell1().second][1] == 'P' and
      chessai->getCell2().first == 3.5 + Pmode * 3.5 and
      CheckMovePawn(chessai->getCell1(), chessai->getCell2(),
                    NotMoverColor[0])) {
    board[chessai->getCell2().first][chessai->getCell2().second] =
        NotMoverColor + "Q";
    board_b[secondCell.first][secondCell.second]->setIcon(
        QPixmap(":/img/" + moverColor + "Queen.png"));
  }
  Pmode = -Pmode;
  if(Pmode == -1){
    player1_l->setStyleSheet(QString("font-size: %1px").arg(26));
    player2_l->setStyleSheet("background : yellow;"+QString("font-size: %1px").arg(26));
  }
  else{
      player2_l->setStyleSheet(QString("font-size: %1px").arg(26));
      player1_l->setStyleSheet("background : yellow;"+QString("font-size: %1px").arg(26));
  }
}

void Chess::cellClick() {
  MyQPushButton* thisButton = dynamic_cast<MyQPushButton*>(sender());
  std::pair<int, int> thisCel(thisButton->i, thisButton->j);
  QChar MoverColor = (Pmode == 1) ? 'W' : 'B';
  QChar NotMoverColor = (Pmode == 1) ? 'B' : 'W';
  bool flagCastling = false;
  if (firstCell.first != boardSize and firstCell.first == thisButton->i and
      firstCell.second == thisButton->j) {
    setCellColor(firstCell.first, firstCell.second);
    firstCell.first = boardSize;
    return;
  }
  if (board[thisCel.first][thisCel.second][0] == MoverColor) {
    if (firstCell.first != boardSize) {
      if (board[firstCell.first][firstCell.second][1] == 'K' and
          board[thisCel.first][thisCel.second][1] == 'R' and
          board[thisCel.first][thisCel.second][0] == MoverColor)
        flagCastling = true;
      else
        setCellColor(firstCell.first, firstCell.second);
    }
    if (!flagCastling) {
      firstCell.first = thisButton->i;
      firstCell.second = thisButton->j;
      thisButton->setStyleSheet("background : yellow");
      return;
    }
  }
  if (board[thisCel.first][thisCel.second][0] == ' ' and
      firstCell.first == boardSize) {
    sendMessage("This cell haven`t piece");
    return;
  }
  if (board[thisCel.first][thisCel.second][0] != MoverColor and
      firstCell.first == boardSize) {
    sendMessage("This piece of your enemy");
    return;
  }
  secondCell.first = thisButton->i;
  secondCell.second = thisButton->j;
  if (board[firstCell.first][firstCell.second][1] == 'P' and
      secondCell.first == 3.5 + Pmode * 3.5 and
      CheckMovePawn(firstCell, secondCell, NotMoverColor)) {
    PawnToPiece();
    return;
  }

  if (Move(firstCell, secondCell)) {
    setCellColor(firstCell.first, firstCell.second);
    firstCell.first = boardSize;
    secondCell.first = boardSize;
    Pmode = -Pmode;
    if (not ExistenceMove(MoverColor)) {
      if (Shah())
        sendMessage("Checkmate" + QString(((Pmode == 1) ? "white" : "black")) +
                    " player");
      else
        sendMessage("Stalemate " + QString(((Pmode == 1) ? "white" : "black")) +
                    " player");
      EnabledCellButtons(false);
    }
  } else {
    return;
  }
  if(Pmode == -1){
    player1_l->setStyleSheet(QString("font-size: %1px").arg(26));
    player2_l->setStyleSheet("background : yellow;"+QString("font-size: %1px").arg(26));
  }
  else{
      player2_l->setStyleSheet(QString("font-size: %1px").arg(26));
      player1_l->setStyleSheet("background : yellow;"+QString("font-size: %1px").arg(26));
  }
  if (computerMode) { 
    computerMove();
  }
}

void Chess::PawnToPiece() {
  EnabledCellButtons(false);
  for (uint i = 0; i < pawnTo_b.size(); i++) {
    pawnTo_b[i] = new MyQPushButton(this);
    pawnTo_b[i]->i = i;
    pawnTo_b[i]->setGeometry(QRect(QPoint(exit_b->x() + i * exit_b->width() / 4,
                                          exit_b->y() + exit_b->height()),
                                   QSize(exit_b->width() / 4, cellSize)));
    pawnTo_b[i]->show();
    connect(pawnTo_b[i], SIGNAL(released()), this, SLOT(pawnTo()));
  }
  pawnTo_b[0]->setText("Quenn");
  pawnTo_b[1]->setText("Rook");
  pawnTo_b[2]->setText("Bishop");
  pawnTo_b[3]->setText("Knight");
}

void Chess::pawnTo() {
  QString moverColor = (Pmode == 1) ? "W" : "B";
  QPixmap pix;
  MyQPushButton* button = dynamic_cast<MyQPushButton*>(sender());
  if (button->i == 0) {
    pawnTo_S = moverColor + "Q";
    pix.load(":/img/" + moverColor + "Queen.png");
  }
  if (button->i == 1) {
    pawnTo_S = moverColor + "R";
    pix.load(":/img/" + moverColor + "Rook.png");
  }
  if (button->i == 2) {
    pawnTo_S = moverColor + "B";
    pix.load(":/img/" + moverColor + "Bishop.png");
  }
  if (button->i == 3) {
    pawnTo_S = moverColor + "N";
    pix.load(":/img/" + moverColor + "Knight.png");
  }
  EnabledCellButtons();
  Move(firstCell, secondCell);
  board[secondCell.first][secondCell.second] = pawnTo_S;
  board_b[secondCell.first][secondCell.second]->setIcon(pix);
  Pmode = -Pmode;
  for (MyQPushButton* i : pawnTo_b) delete i;
  if (computerMode) {
      player1_l->setStyleSheet(QString("font-size: %1px").arg(26));
      player2_l->setStyleSheet("background : yellow;"+QString("font-size: %1px").arg(26));
    computerMove();
  }
}

bool Chess::Move(const std::pair<int, int>& cell1,
                 const std::pair<int, int>& cell2, bool message) {
  int Castling;
  QString FigureCell2 = "  ";
  QIcon iconCell2;
  int move = CheckMove(cell1, cell2, message);
  if (move == 0) return false;
  if (move == 1 or move == 2) {
    FigureCell2 = board[cell2.first][cell2.second];
    iconCell2 = board_b[cell2.first][cell2.second]->icon();
    board[cell2.first][cell2.second] = board[cell1.first][cell1.second];
    board_b[cell2.first][cell2.second]->setIcon(
        board_b[cell1.first][cell1.second]->icon());
    board_b[cell2.first][cell2.second]->setIconSize(QSize(cellSize, cellSize));
    board_b[cell1.first][cell1.second]->setIcon(QPixmap());
    board[cell1.first][cell1.second] = "  ";
    if (move == 2) {
      board[cell2.first - Pmode][cell2.second] = "  ";
      board_b[cell2.first - Pmode][cell2.second]->setIcon(QPixmap());
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
    board_b[cell1.first][cell1.second]->setIcon(
        board_b[cell2.first][cell2.second]->icon());
    if (move) {
      board[cell2.first][cell2.second] = FigureCell2;
      board_b[cell2.first][cell2.second]->setIcon(iconCell2);
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
      board_b[cell2.first - Pmode][cell2.second]->setIcon(
          QPixmap(":/img/" + QString(((Pmode == 1) ? "W" : "B")) + "Pawn.png"));
      board_b[cell2.first - Pmode][cell2.second]->setIconSize(
          QSize(cellSize, cellSize));
    }
    if (shah and (move == 1 or move == 2)) return false;
  }
  if (move == 3) Castling = 1;
  if (move == 4) Castling = -1;
  if (move > 2) {
    board[cell1.first][cell1.second - 1 * Castling] =
        board[cell2.first][cell2.second];
    board_b[cell1.first][cell1.second - 1 * Castling]->setIcon(
        board_b[cell2.first][cell2.second]->icon());
    board[cell1.first][cell1.second - 2 * Castling] =
        board[cell1.first][cell1.second];
    board_b[cell1.first][cell1.second - 2 * Castling]->setIcon(
        board_b[cell1.first][cell1.second]->icon());
    board[cell1.first][cell1.second] = "  ";
    board_b[cell1.first][cell1.second]->setIcon(QPixmap());
    board[cell2.first][cell2.second] = "  ";
    board_b[cell2.first][cell2.second]->setIcon(QPixmap());
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
