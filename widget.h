#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "printwidget.h"
#include <QDebug>
#include <QGridLayout>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QList>
#include <QMessageBox>

struct PrWdg_draw
{
    PrWdg prWdg_start;
    PrWdg prWdg_end;
};
Q_DECLARE_METATYPE(PrWdg_draw)

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void DrawCoordinate();
    void GridlayoutCoor();
    QSize GetInFouse();
    void ChangeDrawWinSize();
    void DeleteWindows();
    void paintDrawRectangle(QPainter &);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);
    void paintEvent(QPaintEvent *);
    bool theWndIsInList(PrintWidget *, QList<PrintWidget *> &);

signals:
    void signal_start_merge();

public slots:
    void Slot_Get_Start_Draw();
    void Slot_Start_Draw(PrWdg);
    void Slot_Start_Merge();
    void Slot_Splite_Screen();

private:
    Ui::Widget *ui;
    int m_nWidth;
    int m_nHeight;
    int m_nAverage;// 平均值 界面与实际屏的比值
    int m_nMinWidget;// 最小的宽
    int m_nMinHeight;// 最小的高
    PrintWidget *m_printWidget; // 打印的屏幕
    QList<PrintWidget *> m_qlistprintw;
    PrWdg_draw m_pd;
    QPoint m_pStart;
    QPoint m_pEnd;

};

#endif // WIDGET_H
