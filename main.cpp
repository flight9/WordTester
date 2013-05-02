#include <QtGui/QApplication>
#include "mdialog.h"
#include "main.h"

#include <QDebug>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    loadLanguage("qt");
    loadLanguage("WordTester");

    MDialog w;
    w.show();
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
