add_gtest(SortedTuple_Concepts
  SOURCES concepts.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(SortedTuple_Core
  SOURCES core.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(SortedTuple_Ops
  SOURCES ops.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(SortedTuple_Qualifiers
  SOURCES qualifiers.cpp
  LIBRARIES
    project_coverage
    libkxi
)
