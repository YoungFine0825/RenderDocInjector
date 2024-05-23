# RenderDocInjector

Renderdoc.dll injector based on SeanPesce/d3d11-wrapper

    基于[SeanPesce/d3d11-wrapper: Unfinished, experimental d3d11.dll wrapper for intercepting DirectX 11 function calls. (github.com)](https://github.com/SeanPesce/d3d11-wrapper)实现的Renderdoc.dll 注入器，用于mumu模拟器抓帧使用。

## Build

        使用Visual Studio 2022打开d3d11_wrapper/d3d11_wrapper.sln，在解决方案资源管理器窗口中，右键点击"d3d11_wrapper"项目，选择“生成”。完成后在d3d11_wrapper目录下的Debug或Release目录内可以找到生成的"d3d11.dll"。

## Usage

        以MuMu模拟器为例：将生成的d3d11.dll和renderdoc.dll（在RenderDoc安装目录下）拷贝至MuMu模拟器安装目录中的device文件夹内（比如：C:\Program Files\Netease\MuMuPlayer-12.0\device）。完成后运行模拟器，打开renderdoc，attach到进程上（点击：File->Attach to Running Instance）， 之后可以在Launch Application窗口中Capture了 。

        PC端游戏也是类似的操作，将两个dll拷贝至游戏可执行文件（.exe）同级目录（需要注意区分启动器.exe文件和实际的游戏exe文件）。但PC端防御机制比较完善，未必能成功。



## Reference

[RenderDoc 学习使用（一）：Windows平台Android模拟器环境下对特殊项目进行截帧分析 - VestLee - 博客园 (cnblogs.com)](https://www.cnblogs.com/vestlee/p/17003036.html)
