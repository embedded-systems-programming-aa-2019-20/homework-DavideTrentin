add_executable(client data_reader.cpp)
add_definitions(-Wall -Wextra)

target_link_libraries(client PRIVATE clientlib)
