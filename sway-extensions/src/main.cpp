#include <QCoreApplication>
#include <QtDBus/qdbusconnection.h>
#include <QtDBus/qdbusinterface.h>
#include <QtDBus/qdbusservicewatcher.h>
#include <qcoreapplication.h>
#include <qlogging.h>
#include <qobject.h>
#include <qtmetamacros.h>

#include "screensaver.h"
#include "screensaveradaptor.h"

auto SERVICE_NAME = "org.freedesktop.ScreenSaver";

int main(int argc, char* argv[]) {
	QCoreApplication app(argc, argv);

	ScreenSaver screensaver;
	new ScreenSaverAdaptor(&screensaver);

	auto conn = QDBusConnection::sessionBus();
	conn.registerObject("/ScreenSaver"                , &screensaver);
	conn.registerObject("/org/freedesktop/ScreenSaver", &screensaver);

	if (!conn.registerService(SERVICE_NAME)) {
		qWarning() << "Failed to register" << SERVICE_NAME;
		return 1;
	}
	return app.exec();
}
