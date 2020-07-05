#ifndef ZNDKANDROIDDLG_H
#define ZNDKANDROIDDLG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QtAV>
#include <QtAV/AVPlayer.h>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>

#include <QRotationReading>
#include <QRotationSensor>
#include <QScreen>
#include <QApplication>
#include <QtGlobal>

#include "zndkavwidget.h"
#include "zndksettingdlg.h"

using namespace QtAV;

class ZNDKAndroidDlg : public QDialog
{
    Q_OBJECT

public:
    ZNDKAndroidDlg(QWidget *parent = 0);
    ~ZNDKAndroidDlg();

    QString m_strURL;

    QLabel* lab;
    QPushButton* m_btnSetting;

    AVPlayer*   player;
    ZNDKAVWidget*   m_pAVWidget;

    QRotationSensor*    m_rSensor;
    QRotationReading* sensorReader;
    QScreen* screen;
    int Orientation;

protected slots:
    void    videoStateChange(QtAV::AVPlayer::State state);
    void    errorChange(QtAV::AVError error);
    void    onSetting(bool);

    void    sensorChange();
    void    orientChanged(Qt::ScreenOrientation);
};

#endif // ZNDKANDROIDDLG_H
