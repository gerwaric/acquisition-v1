#pragma once

#include <QDateTime>
#include <QStringView>

namespace rfc2822 {

	QDateTime parse(QStringView s);

	inline QDateTime parse(const QByteArray& utf) {
		return rfc2822::parse(QString::fromUtf8(utf));
	}

}
