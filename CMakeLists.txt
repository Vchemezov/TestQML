cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(TestQML)

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Widgets REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Qml_INCLUDE_DIRS})

set(QTDIR ${Qt5Core_DIR}/../../../bin)

add_compile_definitions($<$<CONFIG:Release>:QT_NO_DEBUG>)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: Debug Release."
      FORCE)
ENDIF()

#Guid("94536FB3-8FA2-471E-BA89-19A5096408F1")
   set(TEST_GUID "94536FB38FA2471EBA8919A5096408F1")

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

set (QML_RESOURCES
        qml/OpenFileDialog.qml
        qml/mainWindow.qml
)

set(ALL_SRCS
    src/main.cpp
    src/reader.h
    src/visualiser.h
    src/error.h
    ${QML_RESOURCES}
)

qt5_wrap_cpp(MOC_HEADERS src/visualiser.h)
qt5_wrap_cpp(MOC_HEADERS src/error.h)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/qml
     DESTINATION ${PROJECT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${ALL_SRCS} ${MOC_HEADERS})

#if(UNIX)
#  if(CMAKE_SYSTEM_NAME MATCHES "Linux")
#    if(CMAKE_COMPILER_IS_GNUCXX OR CLANG)
#       get_target_property(QtCorePath Qt5::Core LOCATION)
#       get_filename_component(QtCoreLocation ${QtCorePath} PATH)
#       set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-rpath-link=${QtCoreLocation}")
#    endif()
#  endif()
#endif()

set(COMMON_LIBS Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} ${COMMON_LIBS} Qt5::WinMain)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS" AUTOMOC ON)
else()
    target_link_libraries(${PROJECT_NAME} ${COMMON_LIBS})
    set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)
endif()

# INSTALL part
get_filename_component(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/../${PROJECT_NAME}_install" ABSOLUTE)

INSTALL(
    TARGETS ${PROJECT_NAME}
    DESTINATION .
    BUNDLE DESTINATION . COMPONENT Runtime
)

set (TEST_RESOURCES test_data)

INSTALL(
    DIRECTORY ${TEST_RESOURCES}
    DESTINATION .
)

INSTALL(
    FILES ${QML_RESOURCES}
    DESTINATION ./qml
)



