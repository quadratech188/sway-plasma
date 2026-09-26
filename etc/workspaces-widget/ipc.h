#include <qbytearrayview.h>
#include <qjsondocument.h>
#include <qlocalsocket.h>
#include <qobject.h>
#include <qstringview.h>
#include <qtmetamacros.h>
#include <QJsonDocument>

#ifndef IPC_H
#define IPC_H

enum SwayMsgType {
	GET_WORKSPACES = 1,
	SUBSCRIBE = 2,
};

class IPC: public QLocalSocket {
	Q_OBJECT

public:
	IPC(QObject* parent = nullptr);
	void send(SwayMsgType type, QByteArrayView doc);

	void send(SwayMsgType type, QJsonDocument const& doc) {
		send(type, doc.toJson(QJsonDocument::JsonFormat::Compact));
	}
private:
	QByteArray buffer;
	void handle_reply();

signals:
	void recv(uint32_t type, QJsonDocument const& doc);
};

#endif

