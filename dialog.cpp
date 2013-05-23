#include "dialog.h"
#include "ui_dialog.h"

#include <QtGui>
#include <QSettings>
#include "time.h"
#include "wbutton.h"
#include "tdialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //
    _wsheet = NULL;
    setWindowFlags( Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);

    //
    _playing = false;
    _paused = true;
    _playno = 0;

    //
    QSettings set("WordTester.cfg", QSettings::IniFormat);
    set.beginGroup("Dialog");
    int spaceTime = set.value("SpaceTime", 2).toInt();
    set.endGroup();

    //
    QHBoxLayout* playLayout = new QHBoxLayout;
    playLayout->setMargin(4);
    _playBtn = new QPushButton(this);
    _playBtn->setFlat(true);
    _playBtn->setIconSize(QSize(32,32));
    _playBtn->setToolTip(tr("Read all words above"));
    setPlaybtnState( false);
    _stopBtn = new QPushButton(this);
    _stopBtn->setFlat(true);
    _stopBtn->setIcon(QIcon(":/images/playoff.ico"));
    _stopBtn->setIconSize(QSize(32,32));
    _stopBtn->setToolTip(tr("Stop"));
    _stopBtn->setEnabled(false);
    _progLabel = new QLabel(this);
    _progLabel->setFixedWidth(30);
    _spaceSpin = new QSpinBox(this);
    _spaceSpin->setFocusPolicy(Qt::ClickFocus);
    _spaceSpin->setMinimum(1);
    _spaceSpin->setMaximum(10);
    _spaceSpin->setValue(spaceTime);
    connect(_playBtn, SIGNAL(clicked()), this, SLOT(handlePlaybtnClicked()));
    connect(_stopBtn, SIGNAL(clicked()), this, SLOT(handleStopbtnClicked()));
    playLayout->addWidget(_progLabel);
    playLayout->addStretch();
    playLayout->addWidget( _playBtn);
    playLayout->addWidget( _stopBtn);
    playLayout->addStretch();
    playLayout->addWidget(new QLabel(tr("Spacing:"), this));
    playLayout->addWidget(_spaceSpin);
    QGroupBox* playBar = new QGroupBox(this);
    playBar->setLayout( playLayout);
    playBar->setVisible( false);

    _gridLayout = new QGridLayout;

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    QPushButton* showPlayBar = new QPushButton(this);
    showPlayBar->setText(tr("Read All"));
    showPlayBar->setCheckable(true);
    showPlayBar->setFocusPolicy(Qt::ClickFocus);
    connect( showPlayBar, SIGNAL(clicked(bool)),
             playBar, SLOT(setVisible(bool)));
    buttonsLayout->addWidget( showPlayBar);
    QLabel* seperator = new QLabel(this);
    seperator->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    buttonsLayout->addWidget( seperator, 1);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout( _gridLayout);
    mainLayout->addLayout( buttonsLayout);
    mainLayout->addWidget( playBar);
    setLayout( mainLayout);
}

Dialog::~Dialog()
{
    delete ui;
}

int random(int minValue, int maxValue)
{
    Q_ASSERT(minValue <= maxValue);

    if( minValue != maxValue) {
        return rand()% (maxValue-minValue)+ minValue;
    }
    else {
        return minValue;
    }
}

void Dialog::setSheetAndTable(WSheet* wsheet, WTable* table, int order)
{
    if( wsheet->size()<= 0)
        return;

    if( wsheet->size()> 100) {
        QMessageBox::warning(this, tr("Test"),
                             tr("The list is too long, we only"
                                " select 100 of them to test."));
    }

    // seed of rand()
    srand( time(NULL));

    // produce shuffle order
    const int WordNums = wsheet->size();
    _series.clear();

    for(int i=0; i< WordNums; ++i)
        _series.push_back(i);

    if( order == Word::OrderShuffle) {
        for(int i=0; i< WordNums; ++i) {
            swap(_series[i], _series[random(i, WordNums)]);  // a good method to set random
        }
    }

    // dynamically add toolbuttons
    const int MaxRows = 10;
    const int MaxColums = 10;
    int Colums = (WordNums<MaxColums)? WordNums: MaxColums;
    int Rows = ceil((float)WordNums/Colums);
    Rows = min( Rows, MaxRows);

    bool readerOk = wsheet->initReader();
    for(int i=0; i<Rows; ++i) {
        for(int j=0; j<Colums; ++j) {
            int num = i*MaxColums+ j;
            if( num+1 > WordNums)
                break;
            //.create buttons
            int id = _series.at(num);
            WButton* button = new WButton( id);
            button->setFont( QFont(tr("Arial"), 20));
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            button->setText( QString::number( num+1));
            if( readerOk && num%4 == 0)
                button->setToolTip(tr("Click <img src=\":/images/speaker.png\" /> or press 'S' to speak"));
            if( readerOk)
                button->showSpeaker();
            _gridLayout->addWidget(button, i, j);
            connect( button, SIGNAL(clicked()),
                     this, SLOT(handleButtonClicked()));
            connect( button, SIGNAL(speakerClicked()),
                     this, SLOT(handleSpeakerClicked()));
            connect( button, SIGNAL(stateChange(bool)),
                     this, SLOT(handleStateChange(bool)));
            //.disable remembered
            if( wsheet->isRememberedAt(id))
                button->setIconStatus( true);
        }
    }

    // save points
    _wsheet = wsheet;
    _table = table;
    _playBtn->setEnabled( readerOk);
    connect( wsheet, SIGNAL(voiceCompleted()), this, SLOT(handleVoiceCompleted()));
}

