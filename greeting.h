#include "lib/qhttpserverfwd.h"

#include <QObject>

/// Greeting

class Greeting : public QObject
{
    Q_OBJECT

public:
    Greeting();

private:
    QString m_ip;
    QString m_mac;

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);
};
