#include "filewidget.h"


fileWidget::fileWidget(QWidget *parent)
    : QWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);



        m_menu = new QMenu(this);
        m_createFile = new QAction(this);
        m_createCatalog = new QAction(this);
        m_actionThree = new QAction(this);

        m_createFile->setText("新建文件");
        m_createCatalog->setText("新建文件夹");
        m_actionThree->setText("ActionThree");


        m_menu->addAction(m_createFile);
        m_menu->addAction(m_createCatalog);
        m_menu->addAction(m_actionThree);


        connect(m_createFile, SIGNAL(triggered()), this, SLOT(createFile()));
        connect(m_createCatalog, SIGNAL(triggered()), this, SLOT(createCatalog()));
        connect(m_actionThree, SIGNAL(triggered()), this, SLOT(actionThreeSlot()));

        inistcatalogLineEdit();
        inistBtnList();
        inistBtn();

        currentCatalog->setReadOnly(true);

        connect(backBtn,&QPushButton::clicked,this,&fileWidget::back);
        connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMenu(const QPoint&)));
}

void fileWidget::inistBtnList(){
    qDebug()<<catalog0.file_level(catalog0.String2vector("gen"));

    CSTree pNode=current;
    while (pNode != nullptr)
            {
                cout<<pNode->name<<"  ";
                pNode=pNode->nextsibling;
            }
}
void fileWidget::inistcatalogLineEdit(){
    current=Root_directory;
    currentCatalog=new QLineEdit(this);
    currentCatalog->setText(QString::fromStdString(current->name));
    currentCatalog->setFixedSize(450,20);
    currentCatalog->move(50,30);
    currentCatalog->show();
}
void fileWidget::fileIsOpned(file f){
    qDebug()<<"slot:open"<<QString::fromStdString( f.fileName);
    emit fileOnUsing(f);
}
void fileWidget::inistBtn(){
    backBtn=new QPushButton(this);
    backBtn->setFixedSize(20,20);
    backBtn->move(30,30);
    backBtn->show();
    backBtn->setIcon(QIcon(":/up.png"));
    backBtn->setIconSize(QSize(20,20));

}
void fileWidget::showMenu(const QPoint &point)
{
    m_menu->exec(mapToGlobal(point));
}

void fileWidget::createFile()
{
    qDebug() << "createFile";
    bool ok;
    QString text = QInputDialog::getText(this, tr("创建文件"),tr("文件名字"), QLineEdit::Normal,0, &ok);
    QString fileSize = QInputDialog::getText(this, tr("创建文件"),tr("文件大小"), QLineEdit::Normal,0, &ok);

    if(ok&&!text.isEmpty()){
        filebtn *temp=new filebtn(this);
        file fdata;
        //防止重名
        for(int i=0;i<btnList.size();i++){
            if(text==btnList[i]->getName()){
                btnList[i]->againTimes++;
                text=text+"("+QString::number(btnList[i]->againTimes)+")";
            }
        }
        fdata.fileName=text.toStdString();
        fdata.fileSize=fileSize.toUInt();
        fdata.blocknumber=2;
        temp->inistFile(text,currentCatalog->text(),true,fdata);
        temp->move(50+btnList.size()%6*80,60+(btnList.size()/6)*90);
        btnList.push_back(temp);
        connect(temp,&filebtn::willbeDelet,this,&fileWidget::deletFile);
        connect(temp,&filebtn::openSignal,this,&fileWidget::openFile);
        connect(temp,&filebtn::fileOnUsing,this,&fileWidget::fileIsOpned);
        qDebug()<<QString::fromStdString(fdata.fileName);
        catalog0.creatFile(fdata,catalog0.String2vector(currentCatalog->text().toStdString()));
        temp->show();
        emit creatFileToDisk(fdata.fileSize);
    }

}

void fileWidget::createCatalog()
{
    qDebug() << "createCatalog";
    bool ok;
    QString text = QInputDialog::getText(this, tr("创建文件夹"),tr("文件夹名字"), QLineEdit::Normal,0, &ok);
    if(ok&&!text.isEmpty()){
        filebtn *temp=new filebtn(this);
        file fdata;
        //防止重名
        for(int i=0;i<btnList.size();i++){
            if(text==btnList[i]->getName()){
                btnList[i]->againTimes++;
                text=text+"("+QString::number(btnList[i]->againTimes)+")";
            }
        }
        temp->inistFile(text,currentCatalog->text(),false,fdata);
        temp->move(50+btnList.size()%6*80,60+(btnList.size()/6)*90);
        btnList.push_back(temp);
        connect(temp,&filebtn::willbeDelet,this,&fileWidget::deletFile);
        connect(temp,&filebtn::openSignal,this,&fileWidget::openFile);
        catalog0.createCatalogue(catalog0.String2vector(currentCatalog->text().toStdString()),text.toStdString());

        temp->show();


    }



}

