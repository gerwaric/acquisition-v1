#pragma once

#include <acquisition/common/json_struct_qt.h>
#include <acquisition/common/logging.h>

#include <QString>
#include <QByteArray>

#include <string>

namespace json {

	template<typename T>
	void parse(const std::string& json, T& out) {
		JS::ParseContext context(json);
		if (context.parseTo<T>(out) != JS::Error::NoError) {
			const QString type_name(typeid(T).name());
			const QString error_message = QString::fromStdString(context.makeErrorString());
			LOG_ERROR("Error parsing json into '{}':", type_name, error_message);
		};
	}

	template<typename T>
	inline void parse(const QByteArray& json, T& out) {
		parse<T>(json.toStdString(), out);
	}

	template<typename T>
	inline void parse(const QString& json, T& out) {
		parse<T>(json.toUtf8(), out);
	}

	template<typename T>
	inline T parse(const QByteArray& json) {
		T result;
		parse<T>(json, result);
		return result;
	}

	template<typename T>
	inline T parse(const QString& json) {
		T result;
		parse<T>(json.toUtf8(), result);
		return result;
	}
}
