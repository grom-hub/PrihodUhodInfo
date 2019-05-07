

cmake_minimum_required(VERSION 3.0)

project(PrihodUhodInfo VERSION 1.0 LANGUAGES C CXX)

add_executable(PrihodUhodInfo main.cpp curlConnector.cpp configParser.cpp)

target_link_libraries(PrihodUhodInfo curl config++)

# Установить свойству 'CXX_STANDARD' значение "11",
# а свойству 'CXX_STANDARD_REQUIRED' значение "ON":
set_target_properties(PrihodUhodInfo PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)

