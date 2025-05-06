#include "appcontroller.h"

AppController::AppController(QObject* parent)
	: QObject(parent)
	, m_footer_controller(this) {}

QObject* AppController::footerController() {
	return &m_footer_controller;
}
