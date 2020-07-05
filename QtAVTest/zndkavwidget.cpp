#include "ZNDKAVWidget.h"
#include <QDebug>

ZNDKAVWidget::ZNDKAVWidget(QWidget *parent, Qt::WindowFlags f)
    : WidgetRenderer(parent, f)
{
    setBackgroundColor(QColor(200, 200, 200));

    m_bGridReady = false;

    m_color = "White";

    m_bShowGrid = true;
    m_dCamHeight = 6;
    m_rotateX = 30;
    m_rotateY = 0;
    m_rotateZ = 0;

    m_gridRatio = 2;
    m_dCamFocus = 1; //没有用到
    m_verticalAngle = 90;
    m_nearPlane = 0.1;
    m_farPlane = 5000;
    m_aspectRatio = 16.0/9;
    m_dStep = 0.5;

    if(prepareGrid())
    {
        m_bGridReady = true;
        repaint();
    }
}

ZNDKAVWidget::~ZNDKAVWidget()
{

}

void ZNDKAVWidget::paintEvent(QPaintEvent* event)
{
    WidgetRenderer::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    if(m_bShowGrid)
        paintGridBox(&painter);
}

// /////////////////////Grid////////////////////
void ZNDKAVWidget::getData(QJsonObject &json)
{
    json.insert("SHOWGRID", m_bShowGrid);
    json.insert("H", m_dCamHeight);
    json.insert("X", m_rotateX);
    json.insert("Y", m_rotateY);
    json.insert("Z", m_rotateZ);
    json.insert("STEP", m_dStep);
}

void ZNDKAVWidget::setData(QJsonObject json)
{
    m_bShowGrid = json.value("SHOWGRID").toBool();
    m_dCamHeight = json.value("H").toDouble();
    m_rotateX = json.value("X").toDouble();
    m_rotateY = json.value("Y").toDouble();
    m_rotateZ = json.value("Z").toDouble();
    m_dStep = json.value("STEP").toDouble();

    if(m_bShowGrid)
    {
        prepareGrid();
        update();
    }
}

bool ZNDKAVWidget::prepareGrid()
{
    if(m_dCamHeight <= 0 || m_rotateX <= 0)
        return false;

    m_matrix = QMatrix4x4();
    //单位米
    m_maxDis = m_dCamHeight/tan(m_rotateX*M_PI/180);
    float maxZVal = m_maxDis/cos(m_rotateX*M_PI/180);

    //变换矩阵
   // m_matrix.translate(this->width()/2, this->height(), 0);
    m_matrix.rotate(-m_rotateX, 1, 0, 0);
    m_matrix.rotate(m_rotateY, 0, 1, 0);
    m_matrix.rotate(m_rotateZ, 0, 0, 1);
    m_matrix.translate(0, 0, -m_dCamHeight);
    m_matrix.perspective(m_verticalAngle,m_aspectRatio,m_nearPlane,m_farPlane);


    QVector3D pFar(0, m_maxDis, maxZVal);
    m_ptMiddle = m_matrix.map(pFar).toPointF();
    float near2FarDis = maxZVal * sin(m_verticalAngle*M_PI/180/2) / sin((m_rotateX+m_verticalAngle/2)*M_PI/180);//地面上中心点与最近点米数
    m_minDis = m_maxDis - near2FarDis;

    QVector3D pNear(0, m_maxDis-near2FarDis, maxZVal-near2FarDis*cos(m_rotateX*M_PI/180));
    QPointF pN = m_matrix.map(pNear).toPointF();
    m_scaleFactor = this->height()/2 / fabs(m_ptMiddle.y()-pN.y());//米->像素


    QVector3D pNormal(m_maxDis, m_maxDis, maxZVal);
    pNormal = m_matrix.map(pNormal);

    pFar = m_matrix.map(pFar);
    pNear = m_matrix.map(pNear);

    //平面方程
    float x1 = pNormal.x();
    float y1 = pNormal.y();
    float z1 = pNormal.z();
    float x2 = pFar.x();
    float y2 = pFar.y();
    float z2 = pFar.z();
    float x3 = pNear.x();
    float y3 = pNear.y();
    float z3 = pNear.z();


    A = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
    B = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
    C = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
    D = -x1*(y2*z3-y3*z2) - x2*(y3*z1-y1*z3) - x3*(y1*z2-y2*z1);

    return true;
}

