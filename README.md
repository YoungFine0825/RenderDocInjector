# RenderDocInjector

Renderdoc.dll injector based on SeanPesce/d3d11-wrapper



基于[SeanPesce/d3d11-wrapper: Unfinished, experimental d3d11.dll wrapper for intercepting DirectX 11 function calls. (github.com)](https://github.com/SeanPesce/d3d11-wrapper)实现的Renderdoc.dll 注入器，用于mumu模拟器抓帧使用。



## Build

使用Visual Studio 2022打开d3d11_wrapper/d3d11_wrapper.sln，生成项目。



## Usage

将生成的d3d11.dll和renderdoc.dll（在RenderDoc安装目录下）拷贝至模拟器可执行文件同级目录下，以MuMu模拟器为例：将dll拷贝至E:\Program Files\Netease\MuMuPlayer-12.0\device目录下。完成后运行模拟器，打开renderdoc，attach到进程上，点击：File->Attach to Running Instance 之后可以在界面中Capture了 。
