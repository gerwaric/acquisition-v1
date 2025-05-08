#include "footercontroller.h"

FooterController::FooterController(QObject* parent)
	: QObject(parent)
	, m_auth_button_text("Settings") {}

QString FooterController::authButtonText() const {
	return m_auth_button_text;
}

void FooterController::setAuthButtonText(const QString& text) {
	if (m_auth_button_text != text) {
		m_auth_button_text = text;
		emit authButtonTextChanged();
	}
}

void FooterController::onAuthButtonClicked() {
	// Do your C++ logic here
	qDebug("Button clicked, doing stuff...");

	// Update the text
	setAuthButtonText("Done");
}
