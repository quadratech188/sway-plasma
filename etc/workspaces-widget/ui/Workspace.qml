import QtQuick
import QtQuick.Layouts

import org.kde.ksvg as KSvg
import org.kde.plasma.components as Components

import plasma.applet.com.github.quadratech188.sway_workspaces as Sway

Item {
	id: root
	required property Sway.workspace modelData

	Layout.fillHeight: true
	Layout.preferredWidth: height

	HoverHandler {
		id: hoverHandler
	}

	KSvg.FrameSvgItem {
		id: frame
		anchors.fill: parent
		imagePath: "widgets/tasks"

		visible: root.modelData.focused
			|| root.modelData.urgent
			|| hoverHandler.hovered

		readonly property string base_prefix: root.modelData.urgent? "attention"
				: root.modelData.focused? "focus": "normal"

		prefix: hoverHandler.hovered? [
			"south-" + base_prefix + "-hover",
			base_prefix + "-hover",
	    		"south-hover",
			"hover",
			"south-" + base_prefix,
			base_prefix
		]: [
			"south-" + base_prefix,
			base_prefix
		]
	}
	Components.Label {
		anchors.centerIn: parent
		text: root.modelData.name
		font.pixelSize: 0.5 * root.height
	}
}
