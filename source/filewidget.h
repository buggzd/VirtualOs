#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <qwidget.h>
#include <qmenu.h>
#include <qdebug.h>
#include <QMouseEvent>
#include <deque>
#include "filebtn.h"
#include <QLineEdit>
#include<QInputDialog>
using namespace std;
class fileWidget: public QWidget
{
    Q_OBJECT
public:
    fileWidget(QWidget *parent = nullptr);
protected slots:
    void showMenu(const QPoint &point);
    void createFile();
    void createCatalog();
    void actionThreeSlot();

    void openFile(QString addr);
    void deletFile(QString addr,int size);
    void back();
    void switchCatalog(QString s,CSTree temp);

    void fileIsOpned(file f);
signals:
    void fileOnUsing(file f);

    void creatFileToDisk(int length);
    void deletFileToDisk(int length);
protected:
    // 重写父类的鼠标事件
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    void inistcatalogLineEdit();
    void inistBtnList();
    void updataBtnShow();
    void inistBtn();

    QMenu *m_menu;
    QAction *m_createFile;
    QAction *m_createCatalog;
    QAction *m_actionThree;

    QList<filebtn*>btnList;

    CSTree current;

    QLineEdit *currentCatalog;
    QPushButton *backBtn;
    QPushButton *enterBtn;

    QPoint _offset;
    bool TitleBarPress;
};

#endif // FILEWIDGET_H
