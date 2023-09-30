
#include "widget.h"
#include "ui_widget.h"
#include <QGraphicsDropShadowEffect>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  this->setWindowIcon(QIcon(":/icon/icon.ico"));
  //  ui->label->setTextFormat(Qt::MarkdownText);
  this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
  //    this->setAttribute(Qt::WA_TranslucentBackground);
  //    ui->label->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
  //    ui->label->setAttribute(Qt::WA_TranslucentBackground);
  this->setWindowOpacity(0.9);
  //    QGraphicsDropShadowEffect *windowShadow;        //阴影效果
  //    windowShadow = new QGraphicsDropShadowEffect(this);
  //    windowShadow->setBlurRadius(30);
  //    windowShadow->setColor(QColor(0, 0, 0));
  //    windowShadow->setOffset(0, 0);
  //    this->setGraphicsEffect(windowShadow);
}

Widget::~Widget() { delete ui; }
