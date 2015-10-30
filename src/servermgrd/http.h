#ifndef HTTP_H
#define HTTP_H

#include <QObject>
#include <QTcpSocket>
#ifndef DISABLE_SSL
#include <QSslSocket>
#endif

class http : public QObject
{
    Q_OBJECT
public:
    http(QObject *parent = 0);

};

#endif // HTTP_H
