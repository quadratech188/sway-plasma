import QtQuick

import org.kde.kirigami as Kirigami
import org.kde.ksvg as KSvg
import org.kde.plasma.components as Components
import org.kde.plasma.core as PlasmaCore
import org.kde.plasma.plasmoid

import com.github.quadratech188.swaystate as Sway

Components.Button {
	id: root
	required property Sway.workspace modelData

	implicitWidth: Math.max(0.8 * height, contentItem.implicitWidth)

	onClicked: Sway.SwayState.set_workspace(modelData.name)

	contentItem: Components.Label {
		text: root.modelData.name
		font.pixelSize: 0.45 * root.height

		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter

		leftPadding: Kirigami.Units.mediumSpacing
		rightPadding: Kirigami.Units.mediumSpacing
	}

	background: KSvg.FrameSvgItem {
		anchors.fill: parent
		imagePath: 'widgets/tasks'

		visible: root.modelData.focused
			|| root.modelData.urgent
			|| root.hovered

		readonly property string base_prefix: root.modelData.urgent? 'attention'
				: root.modelData.focused? 'focus': 'normal'

		readonly property string location_prefix: {
			let result
			switch (Plasmoid.location) {
			case PlasmaCore.Types.LeftEdge:
				result = 'west'
				break
			case PlasmaCore.Types.TopEdge:
				result = 'north'
				break
			case PlasmaCore.Types.RightEdge:
				result = 'east'
				break
			default:
				result = 'south'
				break
			}
			result
		}

		prefix: {
			function gen(p) {
				return [location_prefix + '-' + p, p]
			}
			root.hovered
				? [...gen(base_prefix + '-hover'), ...gen('hover'), ...gen(base_prefix)]
				: gen(base_prefix)
		}
	}
}
