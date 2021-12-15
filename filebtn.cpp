#include "filebtn.h"

filebtn::filebtn(QWidget *parent)
    : QPushButton(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);

        //this->setIcon(QIcon(QPixmap(":/FolderClosed.png")));
        //this->setIconSize(QSize(70,50));

        this->setStyleSheet(
                            "QPushButton{border-image: url(:/FolderClosedquat.png);}"
                            "QPushButton:hover{border-image: url(:/Folderopenquat.png);}"
                            "QPushButton:pressed{border-image: url(:/Folderopenquat.png);}"
                           );

        m_menu = new QMenu(this);
        m_open = new QAction(this);
        m_actionTwo = new QAction(this);
        m_actionThree = new QAction(this);
        m_deletFile=new QAction(this);

        m_open->setText("打开");
        m_actionTwo->setText("ActionTwo");
        m_actionThree->setText("ActionThree");
        m_deletFile->setText("删除文件");

        m_menu->addAction(m_open);
        m_menu->addAction(m_actionTwo);
        m_menu->addAction(m_actionThree);
        m_menu->addAction(m_deletFile);

        connect(m_open, SIGNAL(triggered()), this, SLOT(openSlot()));
        connect(m_actionTwo, SIGNAL(triggered()), this, SLOT(actionTwoSlot()));
        connect(m_actionThree, SIGNAL(triggered()), this, SLOT(actionThreeSlot()));
        connect(m_deletFile,SIGNAL(triggered()), this, SLOT(deletFileSlot()));

        connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMenu(const QPoint&)));



        text=new QLabel(this);
        text->move(0,60);
        text->setFixedSize(70,20);
        text->setAlignment(Qt::AlignHCenter);



        this->setFixedSize(70,80);
        againTimes=0;
}
void filebtn::mouseDoubleClickEvent(QMouseEvent *){
    if(this->isFile){
        emit fileOnUsing(this->fileData);
    }else{
        emit openSignal(fileName);
    }

}
QString filebtn::getName(){
    return this->fileName;

}
void filebtn::showMenu(const QPoint &point)
{
    m_menu->exec(mapToGlobal(point));
}

void filebtn::openSlot()
{
     qDebug()<<"Open "<<fileName;
     emit openSignal(fileName);
}

void filebtn::actionTwoSlot()
{
    qDebug() << "actionTwo";
}

void filebtn::actionThreeSlot()
{
    qDebug() << "actionThree";
}

void filebtn::deletFileSlot(){
    qDebug() <<fileName<< "willbedelet";
    emit willbeDelet(fileName,fileData.fileSize);
};

void filebtn::inistFile(QString fname,QString fContent,bool isF,file fdata){
    this->fileName=fname;
    this->fileContent=fContent;
    isFile=isF;
    if(isF){
        this->setStyleSheet(
                            "QPushButton{border-image: url(:/DocumentClose.png);}"
                            "QPushButton:hover{border-image: url(:/DocumentOpen.png);}"
                            "QPushButton:pressed{border-image: url(:/DocumentOpen.png);}"
                           );
    }
    fileData=fdata;
    this->text->setText(fname);
}
