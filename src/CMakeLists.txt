set(CHIP_SRC
	chip8.cpp
    main.cpp)

add_executable(${PROJECT_NAME} ${CHIP_SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)