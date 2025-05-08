/*
    Copyright (C) 2014-2024 Acquisition Contributors

    This file is part of Acquisition.

    Acquisition is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Acquisition is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Acquisition.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <acquisition/oauth/oauthtoken.h>

#include <QObject>
#include <QTimer>

class QHttpServer;
class QHttpServerRequest;
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class QTcpServer;

class OAuthManager : public QObject {
    Q_OBJECT
public:
	explicit OAuthManager(QNetworkAccessManager& network_manager);
    ~OAuthManager();
    void setAuthorization(QNetworkRequest& request);
    const OAuthToken& token() const { return m_token; };
public slots:
    void requestAccess();
    void requestRefresh();
    void showStatus();
signals:
    void accessGranted(const OAuthToken& token);
private:
    void createHttpServer();
    void requestAuthorization(const QString& state, const QString& code_challenge);
    QString receiveAuthorization(const QHttpServerRequest& request, const QString& state);
    void requestToken(const QString& code);
    void receiveToken(QNetworkReply* reply);
    void setRefreshTimer();

    static QString authorizationError(const QString& message);

    QNetworkAccessManager& m_network_manager;

    // I can't find a way to shutdown a QHttpServer once it's started
    // listening, so use a unique pointer so that we can destory the
    // server once authentication is complete, so it won't stay
    // running in the background.
    QHttpServer* m_http_server;
    QTcpServer* m_tcp_server;

    OAuthToken m_token;
    QString m_code_verifier;
    QString m_redirect_uri;

    QTimer m_refresh_timer;

    static const QString SUCCESS_HTML;
    static const QString ERROR_HTML;
};
