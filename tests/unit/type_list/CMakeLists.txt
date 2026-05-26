add_gtest(TypeList_Core
  SOURCES core.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(TypeList_Concepts
  SOURCES concepts.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(TypeList_Ops
  SOURCES ops.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(TypeList_Pack
  SOURCES pack.cpp
  LIBRARIES
    project_coverage
    libkxi
)

add_gtest(TypeList_Sort
  SOURCES sort.cpp
  LIBRARIES
    project_coverage
    libkxi
)
