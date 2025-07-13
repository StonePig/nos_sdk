cmake_minimum_required (VERSION 3.5)

project (nos_prj)

MACRO(SOURCE_GROUP_BY_FOLDER target root_name all_file_list)        # 将源文件和头文件进行分组
SET(SOURCE_GROUP_DELIMITER "/")
SET(last_dir "")
SET(files "")
file(GLOB_RECURSE src_file ${all_file_list})
FOREACH(file ${src_file})          
    file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}/${target}" ${file})        
    GET_FILENAME_COMPONENT(dir "${relative_file}" PATH)
    IF (NOT "${dir}" STREQUAL "${last_dir}")
        IF (files)
            source_group("${root_name}/${last_dir}" FILES ${files})
            # message(STATUS "[SOURCE_GROUP]" ${last_dir} "   " ${files})
        ENDIF (files)
        SET(files "")
    ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
    SET(files ${files} ${file})
    SET(last_dir "${dir}")
ENDFOREACH(file)
IF (files)
    source_group("${root_name}/${last_dir}" FILES ${files})
    # message(STATUS "[SOURCE_GROUP]" ${last_dir} "   " ${files})
ENDIF (files)
ENDMACRO(SOURCE_GROUP_BY_FOLDER)

set(CMAKE_MFC_FLAG 2)

if(MSVC)
    add_compile_options("/utf-8")
    add_compile_options("/MP")
    set(CMAKE_CXX_STANDARD 14)
    # set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
endif()

set(REL_SDK "ON")

set(ROOT_DIR "../../..")


# 有且只有一个应用程序被启用
# LED_DEMO
# ECU_HW_TUNNING
# IEM_TUNNING
# MODBUS_DEMO
# WATCH
# STUDENT_CARD
# SMART_GADGET
set(APP "MODBUS_DEMO") # 设置默认应用程序

# ${APP}转成小写字符
string(TOLOWER "${APP}" APP_LOWER)
set(APP_DIR_ROOT_NAME "app/${APP_LOWER}")


# MODBUS_DEMO
if(APP STREQUAL "MODBUS_DEMO")
    set(MODBUS_ENABLE "ON")
endif()

# ECU_HW_TUNNING
if(APP STREQUAL "ECU_HW_TUNNING")
    set(LVGL_ENABLE "ON")
endif()

# WATCH
if(APP STREQUAL "WATCH")
    set(LVGL_ENABLE "ON")
    set(APP_DIR_ROOT_NAME "app/lvgl_app/${APP_LOWER}")
endif()

# LVGL_STUDENT_CARD
if(APP STREQUAL "STUDENT_CARD")
    set(LVGL_ENABLE "ON")
    set(APP_DIR_ROOT_NAME "app/lvgl_app/${APP_LOWER}")
endif()

# SMART_GADGET
if(APP STREQUAL "SMART_GADGET")
    set(LVGL_ENABLE "ON")
    set(APP_DIR_ROOT_NAME "app/lvgl_app/${APP_LOWER}")
endif()


