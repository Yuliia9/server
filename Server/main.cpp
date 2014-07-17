#include "server.h"
#include <QCoreApplication>
#include <QtCore/QDebug>

#define PORT 80

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server* pserver =  new Server(PORT);
    QObject::connect(pserver, &Server::closed, &a, &QCoreApplication::quit);
    //QObject::connect(pserver, SIGNAL(makeAuthorization(QString,QString)), pserver, SLOT(onAuthorization(QString, QString)));

    return a.exec();

}
