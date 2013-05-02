#ifndef TDIALOG_H
#define TDIALOG_H

#include <QDialog>

#include "WSheet.h"

namespace Ui {
    class TDialog;
}

class QAbstractButton;

class TDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TDialog(WSheet* wsheet, int id, QString num, QWidget *parent = 0);
    ~TDialog();

protected:
    bool eventFilter(QObject *ob, QEvent *ev);

private:
    Ui::TDialog *ui;
    WSheet* _wsh;
    int _id;

private slots:
    void on_answerButton_clicked();
    void on_yesButton_clicked();
    void on_noButton_clicked();
};

#endif // TDIALOG_H
