add_executable(client data_reader.cpp)

target_link_libraries(client PRIVATE clientlib)
