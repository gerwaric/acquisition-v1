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

#include <oauth/oauthmanager.h>

#include <QByteArray>
#include <QCryptographicHash>
#include <QDesktopServices>
#include <QtHttpServer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QString>
#include <QTcpServer>
#include <QTimer>
#include <QUrl>
#include <QUrlQuery>
#include <QUuid>

#include <utils/logging.h>

#include "appconstants.h"

// Hard-code authorization stuff.
constexpr const char* AUTHORIZE_URL = "https://www.pathofexile.com/oauth/authorize";
constexpr const char* TOKEN_URL = "https://www.pathofexile.com/oauth/token";
constexpr const char* CLIENT_ID = "acquisition";
constexpr const char* SCOPE = "account:leagues account:stashes account:characters";
constexpr const char* REDIRECT_URL = "http://127.0.0.1";
constexpr const char* REDIRECT_PATH = "/auth/path-of-exile";

// Refresh a token an hour before it's due to expire.
constexpr int EXPIRATION_BUFFER_SECS = 3600;

OAuthManager::OAuthManager(
	QNetworkAccessManager& network_manager)
    : m_network_manager(network_manager)
    , m_http_server(nullptr)
    , m_tcp_server(nullptr)
    , m_refresh_timer(this)
{
	LOG_TRACE("OAuthManager::OAuthManager() entered");

    // Configure the refresh timer.
    m_refresh_timer.setSingleShot(true);
    connect(&m_refresh_timer, &QTimer::timeout, this, &OAuthManager::requestRefresh);
}

OAuthManager::~OAuthManager() {};

void OAuthManager::setAuthorization(QNetworkRequest& request) {
	LOG_TRACE("OAuthManager::setAuthorization() entered");
    if (m_token.access_token.isEmpty()) {
		LOG_ERROR("Cannot set OAuth authorization header: there is no token.");
        return;
    };
    if (m_token.access_expiration.value_or(QDateTime()) <= QDateTime::currentDateTime()) {
		LOG_ERROR("Cannot set OAuth authorization header: the token has expired.");
        return;
    };
    const QString bearer = "Bearer " + m_token.access_token;
    request.setRawHeader("Authorization", bearer.toUtf8());
}

void OAuthManager::setRefreshTimer() {
	LOG_TRACE("OAuthManager::setRefreshTimer() entered");
    const QDateTime refresh_date = m_token.access_expiration.value_or(QDateTime()).addSecs(-EXPIRATION_BUFFER_SECS);
    const unsigned long interval = QDateTime::currentDateTime().msecsTo(refresh_date);
    m_refresh_timer.setInterval(interval);
    m_refresh_timer.start();
	LOG_INFO("OAuth: refreshing token again at {}", refresh_date.toString());
}

void OAuthManager::requestAccess() {
	LOG_TRACE("OAuthManager::setAccess() entered");

    // Build the state.
    const auto state_data = (
        QUuid::createUuid().toString(QUuid::WithoutBraces) +
        QUuid::createUuid().toString(QUuid::WithoutBraces)).toLatin1(); // 43 <= length <= 128
    const auto state_hash = QCryptographicHash::hash(state_data, QCryptographicHash::Sha256);
    const auto state = state_hash.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);

    // Create the code challenge.
    m_code_verifier = (
        QUuid::createUuid().toString(QUuid::WithoutBraces) +
        QUuid::createUuid().toString(QUuid::WithoutBraces)).toLatin1(); // 43 <= length <= 128
    const auto code_hash = QCryptographicHash::hash(m_code_verifier.toUtf8(), QCryptographicHash::Sha256);
    const auto code_challenge = code_hash.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);

    // Setup an http server so we know what port to listen on.
    createHttpServer();
    if (m_http_server == nullptr) {
		LOG_ERROR("OAuth: unable to create the http server authorization.");
        return;
    };

    // Get the port for the callback.
    const quint16 port = m_tcp_server->serverPort();
    if (port == 0) {
		LOG_ERROR("OAuth: the tcp server is not listening");
        return;
    };

    // Build the callback URI.
    QUrl url(REDIRECT_URL);
    url.setPort(port);
    url.setPath(REDIRECT_PATH);
    m_redirect_uri = url.toString();

    // Make the authorization request.
    requestAuthorization(state, code_challenge);
}

