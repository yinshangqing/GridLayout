#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_nWidth = this->width();
    m_nHeight = this->height();
    m_printWidget = new PrintWidget;
    // 最小值为最小的单位
    m_nMinHeight = m_printWidget->height();
    m_nMinWidget = m_printWidget->width();
    DrawCoordinate();
    GridlayoutCoor();
    this->setMouseTracking(true);
    connect(this,SIGNAL(signal_start_merge()),this,SLOT(Slot_Start_Merge()));
    m_pStart == QPoint(0,0);
    m_pEnd == QPoint(0,0);
//    setWindowOpacity(0.7);
}

Widget::~Widget()
{
    delete ui;
}
// 绘制坐标系
void Widget::DrawCoordinate()
{
    m_qlistprintw.clear();
    int nOldRowSpan = 0;
    int nOldColSpan = 0;
    for(int i=0;i<4;i++)
    {
        PrintWidget *printW = new PrintWidget;
        connect(printW,SIGNAL(signal_start_draw_update()),SLOT(Slot_Get_Start_Draw()));
        connect(printW,SIGNAL(signal_mouse_action(PrWdg)),SLOT(Slot_Start_Draw(PrWdg)));
        connect(printW,SIGNAL(signal_splite_screen()),this,SLOT(Slot_Splite_Screen()));
        printW->installEventFilter(this);

        printW->SetSize(8,8);
        PrWdg prg;
        prg.nRow = i / 2 * 8;
        prg.nCol = i % 2 * 8;
        nOldRowSpan = nOldColSpan = prg.nRowSpan = prg.nColSpan = 8;

        prg.bLive = true;
        printW->SetPrWdg(prg);
        m_qlistprintw.push_back(printW);

    }

}

void Widget::GridlayoutCoor()
{
    QGridLayout *glayout = new QGridLayout;
    QLayout *oldLay = this->layout();
    delete oldLay;
    foreach (PrintWidget *printW, m_qlistprintw)
    {
        PrWdg prg = printW->GetPrWdg();
        if(prg.bLive)
        {
            glayout->addWidget(printW,prg.nRow,prg.nCol,prg.nRowSpan,prg.nColSpan);
        }
    }
    glayout->setMargin(0);
    glayout->setContentsMargins(5,0,0,0);
    glayout->setSpacing(0);

    this->setLayout(glayout);
}

void Widget::DeleteWindows()
{
    for(int i=0;i<m_qlistprintw.size();++i)
    {
        PrWdg prg = m_qlistprintw[i]->GetPrWdg();
        if(prg.nRow >= m_pd.prWdg_start.nRow && prg.nRow <= m_pd.prWdg_end.nRow)
        {
            if(prg.nCol >= m_pd.prWdg_start.nCol && prg.nCol <= m_pd.prWdg_end.nCol)
            {
                // 死亡了
                prg.bLive = false;
                m_qlistprintw[i]->SetPrWdg(prg);
                m_qlistprintw[i]->hide();
            }
        }
    }
}

void Widget::mousePressEvent(QMouseEvent *e)
{
//    QWidget::event(e);
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    //    QWidget::event(e);
}

bool Widget::eventFilter(QObject *target, QEvent *event)
{
    static bool gbPress = false;
    if(theWndIsInList((PrintWidget *)target,m_qlistprintw))
    {
        QMouseEvent *e = (QMouseEvent *)event;
        if(event->type() == QEvent::MouseButtonPress)
        {
            if(e->button() == Qt::LeftButton)
            {
//            emit signal_start_point(QCursor::pos());
                m_pStart = this->mapFromGlobal(QCursor::pos());
                gbPress = true;
            }
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
//            emit signal_end_point(QCursor::pos());
            m_pStart = m_pEnd = QPoint(0,0);
            gbPress = false;
            update();
        }
        else if(event->type() == QEvent::MouseMove)
        {
            if(gbPress)
            {
                m_pEnd = this->mapFromGlobal(QCursor::pos());
                update();
            }
        }
        return false;
    }
    return QWidget::eventFilter(target, event);
}

void Widget::Slot_Get_Start_Draw()
{
    qDebug() << "Slot_Get_Start_Draw()";
}

