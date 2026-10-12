import QtQuick
import QtQuick.Layouts
import QtQuick.Window

import org.kde.plasma.plasmoid

import plasma.applet.com.github.quadratech188.sway_workspaces as Sway

PlasmoidItem {
	id: root
	
	RowLayout {
		anchors.fill: parent
	    	spacing: 0
	
	    	Repeater {
			// Keep the model stable when the panel moves between screens.
			model: SwayState.workspaces
			Workspace {
				visible: modelData.output === root.Screen.name
			}
	    	}
	
	    	Item {
	    		Layout.fillWidth: true
	    	}
	}
}
