#pragma once

#include <QObject>
#include <QString>

class FooterController : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString authButtonText READ authButtonText WRITE setAuthButtonText NOTIFY authButtonTextChanged)
public:
	explicit FooterController(QObject* parent = nullptr);
	QString authButtonText() const;
	void setAuthButtonText(const QString& text);
	Q_INVOKABLE void onAuthButtonClicked();
signals:
	void authButtonTextChanged();
private:
	QString m_auth_button_text;
};