void Widget::Slot_Start_Draw(PrWdg prwd)
{
    static int a = 1;
    ++ a;
    qDebug() << "Slot_Start_Draw";
    if(a % 2 == 0)
    {
        m_pd.prWdg_start = prwd;
    }
    else
    {
        m_pd.prWdg_end = prwd;
        emit signal_start_merge();
    }

}

void Widget::Slot_Start_Merge()
{
    DeleteWindows();
    GridlayoutCoor();
    qDebug() << "Slot_Start_Merge";
}

// 分屏
void Widget::Slot_Splite_Screen()
{
    PrintWidget *pwt = (PrintWidget *)sender();
    // 得到窗口的TIP坐标点
    QPoint pt_father = ui->widget->mapToGlobal(ui->widget->pos());
    qDebug() << "pt_father.x: " << pt_father.x() << "pt_father.y: " << pt_father.y();
    pt_father = QPoint(pt_father.x()+10,pt_father.y());
    QPoint pt = pwt->mapToGlobal(pwt->pos());
    qDebug() << "pt.x: " << pt.x() << "pt.y: " << pt.y();
    PrWdg pg = pwt->GetPrWdg();
    pg.bLive = false;
    int nWidth = pg.nRowSpan;
    int nHeight = pg.nColSpan;
    if(pt_father.x() == pt.x() && pt_father.y() == pt.y())
    {
        for(int i=0;i<64;++i)
        {
            PrintWidget *printW = new PrintWidget;
            connect(printW,SIGNAL(signal_start_draw_update()),SLOT(Slot_Get_Start_Draw()));
            connect(printW,SIGNAL(signal_mouse_action(PrWdg)),SLOT(Slot_Start_Draw(PrWdg)));
            connect(printW,SIGNAL(signal_splite_screen()),this,SLOT(Slot_Splite_Screen()));
            printW->installEventFilter(this);

            printW->SetSize(1,1);
            PrWdg prg;
            prg.nRow = i / 8;
            prg.nCol = i % 8;
            prg.nRowSpan = prg.nColSpan = 1;

            prg.bLive = true;
            printW->SetPrWdg(prg);
            m_qlistprintw.push_back(printW);
        }
    }
    DeleteWindows();
    GridlayoutCoor();
    delete pwt;

}


void Widget::paintDrawRectangle(QPainter &painter)
{
    if(m_pStart == QPoint(0,0) && m_pEnd == QPoint(0,0))
    {
        return;
    }
    QPen pen(Qt::blue,1,Qt::DashLine,Qt::FlatCap,Qt::RoundJoin);
    QBrush brush(QColor(0, 0, 255), Qt::Dense4Pattern);//创建画刷
    painter.setPen(pen); //使用画笔
    painter.setBrush(brush);//设置画笔形式

    int nWidth = ((m_pEnd.x() - m_pStart.x() > 0) ? (m_pEnd.x() - m_pStart.x()) : (m_pStart.x() - m_pEnd.x()));
    int nHeight = ((m_pEnd.y() - m_pStart.y() > 0) ? (m_pEnd.y() - m_pStart.y()) : (m_pStart.y() - m_pEnd.y()));
    QPoint pd = m_pStart;
    if(m_pEnd.x() - m_pStart.x() < 0 && m_pEnd.y() - m_pStart.y() < 0)
    {
        pd = m_pEnd;
    }
    else if(m_pEnd.x() - m_pStart.x() > 0 && m_pEnd.y() - m_pStart.y() < 0)
    {
        pd = QPoint(m_pEnd.x() - nWidth,m_pEnd.y());
    }
    else if(m_pEnd.x() - m_pStart.x() < 0 && m_pEnd.y() - m_pStart.y() > 0)
    {
        pd = QPoint(m_pEnd.x(),m_pEnd.y() - nHeight);
    }
    painter.drawRect(pd.x(),pd.y(),nWidth,nHeight);//画矩形
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(80,80,80)));
    painter.drawRect(rect());
    paintDrawRectangle(painter);

}

bool Widget::theWndIsInList(PrintWidget *ptet,QList<PrintWidget *> &list)
{
    foreach (PrintWidget *pwt, list)
    {
        if(pwt == ptet)
        {
            return true;
        }
    }
    return false;
}













