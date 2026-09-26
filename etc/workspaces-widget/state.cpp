#include "state.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <cstdint>
#include <qcontainerfwd.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qlocalsocket.h>
#include <qlogging.h>
#include <qobject.h>
#include <qstringview.h>
#include <qtenvironmentvariables.h>
#include <qtmetamacros.h>
#include <qtypes.h>

SwayState::SwayState(QObject* parent): QObject(parent), socket(this) {
	auto swaysock = qEnvironmentVariable("SWAYSOCK");
	if (swaysock == "") {
		throw_err("SWAYSOCK not set");
	}

	connect(&socket, &QLocalSocket::errorOccurred, this, [this]() {
		throw_err("Failed to connect to socket");
		qWarning() << socket.error();
	});
	connect(&socket, &QLocalSocket::connected, this, [this]() {
		socket.send(SUBSCRIBE, QByteArray("['workspace']"));
		// get_workspaces doesn't give us node ids, get_tree does
		socket.send(GET_WORKSPACES, QByteArray());
	});
	connect(&socket, &IPC::recv, this, &SwayState::handle_reply);
	socket.connectToServer(swaysock);
}

static Workspace parse_workspace(QJsonObject const& obj) {
	return Workspace {
		.name = obj["name"].toString(),
		.focused = obj["focused"].toBool(),
		.urgent= obj["urgent"].toBool()
	};
}

void SwayState::handle_reply(uint32_t type, QJsonDocument const &doc) {
	switch (type) {
		case GET_WORKSPACES:
			_workspaces.clear();
			for (auto obj: doc.array()) {
				_workspaces.push_back(parse_workspace(obj.toObject()));
			}
			emit workspaces_changed();
			break;
		case 0x80000000: // workspace
			socket.send(GET_WORKSPACES, QByteArray());
			break;
		default:
			break;
	}
}

void SwayState::throw_err(QString const& err) {
	qWarning() << err;
	_error = err;
	error_changed();
}
