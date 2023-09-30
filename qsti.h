#ifndef QSTI_H
#define QSTI_H

#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QSystemTrayIcon>

class QSTI : public QSystemTrayIcon {
  Q_OBJECT
public:
  QSTI(QString pngPath);
  QMenu trayIconMenu;
  QAction quitAction;
public slots:
  void close();
};

#endif // QSTI_H
