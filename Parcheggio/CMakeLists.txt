# Works with 3.10.2
cmake_minimum_required(VERSION 3.10.2)

project (homework_park
        VERSION 0.1
        DESCRIPTION "Simulatore di parcheggio per homework"
        LANGUAGES CXX)

# The compiled library code is here
add_subdirectory(libs)

# The executable code is here
add_subdirectory(src)
