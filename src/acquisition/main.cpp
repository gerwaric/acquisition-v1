#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>

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


	engine.loadFromModule("com.gerwaric.acquisition", "Main");

	return app.exec();
}
