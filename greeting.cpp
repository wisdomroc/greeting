#include "greeting.h"

#include <QCoreApplication>
#include <QRegExp>
#include <QStringList>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QNetworkAddressEntry>
#include <QList>

#include "lib/qhttpserver.h"
#include "lib/qhttprequest.h"
#include "lib/qhttpresponse.h"
#include "singleapplication.h"

/// Greeting


QString getIP(QString localHost)
{
    QString ipAddr;
    QHostInfo info = QHostInfo::fromName(localHost);

    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol){
            qDebug() << "address-->" << address.toString();
            ipAddr = address.toString();
        }
    }
    return ipAddr;
}

QString getMAC()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning)
                && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}


Greeting::Greeting()
{
    QString localHost = QHostInfo::localHostName();
    m_ip = getIP(localHost);
    m_mac = getMAC();


    QHttpServer *server = new QHttpServer();
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

    server->listen(QHostAddress::Any, 8080);
}


void Greeting::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    QRegExp exp("^/user/([a-z]+)$");
    qDebug() << "req->path():" << req->path();
    if( exp.indexIn(req->path()) != -1 )
    {
        resp->setHeader("Content-Type", "text/html");
        resp->writeHead(200);
        
        QString name = exp.capturedTexts()[1];
        QString body = tr("<html><head><title>Greeting App</title></head><body><h1>Hello %1!</h1></body></html>");
        resp->end(body.arg(name).toUtf8());
    }
    else
    {
        //resp->writeHead(403);
        resp->setHeader("Content-Type", "text/html");
		resp->setHeader("Access-Control-Allow-Origin", "*");
		resp->setHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS,DELETE,PUT");
		resp->setHeader("Access-Control-Allow-Headers", "Test");
        resp->writeHead(200);
		m_ip.remove(".");
		m_mac.remove(":");
        resp->end((tr("%1%2").arg(m_ip).arg(m_mac)).toLocal8Bit());
    }
}

/// main

int main(int argc, char **argv)
{
    SingleApplication app(argc, argv, "some unique key string");
    if (app.isRunning())
    {
        app.sendMessage("message from other instance.");
        return 0;
    }

	app.addLibraryPath("./plugins");

    //QCoreApplication app(argc, argv);
    Greeting greeting;
    app.exec();
}
