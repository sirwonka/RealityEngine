# RealityEngine Build & Launch Script

$VS_PATH = "C:\Program Files\Microsoft Visual Studio\2022\Community"
$CMAKE_DIR = "$VS_PATH\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
$GIT_DIR = "$VS_PATH\Common7\IDE\CommonExtensions\Microsoft\TeamFoundation\Team Explorer\Git\cmd"

Write-Host "[*] Configuring Environment for RealityEngine..." -ForegroundColor Green

# Temporarily add tools to PATH
$env:PATH = "$CMAKE_DIR;$GIT_DIR;" + $env:PATH

# Use CMake to generate the build system
cmake -B build -S .

# Compile the project
cmake --build build --config Release

if ($LASTEXITCODE -eq 0) {
    Write-Host "[*] Build Successful. Launching RealityEngine..." -ForegroundColor Cyan
    .\build\Release\RealityEngine.exe
} else {
    Write-Host "[!] Build Failed. Check logs above." -ForegroundColor Red
}
