add_gtest(FlatTuple_Core
  SOURCES core.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(FlatTuple_Concepts
  SOURCES concepts.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(FlatTuple_Constexpr
  SOURCES constexpr.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(FlatTuple_Layout
  SOURCES layout.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(FlatTuple_MoveCount
  SOURCES move_count.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(FlatTuple_Ops
  SOURCES ops.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(FlatTuple_Qualifiers
  SOURCES qualifiers.cpp
  LIBRARIES
    project_coverage
    libkxi
)
