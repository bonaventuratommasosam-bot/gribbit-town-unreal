# Toolchain setup (Gribbits Town)

The C++ module requires a working build toolchain. The machine currently has
**Visual Studio 2026 Insiders (IDE only)** — it does NOT include the C++ workload
or the Windows SDK, so Unreal Engine 5.5 cannot build for Win64 ("Win64 is not a
valid platform to build").

## What to install

1. **Visual Studio 2022** (Community/Professional/Enterprise) — NOT 2026.
   UE 5.5 targets the VS 2022 (v143) toolset. 2026 is not in UE's supported list.
   - During install pick: **Desktop development with C++**
   - Individual components: MSVC v143 build tools, Windows 10/11 SDK
     (10.0.19041 or later), C++ CMake tools, .NET 6/8 runtime.

2. **Windows SDK** (10.0.19041.0 or newer) — included in the workload above, or
   install separately from the Windows SDK archive.

3. **.NET 8 / .NET 6** runtime (UE's build scripts use it).

## Make UE 5.5 accept the toolchain

If you only have VS 2022 and the build still complains, tell UE where it is.
Create `Documents/Unreal Engine/UnrealBuildTool/BuildConfiguration.xml`:

```xml
<?xml version="1.0" encoding="utf-8" ?>
<Configuration xmlns="https://www.unrealengine.com/BuildConfiguration">
  <WindowsPlatform>
    <Compiler>VisualStudio2022</Compiler>
    <CompilerVersion>14.3x</CompilerVersion>
  </WindowsPlatform>
</Configuration>
```

## Verify

Open a **x64 Native Tools Command Prompt for VS 2022** and run:

```
cl
```

If `cl` prints version info, the toolchain is present. Then follow `BUILD_NOW.md`.