QPointF ZNDKAVWidget::ptScreen2Scene(QPoint screenPt)
{
    if(screenPt.y() < this->height()/2)
        return QPoint(-1, -1);

    float theltaX = this->width()/2-m_ptMiddle.x();    //与屏幕中心对齐
    float theltaY = this->height()/2-m_ptMiddle.y();
    screenPt.setX(screenPt.x() - theltaX);
    screenPt.setY(screenPt.y() - theltaY);

    QPointF newScreenPt;
    newScreenPt.setX(m_ptMiddle.x()+(screenPt.x()-m_ptMiddle.x())/m_scaleFactor);
    newScreenPt.setY(m_ptMiddle.y()+(screenPt.y()-m_ptMiddle.y())/m_scaleFactor);

    float zVal = -(D + A*newScreenPt.x() + B*newScreenPt.y()) / C;
    QVector3D scenePt3D(newScreenPt.x(), newScreenPt.y(), zVal);
    bool invertable = true;
    if(m_matrix.determinant() == 0)
    {
        invertable = false;
        return QPoint(-1, -1);
    }
    QMatrix4x4 invertedMatrix = m_matrix.inverted(&invertable);
    QVector3D originScenePt3D = invertedMatrix.map(scenePt3D);
    return QPointF(originScenePt3D.x(), originScenePt3D.y());
}

void ZNDKAVWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(!m_bGridReady)
        return;

    QPoint  mousePt = event->pos();
    QPoint globalPt = event->globalPos();

    if(mousePt.y() < this->height()/2)
        return;

    QPointF scenePt = ptScreen2Scene(mousePt);
    if(scenePt.x() == -1)
        return;
    QString ptTip = QString("(%1,%2)").arg(-scenePt.x()).arg(scenePt.y());
    QToolTip::showText(globalPt, ptTip, this, this->rect());

    update();
}

void ZNDKAVWidget::resizeEvent(QResizeEvent* e)
{
    prepareGrid();
    WidgetRenderer::resizeEvent(e);
}


void ZNDKAVWidget::paintGridBox(QPainter* painter)
{
   // painter->begin(this);
    QPen oldPen = painter->pen();

    QPen newPen;
    if(m_color == "Black")
        newPen.setColor(Qt::black);
    else if(m_color == "White")
        newPen.setColor(Qt::white);
    else
        newPen.setColor(Qt::red);
    painter->setPen(newPen);

    painter->drawLine(QPoint(0, this->height()/2), QPoint(this->width(), this->height()/2));
    painter->drawLine(QPoint(this->width()/2, 0), QPoint(this->width()/2, this->height()));

    //单位米
    float maxZVal = m_maxDis/cos(m_rotateX*M_PI/180);
    float minZVal = maxZVal-m_maxDis*cos(m_rotateX*M_PI/180);
    painter->drawText(QPoint(0, this->height()/2), QString::number(m_maxDis));
    painter->drawText(QPoint(0, this->height()), QString::number(m_minDis));

    painter->translate(this->width()/2, this->height()/2);
    for(float i=m_maxDis; i>=0; i-=m_dStep)
    {
        float zVal = maxZVal-(m_maxDis-i)*cos(m_rotateX*M_PI/180);

        QVector3D pH1(-m_maxDis, i, zVal);
        QPointF ph1 = m_matrix.map(pH1).toPointF();
        QVector3D pH2(m_maxDis, i, zVal);
        QPointF ph2 = m_matrix.map(pH2).toPointF();

        // H line
        painter->drawLine(QPoint((ph1.x()-m_ptMiddle.x())*m_scaleFactor, (ph1.y()-m_ptMiddle.y())*m_scaleFactor),
                          QPoint((ph2.x()-m_ptMiddle.x())*m_scaleFactor, (ph2.y()-m_ptMiddle.y())*m_scaleFactor));
    }
    for(float i=0; i<=m_maxDis*m_gridRatio; i+=m_dStep)
    {
        //左竖线
        QVector3D pV1(i, m_maxDis, maxZVal);
        QPointF pv1 = m_matrix.map(pV1).toPointF();
        QVector3D pV2(i, 0, minZVal);
        QPointF pv2 = m_matrix.map(pV2).toPointF();

        //右竖线
        QVector3D pV3(-i, m_maxDis, maxZVal);
        QPointF pv3 = m_matrix.map(pV3).toPointF();
        QVector3D pV4(-i, 0, minZVal);
        QPointF pv4 = m_matrix.map(pV4).toPointF();

        // V line
        painter->drawLine(QPoint((pv1.x()-m_ptMiddle.x())*m_scaleFactor, (pv1.y()-m_ptMiddle.y())*m_scaleFactor),
                          QPoint((pv2.x()-m_ptMiddle.x())*m_scaleFactor, (pv2.y()-m_ptMiddle.y())*m_scaleFactor));
        painter->drawLine(QPoint((pv3.x()-m_ptMiddle.x())*m_scaleFactor, (pv3.y()-m_ptMiddle.y())*m_scaleFactor),
                          QPoint((pv4.x()-m_ptMiddle.x())*m_scaleFactor, (pv4.y()-m_ptMiddle.y())*m_scaleFactor));
    }

  //  painter->end();
    painter->setPen(oldPen);
}
