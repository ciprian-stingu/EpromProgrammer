project(EpromProgrammer C CXX)
cmake_minimum_required(VERSION 2.8)

#required packages
find_package(PkgConfig REQUIRED)

#config
#set(CMAKE_BUILD_TYPE Release CACHE STRING "Default release build." FORCE)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Default debug build." FORCE)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -W -O3 -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -W -O0 -pedantic")

#common files
set(COMMON_CPP_FILES "")
list(APPEND COMMON_CPP_FILES "src/eprom.cpp")
list(APPEND COMMON_CPP_FILES "src/ArduinoIO.cpp")

#QT5 Application
set(COMPILE_QT_APPLICATION ON)
if(COMPILE_QT_APPLICATION)
    set(QT_APPLICATION_NAME "EpQt")
    set(RECREATE_QT_ICONS OFF)
endif()

if(WIN32)
    
else()    
    if(COMPILE_QT_APPLICATION)
        find_package(Qt5 COMPONENTS Core Widgets SerialPort REQUIRED)
        SET(CMAKE_AUTOMOC ON)
        #SET(CMAKE_AUTOUIC ON)
        #SET(CMAKE_AUTORCC ON)

        if(RECREATE_QT_ICONS)
            add_custom_command (
                OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/QtIcons.h
                COMMAND $(CMAKE_COMMAND) -E echo "#ifndef QT_ICONS_H" > ${CMAKE_CURRENT_SOURCE_DIR}/src/QtIcons.h
                COMMAND $(CMAKE_COMMAND) -E echo "#define QT_ICONS_H" >> ${CMAKE_CURRENT_SOURCE_DIR}/src/QtIcons.h
                COMMAND convert ${CMAKE_CURRENT_SOURCE_DIR}/icons/guiIcon.png ${CMAKE_CURRENT_SOURCE_DIR}/icons/guiIcon.xpm
                COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/icons/guiIcon.xpm >> ${CMAKE_CURRENT_SOURCE_DIR}/src/QtIcons.h    
                COMMAND $(CMAKE_COMMAND) -E echo "#endif" >> ${CMAKE_CURRENT_SOURCE_DIR}/src/QtIcons.h
                COMMAND rm -f ${CMAKE_CURRENT_SOURCE_DIR}/icons/guiIcon.xpm
                COMMAND sed -i "s/static char/const char/g" ${CMAKE_CURRENT_SOURCE_DIR}/src/QtIcons.h
                VERBATIM
            )
        endif()
        if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
            add_executable(${QT_APPLICATION_NAME} main.cpp src/QtIcons.h src/QtGui.cpp ${COMMON_CPP_FILES})
            target_link_libraries(${QT_APPLICATION_NAME} Qt5::Widgets Qt5::SerialPort)
        endif()
        
        set_target_properties(${QT_APPLICATION_NAME} PROPERTIES COMPILE_FLAGS "-DQT_APPLICATION -DAPP_NAME=${QT_APPLICATION_NAME}")
        install(TARGETS ${QT_APPLICATION_NAME} RUNTIME DESTINATION bin)
	endif()
endif()
