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
			model: SwayState.workspaces.filter(x => x.output == Screen.name)
			Workspace {}
	    	}
	
	    	Item {
	    		Layout.fillWidth: true
	    	}
	}
}
