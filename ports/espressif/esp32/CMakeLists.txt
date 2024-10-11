file(GLOB SOURCES *.c)

idf_component_register(
    SRCS                ${SOURCES}
    INCLUDE_DIRS        "."
    REQUIRES            "xf_log"
                        "xf_task"
                        "xf_utils"
                        "xf_init"
                        "xf_hal"
                        "xf_wal"
                        "xf_sys"
                        "esp_timer"
                        "esp_adc"
                        "driver"
                        "nvs_flash"
                        "esp_wifi"
    LDFRAGMENTS         "xf_linker.esp.lf"   # xfusion 自动初始化链接脚本
    WHOLE_ARCHIVE       # 确保自动初始化时能正确访问到相应段中的数据
)
