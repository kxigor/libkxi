add_library(libkxi INTERFACE)
add_library(kxi::libkxi ALIAS libkxi)

target_include_directories(libkxi INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_compile_features(libkxi INTERFACE cxx_std_23)
