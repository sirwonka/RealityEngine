# RealityEngine 

**A bare-metal, dependency-free C++ 3D modeling engine.**

Most modern CAD software (AutoCAD, SolidWorks, Maya) requires gigabytes of storage, mandating subscriptions and phoning home with telemetry. `RealityEngine` is an anarchist counter-proposal.

This is a sub-200MB, hyper-optimized solid geometry modeler compiled directly to metal. It runs entirely locally, bypassing commercial OS wrappers to speak directly to your GPU. 

## ⚡ Core Capabilities
* **Raw GPU Hooks:** Custom `InjectGPUHooks()` bypasses bloated OpenGL extension loaders (GLEW/GLAD) to query GPU drivers directly.
* **Asynchronous CSG (Constructive Solid Geometry):** Perform real-time Boolean operations (Difference, Union, Intersect) across shapes. Handled entirely on background CPU threads to ensure the UI render loop never drops below 144hz.
* **Physical Lighting Pipeline:** Custom hardware-accelerated GLSL fragment shaders calculating normal matrices for real-time diffuse illumination.
* **ImGui Docking Matrix:** AAA-grade immediate mode UI overlay for absolute architectural control.
* **.OBJ Slicer Exporting:** Serialize your Boolean intersections instantly to `.obj` format for direct 3D printing.

## 🛠️ The Architecture
* **Language:** C++20
* **Graphics API:** Raw OpenGL 3.3 Core Profile
* **Mathematics:** GLM (OpenGL Mathematics) & BSP-Tree Boolean logic
* **Dependencies:** Zero manual installations. CMake automatically fetches `GLFW`, `ImGui`, and `GLM` from source and fuses them into the singular executable payload.

## 🚀 Execution Protocol (Windows)
1. Ensure you have the Visual Studio C++ Compiler and CMake installed.
2. Clone this repository.
3. Execute the payload:
   ```powershell
   .\compile.ps1
   ```

---
*Forged for developers who demand secure, local, bare-metal MVP architecture without the corporate bloatware.*