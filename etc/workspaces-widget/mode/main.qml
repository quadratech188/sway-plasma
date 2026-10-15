import QtQuick
import QtQuick.Layouts

import org.kde.plasma.plasmoid
import org.kde.plasma.components as Components
import org.kde.kirigami as Kirigami

import com.github.quadratech188.swaystate as Sway

pragma ComponentBehavior: Bound

PlasmoidItem {
	id: root

	readonly property bool active: Sway.SwayState.mode !== "default"

	implicitWidth: active? indicator.implicitWidth: Kirigami.Units.largeSpacing
	Layout.preferredWidth: implicitWidth

	Components.Label {
		id: indicator
		visible: root.active

		anchors.fill: parent

		text: Sway.SwayState.mode
		font.pixelSize: 0.45 * root.height

		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter

		leftPadding: Kirigami.Units.mediumSpacing
		rightPadding: Kirigami.Units.mediumSpacing
	}
}
