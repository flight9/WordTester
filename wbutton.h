#ifndef WBUTTON_H
#define WBUTTON_H

#include <QToolButton>
#include <QImage>

class WButton : public QToolButton
{
    Q_OBJECT
public:
    WButton(int id);
    void setIconStatus(bool yes);
    int getId()  { return _id; }

public:
    void showSpeaker();
    void hideSpeaker();

signals:
    void speakerClicked();
    void stateChange(bool remembered);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *ev);
    void keyPressEvent(QKeyEvent *ev);

protected:
    int  _id;
    bool _showSpeaker;
    QImage _speaker;
};

#endif // WBUTTON_H
