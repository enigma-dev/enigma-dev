set(NETWORKING_SOURCES
    BSip.cpp
    BSbrowser.cpp
    BSsocket.cpp
    BSnet.cpp
)

add_library("NETWORKING_${NETWORKING}" "${NETWORKING_SOURCES}")
target_link_libraries("NETWORKING_${NETWORKING}" dplayx dxguid ole32)
