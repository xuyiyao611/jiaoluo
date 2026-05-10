# CornerMatchQt

这是 `角落消消` 的 Qt + C++ 项目入口。

推荐在 Qt Creator 中直接打开：

- `D:\CornerMatchQt\CMakeLists.txt`

说明：

- `D:\CornerMatchQt` 是指向当前工程目录的英文路径联接，便于 Qt Creator 与 MinGW 使用。
- 实际工程内容仍位于：
  - `D:\大作业2\消除大师\qt-cpp`

构建建议：

- Qt Kit: `Qt 6.11.0 MinGW 64-bit`
- 构建系统: `CMake + Ninja`

主要目录：

- `src/app`：应用状态与基础逻辑
- `src/ui`：Qt 界面
- `resources`：图片资源与 qrc
