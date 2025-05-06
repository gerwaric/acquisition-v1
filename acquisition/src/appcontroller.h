#pragma once

#include <QObject>

#include "footercontroller.h"

class AppController : public QObject {
	Q_OBJECT
	Q_PROPERTY(QObject* footerController READ footerController CONSTANT)

public:
	explicit AppController(QObject* parent = nullptr);

	QObject* footerController();

private:
	FooterController m_footer_controller;
};
