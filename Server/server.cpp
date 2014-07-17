#include "server.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextCodec>
#include <QFile>


QT_USE_NAMESPACE


//constructor
Server::Server(quint16 port, QObject *parent):
     QObject(parent),
     pWebSocketServer(new QWebSocketServer(QStringLiteral("Server"),
                                                 QWebSocketServer::NonSecureMode, this)),
     pclients()
{
    /*Tells the server to listen for incoming connections*/
    if (pWebSocketServer->listen(QHostAddress::Any, port))
     {
        qDebug() << "Server listening on port" << port;
        connect(pWebSocketServer, &QWebSocketServer::newConnection,
                this, &Server::onNewConnection);
        connect(pWebSocketServer, &QWebSocketServer::closed, this, &Server::closed);

    }
 }


//destructor
Server::~Server()
{
    pWebSocketServer->close();
    qDeleteAll(pclients.begin(), pclients.end());
}



void Server::onNewConnection()
{
    //Returns the next pending connection as a connected QWebSocket object
    QWebSocket *pSocket = pWebSocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);

    connect(pSocket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &Server::processBinaryMessage);

    //adds new client to the list
    pclients << pSocket;
}



void Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        pclients.removeAll(pClient);
        pClient->deleteLater();
    }
}

QString Server::onAuthorization(QString login, QString password)
{
    qDebug() << "Authorization";
    QString result = SUCCESS;
    //Success + user rigths(admin, ...)
    return result;
}


/*
* @brief					Process text messages from users
*                           first byte of message is action that user wants to do
* @param	[QString]       _message - text message from user
* @return	void
*/
void Server::processTextMessage(QString _message)
{
    qDebug() << "Text Message received";

    QWebSocket* pClient = qobject_cast<QWebSocket* >(sender());
    if(pClient == NULL)
    {
        qDebug() << "Unapropriate user connection\n";
        return;
    }

    /*Process user request, first character represent the user's action */
    switch(_message[0].digitValue())
    {
    case authorization:
        if(pClient)
        {
            QString login, password, response;
            response += QString::number(authorization) + "|";

            QStringList substrings = _message.split("|") ;
            if(substrings.length() > 1)
            {
                password = substrings.value( substrings.length() - 1 );
                login = substrings.value(substrings.length()- 2 );
                response.append(onAuthorization(login, password));
                //emit makeAuthorization(login, password);
            }
            else
            {
                response += FAIL;
            }

            pClient->sendTextMessage(response);
        }
        break;

    case seeHistory:
        if(pClient)
        {
            pClient->sendTextMessage(_message);
        }
        break;

    case addUser:
     if(pClient)
     {
         pClient->sendTextMessage(_message);
     }
     break;

     /*if server doesn't support user's request*/
    default:
        QString response;
        response.append(_message[0]);
        response += FAIL;
        pClient->sendTextMessage(response);
        break;
    }
}


/*
* @brief					Process binary messages from users
*                           first byte of message is action that user wants to do
*                           next goes name of file that user wants to send
*                           then filesize
*                           next binary messages will be bytes of file
* @param	[QByteArray]    data - binary message
* @return	void
*/
void Server::processBinaryMessage(QByteArray data)
{
    package += data;
    qDebug() << package.size();
    /*
    QString request = QTextCodec::codecForMib(106)->toUnicode(data);
    qDebug() << request;
    */

    /*
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        pClient->sendTextMessage(request);
    }
    */

    /*
    QString response;
    response += QString::number(receiveVideo);
    response += "OK";
    QWebSocket* pClient = qobject_cast<QWebSocket* >(sender());
    if(pClient)
    {
        pClient->sendTextMessage(response);
        }
    else
        qDebug() << "Client disconnected";
    quint32 size = data.size();

    qDebug() << "Binary message received: " << size;
    QString filename;
    Last byte shows the action that user want to do
    quint32 action = data[size - 1];

    Last byte in binary message shows if it is a video or image
    switch(action)
    {
    case receiveVideo:
        filename = "D://video.avi";
        break;

    case addPicture:
        filename = "D://image.jpg";
        break;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    file.write(data, size - 1);
    file.close();
    */

}





