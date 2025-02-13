#include <qapplication.h>
#include <qqml.h>
#include <qqmlcomponent.h>
#include <qobject.h>
#include <qqmlengine.h>
#include <QtQuick/qquickwindow.h>
#include <qsurfaceformat.h>
#include <qmessagebox.h>

#include "qlabel.h"

#include "visualiser.h"

#ifdef WIN32
#include <Windows.h>
#endif

void errorMessage1(QString text) {
	QMessageBox box;
	box.setText(text);
	box.exec();
	return;
}

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	//Register our component type with QML.
	qmlRegisterType<DataVisualyzer>("custom", 1, 0, "DataVisualyzer");
	qmlRegisterType<ErrorProvider>("custom", 1, 0, "ErrorProvider");

	QQmlEngine engine;
	QQmlComponent* component = new QQmlComponent(&engine,
		QUrl::fromLocalFile("qml/mainWindow.qml"));

	QObject::connect(&engine, SIGNAL(quit()), &app, SLOT(quit()));

	if (!component->isReady()) {
		qWarning("%s", qPrintable(component->errorString()));
		QMessageBox box;
		box.setText(component->errorString());
		box.exec();
		return -1;
	}

	QObject* windowObject = component->create();
	QQuickWindow* mainWindow = qobject_cast<QQuickWindow*>(windowObject);

	QSurfaceFormat surfaceFormat = mainWindow->requestedFormat();
	mainWindow->setFormat(surfaceFormat);
	mainWindow->show();

	int retCode = app.exec();

	delete component;

	return retCode;
}