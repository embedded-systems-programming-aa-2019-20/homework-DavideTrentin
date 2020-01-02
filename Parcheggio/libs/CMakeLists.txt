# Make an automatic library - will be static or dynamic based on user setting
add_library(carlib SHARED car.cpp)
add_library(parklib SHARED park.cpp)
add_library(sorterlib SHARED sorter.cpp)
add_library(datalib SHARED data.cpp)
add_library(serverlib SHARED server.cpp)
add_library(main_funclib SHARED main_functions.cpp)

# We need this directory, and users of our library will need it too
target_include_directories(carlib PUBLIC ../include)
target_include_directories(parklib PUBLIC ../include)
target_include_directories(sorterlib PUBLIC ../include)
target_include_directories(datalib PUBLIC ../include)
target_include_directories(serverlib PUBLIC ../include)
target_include_directories(main_funclib PUBLIC ../include)
