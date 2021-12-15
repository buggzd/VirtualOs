#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
{
    // 设置窗体为无边框样式
    setWindowFlag(Qt::FramelessWindowHint);
}

void MyWidget::mousePressEvent(QMouseEvent *e)
{
    // 保存鼠标按下时的位置（相对该窗体左上角的偏移量）
    if(e->button()==Qt::LeftButton){
        _offset = e->pos();
        //qDebug()<<"left click";
        TitleBarPress=true;
    }
    this->raise();
}

void MyWidget::mouseMoveEvent(QMouseEvent *e)
{
    // 计算鼠标移动时，窗体应移动到的目标位置
    if(TitleBarPress){
        auto pos = mapToParent(e->pos());
        move(pos - _offset);
        //qDebug()<<"left move";
    }

}
void MyWidget::mouseReleaseEvent(QMouseEvent *event){
    TitleBarPress=false;
}