set(APP_DIR "../../../${APP_DIR_ROOT_NAME}")
set(ALL_FILE_TYPE ${APP_DIR}/*.c ${APP_DIR}/*.h)
include_directories(${APP_DIR})
SOURCE_GROUP_BY_FOLDER(${APP_DIR} ${APP_DIR_ROOT_NAME} ${ALL_FILE_TYPE})
file(GLOB_RECURSE APP_LIST ${ALL_FILE_TYPE})

# LVGL_ENABLE
if(LVGL_ENABLE STREQUAL "ON")
    set(LVGL_ROOT_NAME "middle/external/lvgl/src")
    set(LVGL_DIR "../../../middle/external/lvgl/src")
    set(ALL_FILE_TYPE ${LVGL_DIR}/*.c ${LVGL_DIR}/*.h)
    include_directories(${LVGL_DIR})
    include_directories(${ROOT_DIR}/app/lvgl_app/porting)

    SOURCE_GROUP_BY_FOLDER(${LVGL_DIR} ${LVGL_ROOT_NAME} ${ALL_FILE_TYPE})
    file(GLOB_RECURSE LVGL_LIST_All ${ALL_FILE_TYPE})

    # porting
    aux_source_directory(${ROOT_DIR}/app/lvgl_app/porting LVGL_PORTING_LIST)
    source_group(app/lvgl_app/porting FILES ${LVGL_PORTING_LIST})
endif()


# FREERTOS_ENABLE
if(FREERTOS_ENABLE STREQUAL "ON")
    set(FREERTOS_ROOT_NAME "middle/external/FreeRTOS/Source")
    set(FREERTOS_DIR "../../../middle/external/FreeRTOS/Source")
    set(ALL_FILE_TYPE ${FREERTOS_DIR}/*.c ${FREERTOS_DIR}/*.h)
    include_directories(${FREERTOS_DIR}/include)
    include_directories(${FREERTOS_DIR}/portable)
    SOURCE_GROUP_BY_FOLDER(${FREERTOS_DIR} ${FREERTOS_ROOT_NAME} ${ALL_FILE_TYPE})
    file(GLOB_RECURSE FREERTOS_LIST_All ${ALL_FILE_TYPE})
endif()

# MODBUS_ENABLE
if(MODBUS_ENABLE STREQUAL "ON")
    aux_source_directory(${ROOT_DIR}/middle/external/modbus MODBUS_LIST)
    source_group(middle/external/modbus FILES ${MODBUS_LIST})
endif()

# 删除多个文件
file(REMOVE 
    ./ui.ini
    ${ROOT_DIR}/config.h)
# copy files
file(COPY ${APP_DIR}/ui.ini DESTINATION ./)
file(COPY ${APP_DIR}/config.h DESTINATION ${ROOT_DIR}/)


set(algorithm_LIST ${ROOT_DIR}/middle/external/algorithm/pid.c)
set(GENERAL_LIST ${ROOT_DIR}/middle/external/general/cqueue.c)
aux_source_directory(${ROOT_DIR}/middle MID_LIST)
aux_source_directory(${ROOT_DIR}/middle/external/easylogger ELOG_LIST)
aux_source_directory(${ROOT_DIR}/platform/win32/drv DRV_LIST)
aux_source_directory(${ROOT_DIR}/platform/win32/nos_simu WIN32_LIST)
aux_source_directory(${ROOT_DIR}/platform/win32/nos_simu/ChartCtrl CHARTCTRL_LIST)
file(GLOB WIN32_LIST_INC_FILES "${ROOT_DIR}/platform/win32/nos_simu/*.h")
set(WIN32_LIST ${WIN32_LIST})
set(WIN32_RC_LIST ${ROOT_DIR}/platform/win32/nos_simu/nossimu.rc ${ROOT_DIR}/platform/win32/nos_simu/nos_simu.cpp)

# 排除nos_simu.cpp文件
list(FILTER WIN32_LIST EXCLUDE REGEX ".*nos_simu\\.cpp$")

source_group(middle/external/easylogger FILES ${ELOG_LIST})
source_group(middle/external/algorithm FILES ${algorithm_LIST})
source_group(middle/external/general FILES ${GENERAL_LIST})
source_group(middle FILES ${MID_LIST})
source_group(drv FILES ${DRV_LIST})
source_group(win32 FILES ${WIN32_LIST})
source_group(win32/chartctrl FILES ${CHARTCTRL_LIST})

# 主程序包含所有业务代码和资源
add_executable(${APP} WIN32
    ${WIN32_RC_LIST}
    ${APP_LIST}
    ${ELOG_LIST}
    ${algorithm_LIST}
    ${GENERAL_LIST}
    ${MODBUS_LIST}
    ${LVGL_LIST_All}
    ${FREERTOS_LIST_All}
    ${KB2XV20_LIST_All}
    ${LVGL_PORTING_LIST}
)

target_link_libraries(${APP} PRIVATE Release/nos_sdk)

target_compile_definitions(${APP} PRIVATE
    __${APP}_ENABLE__
    $<$<STREQUAL:${LVGL_ENABLE},ON>:LV_CONF_INCLUDE_SIMPLE>
    $<$<STREQUAL:${LVGL_ENABLE},ON>:LV_LVGL_H_INCLUDE_SIMPLE>
    $<$<STREQUAL:${LVGL_ENABLE},ON>:LV_USE_DEV_VERSION>
    $<$<STREQUAL:${LVGL_ENABLE},ON>:LVGL_EN=1>
    $<$<NOT:$<STREQUAL:${LVGL_ENABLE},ON>>:LVGL_EN=0>
    $<$<STREQUAL:${FREERTOS_ENABLE},ON>:FREERTOS_EN=1>
    $<$<NOT:$<STREQUAL:${FREERTOS_ENABLE},ON>>:FREERTOS_EN=0>
    -D_CRT_SECURE_NO_WARNINGS -DWIN32 -D_WINDOWS -D_UNICODE -DUNICODE -D_AFXDLL
)
target_link_options(${APP} PRIVATE /ENTRY:wWinMainCRTStartup)

# 启动时不出现console  add_executable 增加 WIN32， add_link_options增加连接参数-mwindows
add_link_options(${APP} PRIVATE -mwindows)

# 设置启动项目
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP})


