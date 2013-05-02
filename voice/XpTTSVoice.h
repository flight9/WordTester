#ifndef XPTTSVOICE_H
#define XPTTSVOICE_H

// 2013.02 v1.0 by Liangzm
// 2013.04 v1.1
// add "CONFIG += qaxcontainer" to your .pro file

#include <QObject>
#include <QAxObject>

class XpTTSVoice : public QObject
{
    Q_OBJECT
public:
    explicit XpTTSVoice(QObject *parent = 0);

public:
    bool initial();
    int  speak(QString& txt);
    int  speakJapanese(QString& txt);
    int  speakEnglish(QString& txt);
    int  speakChinese(QString& txt);
    int  speakForeign(QString& txt);
    void pause();
    void resume();

signals:
    void readingStarted();
    void readingCompleted();

private slots:
    void handleActivxSignal(QString, int, void*);

private:
    QAxObject _activx;
    bool _tok;
    bool _busy;
};

#endif // XPTTSVOICE_H
