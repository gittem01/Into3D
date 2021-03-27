set (IMGUI_HEADERS
    "${CMAKE_SOURCE_DIR}/external/imgui/include/imconfig.h"
    "${CMAKE_SOURCE_DIR}/external/imgui/include/imgui.h"
    "${CMAKE_SOURCE_DIR}/external/imgui/include/imgui_internal.h"
    "${CMAKE_SOURCE_DIR}/external/imgui/include/imstb_rectpack.h"
    "${CMAKE_SOURCE_DIR}/external/imgui/include/imstb_textedit.h"
    "${CMAKE_SOURCE_DIR}/external/imgui/include/imstb_truetype.h"
    PARENT_SCOPE
)

set (IMGUI_SOURCES
    "${CMAKE_SOURCE_DIR}/external/imgui/src/imgui.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/src/imgui_demo.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/src/imgui_draw.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/src/imgui_tables.cpp"
    "${CMAKE_SOURCE_DIR}/external/imgui/src/imgui_widgets.cpp"
    PARENT_SCOPE
)
