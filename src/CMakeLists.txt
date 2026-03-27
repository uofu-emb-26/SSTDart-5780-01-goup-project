add_executable(sstdart
    main.c
)

set_target_properties(sstdart PROPERTIES ADDITIONAL_CLEAN_FILES sstdart.map)

target_link_libraries(sstdart
    STM32_Drivers
)

flash_target(sstdart)
