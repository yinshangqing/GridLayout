#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QMenu>

namespace Ui {
class PrintWidget;
}

struct MouseAction
{
    QPoint point;
    int nCode;// 1.press 2.repress -1.fail

};
Q_DECLARE_METATYPE(MouseAction)
struct PrWdg
{
    int nRow;
    int nCol;
    int nRowSpan; // 最小值的倍数
    int nColSpan;
    bool bLive;
};
Q_DECLARE_METATYPE(PrWdg)
class PrintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrintWidget(QWidget *parent = 0);
    ~PrintWidget();

    MouseAction GetMouseAction();
    void SetPrWdg(PrWdg prWdg);
    PrWdg GetPrWdg();

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void SetSize(int nX,int nY);
    const QSize gnsize = QSize(80,40);
    bool nowIsCanSplite();
//    QPoint GetGl

signals:
    void signal_start_draw_update();
    void signal_mouse_action(PrWdg);
    void signal_splite_screen();

public slots:
    void Slot_Show_Contex_Menu(const QPoint&);
    void Slot_Splite_Screen();

private:
    Ui::PrintWidget *ui;
    QPoint m_pStart;
    QPoint m_pEnd;
    bool   m_bIsPreChoose;
    bool   m_bIsRepreChoose;
    PrWdg  m_prWdg;
    QSize  m_size;
    int    m_nSpliteCount;
    QSize  m_nowSize;
};

#endif // PRINTWIDGET_H
