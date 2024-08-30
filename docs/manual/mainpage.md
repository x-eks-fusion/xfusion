<!--
    此 markdown 文件用做 doxygen 首页，可用 doxygen 命令。
 -->

# API 参考

@author XFusion Development Team.
@version 1.0.0
@date 2024-08-12

这里是 xfusion 的 API 参考文档。

## API 组织方式说明

components 内的 API 以组件为单位划分。

```
components(内部)/
┣ xf_hal/
┣ xf_heap/
┣ xf_init/
┣ xf_log/
┣ xf_task/
┣ xf_utils/
┗ xf_wal/            # TODO
```

每个组件中的 API 均划分为:

-   **用户接口**

    用户接口包含调用具体功能的接口。如初始化、配置、输入输出等。

-   **移植接口**

    移植接口是从具体平台的功能对接到 xfusion 内组件的接口。比如对接具体硬件驱动到 xf_hal 中的接口。

-   **内部接口**

    内部接口是组件内部实现某些功能时定义的接口。用户不一定需要。

## 模块文档

详情见侧边栏 `Topics`。

### 内部组件

@todo - 内部组件详情文档，如 `components/xf_init/xf_init/README.md`.

@todo - xf_wal 文档

### 对接情况

@todo - 内部组件详情文档，如 `ports/espressif/esp32`.