void Dialog::closeEvent( QCloseEvent* /*event*/ )
{
    int yes = 0;
    int no = 0;
    int total = _wsheet->size();
    float rate;
    QString str1;

    for(int i=0; i<total; i++) {
        if( _wsheet->isRememberedAt(i))
            yes++;
        if( _wsheet->isForgottenAt(i))
            no++;
    }

    if( yes>0 || no>0) {
        rate = (total==0)? 0:(float)yes / total * 100;
        str1 = tr("Remembered: %1 <br>"
                  "Forgotton: %2 <br>"
                  "Total Number: %3 <br>"
                  "-----------------------------<br>"
                  ).arg(yes).arg(no).arg(total);
        str1 += (rate>=60? "<font color=green>": "<font color=red>")
                  + tr("Completeness: %1 %</font>").arg( rate, 0, 'f', 1);
        QMessageBox::information(this, tr("Result"), str1);
    }

    QSettings set("WordTester.cfg", QSettings::IniFormat);
    set.beginGroup("Dialog");
    set.setValue("SpaceTime", _spaceSpin->value());
    set.endGroup();

}

void Dialog::handleButtonClicked()
{
    WButton* button = (WButton*)sender();
    int id = button->getId();
    QString num = button->text();

    //.show test dialog
    TDialog tdlg( _wsheet, id, num, this);
    int result = tdlg.exec();

    //.update result
    if( result == QMessageBox::Yes)
        setWButtonState(button, true);
    else if( result == QMessageBox::No)
        setWButtonState(button, false);
}

void Dialog::handleStateChange(bool remembered)
{
    WButton* button = (WButton*)sender();
    setWButtonState(button, remembered);
}

void Dialog::handleSpeakerClicked()
{
    WButton* button = (WButton*)sender();
    int id = button->getId();
    _wsheet->readTextAt(id);
}

void Dialog::handlePlaybtnClicked()
{
    _playing = true;
    _stopBtn->setEnabled(true);
    _spaceSpin->setEnabled(false);

    if( _paused) {
        setPlaybtnState(true);
        readWordAtPlayno();
    }
    else {
        setPlaybtnState(false);
    }
}

void Dialog::handleVoiceCompleted()
{
    if( _playing && !_paused ) {
        // make some delay
        int s = _spaceSpin->value();
        QTimer::singleShot(s*1000, this, SLOT(readWordAtPlayno()));
    }
}

void Dialog::handleStopbtnClicked()
{
    _playno = 0;
    _playing = false;
    _progLabel->clear();
    _stopBtn->setEnabled(false);
    _spaceSpin->setEnabled(true);
    setPlaybtnState(false);
}

void Dialog::setPlaybtnState(bool on)
{
    if( _playBtn) {
        _paused = !on;
        _playBtn->setIcon(on? QIcon(":/images/pause.ico")
                            : QIcon(":/images/playon.ico"));
        _playBtn->setText(on? tr("Pause "): tr("Speak "));
    }
}

void Dialog::readWordAtPlayno()
{
    if( _paused)
        return;
    if( _playno >= _series.size()) {
        handleStopbtnClicked();
        return;
    }

    qDebug()<< "next playno="<< _playno+1;
    _progLabel->setText(tr("%1/%2")
                        .arg(_playno+1, 2)
                        .arg(_wsheet->size()));
    _wsheet->readTextAt(_series.at(_playno++));
}

void Dialog::setWButtonState(WButton* button, bool rememb)
{
    int id = button->getId();
    Word::WordStatus oldstat = _wsheet->statusAt(id);
    Word::WordStatus newstat = rememb
            ? Word::Remembered: Word::Forgotten;

    _wsheet->setStatus(newstat, id);
    button->setIconStatus( rememb);
    if( oldstat != newstat)
        _table->handleStatusChanged(id);
}
