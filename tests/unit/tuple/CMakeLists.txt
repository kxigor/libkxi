add_gtest(TupleConcepts_Concepts
  SOURCES concepts.cpp
  LIBRARIES 
    project_coverage
    libkxi
)

add_gtest(TupleConcepts_Core
  SOURCES core.cpp
  LIBRARIES 
    project_coverage
    libkxi
)

add_gtest(TupleConcepts_MoveCount
  SOURCES move_count.cpp
  LIBRARIES 
    project_coverage
    libkxi
)

add_gtest(TupleConcepts_Ops
  SOURCES ops.cpp
  LIBRARIES 
    project_coverage
    libkxi
)

add_gtest(TupleConcepts_Qualifiers
  SOURCES qualifiers.cpp
  LIBRARIES 
    project_coverage
    libkxi
)
