#include "tdialog.h"
#include "ui_tdialog.h"

#include <QMessageBox>
#include <QKeyEvent>

TDialog::TDialog(WSheet* wsh, int id, QString num, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialog)
{
    ui->setupUi(this);

    setWindowTitle( tr("(%1) Do you know:").arg(num));

    // set labels
    ui->wordLabel->setText( wsh->textAt(id));
    ui->answerButton->setEnabled( wsh->hasAnswerAt(id));
    ui->answerButton->setText( wsh->hasAnswerAt(id)?
                               tr("Show the Answer"): tr("No Answer"));
    if( wsh->hasAnswerAt(id)) {
        ui->answerEdit->setText( wsh->answerAt(id));
        ui->answerButton->setFlat( false);
    }

    // filter click for icon
    ui->iconWidget->installEventFilter(this);
    _wsh = wsh;
    _id = id;

    // set page 0
    ui->stackedWidget->setCurrentIndex(0);

    // bug fix 1: for long-text word
    this->adjustSize();
}

TDialog::~TDialog()
{
    delete ui;
}

bool TDialog::eventFilter(QObject *ob, QEvent *ev)
{
    if( ob == ui->iconWidget &&
            ev->type() == QEvent::MouseButtonPress) {
        _wsh->readTextAt(_id);
        return true;
    }
    return false;
}

void TDialog::keyPressEvent(QKeyEvent *ev)
{
    if( ev->key() == Qt::Key_S) {
        _wsh->readTextAt(_id);
        return;
    }
    else if( ev->key() == Qt::Key_R) {
        on_yesButton_clicked();
        return;
    }
    else if( ev->key() == Qt::Key_F) {
        on_noButton_clicked();
        return;
    }
    QDialog::keyPressEvent(ev);
}

void TDialog::on_answerButton_clicked()
{
    // set page 1
    ui->stackedWidget->setCurrentIndex(1);
}

void TDialog::on_yesButton_clicked()
{
    this->done(QMessageBox::Yes);
}

void TDialog::on_noButton_clicked()
{
    this->done(QMessageBox::No);
}
