# Make an automatic library - will be static or dynamic based on user setting
add_library(car SHARED car.cpp)
add_library(park SHARED park.cpp)

# We need this directory, and users of our library will need it too
target_include_directories(park PUBLIC ../include)
target_include_directories(car PUBLIC ../include)
