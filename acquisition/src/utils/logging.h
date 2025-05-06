#pragma once

#include <spdlog/spdlog.h>

#include <QString>
#include <QByteArray>
#include <QDateTime>

template <>
struct fmt::formatter<QString> : fmt::formatter<std::string_view> {
	template <typename FormatContext>
	auto format(const QString& str, FormatContext& context) const {
		const QByteArray utf8 = str.toUtf8();
		return fmt::formatter<std::string_view>::format(utf8.constData(), context);
	}
};

template <>
struct fmt::formatter<QByteArray> : fmt::formatter<std::string_view> {
	template <typename FormatContext>
	auto format(const QByteArray& utf8, FormatContext& context) const {
		return fmt::formatter<std::string_view>::format(utf8.constData(), context);
	}
};

template <>
struct fmt::formatter<QDateTime> : fmt::formatter<std::string_view> {
	template <typename FormatContext>
	auto format(const QDateTime& date, FormatContext& context) const {
		const QByteArray utf8 = date.toString().toUtf8();
		return fmt::formatter<std::string_view>::format(utf8.constData(), context);
	}
};

#define SHOULD_LOG(X)     spdlog::default_logger_raw() && spdlog::default_logger_raw()->should_log(spdlog::level::X)
#define LOG_TRACE(...)    do { if (SHOULD_LOG(trace)) spdlog::trace(__VA_ARGS__); } while (0)
#define LOG_DEBUG(...)    do { if (SHOULD_LOG(debug)) spdlog::debug(__VA_ARGS__); } while (0)
#define LOG_INFO(...)     do { if (SHOULD_LOG(info)) spdlog::info(__VA_ARGS__); } while (0)
#define LOG_WARN(...)     do { if (SHOULD_LOG(warn)) spdlog::warn(__VA_ARGS__); } while (0)
#define LOG_ERROR(...)    do { if (SHOULD_LOG(err)) spdlog::error(__VA_ARGS__); } while (0)
#define LOG_CRITICAL(...) do { if (SHOULD_LOG(critical)) spdlog::critical(__VA_ARGS__); } while (0)
