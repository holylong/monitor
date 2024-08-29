#include "tipsdialog.h"
#include "./ui_tipsdialog.h"
#include <QThread>
#include <QDebug>

TipsDialog::TipsDialog(QWidget *parent)
    : QDialog(parent)
  , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    qDebug() << __FUNCTION__ << " enter";
    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    setFixedSize(400, 300);
    qDebug() << __FUNCTION__ << " exit";
}

bool TipsDialog::event(QEvent *event)
{
    switch (event->type()) {
        case QEvent::Show:
            qDebug() << "dialog is show";
            QTimer::singleShot(2000, this, SLOT(hide()));
            break;
        case QEvent::Hide:
            qDebug() << "dialog is hide";
            break;
    }

    return QDialog::event(event);
}

void TipsDialog::setText(QString& text)
{

    QString title = "提示";
//    ui->labelTitle->setScaledContents(true);
    ui->labelTitle->adjustSize();
    ui->labelTitle->setWordWrap(true);
    ui->labelTitle->setAlignment(Qt::AlignTop);
    ui->labelTitle->setText(title);

    QFont fontTitle;
    fontTitle.setWeight(4);
    fontTitle.setPointSize(20);
    ui->labelTitle->setFont(fontTitle);

    ui->label->setScaledContents(true);
    ui->label->setText(text);
    QFont fontCtx;
    fontCtx.setWeight(4);
    fontCtx.setPointSize(15);
    ui->label->setFont(fontCtx);

}
