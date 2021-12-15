#include <QWidget>
#include <QMouseEvent>
#include <qdebug.h>
class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr);

protected:
    // 重写父类的鼠标事件
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    QPoint _offset;
    bool TitleBarPress;

};
