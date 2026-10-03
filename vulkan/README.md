# Vulkan模式下从MuMu模拟器中截帧的方法

让 **RenderDoc 能 attach 到 MuMu 模拟器的 Vulkan 渲染进程**，从而抓取渲染帧。

## ⚠️ 用之前先把路径改成你自己的

仓库里的脚本写死了下面两条默认路径，**都是原作者机器上的位置**，直接跑会失败：

| # | 要改什么 | 在哪儿改 | 默认值 |
|---|---|---|---|
| 1 | **RenderDoc 安装路径** | `tools\gen_manifest.py` 顶部两个常量：`RENDERDOC_JSON`、`RENDERDOC_DLL` | `D:\Env\RenderDoc\renderdoc.json` / `D:\Env\RenderDoc\renderdoc.dll` |
| 2 | **MuMu 模拟器路径** | 仓库根 `README.md` 里 d3d11 注入器那条流程（本 vulkan 方案**不需要** MuMu 路径，见下） | `C:\Program Files\Netease\MuMuPlayer-12.0\device` |

改完第 1 条后**必须重跑脚本并重新注册** —— 因为生成的 manifest 里 `library_path`
是绝对路径，不重跑就还指着旧位置：

```
python tools\gen_manifest.py
deploy\register-layer.bat
```

> **为什么本 vulkan 方案不用改 MuMu 路径**：它只往注册表写一条
> `HKCU\SOFTWARE\Khronos\Vulkan\ImplicitLayers`，由 Vulkan loader 全局读取，
> 跟 MuMu 装在哪个目录无关。
>
> 另外注意：默认路径里 `D:\Env\RenderDoc\` 只是 RenderDoc 常见的安装位置，不是必须的
> —— 以你自己的实际安装目录为准。

## 前置要求（Requirements）

| 项 | 要求 | 说明 |
|---|---|---|
| 操作系统 | Windows 10 / 11 **x64** | 本方案依赖 Windows 上的 Vulkan loader 注册表机制 |
| Vulkan loader | `%SystemRoot%\System32\vulkan-1.dll` 必须存在（其 Product 名为 `Vulkan Runtime`） | **不是 Vulkan SDK**：这个 loader 通常随**显卡驱动**安装，Windows 自身不附带。没有它，MuMu 的 Vulkan 模式本身就起不来 |
| RenderDoc | **已安装**（实测 v1.31） | 默认路径 `D:\Env\RenderDoc\`。需要其中的 `renderdoc.dll`（64 位那份），且**版本必须与 qrenderdoc.exe 一致**（用 `(Get-Item 文件).VersionInfo` 对比） |
| MuMu 模拟器 | MuMu 12，渲染模式必须是 **Vulkan（高性能）** | DirectX / 稳定模式 = ANGLE 的 D3D11 后端，最多 GLES 3.1，很多游戏会直接崩，本方案在该模式下也不生效 |
| 权限 | **无需管理员** | 只写 `HKCU`，不改机器级设置 |

**Vulkan SDK 不需要**：本方案不编译任何代码。

## 用（三步）

```
deploy\register-layer.bat      :: 1. 注册（HKCU，无需管理员）
                               :: 2. 完全退出并重启 MuMu，渲染模式保持 Vulkan（高性能）
                               :: 3. 起游戏 → qrenderdoc → File → Attach to Running Instance
```

⚠️ 撤销：

```
deploy\unregister-layer.bat
```

⚠️ 抓帧分析完记得撤销 —— 注册期间**你这个用户跑的所有 Vulkan 程序都会加载 RenderDoc's layer**。

## 原理一句话

RenderDoc 的 Vulkan 支持是个 **layer**，必须由 Vulkan loader 自己加载；它默认被
`enable_environment: ENABLE_VULKAN_RENDERDOC_CAPTURE=1` 关着。而 MuMu 的渲染进程
（`MuMuVMMHeadless.exe`）由服务链启动，继承的是**服务环境**，用户设的环境变量传不进去，
DLL 代理劫持 `vulkan-1.dll` 也实测无效。于是唯一可行的办法就是：**注册一份"去掉门槛"的
用户级（HKCU）隐式 layer**，让 loader 自己把它挂上。

## 目录

| 路径 | 说明 |
|---|---|
| `deploy\renderdoc-layer-ungated.json` | 去门槛的 layer manifest（由 gen_manifest.py 生成） |
| `deploy\register-layer.bat` | 注册到 `HKCU\SOFTWARE\Khronos\Vulkan\ImplicitLayers` |
| `deploy\unregister-layer.bat` | 撤销（删那一条注册表值） |
| `tools\gen_manifest.py` | 从 RenderDoc 自带的 `renderdoc.json` 重新生成 manifest（**顶部路径常量按你的安装位置改**） |

## 验证装没装成功

注册后，`MuMuVMMHeadless.exe` 的模块表里应出现 `D:\Env\RenderDoc\renderdoc.dll`：

```powershell
Get-Process MuMuVMMHeadless | %{ $_.Modules | ? ModuleName -match renderdoc }
```

注意：**不要**用 `vkEnumerateInstanceLayerProperties` 判断 —— 被门槛关掉的 layer
照样会出现在那个列表里（实测），是无效判据。详见原理 §7。
