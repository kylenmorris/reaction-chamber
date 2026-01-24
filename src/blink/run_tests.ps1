# run_tests.ps1

# Stop if any command fails
$ErrorActionPreference = "Stop"

cd C:\Users\kylen\gth842\capstone\pathoscan\src\blink

Write-Host "--- Moving to build_tests dir ---" -ForegroundColor Cyan
cd .\build_tests

Write-Host "--- Configuring CMake ---" -ForegroundColor Cyan
cmake -DBUILD_TESTS=ON ..

Write-Host "--- Building Project ---" -ForegroundColor Cyan
cmake --build .

Write-Host "--- Running Tests ---" -ForegroundColor Cyan
.\Debug\test_main.exe

Write-Host "--- Moving to parent dir ---" -ForegroundColor Cyan
cd ..