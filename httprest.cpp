#include "httprest.h"
#include <QDebug>

HttpRest::HttpRest(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    loop = new QEventLoop;
    withAuth = false;
}



QNetworkRequest HttpRest::getDefaultRequest(QString url)
{
    setLastRequestWasOnError(false);
    setLastRequestWasSuccessful(false, QString("getDefaultRequest"));

    QNetworkRequest request;

    if (withAuth)
    {
        request.setRawHeader("Authorization", "Basic " +
                             QByteArray(QString("%1:%2").arg(user).arg(password).toUtf8().toBase64())
                             );
    }

    request.setRawHeader("Accept", headerAccept.toUtf8());
    //request.setRawHeader("Content-Type", "application/vnd.agent-submission.v1+json");

    request.setUrl(QUrl(url));

    return request;
}

bool HttpRest::getWithAuth() const
{
    return withAuth;
}

void HttpRest::setWithAuth(bool value)
{
    withAuth = value;
}
bool HttpRest::getLastRequestWasSuccessful() const
{
    return lastRequestWasSuccessful;
}

void HttpRest::setLastRequestWasSuccessful(bool value, QString sourceName)
{
    if(sourceName.toStdString() == "onSuccess")
    {
        if (!getLastRequestWasOnError())
        {
            lastRequestWasSuccessful = value;
            setLastRequestWasOnError(false);
            qDebug() << "RESTful SUCCESS";
        }
    }else if(sourceName.toStdString() == "onError")
    {
        lastRequestWasSuccessful = value;
        setLastRequestWasOnError(true);
        qDebug() << "RESTful ERROR";
    }else if(sourceName.toStdString() == "getDefaultRequest")
    {
        lastRequestWasSuccessful = value;
    }else{
        lastRequestWasSuccessful = value;
    }
}
bool HttpRest::getLastRequestWasOnError() const
{
    return lastRequestWasOnError;
}

void HttpRest::setLastRequestWasOnError(bool value)
{
    lastRequestWasOnError = value;
}
QString HttpRest::getUser() const
{
    return user;
}

void HttpRest::setUser(const QString &value)
{
    user = value;
}
QString HttpRest::getPassword() const
{
    return password;
}

void HttpRest::setPassword(const QString &value)
{
    password = value;
}
QString HttpRest::getBaseServerUrl() const
{
    return baseServerUrl;
}

void HttpRest::setBaseServerUrl(const QString &value)
{
    baseServerUrl = value;
}
QString HttpRest::getHeaderContentType() const
{
    return headerContentType;
}

void HttpRest::setHeaderContentType(const QString &value)
{
    headerContentType = value;
}
QString HttpRest::getHeaderAccept() const
{
    return headerAccept;
}

void HttpRest::setHeaderAccept(const QString &value)
{
    headerAccept = value;
}
QString HttpRest::getRestReturn() const
{
    return restReturn;
}

void HttpRest::replyFinished(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }
    else
    {
        //        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        //        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();;
        //        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        //        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        //        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        restReturn = QString(reply->readAll());
    }

    //reply->deleteLater();
    reply->disconnect();
    delete reply;

    loop->exit();
}

void HttpRest::onSuccess()
{
    setLastRequestWasSuccessful(true, QString("onSuccess"));
    loop->quit();
}

void HttpRest::onError()
{
    setLastRequestWasSuccessful(false, QString("onError"));
    loop->quit();
}



void HttpRest::post(QString url, QString data)
{
    QNetworkRequest request = getDefaultRequest(baseServerUrl.toUtf8() + url.toUtf8());

    request.setRawHeader("Content-Type", "application/json");

    manager->post(request, data.toUtf8());
    //loop->exec();
}

void HttpRest::put(QString url, QString data)
{
    QNetworkRequest request = getDefaultRequest(baseServerUrl.toUtf8() + url.toUtf8());

    request.setRawHeader("Content-Type", "application/json");

    manager->put(request, data.toUtf8());
    loop->exec();
}

void HttpRest::get(QString url)
{
    QNetworkRequest request = getDefaultRequest(baseServerUrl.toUtf8() + url.toUtf8());

    manager->get(request);
    loop->exec();
}

