file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.hpp)

init_target(list_vs_vector)

build_test (${TARGET_NAME} ${SOURCE_FILES} ${HEADER_FILES})

link_gtest()
link_benchmark()
final_target()

set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "test")