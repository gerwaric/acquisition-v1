#pragma once

#include <acquisition/common/json_struct_qt.h>
#include <acquisition/common/logging.h>

#include <QObject>
#include <QDateTime>
#include <QSqlDatabase>
#include <QString>

class UserStore : public QObject {
    Q_OBJECT
public:
	explicit UserStore(const QString& directory,  QObject* parent);

	void setInt(const QString& name, int value) { setValue(name, value); };
	void setBool(const QString& name, bool value) { setValue(name, value); };
	void setString(const QString& name, const QString& value) { setValue(name, value); };
	void setDateTime(const QString& name, const QDateTime& value) { setValue(name, value); };

	int getInt(const QString& name) { return getValue(name).toInt(); };
	bool getBool(const QString& name) { return getValue(name).toBool(); };
	QString getString(const QString& name) { return getValue(name).toString(); };
	QDateTime getDateTime(const QString& name) { return QDateTime::fromString(getValue(name).toString()); };

    template<typename T>
	void set(const QString& name, const T& value)
    {
		const QString data = QString::fromStdString(JS::serializeStruct(value, JS::SerializerOptions::Compact));
		setValue(name, data);
    };

    template<typename T>
	T get(const QString& name)
    {
        T value;
		const std::string data = getValue(name).toString().toUtf8().toStdString();
		JS::ParseContext parseContext(data);
		const JS::Error result = parseContext.parseTo(value);
		if (result != JS::Error::NoError) {
			LOG_ERROR("Error parsing {} from user datastore: {}", name, parseContext.makeErrorString());
		};
        return value;
    };

public slots:

    void setUsername(const QString& username);

private:

    QVariant getValue(const QString& name);
    void setValue(const QString& name, const QVariant& value);

	QString m_datadir;
    QString m_username;
    QSqlDatabase m_db;

};
