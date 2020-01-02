set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(main main.cpp ../libs/car.cpp ../libs/sorter.cpp)

target_link_libraries(main PRIVATE carlib)
target_link_libraries(main PRIVATE parklib)
target_link_libraries(main PRIVATE sorterlib)
target_link_libraries(main PRIVATE datalib)
target_link_libraries(main PRIVATE serverlib)
target_link_libraries(main PRIVATE main_funclib)
target_link_libraries(main PRIVATE pthread)
