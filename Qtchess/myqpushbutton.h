#ifndef MYQPUSHBUTTON_H
#define MYQPUSHBUTTON_H
#include <QPushButton>

class MyQPushButton : public QPushButton {
 public:
  MyQPushButton(QWidget* parrent = nullptr);
  uint i, j;
  ~MyQPushButton();
};

#endif  // MYQPUSHBUTTON_H
