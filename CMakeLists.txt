cmake_minimum_required(VERSION 3.28)

add_library(MapGraphics STATIC
    CircleObject.cpp
    MapGraphicsObject.cpp
    MapGraphicsScene.cpp
    MapGraphicsView.cpp
    MapTileSource.cpp
    Position.cpp
    LineObject.cpp
    PolygonObject.cpp
    guts/Conversions.cpp
    guts/MapGraphicsNetwork.cpp
    guts/MapTileGraphicsObject.cpp
    guts/MapTileLayerListModel.cpp
    guts/MapTileSourceDelegate.cpp
    guts/CompositeTileSourceConfigurationWidget.cpp
    guts/PrivateQGraphicsInfoSource.cpp
    guts/PrivateQGraphicsObject.cpp
    guts/PrivateQGraphicsScene.cpp
    guts/PrivateQGraphicsView.cpp
    tileSources/CompositeTileSource.cpp
    tileSources/GridTileSource.cpp
    tileSources/OSMTileSource.cpp
    tileSources/LocalTileSource.cpp
    resources.qrc
)

target_include_directories(MapGraphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(MapGraphics PUBLIC
    Qt6::Widgets
    Qt6::Network
    Qt6::Sql
)

target_compile_definitions(MapGraphics PRIVATE
    MAPGRAPHICS_LIBRARY
    $<$<CONFIG:Release>:QT_NO_DEBUG_OUTPUT>
)

set_target_properties(MapGraphics PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}
)
