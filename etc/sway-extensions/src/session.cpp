#include "session.h"
#include <qlogging.h>
#include <qobject.h>

Session::Session(QObject* parent):
	QObject(parent) {}

bool Session::closeWaylandWindows() {
	qWarning() << "STUB: Session::closeWaylandWindows";

	// Just return true to let logout continue
	return true;
}
