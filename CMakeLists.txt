cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(MidiForwarding)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Buscar todos los archivos fuente (.cpp) en el directorio actual
file(GLOB SOURCES *.cpp)

# Incluir directorios de encabezados (si tienes headers en subdirectorios)
# include_directories(${CMAKE_SOURCE_DIR})

# Agregar el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES})

# Vincular bibliotecas externas (como RtMidi y pthread)
find_package(PkgConfig REQUIRED)
pkg_check_modules(RTMIDI REQUIRED rtmidi)

target_link_libraries(${PROJECT_NAME} ${RTMIDI_LIBRARIES} pthread)
target_include_directories(${PROJECT_NAME} PRIVATE ${RTMIDI_INCLUDE_DIRS})
