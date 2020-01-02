# Works with 3.10.2
cmake_minimum_required(VERSION 3.10.2)

project (homework_client_park
        VERSION 0.1
        DESCRIPTION "Client per il simulatore di parcheggio per homework"
        LANGUAGES CXX)

# The compiled library code is here
add_subdirectory(libs)

# The executable code is here
add_subdirectory(src)
