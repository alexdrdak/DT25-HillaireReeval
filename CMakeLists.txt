cmake_minimum_required(VERSION 3.22)
project(dp_hillaire_reeval)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

set(COMMON_SOURCES
    "src/main.cpp"
    "src/shaders/d_Shader.cpp")
set(COMMON_LIBS
    glfw OpenGL::GL GLEW::GLEW glm::glm-header-only)

add_executable(${CMAKE_PROJECT_NAME} ${COMMON_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${COMMON_LIBS})


set(BATCH_FILE "${CMAKE_BINARY_DIR}/d_runtests.bat")
file(WRITE ${BATCH_FILE} "")

foreach(TYPE RANGE 0 2)
    foreach(VARIANT RANGE 1 3)
        set(EXEC_NAME "${CMAKE_PROJECT_NAME}-${TYPE}_${VARIANT}-hq")
        add_executable(${EXEC_NAME} ${COMMON_SOURCES})
        target_compile_definitions(${EXEC_NAME} PRIVATE D_VEC=${VARIANT} D_INSTANCED=${TYPE})
        target_link_libraries(${EXEC_NAME} PRIVATE ${COMMON_LIBS})

        file(APPEND ${BATCH_FILE} "start /wait ${CMAKE_PROJECT_NAME}-${TYPE}_${VARIANT}.exe\n")
    endforeach()
endforeach()
