ecm_add_qml_module(
	swaystate
	URI "com.github.quadratech188.swaystate"
	GENERATE_PLUGIN_SOURCE
)

target_sources(swaystate PRIVATE
    state.h
    state.cpp
    ipc.h
    ipc.cpp
)

ecm_finalize_qml_module(swaystate DESTINATION ${KDE_INSTALL_QMLDIR})
