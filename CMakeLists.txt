cmake_minimum_required(VERSION 3.13)

# Ustaw nazwę projektu.
project(NetSim)

# Ustaw standard C++ (tu: C++17).
set(CMAKE_CXX_STANDARD 17)

# Dodaj flagi kompilacji wymuszające pisanie kodu zgodnego ze standardem.
add_compile_options(-Wall -Wextra -Wpedantic -pedantic-errors)

# Dodaj podkatalog `googletest-master`.
add_subdirectory(googletest-master)

# Dodaj katalogi z plikami nagłówkowymi dla wszystkich konfiguracji.
include_directories(
        include
)

# Ustaw zmienną `SOURCE_FILES`, która będzie przechowywać ścieżki do
# tych plików źródłowych, które będą niezbędne w każdej konfiguracji.
set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
)

# Dodaj konfigurację typu `Debug`.
add_executable(NetSim_debug ${SOURCE_FILES} main.cpp)

# 1. Pokaż mainowi, gdzie są pliki .h z GoogleTest (żeby #include nie świecił na czerwono)
target_include_directories(NetSim_debug PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
)

# 2. Podłącz biblioteki do maina (żeby funkcje działały)
target_link_libraries(NetSim_debug gtest gmock)

# == Unit testing using Google Testing Framework ==

# Ustaw zmienną `SOURCES_FILES_TESTS`, która będzie przechowywać ścieżki do
# plików źródłowych z testami jednostkowymi.
set(SOURCES_FILES_TESTS
        test/tests.cpp

)

# Dodaj konfigurację typu `Test`.
add_executable(NetSim_test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} main.cpp)

# Dodaj katalogi z plikami nagłówkowymi wyłącznie do konkretnej konfiguracji (tu: `Test`).
target_include_directories(NetSim_test PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
)


# Podlinkuj bibliotekę o identyfikatorze `gmock` (w pliku CMake) wyłącznie do konkretnej
# konfiguracji (tu: `Test`).
target_link_libraries(NetSim_test gmock)