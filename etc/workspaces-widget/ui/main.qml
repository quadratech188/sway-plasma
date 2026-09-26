import QtQuick
import QtQuick.Layouts

import org.kde.plasma.plasmoid

import plasma.applet.com.github.quadratech188.sway_workspaces as Sway

PlasmoidItem {
	id: root
	
	Sway.SwayState {
		id: state
	}
	
	RowLayout {
		anchors.fill: parent
	    	spacing: 0
	
	    	Repeater {
	    		model: state.workspaces
	    	    	Workspace {}
	    	}
	
	    	Item {
	    		Layout.fillWidth: true
	    	}
	}
}
