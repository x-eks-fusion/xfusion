import xf_build
from xf_build import api
from pathlib import Path

example_components_path: Path = api.XF_ROOT / "examples" / "example_components"

example_components = []
example_components.append((example_components_path / "ex_easy_wifi").as_posix())

xf_build.project_init(user_dirs=example_components)
xf_build.program()