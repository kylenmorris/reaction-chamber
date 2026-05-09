# clean_build.ps1

# Stop if any command fails
$ErrorActionPreference = "Stop"

cd C:\Users\kylen\gth842\capstone\pathoscan\code

rmdir build -Force -Recurse
rmdir build_tests -Force -Recurse

mkdir build
mkdir build_tests

