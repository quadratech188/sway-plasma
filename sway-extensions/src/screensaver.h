#include <QtDBus/qdbusabstractadaptor.h>
#include <QtDBus/qdbusextratypes.h>
#include <qdbuscontext.h>
#include <qdbusreply.h>
#include <qdbusservicewatcher.h>
#include <qhashfunctions.h>
#include <qmap.h>
#include <qobject.h>
#include <qset.h>
#include <qtmetamacros.h>

#ifndef SCREENSAVER_H
#define SCREENSAVER_H

class ScreenSaver: public QObject, protected QDBusContext {
	Q_OBJECT
public:
	ScreenSaver(QObject* obj = nullptr);

private:
	QMultiMap<QString, uint> known_cookies;
	QDBusServiceWatcher service_watcher;

	void handle_service_unregister(QString const& name);

public slots:
	uint Inhibit(QString const& name, QString const& reason);
	void UnInhibit(uint cookie);
};

#endif
