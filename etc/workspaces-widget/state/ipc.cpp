#include "ipc.h"
#include <QDebug>
#include <cstdint>
#include <cstring>
#include <qbytearrayview.h>
#include <qjsondocument.h>
#include <qjsonparseerror.h>
#include <qlocalsocket.h>
#include <qlogging.h>
#include <qobject.h>
#include <qstringview.h>

IPC::IPC(QObject* parent): QLocalSocket(parent), buffer() {
	connect(this, &QLocalSocket::readyRead, this, &IPC::handle_reply);
}

QByteArray const MAGIC = "i3-ipc";

void IPC::handle_reply() {
	buffer += readAll();
	size_t ptr = 0;


	while (true) {
		QByteArrayView view(buffer);
		view.slice(ptr);

		if (view.length() < MAGIC.size() + 8) {break;}

		if (view.sliced(0, MAGIC.size()) != MAGIC) {
			qWarning() << "Invalid magic string";
			buffer.clear();
			return;
		}
		view.slice(MAGIC.size());

		uint32_t len;
		std::memcpy(&len, view.data(), sizeof(uint32_t));
		view.slice(sizeof(uint32_t));

		uint32_t type;
		std::memcpy(&type, view.data(), sizeof(uint32_t));
		view.slice(sizeof(uint32_t));

		if (view.length() < len) {break;}

		auto excerpt = view.sliced(0, len).toByteArray();
		view.slice(len);

		ptr += (MAGIC.size() + 8 + len);

		QJsonParseError err;
		auto payload = QJsonDocument::fromJson(excerpt, &err);
		if (err.error != QJsonParseError::NoError) {
			qWarning() << "Failed to parse response JSON:" << err.errorString();
			continue;
		}
		emit recv(type, payload);

	}
	buffer.remove(0, ptr);
}

void IPC::send(SwayMsgType type, QByteArrayView data) {
	uint32_t len = data.length();
	uint32_t type_int = type;

	write(MAGIC);
	write((char*)&len, sizeof(uint32_t));
	write((char*)&type_int, sizeof(uint32_t));
	write(data.data(), data.length());
}
