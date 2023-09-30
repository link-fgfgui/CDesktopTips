#include "qsti.h"
#include "timer.h"
#include "ui_widget.h"
#include "widget.h"
#include <QApplication>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QProcessEnvironment>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QTime>
#include <QWidget>
#include <qtsingleapplication.h>

QString path;
QString thing;
int tspp;
bool bfonc = false;
QList<QStringList> qsl;
Widget *w = nullptr;
timer *t = nullptr;
QtSingleApplication *a = nullptr;
QStringList outqsl1 = {
    {"<html><body><p align=\"center\"><span style=\"font-size:30pt\">上节课:</"
     "span><span style=\"font-size:40pt;font-weight:600;color:#ff0000\">",
     "</span></p><p align=\"center\"><span style=\"font-size:30pt\">下节课:</"
     "span><span style=\"font-size:40pt;font-weight:600;color:#0000ff\">",
     "</span></"
     "p><p align=\"center\"><span style=\"font-size:22pt;color:#000000\">",
     "</span></p></body></html>"}};
QStringList outqsl2 = {
    {"<html><body><p align=\"center\"><span style=\"font-size:30pt;color:#"
     "0000ff\">这节课:</"
     "span><span style=\"font-size:40pt;font-weight:600;color:#ff0000\">",
     "</span></p><br/"
     "><p align=\"center\"><span style=\"font-size:22pt;color:#000000\">",
     "</span></p></body></html>"}};

void Widget::on_pushButton_clicked() {
  bfonc = true;
  qDebug() << "clicked";
}

QSTI::QSTI(QString pngPath) {

  quitAction.setText("Exit");
  quitAction.setIcon(QIcon::fromTheme("application-exit"));
  connect(&quitAction, SIGNAL(triggered()), this, SLOT(close()));
  trayIconMenu.addAction(&quitAction);
  this->setContextMenu(&trayIconMenu);
  this->setIcon(QIcon(pngPath));
  this->show();
}
void QSTI::close() {
  t->stop();
  w->close();
  a->exit();
}

timer::timer() {
  for (int i=0; i < modellen; ++i) {
    model[i] = i + 1;
  }
  eachSecond();
  //  dttspp = QDateTime::fromTime_t(tspp);
  this->setTimerType(Qt::VeryCoarseTimer);
  connect(this, &QTimer::timeout, this, eachSecond);
  this->start(1000);
}
// void timer::print() {
//   //    if (){}

//  //    if (this->now)
//  //

