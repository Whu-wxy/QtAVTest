#ifndef ZNDKSETTINGDLG_H
#define ZNDKSETTINGDLG_H

#include <QObject>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>
#include <QJsonObject>
#include <QDebug>

class ZNDKSettingDlg : public QDialog
{
    Q_OBJECT
public:
    ZNDKSettingDlg();

    void    updateData(QJsonObject json);
    QJsonObject    returnData();

private:
    QLabel*     m_labURL;
    QLabel*     m_labShowGrid;
    QLabel*     m_labHeight;
    QLabel*     m_labRotateX;
    QLabel*     m_labRotateY;
    QLabel*     m_labRotateZ;
    QLabel*     m_labStep;
    QLineEdit*  m_editURL;
    QPushButton*  m_btnShowGrid;
    QLineEdit*  m_editHeight;
    QLineEdit*  m_editRotateX;
    QLineEdit*  m_editRotateY;
    QLineEdit*  m_editRotateZ;
    QLineEdit*  m_editStep;

    QPushButton* m_btnCancel;
    QPushButton* m_btnOK;


    QString     m_strURL;

    bool        m_bShowGrid;
    float       m_fHeight;
    float       m_fRotateX;
    float       m_fRotateY;
    float       m_fRotateZ;
    float       m_fStep;

protected slots:
    void    onCancel();
    void    onOK();
    void    changeShowGridBtn(bool);

};

#endif // ZNDKSETTINGDLG_H
