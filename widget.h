#ifndef WIDGET_H
#define WIDGET_H

#pragma once
#include <QCursor>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

protected:
  QPoint m_position;
  bool m_flag = false;
  void mousePressEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      m_flag = true;
      m_position = event->globalPos() - this->pos();
      event->accept();
      this->setCursor(QCursor(Qt::OpenHandCursor));
    }
  }
  void mouseMoveEvent(QMouseEvent *event) override {
    if (event->buttons() & Qt::LeftButton &&
        m_flag) // 只要鼠标左键被按压就执行（左右同按也执行）
                //        if (event->buttons() == Qt::LeftButton)
                //        //只有鼠标左键被按压才执行
                //        qDebug()<<"moving"<<Qt::endl;
      this->move(event->globalPos() - m_position), event->accept();
  }

  void mouseReleaseEvent(QMouseEvent *event) override {
    m_flag = true;
    this->setCursor(Qt::ArrowCursor);
  };
  void paintEvent(QPaintEvent *event) override {

    QPainter custompainter(this);
    custompainter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(rect(), 8, 8);
    QRegion maskRegion(path.toFillPolygon().toPolygon());
    setMask(maskRegion);

    custompainter.fillRect(rect(), Qt::white);
  }

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

public:
  Ui::Widget *ui;
public slots:
    void on_pushButton_clicked();
};
#endif // WIDGET_H
