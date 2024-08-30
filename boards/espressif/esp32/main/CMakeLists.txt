file(GLOB SOURCES *.c)

idf_component_register(
    SRCS                ${SOURCES}
    INCLUDE_DIRS        "."
    WHOLE_ARCHIVE       # 确保自动初始化时能正确访问到相应段中的数据
    )
