#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>

#include "WSheet.h"
#include "wtable.h"

class WButton;

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setSheetAndTable(WSheet* _words, WTable* table, int _order=Word::OrderShuffle);

protected:
    void closeEvent( QCloseEvent* event );

private slots:
    void handleButtonClicked();
    void handleStateChange(bool);
    void handleSpeakerClicked();
    void handlePlaybtnClicked();
    void handleVoiceCompleted();
    void handleStopbtnClicked();
    void readWordAtPlayno();

private:
    void setPlaybtnState(bool);
    void setWButtonState(WButton*, bool);

private:
    Ui::Dialog *ui;
    WSheet* _wsheet;
    WTable* _table;
    QGridLayout* _gridLayout;
    QLabel* _progLabel;

    QVector<int> _series;
    QPushButton* _playBtn;
    QPushButton* _stopBtn;
    QSpinBox* _spaceSpin;
    bool _playing;
    bool _paused;
    int  _playno;
};

#endif // DIALOG_H
