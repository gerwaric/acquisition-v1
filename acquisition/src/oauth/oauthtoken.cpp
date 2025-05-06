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

#include <oauth/oauthtoken.h>

#include <QNetworkReply>

#include <utils/json.h>
#include <utils/rfc2822.h>

// Hard-code the token refresh lifetime for a public client:
// https://www.pathofexile.com/developer/docs/authorization#clients-public
constexpr long int REFRESH_LIFETIME_DAYS = 7;

OAuthToken::OAuthToken(const QString& json)
{
	json::parse<OAuthToken>(json, *this);
}

OAuthToken::OAuthToken(QNetworkReply* reply)
{
    const QByteArray bytes = reply->readAll();
    reply->deleteLater();

	json::parse<OAuthToken>(bytes, *this);

    // Determine birthday and expiration time.
	birthday = rfc2822::parse(reply->rawHeader("Date")).toLocalTime();
    access_expiration = birthday->addSecs(expires_in);
    refresh_expiration = birthday->addDays(REFRESH_LIFETIME_DAYS);
}
