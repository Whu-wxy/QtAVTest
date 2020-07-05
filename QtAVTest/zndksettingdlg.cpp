#include "zndksettingdlg.h"

ZNDKSettingDlg::ZNDKSettingDlg()
{
    setWindowState(Qt::WindowMaximized);

    QGridLayout*    mainLayout = new QGridLayout(this);

    m_labURL = new QLabel("URL:");
    m_labShowGrid = new QLabel("是否显示网格:");
    m_labHeight = new QLabel("高度:");
    m_labRotateX = new QLabel("X:");
    m_labRotateY = new QLabel("Y:");
    m_labRotateZ = new QLabel("Z:");
    m_labStep = new QLabel("单位长度：");
    m_labURL->setAlignment(Qt::AlignCenter);
    m_labShowGrid->setAlignment(Qt::AlignCenter);
    m_labHeight->setAlignment(Qt::AlignCenter);
    m_labRotateX->setAlignment(Qt::AlignCenter);
    m_labRotateY->setAlignment(Qt::AlignCenter);
    m_labRotateZ->setAlignment(Qt::AlignCenter);
    m_labStep->setAlignment(Qt::AlignCenter);

    m_editURL = new QLineEdit("rtmp://58.200.131.2:1935/livetv/gxtv", this);
    m_btnShowGrid = new QPushButton("ON", this);
    m_editHeight = new QLineEdit("5", this);
    m_editRotateX = new QLineEdit("30", this);
    m_editRotateY = new QLineEdit("0", this);
    m_editRotateZ = new QLineEdit("0", this);
    m_editStep = new QLineEdit("0.5", this);

    QDoubleValidator* validator1 = new QDoubleValidator(0, 9999, 2, this);
    QDoubleValidator* validator2 = new QDoubleValidator(0.0001, 89.999, 2, this);
    QDoubleValidator* validator3 = new QDoubleValidator(-179, 179, 2, this);

    m_editHeight->setValidator(validator1);
    m_editRotateX->setValidator(validator2);
    m_editRotateY->setValidator(validator3);
    m_editRotateZ->setValidator(validator3);
    m_editStep->setValidator(validator1);

    m_btnCancel = new QPushButton("取消", this);
    m_btnOK = new QPushButton("确认", this);

    mainLayout->addWidget(m_labURL, 0, 0, 1, 1);
    mainLayout->addWidget(m_labShowGrid, 1, 0, 1, 1);
    mainLayout->addWidget(m_labHeight, 2, 0, 1, 1);
    mainLayout->addWidget(m_labRotateX, 3, 0, 1, 1);
    mainLayout->addWidget(m_labRotateY, 4, 0, 1, 1);
    mainLayout->addWidget(m_labRotateZ, 5, 0, 1, 1);
    mainLayout->addWidget(m_labStep, 6, 0, 1, 1);
    mainLayout->addWidget(m_editURL, 0, 1, 1, 1);
    mainLayout->addWidget(m_btnShowGrid, 1, 1, 1, 1);
    mainLayout->addWidget(m_editHeight, 2, 1, 1, 1);
    mainLayout->addWidget(m_editRotateX, 3, 1, 1, 1);
    mainLayout->addWidget(m_editRotateY, 4, 1, 1, 1);
    mainLayout->addWidget(m_editRotateZ, 5, 1, 1, 1);
    mainLayout->addWidget(m_editStep, 6, 1, 1, 1);
    mainLayout->addWidget(m_btnCancel, 8, 0, 1, 1);
    mainLayout->addWidget(m_btnOK, 8, 1, 1, 1);

    connect(m_btnCancel, SIGNAL(clicked(bool)), this, SLOT(onCancel()));
    connect(m_btnOK, SIGNAL(clicked(bool)), this, SLOT(onOK()));
    connect(m_btnShowGrid, SIGNAL(clicked(bool)), this, SLOT(changeShowGridBtn(bool)));
}

void ZNDKSettingDlg::updateData(QJsonObject json)
{
    m_editURL->setText(json.value("URL").toString());
    m_editHeight->setText(QString::number(json.value("H").toDouble()));
    m_editRotateX->setText(QString::number(json.value("X").toDouble()));
    m_editRotateY->setText(QString::number(json.value("Y").toDouble()));
    m_editRotateZ->setText(QString::number(json.value("Z").toDouble()));
    m_editStep->setText(QString::number(json.value("STEP").toDouble()));
    bool bShowGrid = json.value("SHOWGRID").toBool();
    if(bShowGrid)
        m_btnShowGrid->setText("ON");
    else
        m_btnShowGrid->setText("OFF");
}

void ZNDKSettingDlg::changeShowGridBtn(bool)
{
    if(m_btnShowGrid->text() == "ON")
        m_btnShowGrid->setText("OFF");
    else
        m_btnShowGrid->setText("ON");
}

void ZNDKSettingDlg::onCancel()
{
    reject();
}

void ZNDKSettingDlg::onOK()
{
    accept();
}

QJsonObject ZNDKSettingDlg::returnData()
{
    QJsonObject json;
    json.insert("URL", m_editURL->text());
    json.insert("H", m_editHeight->text().toDouble());
    json.insert("X", m_editRotateX->text().toDouble());
    json.insert("Y", m_editRotateY->text().toDouble());
    json.insert("Z", m_editRotateZ->text().toDouble());
    json.insert("STEP", m_editStep->text().toDouble());
    bool bShowGrid = false;
    if(m_btnShowGrid->text() == "ON")
        bShowGrid = true;
    else
        bShowGrid = false;
    json.insert("SHOWGRID", bShowGrid);

    return json;
}
