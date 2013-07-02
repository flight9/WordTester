#include <QtGui/QApplication>
#include "mdialog.h"
#include "main.h"

#include <QDebug>
#include <QTranslator>
#include <QLocale>
#include <QSplashScreen>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    loadLanguage("qt");
    loadLanguage("WordTester");

    QSplashScreen* splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/splash.jpg"));
    splash->show();

    Qt::Alignment align = Qt::AlignLeft|Qt::AlignTop;
    splash->showMessage(QObject::tr("Loading modules..."), align, Qt::white);
    sleep(1000);

    MDialog w;
    w.show();

    splash->finish(&w);
    delete splash;

    return app.exec();

}

bool loadLanguage(const QString name)
{
    QTranslator* trans = new QTranslator(qApp);
    trans->load(name+'_'+QLocale::system().name());
    bool ok = !trans->isEmpty();
    qApp->installTranslator( trans);
    return ok;
}

void sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


