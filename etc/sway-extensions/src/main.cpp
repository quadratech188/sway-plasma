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
#include "session.h"
#include "sessionadaptor.h"

auto SERVICE_NAMES = {"org.freedesktop.ScreenSaver", "org.kde.KWin"};

int main(int argc, char* argv[]) {
	QCoreApplication app(argc, argv);

	auto conn = QDBusConnection::sessionBus();

	ScreenSaver screensaver;
	new ScreenSaverAdaptor(&screensaver);
	conn.registerObject("/ScreenSaver"                , &screensaver);
	conn.registerObject("/org/freedesktop/ScreenSaver", &screensaver);

	Session session;
	new SessionAdaptor(&session);
	conn.registerObject("/Session", &session);

	for (auto name: SERVICE_NAMES) {
		if (!conn.registerService(name)) {
			qWarning() << "Failed to register" << name;
			return 1;
		}
	}

	return app.exec();
}
