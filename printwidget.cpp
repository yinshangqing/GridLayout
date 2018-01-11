#include "printwidget.h"
#include "ui_printwidget.h"

PrintWidget::PrintWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintWidget)
{
    ui->setupUi(this);
    m_bIsPreChoose = m_bIsRepreChoose = false;
    this->setMouseTracking(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(Slot_Show_Contex_Menu(const QPoint &)));
    connect(this,&PrintWidget::customContextMenuRequested,this,&PrintWidget::Slot_Show_Contex_Menu);
    m_nSpliteCount = 0;
}

PrintWidget::~PrintWidget()
{
    delete ui;
}

MouseAction PrintWidget::GetMouseAction()
{
    MouseAction man;
    if((m_bIsPreChoose == true && m_bIsRepreChoose == true) ||(m_bIsPreChoose == false && m_bIsRepreChoose == false))
    {
        man.nCode = -1;
        return man;
    }
    if(m_bIsPreChoose)
    {
        man.nCode = 1;
        man.point = m_pStart;
    }
    else if(m_bIsRepreChoose)
    {
        man.nCode = 2;
        man.point = m_pEnd;
    }
    return man;
}

void PrintWidget::SetPrWdg(PrWdg prWdg)
{
    m_prWdg = prWdg;
}

PrWdg PrintWidget::GetPrWdg()
{
    return m_prWdg;
}

void PrintWidget::mousePressEvent(QMouseEvent *e)
{

}

void PrintWidget::mouseReleaseEvent(QMouseEvent *e)
{

}

void PrintWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
//    p.setBrush(QBrush(QColor(80,80,80)));
//    p.drawRect(rect());
    p.setPen(QColor(255, 164, 0));
    p.drawLine(QPoint(1,1),QPoint(width(),1));
    p.drawLine(QPoint(1,1),QPoint(1,height()));
    p.drawLine(QPoint(width()-1,1),QPoint(width()-1,height()-1));
    p.drawLine(QPoint(1,height()-1),QPoint(width()-1,height()-1));
}

void PrintWidget::SetSize(int nX,int nY)
{
    m_size = QSize(nX * gnsize.width(),nY * gnsize.height());
    m_nowSize = m_size;
    this->setFixedSize(m_size);
}

bool PrintWidget::nowIsCanSplite()
{
    if(m_nowSize.width() <= gnsize.width() && m_nowSize.height() <= gnsize.height())
    {
        return false;
    }
    return true;
}

void PrintWidget::Slot_Show_Contex_Menu(const QPoint &)
{
    static QMenu *cmenu = new QMenu;
    static QAction *add = cmenu->addAction("分屏");
//    connect(add,SIGNAL(triggered()),this,SLOT(Slot_Splite_Screen()));
    connect(add,&QAction::triggered,this,&PrintWidget::Slot_Splite_Screen);
    cmenu->exec(QCursor::pos());

}

void PrintWidget::Slot_Splite_Screen()
{
    qDebug() << "....................";
    emit signal_splite_screen();
}
