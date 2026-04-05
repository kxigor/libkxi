add_gtest(UnitTypeList_Concepts
  SOURCES concepts.cpp
  LIBRARIES 
    project_coverage
    libkxi
)

add_gtest(UnitTypeList_Core
  SOURCES core.cpp
  LIBRARIES 
    project_coverage
    libkxi
)

add_gtest(UnitTypeList_TupleOps
  SOURCES tuple_ops.cpp
  LIBRARIES 
    project_coverage
    libkxi
)

add_gtest(UnitTypeList_Sort
  SOURCES sort.cpp
  LIBRARIES 
    project_coverage
    libkxi
)
