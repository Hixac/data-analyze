cmake_minimum_required(VERSION 3.27)

project(DataAnalyze
        VERSION 0.1
		DESCRIPTION "SEEKING FOR PARIN"
		LANGUAGES CXX)

set(SOURCE
  src/main.cpp src/Base.h src/LoggingSystem.h
  src/window/window.h src/window/window.cpp
  src/extracter.h src/extracter.cpp
  src/lexer.h src/lexer.cpp
  src/parser.h src/parser.cpp
  src/dataunit.h src/dataunit.cpp
  src/imgui/fastcombo.h
  src/imgui/dynamictimer.h src/imgui/dynamictimer.cpp
  src/imgui/deltatime.h
  src/Application.h src/MainApplication.h
  src/shelling/mainshell.h
  src/shelling/parking.h src/shelling/parking.cpp
  src/shelling/example.h src/shelling/example.cpp
  src/shelling/info.h src/shelling/info.cpp
  src/shelling/graph.h src/shelling/graph.cpp
  src/shelling/graph.h src/shelling/graph.cpp
  src/shelling/funcs.h src/shelling/funcs.cpp
  src/utility/filedialog.cpp src/utility/filedialog.hpp
  src/utility/math_expr.hpp
  src/utility/windows_utils.hpp
  src/graphs/axis.h src/graphs/axis.cpp
)

set(EXPRTK_DIR ext/exprtk/)
set(IMPLOT_DIR ext/implot/)
set(IMGUI_DIR ext/imgui/)
set(SPDLOG_DIR ext/spdlog/)

set(EXPRTK_SRC
	${EXPRTK_DIR}exprtk.hpp)

set(IMPLOT_SRC
  ext/implot/implot.cpp
  ext/implot/implot_items.cpp
  ext/implot/implot.h
  ext/implot/implot_demo.cpp
  ext/implot/implot_internal.h)

set(IMGUI_SRC
  ${IMGUI_DIR}misc/cpp/imgui_stdlib.h
  ${IMGUI_DIR}misc/cpp/imgui_stdlib.cpp
  ${IMGUI_DIR}misc
  ${IMGUI_DIR}imstb_truetype.h
  ${IMGUI_DIR}imstb_textedit.h
  ${IMGUI_DIR}imstb_rectpack.h
  ${IMGUI_DIR}imgui_widgets.cpp
  ${IMGUI_DIR}imgui_tables.cpp
  ${IMGUI_DIR}imgui_internal.h
  ${IMGUI_DIR}imgui_impl_opengl3_loader.h
  ${IMGUI_DIR}imgui_impl_opengl3.h
  ${IMGUI_DIR}imgui_impl_opengl3.cpp
  ${IMGUI_DIR}imgui_impl_glfw.h
  ${IMGUI_DIR}imgui_impl_glfw.cpp
  ${IMGUI_DIR}imgui_draw.cpp
  ${IMGUI_DIR}imgui_demo.cpp
  ${IMGUI_DIR}imgui.h
  ${IMGUI_DIR}imgui.cpp
  ${IMGUI_DIR}imconfig.h)

set(SPDLOG_SRC
  ${SRDLOG_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -g)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (MSVC) # to compile exprtk
	add_compile_options(/bigobj)

	enable_language("RC")
	set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/res/app.rc)
else ()
  # add_compile_options(-Wa,-mbig-obj)
endif ()

FILE(COPY res/JetBrainsMonoNLNerdFontMono-Regular.ttf DESTINATION "${CMAKE_BINARY_DIR}")
FILE(COPY res/JetBrainsMonoNLNerdFontMono-Regular.ttf DESTINATION "${CMAKE_BINARY_DIR}/Debug")
add_executable(${PROJECT_NAME} ${WIN32_RESOURCES} ${SOURCE} ${IMPLOT_SRC} ${IMGUI_SRC} ${SPDLOG_SRC} ${EXPRTK_SRC})

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

target_include_directories(${PROJECT_NAME} PRIVATE deps/glfw/include)
add_subdirectory(deps/glfw)
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC nativefiledialog-extended/)
add_subdirectory(ext/nativefiledialog-extended/)  # файлы с библиотекой imgui
target_link_libraries(${PROJECT_NAME} nfd)

target_include_directories(${PROJECT_NAME} PRIVATE ${SPDLOG_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${IMPLOT_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${EXPRTK_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE src/)
target_include_directories(${PROJECT_NAME} PRIVATE src/utility/)
target_include_directories(${PROJECT_NAME} PRIVATE src/window/)
target_include_directories(${PROJECT_NAME} PRIVATE src/graphs/)
