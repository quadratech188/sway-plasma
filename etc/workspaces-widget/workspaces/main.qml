import QtQuick
import QtQuick.Layouts
import QtQuick.Window

import org.kde.plasma.plasmoid
import org.kde.kirigami as Kirigami

import com.github.quadratech188.swaystate as Sway

PlasmoidItem {
	id: root
	Plasmoid.constraintHints: Plasmoid.CanFillArea

	ListView {
		orientation: ListView.Horizontal
		implicitWidth: contentWidth
		height: root.height

		reuseItems: true

		model: Sway.SwayState.workspaces

		delegate: Workspace {
			visible: modelData.output === root.Screen.name
			height: root.height
		}
	}
}
