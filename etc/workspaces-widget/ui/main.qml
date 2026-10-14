import QtQuick
import QtQuick.Layouts
import QtQuick.Window

import org.kde.plasma.plasmoid
import org.kde.kirigami as Kirigami

import plasma.applet.com.github.quadratech188.sway_workspaces as Sway

PlasmoidItem {
	id: root
	Plasmoid.constraintHints: Plasmoid.CanFillArea
	
	RowLayout {
		id: layout
	    	spacing: 0
	
	    	Repeater {
			// HoverHandlers can trigger before the geometry is recalculated.
			// They seem to start out at the beginning of the layout, so
			// workspaces with long names can overlap other workspaces and be briefly highlighted

			// Force repositioning.
			onItemAdded: {
				layout.ensurePolished()
			}
			model: SwayState.workspaces
			Workspace {
				visible: modelData.output === root.Screen.name
				workspaces_root: root
			}
	    	}
	}
}
