#ifndef FILEBTN_H
#define FILEBTN_H

#include <QAbstractItemModel>
#include <QPushButton>
#include <qaction.h>
#include <qmenu.h>
#include <qdebug.h>
#include "catalogue.h"
#include <qstring.h>
#include <QLineEdit>
#include <QLabel>

class filebtn : public QPushButton
{
    Q_OBJECT

public:
    explicit filebtn(QWidget *parent = nullptr);
    void inistFile(QString fname,QString fContent,bool isF,file fdata);
    QString getName();
    int againTimes;//重命名次数
signals:
    void willbeDelet(QString str,int size);
    void openSignal(QString str);
    void fileOnUsing(file f);
protected slots:

    void showMenu(const QPoint &point);
    void openSlot();
    void actionTwoSlot();
    void actionThreeSlot();


    void deletFileSlot();
private:
    void mouseDoubleClickEvent ( QMouseEvent * )override;
    QMenu *m_menu;
    QAction *m_open;
    QAction *m_actionTwo;
    QAction *m_actionThree;
    QAction *m_deletFile;
    //是否为文件
    QString fileName;
    bool isFile;
    file fileData;
    QString fileContent;
    QLabel *icon;
    QLabel *text;

};

#endif // FILEBTN_H
