#include "wbutton.h"

#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>

#include "tdialog.h"

#define X1      1  //offset from (0,0)

WButton::WButton(int id)
{
    _id = id;
    _showSpeaker = false;
    _speaker.load(":/images/speaker.png");
}

void WButton::setIconStatus(bool yes)
{
    QIcon icon;
    if( yes)
        icon.addFile( ":/images/Yes.ico", QSize(), QIcon::Disabled);
    else
        icon.addFile( ":/images/No.ico", QSize(), QIcon::Disabled);
    setIcon( icon);
    setIconSize( QSize(32,32));
    setToolButtonStyle(Qt::ToolButtonIconOnly);

    setEnabled( false);
}

void WButton::showSpeaker()
{
    _showSpeaker = true;
    this->update();
}

void WButton::hideSpeaker()
{
    _showSpeaker = false;
    this->update();
}

void WButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    if( _showSpeaker && !_speaker.isNull()) {
        QPainter painter(this);
        painter.drawImage(X1,X1,_speaker);
    }
}

void WButton::mousePressEvent(QMouseEvent *ev)
{
    if( _showSpeaker && !_speaker.isNull()) {
        QRect cover = _speaker.rect().adjusted(0,0,X1,X1);
        if( cover.contains( ev->pos())) {
            emit speakerClicked();
            return;
        }
    }
    QToolButton::mousePressEvent(ev);
}

void WButton::keyPressEvent(QKeyEvent *ev)
{
    if( ev->key() == Qt::Key_S) {
        emit speakerClicked();
        return;
    }
// Moved into TDialog:
//    else if( ev->key() == Qt::Key_R) {
//        emit stateChange(true);
//        return;
//    }
//    else if( ev->key() == Qt::Key_F) {
//        emit stateChange(false);
//        return;
//    }
    QToolButton::keyPressEvent(ev);
}