void OAuthManager::createHttpServer() {
	LOG_TRACE("OAuthManager::createHttpServer() entered");

    // Create a new HTTP server.
    m_http_server = new QHttpServer(this);

    // Tell the server to ignore favicon requests, even though these
    // should be disabled based on the HTML we are returning.
    m_http_server->route("/favicon.ico",
        [](const QHttpServerRequest& request, QHttpServerResponder& responder) {
            Q_UNUSED(request);
            Q_UNUSED(responder);
			LOG_TRACE("OAuth: ignoring favicon.ico request");
        });

    // Capture all unhandled requests for debugging.
    m_http_server->setMissingHandler(this,
        [](const QHttpServerRequest& request, QHttpServerResponder& responder) {
            Q_UNUSED(responder);
			LOG_TRACE("OAuth: unhandled request:", request.url().toString());
        });

    m_tcp_server = new QTcpServer(this);

    if (!m_tcp_server->listen()) {
		LOG_ERROR("OAuth: cannot start tcp server");
        m_tcp_server = nullptr;
        m_http_server = nullptr;
        return;
    };

    if (!m_http_server->bind(m_tcp_server)) {
		LOG_ERROR("OAuth: cannot bind http server to tcp server");
        m_tcp_server = nullptr;
        m_http_server = nullptr;
        return;
    };

}

void OAuthManager::requestAuthorization(const QString& state, const QString& code_challenge) {
	LOG_TRACE("OAuthManager::requestAuthorization() entered");

    // Create the authorization query.
	QUrlQuery query({
        {"client_id", CLIENT_ID},
        {"response_type", "code"},
        {"scope", SCOPE},
        {"state", state},
        {"redirect_uri", m_redirect_uri},
        {"code_challenge", code_challenge},
		{"code_challenge_method", "S256"}
	});

    // Prepare the url.
    QUrl authorization_url(AUTHORIZE_URL);
    authorization_url.setQuery(query);

    // Make sure the state is passed to the function that receives the authorization response.
    m_http_server->route(REDIRECT_PATH,
        [=](const QHttpServerRequest& request) {
            return receiveAuthorization(request, state);
        });

    // Use the user's browser to open the authorization url.
	LOG_TRACE("OAuthManager::requestAuthorization() opening url");
    QDesktopServices::openUrl(authorization_url);
}

QString OAuthManager::authorizationError(const QString& message) {
	LOG_ERROR("OAuth: authorization error: {}", message);
    return ERROR_HTML.arg(message);
}

QString OAuthManager::receiveAuthorization(const QHttpServerRequest& request, const QString& state) {
	LOG_TRACE("OAuthManager::receiveAuthorization() entered");

    // Shut the server down now that an access token response has been received.
    // Don't do it immediately in case the browser wants to request a favicon, even
    // though I've tried to disable that by including icon links in HTML.
    QTimer::singleShot(1000, this,
        [=]() {
            m_http_server = nullptr;
        });

    const QUrlQuery& query = request.query();

    // Check for errors.
    if (query.hasQueryItem("error")) {
        QString error_message = query.queryItemValue("error");
        const QString error_desription = query.queryItemValue("error_description");
        const QString error_uri = query.queryItemValue("error_uri");
        if (error_desription.isEmpty() == false) {
            error_message += " : " + error_desription;
        };
        if (error_uri.isEmpty() == false) {
            error_message += " : " + error_uri;
        };
        return authorizationError(error_message);
    };

    const QString auth_code = query.queryItemValue("code");
    const QString auth_state = query.queryItemValue("state");

    // Make sure the code and state look valid.
    if (auth_code.isEmpty()) {
        return authorizationError("Invalid authorization response: 'code' is missing.");
    };
    if (auth_state.isEmpty()) {
        return authorizationError("Invalid authorization response: 'state' is missing.");
    };
    if (auth_state != state) {
        return authorizationError("Invalid authorization repsonse: 'state' is invalid!");
    };

    // Use the code to request an access token.
    requestToken(auth_code);

    // Update the user.
    return SUCCESS_HTML;
};

