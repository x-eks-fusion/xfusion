# 关于 example_components

`example_components` 是基于 xfusion api 且更偏向应用的组件。

相较于 `components`，`example_components` 不对通用性做严格要求，在 xfusion 或者外部组件库没有相关功能的情况下，允许通过判断平台包含平台相关代码，或者是实验性相关代码。

`example_components` 通过验证的代码在添加各平台支持后可以加入系统组件 `components`。

## 使用方法

如 `examples/protocols/http_request/xf_project.py` 所示：

```python
import xf_build
from xf_build import api
from pathlib import Path

example_components_path: Path = api.XF_ROOT / "examples" / "example_components_path"

example_components = []
example_components.append((example_components_path / "ex_easy_wifi").as_posix())

xf_build.project_init(user_dirs=example_components)
xf_build.program()
```

通过以上工程脚本实现了添加名为 `ex_easy_wifi` 的组件。

注意，如果添加的组件依赖于另外一个示例组件，也需要手动加入。

## 备注

### 允许通过判断平台包含平台相关代码的示例

在 xf_sys 不存在时，如果示例代码需要用到时间戳，因此创建一个示例中临时使用的时间戳组件如下：

```
📦ex_timestamp
 ┣ 📜ex_timestamp.h
 ┣ 📜ex_timestamp_esp32.c
 ┣ 📜ex_timestamp_ws63.c
 ┗ 📜xf_collect.py
```

其中 `xf_collect.py` 的代码如下：

```python
import xf_build

srcs = []
incs = ["."]
reqs = ["xf_utils"]

platform_esp32  = xf_build.get_define("PLATFORM_ESP32")
platform_ws63   = xf_build.get_define("PLATFORM_WS63")
if platform_esp32 == "y":
    srcs.append("ex_timestamp/ex_timestamp_esp32.c")
    reqs.append("lwip")
    reqs.append("esp_timer")
elif platform_ws63 == "y":
    srcs.append("ex_timestamp/ex_timestamp_ws63.c")

xf_build.collect(srcs=srcs, inc_dirs=incs, requires=reqs)
```
