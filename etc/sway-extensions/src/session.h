#include <qdbuscontext.h>
#include <qobject.h>
#include <qtmetamacros.h>

#ifndef SESSION_H
#define SESSION_H

class Session: public QObject, protected QDBusContext {
	Q_OBJECT
public:
	Session(QObject *obj = nullptr);

public slots:
	bool closeWaylandWindows();
};

#endif
