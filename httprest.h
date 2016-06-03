#ifndef HTTPREST_H
#define HTTPREST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QEventLoop>
#include <QDebug>

class HttpRest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRest(QObject *parent = 0);
    void post(QString url, QString data);
    void put(QString url, QString data);
    void get(QString url);

    QNetworkRequest getDefaultRequest(QString url);

    bool getWithAuth() const;
    void setWithAuth(bool value);

    bool getLastRequestWasSuccessful() const;
    void setLastRequestWasSuccessful(bool value, QString sourceName);

    bool getLastRequestWasOnError() const;
    void setLastRequestWasOnError(bool value);

    QString getUser() const;
    void setUser(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QString getBaseServerUrl() const;
    void setBaseServerUrl(const QString &value);

    QString getHeaderContentType() const;
    void setHeaderContentType(const QString &value);

    QString getHeaderAccept() const;
    void setHeaderAccept(const QString &value);

    QString getRestReturn() const;

private:
    QString restReturn;
    QEventLoop* loop;
    QString user;
    QString password;
    QString baseServerUrl;
    QString headerContentType;
    QString headerAccept;
    QNetworkAccessManager *manager;
    bool withAuth;
    bool lastRequestWasSuccessful;
    bool lastRequestWasOnError;
    //QSslError* sslErrors;

signals:

public slots:
    void replyFinished (QNetworkReply *reply);

private slots:
    void onSuccess (void);
    void onError(void);

};

#endif // HTTPREST_H