void fileWidget::actionThreeSlot()
{
    qDebug() << "actionThree";
    qDebug()<<"current:"<<QString::fromStdString( current->name);
    CSTree temp=current->firstchild;
    while(temp!=nullptr){
        qDebug()<<QString::fromStdString( temp->name);
        temp=temp->nextsibling;
    }
}



void fileWidget::mousePressEvent(QMouseEvent *e)
{
    // 保存鼠标按下时的位置（相对该窗体左上角的偏移量）
    if(e->button()==Qt::LeftButton){
        _offset = e->pos();
        //qDebug()<<"left click";
        TitleBarPress=true;
    }
    this->raise();
}

void fileWidget::mouseMoveEvent(QMouseEvent *e)
{
    // 计算鼠标移动时，窗体应移动到的目标位置
    if(TitleBarPress){
        auto pos = mapToParent(e->pos());
        move(pos - _offset);
        //qDebug()<<"left move";
    }

}
void fileWidget::mouseReleaseEvent(QMouseEvent *event){
    TitleBarPress=false;
}
void fileWidget::switchCatalog(QString s,CSTree temp){

    qDebug()<<"switch to"<<s;
    //catalog0.delete_Catalogue(catalog0.String2vector(s));

    for(int i=0;i<btnList.size();i++){
        btnList[i]->close();
    }
    btnList.clear();
    temp=current->firstchild;
    while(temp!=nullptr){
        filebtn *btn=new filebtn(this);
        file fdata;
        fdata.fileName=temp->name;
        fdata.fileSize=temp->FCB1.fileSize;
        fdata.filecontent=temp->FCB1.filecontent;
        fdata.blocknumber=temp->FCB1.blocks_number;
        fdata.fileType=temp->FCB1.type;

        btn->inistFile(QString::fromStdString(fdata.fileName),QString::fromStdString(fdata.filecontent),!temp->ifcatalog,fdata);

        btn->move(50+btnList.size()%6*80,60+(btnList.size()/6)*90);
        btnList.push_back(btn);
        connect(btn,&filebtn::willbeDelet,this,&fileWidget::deletFile);
        connect(btn,&filebtn::openSignal,this,&fileWidget::openFile);
        connect(btn,&filebtn::fileOnUsing,this,&fileWidget::fileIsOpned);
        btn->show();

        temp=temp->nextsibling;
    }
}
void fileWidget::back(){

    if(currentCatalog->text()=="gen"){
        return;
    }
    QString s=currentCatalog->text();
    int last=0;
    for(int i=0;i<s.length();i++){
        if(s[i]=='/'){
            last=i;
        }
    }
    s.remove(last,s.length()-last);
    currentCatalog->setText(s);
    CSTree temp=current->parent;
    current=temp;
    switchCatalog(s,temp);

}
void fileWidget::openFile(QString addr){
    QString s=currentCatalog->text()+"/"+ addr;
    currentCatalog->setText(s);
    qDebug()<<"switch to"<<s;
    //catalog0.delete_Catalogue(catalog0.String2vector(s));
    CSTree temp=current->firstchild;
    while(temp!=nullptr){
        if(temp->name==addr.toStdString()){
            current=temp;
            break;
        }
        temp=temp->nextsibling;
    }
    switchCatalog(s,temp);
}

void fileWidget::deletFile(QString addr,int size){
    string s=currentCatalog->text().toStdString()+"/"+ addr.toStdString();
    qDebug()<<QString::fromStdString(s);
    //catalog0.delete_Catalogue(catalog0.String2vector(s));
    catalog0.deleteFile(catalog0.String2vector(s));

    emit deletFileToDisk(size);

    for(int i=0;i<btnList.size();i++){
        if(addr==btnList[i]->getName()){
            btnList[i]->close();
            for(int j=btnList.size()-1;j>i;j--){
                btnList[j]->move(btnList[j-1]->pos());
            }
            btnList.removeAt(i);
            break;
        }
    }

}

