set(PROJECT boolcalc)

project(${PROJECT})

set(SOURCES
    Expr.cpp
    Const.cpp
    Var.cpp
    BDD.cpp
    BoolCalc.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/)

add_library(${PROJECT} ${SOURCES}) 
