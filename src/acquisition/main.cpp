#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlEngine>

#include <QDirIterator>
#include <QResource>

#include <acquisition/model/treemodel.h>

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
		&app,
		[]() { QCoreApplication::exit(-1); },
		Qt::QueuedConnection);

	QDirIterator it(":/", QDirIterator::Subdirectories);
	while (it.hasNext()) {
		qDebug() << it.next();
	};

	QString path = ":/qt/qml/com/gerwaric/acquisition/qmldir";
	if (QResource(path).isValid()) {
		qDebug() << "Found embedded qmldir at:" << path;
	} else {
		qDebug() << "qmldir NOT found at:" << path;
	};


	engine.loadFromModule("com.gerwaric.acquisition", "Main");

	return app.exec();
}