void OAuthManager::requestToken(const QString& code) {
	LOG_TRACE("OAuthManager::requestToken() entered");

    QNetworkRequest request;
    request.setUrl(QUrl(TOKEN_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);

	const QUrlQuery query({
        {"client_id", CLIENT_ID},
        {"grant_type", "authorization_code"},
        {"code", code},
        {"redirect_uri", m_redirect_uri},
        {"scope", SCOPE},
		{"code_verifier", m_code_verifier}
	});
    const QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkReply* reply = m_network_manager.post(request, data);

    connect(reply, &QNetworkReply::finished, this,
        [=]() {
            receiveToken(reply);
            reply->deleteLater();
        });
    connect(reply, &QNetworkReply::errorOccurred, this,
        [=]() {
			LOG_ERROR("Error requesting OAuth access token: {}", reply->errorString());
            reply->deleteLater();
        });
}

void OAuthManager::receiveToken(QNetworkReply* reply) {
	LOG_TRACE("OAuthManager::receiveToken() entered");

    if (reply->error() != QNetworkReply::NoError) {
		LOG_ERROR("OAuth: http error {}: {}",
			reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
			reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());
        return;
    };

    // Parse the token and emit it.
	LOG_TRACE("OAuthManager::receiveToken() parsing OAuth access token");
    m_token = OAuthToken(reply);

    emit accessGranted(m_token);

    // Setup the refresh timer.
    setRefreshTimer();

    // Make sure to dispose of the reply.
    reply->deleteLater();
}

void OAuthManager::requestRefresh() {
	LOG_INFO("OAuth: attempting to refresh the access token");

    // Setup the refresh query.
	const QUrlQuery query({
        {"client_id", CLIENT_ID},
        {"grant_type", "refresh_token"},
		{"refresh_token", m_token.refresh_token}
	});
    const QByteArray data = query.toString(QUrl::FullyEncoded).toUtf8();

    // Make and submit the POST request.
    QNetworkRequest request;
    request.setUrl(QUrl(TOKEN_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);
    QNetworkReply* reply = m_network_manager.post(request, data);

    connect(reply, &QNetworkReply::finished, this,
        [=]() {
            // Update the user again after the token has been received.
            receiveToken(reply);
            reply->deleteLater();
			LOG_INFO("OAuth: the oauth token has been refreshed");
        });

    connect(reply, &QNetworkReply::errorOccurred, this,
        [=]() {
            reply->deleteLater();
            const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            const auto reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
			LOG_ERROR("OAuth: network error {} while refreshing token: {}", status, reason);
        });
}

void OAuthManager::showStatus() {
	LOG_TRACE("OAuthManager::showStatus() entered");

	//QMessageBox* msgBox = new QMessageBox;
	//msgBox->setWindowTitle("OAuth Status - " APP_NAME " - OAuth Token Status");
	//msgBox->setModal(false);
	//msgBox->setAttribute(Qt::WA_DeleteOnClose);

    const QDateTime now = QDateTime::currentDateTime();
    const QString json = QString::fromUtf8(JS::serializeStruct(m_token, JS::SerializerOptions::Pretty));
    QStringList message = { "Your current OAuth token:", json };

    if (!m_token.access_expiration) {
        message += "The token's access_expiration is missing";
    };
    if (!m_token.refresh_expiration) {
        message += "The token's refresh_expiration is missing";
    };

    if (m_token.access_expiration && m_token.refresh_expiration) {
        if (now < m_token.access_expiration.value_or(QDateTime())) {
            const QDateTime refresh_time = now.addMSecs(m_refresh_timer.remainingTime());
            const QString refresh_timestamp = refresh_time.toString("MMM d 'at' h:m ap");
            message.append("This token will be automatically refreshed on " + refresh_timestamp);
        } else if (now < m_token.refresh_expiration) {
                message.append("This token needs to be refreshed now");
            } else {
                message.append("No valid token. You are not authenticated.");
            };
    } else {
        message.append("The token's expiration dates are missing");
    };
	const QString s = message.join("\n\n");
	LOG_INFO("OAuth status: {}", s);
	//msgBox->setText(message.join("\n\n"));
	//msgBox->show();
	//msgBox->raise();
}

// Return this HTML to the browser after successful authentication,
// and try to avoid a favicon request.
const QString OAuthManager::SUCCESS_HTML = QString(
    R"html(
		<html>
			<head>
				<link rel="icon" href="data:, ">
				<title>Acquisition</title>
				<style>
					html, body, .container { height: 75%; }
					.container { display: flex; align-items: center; justify-content: center; }
				</style>
			</head>
			<body>
				<h1 class="container">Acquisition has been authorized.<br>You may close this page.</h1>
			</body>
		</html>"
	)html").simplified();

// Use this as a template to show authentication errors.
const QString OAuthManager::ERROR_HTML = QString(
    R"html(
		<html>
			<head>
				<link rel="icon" href="data:, ">
				<title>OAuth Authorization Error</title>
			</head>
			<body>
				<p>%2</p>
			</body>
		</html>
	)html").simplified();
