# Weather Client (C++)

A simple C++ application that uses `curlpp` (a C++ wrapper for `libcurl`) to fetch weather data from an API.
This is simply for studying purpose.

---

## 🚀 Installation Steps

### 1. Install [vcpkg](https://github.com/microsoft/vcpkg)

Clone and bootstrap `vcpkg`:

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

#### Add vcpkg to your environment (optional but recommended):

```bash
echo 'export VCPKG_ROOT=$HOME/vcpkg' >> ~/.zshrc
echo 'export PATH=$VCPKG_ROOT:$PATH' >> ~/.zshrc
source ~/.zshrc
```

### 2. Initialize vcpkg manifest mode
In your project directory:
```bash
vcpkg new --application
```

#### Edit the generated vcpkg.json:
```json
{
  "name": "weather-client",
  "version-string": "0.1.0",
  "dependencies": [
    "curlpp"
  ]
}

```

#### To install the dependency
```bash
vcpkg install
```

### 3. Setup CMakePresets.json
Create a CMakePresets.json file at your project root:

```json
{
  "version": 2,
  "configurePresets": [
    {
      "name": "vcpkg",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      }
    }
  ]
}
```
1. Write your CMakeLists.txt
```bash
cmake_minimum_required(VERSION 3.10)
project(WeatherClient LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Find curlpp
find_package(unofficial-curlpp CONFIG REQUIRED)

add_executable(WeatherClient main.cpp)

# Link curlpp
target_link_libraries(WeatherClient PRIVATE unofficial::curlpp::curlpp)
```

### 5. Lastly, Configure and Build
```bash
cmake --preset=vcpkg
cmake --build build
# The compiled binary will be located at: ./build/WeatherClient
```

🧪 Test
Run the binary (replace main.cpp with your actual logic):

```bash
./build/WeatherClient
```
