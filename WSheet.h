#ifndef WSHEET_H
#define WSHEET_H

#include <QObject>

#include "word.h"
#include "voice/XpTTSVoice.h"

class WSheet : public QObject
{
    Q_OBJECT
public:
    explicit WSheet(QObject *parent = 0);

public:  //out
    QString textAt(int idx);
    Word::WordStatus statusAt(int idx);
    QString statusNameAt(int idx);
    QString answerAt(int idx);
    int  size();
    bool isRememberedAt(int idx);
    bool isForgottenAt(int idx);
    bool hasAnswerAt(int idx);

public:
    bool loadFile( const QString& fileName, bool append);
    bool saveFile(const QString &fileName);
    bool initReader();
    void readTextAt(int idx);
    void readPause();
    void readResume();

public:  //in
    void setText(QString txt, int idx);
    void setStatus(Word::WordStatus sta, int idx);
    void setAnswer(QString ans, int idx);
    void eraseAt(int idx);
    void pushBack(const Word& word);

signals:
    void voiceCompleted();

private:
    WordArray _warry;
    XpTTSVoice _voice;
};

#endif // WSHEET_H
