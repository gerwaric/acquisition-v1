#pragma once

#include <QObject>

#include <QtQml/qqml.h>

//#include "footercontroller.h"

class AppController : public QObject {
	Q_OBJECT
    QML_ELEMENT
//	Q_PROPERTY(QObject* footerController READ footerController CONSTANT)

public:
	explicit AppController(QObject* parent = nullptr);

//	QObject* footerController();

private:
//	FooterController m_footer_controller;
};
