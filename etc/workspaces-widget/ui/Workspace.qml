import QtQuick
import QtQuick.Layouts

import org.kde.ksvg as KSvg
import org.kde.plasma.components as Components
import org.kde.kirigami as Kirigami

import plasma.applet.com.github.quadratech188.sway_workspaces as Sway

Item {
	id: root
	required property Sway.workspace modelData
	required property Item workspaces_root

	implicitHeight: workspaces_root.height
	implicitWidth: label.implicitWidth
	Layout.minimumWidth: 0.8 * root.height

	HoverHandler {
		id: hoverHandler
	}

	TapHandler {
		onTapped: {
			SwayState.set_workspace(modelData.name)
		}
	}

	KSvg.FrameSvgItem {
		id: frame
		anchors.fill: parent
		imagePath: 'widgets/tasks'

		visible: root.modelData.focused
			|| root.modelData.urgent
			|| hoverHandler.hovered

		readonly property string base_prefix: root.modelData.urgent? 'attention'
				: root.modelData.focused? 'focus': 'normal'

		prefix: {
			function gen(p) {
				return [p, 'south-' + p]
			}
			hoverHandler.hovered
				? [...gen(base_prefix + '-hover'), ...gen('hover'), ...gen(base_prefix)]
				: gen(base_prefix)
		}
	}
	Components.Label {
		id: label
		leftPadding: Kirigami.Units.mediumSpacing
		rightPadding: Kirigami.Units.mediumSpacing

		anchors.centerIn: parent
		text: root.modelData.name
		font.pixelSize: 0.45 * root.height
	}
}
