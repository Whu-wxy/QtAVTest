#include "zndkandroiddlg.h"

ZNDKAndroidDlg::ZNDKAndroidDlg(QWidget *parent)
    : QDialog(parent)
{
    //rtmp://58.200.131.2:1935/livetv/gxtv
    //rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov

    screen = qApp->primaryScreen();
    screen->setOrientationUpdateMask(
                    Qt::PortraitOrientation
                    | Qt::LandscapeOrientation
                | Qt::InvertedLandscapeOrientation);

    Orientation = 1;
    if(screen->orientation() == Qt::PortraitOrientation)
    {
        Orientation=0;
    }
    else if(screen->orientation() == Qt::LandscapeOrientation || screen->orientation() == Qt::InvertedLandscapeOrientation)
    {
        Orientation=1;
    }

    m_strURL = "http://ivi.bupt.edu.cn/hls/cctv1.m3u8";
    setWindowState(Qt::WindowMaximized);

    m_rSensor = new QRotationSensor(this);
    m_rSensor->setActive(true);
    m_rSensor->start();
    sensorReader = NULL;

    m_pAVWidget = new ZNDKAVWidget(this);
    if(Orientation == 1)
        m_pAVWidget->setOrientation(1);
    else if(Orientation == 0)
        m_pAVWidget->setOrientation(0);

    m_btnSetting = new QPushButton("设置", this);

    lab = new QLabel("None");

    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(lab);
    hlayout->addWidget(m_btnSetting);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_pAVWidget->widget());
    layout->addLayout(hlayout);
    layout->setStretch(0, 10);
    layout->setStretch(1, 1);

    player = new AVPlayer(this);
    player->setRenderer(m_pAVWidget);

    lab->setText("ori: "+screen->orientation());

    QString protocol = player->supportedProtocols().join(',');
    lab->setText(protocol);

    connect(player, SIGNAL(error(QtAV::AVError)), this, SLOT(errorChange(QtAV::AVError)));
    connect(player, SIGNAL(stateChanged(QtAV::AVPlayer::State)), this, SLOT(videoStateChange(QtAV::AVPlayer::State)));
    connect(m_btnSetting, SIGNAL(clicked(bool)), this, SLOT(onSetting(bool)));
    connect(screen, SIGNAL(orientationChanged(Qt::ScreenOrientation)), this, SLOT(orientChanged(Qt::ScreenOrientation)));

    player->play(m_strURL);

    connect(m_rSensor, SIGNAL(readingChanged()), this, SLOT(sensorChange()));
}

ZNDKAndroidDlg::~ZNDKAndroidDlg()
{
    m_rSensor->stop();
}

void ZNDKAndroidDlg::videoStateChange(QtAV::AVPlayer::State state)
{
    lab->setText("state: "+QString::number(int(state)));

}

void ZNDKAndroidDlg::errorChange(QtAV::AVError error)
{
    lab->setText("error: "+error.string());

}

void ZNDKAndroidDlg::onSetting(bool)
{
    lab->setText("setting...");

    QJsonObject json;
    json.insert("URL", m_strURL);
    m_pAVWidget->getData(json);

    ZNDKSettingDlg dlg;
    dlg.updateData(json);
    int res = dlg.exec();
    if(res == 0)
        return;

    QJsonObject returnJson = dlg.returnData();

    m_pAVWidget->setData(returnJson);

    if(returnJson.value("URL").toString() != json.value("URL").toString())
    {
        m_strURL = returnJson.value("URL").toString();
        player->play(m_strURL);
    }
    if(returnJson.value("SHOWGRID").toBool())
        m_rSensor->start();
    else
        m_rSensor->stop();
}


void ZNDKAndroidDlg::sensorChange()
{
    sensorReader = m_rSensor->reading();
    if(sensorReader == NULL)
        return;

    double x = sensorReader->x();

    lab->setText("X="+QString::number(x)+", Y="+QString::number(sensorReader->y()));

    if(Orientation == 0)
        m_pAVWidget->setRotate(x, sensorReader->y());
    else if(Orientation == 1)
        m_pAVWidget->setRotate(qBound(1.0, qAbs(sensorReader->y()), 89.0), x);

    update();
}

void ZNDKAndroidDlg::orientChanged(Qt::ScreenOrientation ori)
{
    if(ori == Qt::PortraitOrientation)
    {
        Orientation=0;
        m_pAVWidget->setOrientation(0);
    }
    else if(ori == Qt::LandscapeOrientation || ori == Qt::InvertedLandscapeOrientation)
    {
        Orientation=1;
        m_pAVWidget->setOrientation(1);
    }
    m_pAVWidget->prepareGrid();

    update();
}
