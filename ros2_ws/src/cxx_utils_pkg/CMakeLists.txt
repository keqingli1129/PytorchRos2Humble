cmake_minimum_required(VERSION 3.8)
project(cxx_utils_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_ros REQUIRED)

# Find libtorch
find_package(Torch REQUIRED)

add_library(cxx_utils src/cxx_utils.cpp)
target_link_libraries(cxx_utils PUBLIC "${TORCH_LIBRARIES}")

set_property(TARGET cxx_utils PROPERTY CXX_STANDARD 17)
target_compile_features(cxx_utils PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
target_include_directories(cxx_utils PUBLIC ${TORCH_INCLUDE_DIRS})
target_include_directories(cxx_utils PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${TORCH_INCLUDE_DIRS} # Explicitly add Torch includes to the target's interface
)

# Causes the visibility macros to use dllexport rather than dllimport,
# which is appropriate when building the dll but not consuming it.
target_compile_definitions(cxx_utils PRIVATE "CXX_UTILS_PKG_BUILDING_LIBRARY")

install(
  DIRECTORY include/
  DESTINATION include
)
install(
  TARGETS cxx_utils
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# 5. Export for Downstream Packages
# ONLY use ament_export_targets. Remove ament_export_libraries/include_directories
ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)

ament_package()
