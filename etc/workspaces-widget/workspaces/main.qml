import QtQuick
import QtQuick.Layouts
import QtQuick.Window

import org.kde.plasma.plasmoid
import org.kde.kirigami as Kirigami

import com.github.quadratech188.swaystate as Sway

PlasmoidItem {
	id: root
	Plasmoid.constraintHints: Plasmoid.CanFillArea
	implicitWidth: view.contentWidth
	Layout.preferredWidth: implicitWidth

	ListView {
		id: view

		anchors.fill: parent
		orientation: ListView.Horizontal

		reuseItems: true

		model: Sway.SwayState.workspaces

		delegate: Workspace {
			visible: modelData.output === root.Screen.name
			height: root.height
		}
	}
}
