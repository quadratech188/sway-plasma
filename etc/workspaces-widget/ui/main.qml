import QtQuick
import QtQuick.Layouts

import org.kde.plasma.plasmoid

import plasma.applet.com.github.quadratech188.sway_workspaces as Sway

PlasmoidItem {
	id: root
	
	Sway.SwayState {
		id: sway_state
	}
	
	RowLayout {
		anchors.fill: parent
	    	spacing: 0
	
	    	Repeater {
			model: sway_state.workspaces
			Workspace {outer_state: sway_state}
	    	}
	
	    	Item {
	    		Layout.fillWidth: true
	    	}
	}
}
