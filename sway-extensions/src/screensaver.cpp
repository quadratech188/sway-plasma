#include "screensaver.h"
#include <qdbusabstractadaptor.h>
#include <qdbusconnection.h>
#include <qdbuscontext.h>
#include <qdbuserror.h>
#include <qdbusinterface.h>
#include <qdbusmessage.h>
#include <qdbusreply.h>
#include <qlogging.h>
#include <qobject.h>
#include <QRandomGenerator>
#include <qrandom.h>
#include "policyagent.h"

OrgKdeSolidPowerManagementPolicyAgentInterface make_policyagent() {
	return OrgKdeSolidPowerManagementPolicyAgentInterface(
		"org.kde.Solid.PowerManagement",
		"/org/kde/Solid/PowerManagement/PolicyAgent",
		QDBusConnection::sessionBus()
	);
}

uint const CHANGE_SCREEN_SETTINGS = 4;

ScreenSaver::ScreenSaver(QObject* parent):
	QObject(parent), known_cookies(), service_watcher(this) {

	service_watcher.setConnection(QDBusConnection::sessionBus());
	service_watcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);

	connect(
		&service_watcher, &QDBusServiceWatcher::serviceUnregistered,
		this, &ScreenSaver::handle_service_unregister
	);
}

void ScreenSaver::handle_service_unregister(QString const& name) {
	auto cookies = known_cookies.values(name);
	if (cookies.count() == 0) return;

	qDebug() << "Clean up" << cookies.count() << "cookie(s) for" << name;

	known_cookies.remove(name);
	auto policyagent = make_policyagent();

	for (auto cookie: cookies) {
		policyagent.ReleaseInhibition(cookie);
	}
}

uint ScreenSaver::Inhibit(QString const& name, QString const& reason) {
	auto policyagent = make_policyagent();
	auto reply = policyagent.AddInhibition(CHANGE_SCREEN_SETTINGS, name, reason);
	reply.waitForFinished();

	if (reply.isError()) {
		qWarning() << "Failed to Inhibit:" << reply.error().message();
		sendErrorReply(reply.error().type(), reply.error().message());
		return 0;
	}

	uint cookie = reply.value();
	qDebug() << "Add cookie" << cookie << "for" << message().service();
	known_cookies.insert(message().service(), cookie);
	service_watcher.addWatchedService(message().service());
	return cookie;
}

void ScreenSaver::UnInhibit(uint cookie) {
	auto policyagent = make_policyagent();
	auto reply = policyagent.ReleaseInhibition(cookie);

	if (reply.isError()) {
		qWarning() << "Failed to UnInhibit:" << reply.error().message();
		sendErrorReply(reply.error().type(), reply.error().message());
		return;
	}

	qDebug() << "Remove cookie" << cookie << "for" << message().service();
	size_t cnt = known_cookies.remove(message().service(), cookie);
	if (cnt == 0) {
		qWarning()
			<< "Unknown cookie" << cookie << "from" << message().service()
			<< "(Not an error)";
	}
}
