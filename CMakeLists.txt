cmake_minimum_required(VERSION 3.10)
project(WeatherClient LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3rd party libs
find_package(unofficial-curlpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
add_executable(WeatherClient main.cpp)

target_link_libraries(WeatherClient PRIVATE
                      unofficial::curlpp::curlpp
                      nlohmann_json::nlohmann_json
                      cxxopts::cxxopts)

