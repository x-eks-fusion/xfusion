# xf_vfs_littlefs

此仓库修改自 `joltwallet/esp_littlefs`，来源可在同级目录下 `NOTICE` 找到。

此仓库移除了 esp-idf 相关依赖，使用 xf_utils、xf_fal、xf_vfs、xf_osal 作为支持。

xf_vfs_littlefs.c 中使用了 xf_osal 信号量与互斥锁，意味着如果未对接 xf_osal 将无法直接使用此仓库。

## 使用示例

```c
#include "xf_vfs_littlefs.h"

#define LITTLEFS_PARTITION_NAME "littlefs_part"

/* 注册你想使用的分区所在的分区表 */
xf_fal_register_partition_table(partition_table, ARRAY_SIZE(partition_table));
/* 初始化 xf_fal */
xf_fal_init();

/* 配置并注册 littlefs 到 xf_vfs */
xf_vfs_littlefs_conf_t conf = {
    .base_path          = "/littlefs",
    .partition_label    = LITTLEFS_PARTITION_NAME,
    .format_if_mount_failed = true,
    .dont_mount             = false,
};
xf_vfs_littlefs_register(&conf);

/* 使用 xf_vfs 接口操作文件 */
int fd = xf_vfs_open(
    "/littlefs/hello.txt", 
    XF_VFS_O_WRONLY | XF_VFS_O_CREAT | XF_VFS_O_TRUNC,
    XF_VFS_S_IRUSR | XF_VFS_S_IWUSR);
if (fd < 0) {
    return;
}
const char *content = "LittleFS Rocks!\n";
if (xf_vfs_write(fd, content, strlen(content)) < 0) {
    XF_LOGE(TAG, "Failed to write to file");
    xf_vfs_close(fd);
    return;
}
xf_vfs_close(fd);
```

## 注意

### 关于 littlefs 源码

此目录仅用于 xf_vfs 对接 littlefs，不含 littlefs 源码，需要使用以下命令克隆 littlefs 源码仓库到当前文件夹。

```bash
git clone --recursive https://github.com/littlefs-project/littlefs.git -b v2.9.1
```

### 关于版权

版权以及来源已在同级目录下 `NOTICE` 中声明。

原始许可证位于同级目录下 `LICENSE`。
