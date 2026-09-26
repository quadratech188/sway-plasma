#ifndef WORKSPACES_H
#define WORKSPACES_H

#include "ipc.h"
#include <QObject>
#include <QLocalSocket>
#include <qcontainerfwd.h>
#include <qjsondocument.h>
#include <qlocalsocket.h>
#include <qmap.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <qqmlregistration.h>

struct Workspace {
	Q_GADGET
	QML_VALUE_TYPE(workspace)
public:
	Q_PROPERTY(QString name MEMBER name)
	QString name;
	Q_PROPERTY(bool focused MEMBER focused)
	bool focused;
	Q_PROPERTY(bool urgent MEMBER urgent)
	bool urgent;
};

class SwayState: public QObject {
	Q_OBJECT
	QML_ELEMENT

	QString _error;
	QVector<Workspace> _workspaces;
	IPC socket;

public:
	SwayState(QObject* parent = nullptr);
	Q_PROPERTY(QString error READ error NOTIFY error_changed)
	Q_PROPERTY(QVector<Workspace> workspaces READ workspaces NOTIFY workspaces_changed)
	Q_INVOKABLE void set_workspace(QString const& name);

private:
	void handle_reply(uint32_t type, QJsonDocument const& doc);
	void throw_err(QString const& err);

	QString const& error() {return _error;}
	QVector<Workspace> const& workspaces() {return _workspaces;}

	void configure_initial_state();

signals:
	void error_changed();
	void workspaces_changed();
};

#endif
