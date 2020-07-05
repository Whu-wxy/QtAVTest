#ifndef ZNDKAVWIDGET_H
#define ZNDKAVWIDGET_H

#include <QDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QPen>
#include <QMatrix4x4>
#include <QToolTip>
#include <QJsonObject>
#include <QtGlobal>

#include <QtAV/VideoFrame.h>
#include "WidgetRenderer.h"


#include <QtAVWidgets>
using namespace QtAV;

class ZNDKAVWidget : public WidgetRenderer
{
    Q_OBJECT

public:
    ZNDKAVWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~ZNDKAVWidget();

    bool prepareGrid();

    void    getData(QJsonObject &json);
    void    setData(QJsonObject json);
    void    setRotate(double x, double y) {m_rotateX=90-qBound(1.0, x, 89.0);
                                    m_rotateY=qBound(-179.0, y, 179.0);
                                    prepareGrid();
                                    update();}

protected:
    virtual void paintEvent(QPaintEvent*);
    void paintGridBox(QPainter*);
    virtual void mouseMoveEvent(QMouseEvent* event);

    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

private:
    void setCameraParam(double lat= 31.111111,double lon= 127.111111,
                        double angle=0, double distance=1);
    void setGridParam(double camHeight=5, double rotateX=30, double verticalAngle=90, double step=1 ,QString color="Black");

    QPointF ptScreen2Scene(QPoint);

    // grid
    bool    m_bGridReady;
    QMatrix4x4 m_matrix;
    double  m_scaleFactor;        //米->像素
    QPointF m_ptMiddle;           //屏幕中心点，视线中心线上的点
    QString m_color;              //网格颜色

    float   m_maxDis;             //米
    float   m_minDis;

    bool   m_bShowGrid;
    double m_dCamHeight;
    float  m_rotateX;
    float  m_rotateY;
    float  m_rotateZ;
    float  m_gridRatio;       //水平方向相对垂直方向的倍率（若已知垂直50米，需要显示水平方向100米，设置2）
    double m_dCamFocus;       //焦距
    float  m_verticalAngle;   //视角
    float  m_nearPlane;       //近平面
    float  m_farPlane;        //远平面
    float  m_aspectRatio;     //宽高比
    double m_dStep;           //对应现实的多少米

    //投影平面方程
    float   A;
    float   B;
    float   C;
    float   D;
};

#endif // ZNDKAVWidget_H
