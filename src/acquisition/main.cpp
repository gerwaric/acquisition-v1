#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>

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

	//auto m = new TreeModel(&app);

	//engine.rootContext()->setContextProperty("treeModel", m);

	return app.exec();
}