//  w->ui->label->setText(s);
//}
void timer::eachSecond() {
  dt = QDateTime::currentDateTime();
  t = dt.time();
  d = dt.date();
  bool onoroff;
  int week = d.dayOfWeek();
  QStringList k = qsl[week];
  QStringList ks = qsl[0];
  QList<QTime> ksl;
  if (bfonc && t.hour() >= 12)
    t = t.addSecs(10 * 60);
  for (int i = 0; i < ks.length(); ++i) {
    QString ss = ks[i];
    if (ss.length() < 5) {
      ss = "0" + ss;
    }
    QTime x = QTime::fromString(ss, "HH:mm");
    ;
    ksl.append(x);
    int j = t.secsTo(x);
    if (j > 0) {
      if (bfonc && t.hour() >= 12)
        t = t.addSecs(-(10 * 60));
      QString s = "";
      QString nowClass;
      QString nowDateTime =
          d.toString("yy/MM/dd") + QString(" ") + t.toString("HH:mm:ss");
      if (k[i] ==
          "下课") { // 下个时间点是下课，上个时间点必须为上课，认为在上课
        onoroff = true;
        if (i == 0)
          nowClass = QString("下课");
        else
          nowClass = k[i - 1];
        //<html><body><p align=\"center\"><span
        //style=\"font-size:30pt\">上节课:</span><span
        //style=\"font-size:40pt;font-weight:600;color:#ff0000\">LL</span></p><p
        //align=\"center\"><span style=\"font-size:30pt\">下节课:</span><span
        //style=\"font-size:40pt;font-weight:600;color:#0000ff\">NL</span></p><p
        //align=\"center\"><span
        //style=\"font-size:22pt;color:#000000\">%y/%m/%d%H:%M:%S</span></p></body></html>
        s = outqsl2[0] + nowClass + outqsl2[1] + nowDateTime;
      } else if (k[i - 1] != "下课") { // 2.其他情况（定制课程表规避）
        onoroff = true;
        s = outqsl2[0] + k[i - 1] + outqsl2[1] + nowDateTime;
      } else { // 下个时间点不是下课 分1.现在是下课
        onoroff = false;
        if (i < 2)
          nowClass = QString("下课");
        else
          nowClass = k[i - 2];
        s = outqsl1[0] + nowClass + outqsl1[1] + k[i] + outqsl1[2] +
            nowDateTime;
      }
      if (ci >= maxci) {
        ci = 0;
        nextmode();
      }
      if (mode == 1) {
        s = s + QString("<br>离上(下)课还有<br/>") + QString::number(j / 60) +
            QString("分") + QString::number(j % 60) + QString("秒") +
            outqsl2[2];
      } else if (mode == 2) {
        int j2 = tspp - dt.toTime_t();
        s = s + QString("<br>离") + thing + QString("还有<br/>") +
            QString::number(j2 / 86400) + QString("天") +
            QString::number(j % 86400 / 3600) + QString("时") +
            QString::number(j % 86400 % 3600 / 60) + QString("分") +
            QString::number(j % 86400 % 3600 % 60) + QString("秒") + outqsl2[2];
      }
      ++ci;
      w->ui->label->setText(s);
      break;
    }
    //      qDebug()<<ks[i]<<" "<<j<<" "<<x<<" "<<t<<Qt::endl;
  }
  //  std::exit(0);

  //  qDebug()<<ks<<Qt::endl;
}

void loadSettings(QSettings *p) {
  QDir d;
  if (!d.exists(path + "/CDesktopTips")) {
    d.mkdir(path + "/CDesktopTips");
    QFile f(path + "/CDesktopTips/config.ini");
    p->beginGroup("CONFIG");
    //    p->setValue("Debug",false)
    //    p->setValue("Enable",false)
    p->setValue("Thing", "thing");
    p->setValue("ts", 170000000);
    p->endGroup();
  };
  p->beginGroup("CONFIG");
  thing = p->value("Thing").toString();
  tspp = p->value("ts").toInt();
  p->endGroup();
}

void loadCsv(QList<QStringList> &data) {
  QString filePath = "kb.csv";
  QFile file(filePath);
  file.open(QIODevice::ReadOnly | QIODevice::Text);

  QTextStream stream(&file);
  stream.setCodec("GBK");
  while (!stream.atEnd()) {

    QString line = stream.readLine();
    QStringList row = line.split(',', Qt::SkipEmptyParts);
    data.append(row);
  }

  file.close();
}

int main(int argc, char *argv[]) {
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setHighDpiScaleFactorRoundingPolicy(
      Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
  a = new QtSingleApplication("DesktopTips", argc, argv);

  if (a->isRunning())
    return EXIT_SUCCESS;
  path = QProcessEnvironment::systemEnvironment().value("AppData");
  path.replace(QRegExp("\\\\"), "/");

  QSettings *setting =
      new QSettings(path + "/CDesktopTips/config.ini", QSettings::IniFormat);
  setting->setIniCodec(QTextCodec::codecForName("UTF-8"));

  loadSettings(setting);

  loadCsv(qsl);

  //    qDebug()<<qsl<<Qt::endl;
  QDesktopWidget *pdw = QApplication::desktop();
  //        self.move(d.width() - 420, 10)
  w = new Widget();
  w->move(pdw->width() * 0.73, 10);
  QSTI qsti(":/icon/icon.ico");
  t = new timer();
  w->show();
  //    qDebug()<<qsl[7]<<Qt::endl;
  return a->exec();
}
