#pragma once
#ifndef TIMER_H
#define TIMER_H
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QList>
#include <QObject>
#include <QTime>
#include <QTimer>

class timer : public QTimer {
  Q_OBJECT
public:
  timer();
  void nextmode();
  bool sub;
  int now;
  QTime t;
  QDate d;
  QDateTime dt;
  //  QDateTime dttspp;
  int mode = 1;
  static const int modellen = 2;
  int model[modellen];
  int ci = 0;
  static const int maxci = 5;
  void print();
public slots:
  void eachSecond();
  //    self.t
  //    self.eachSecond
};

#endif // TIMER_H
