@echo off
SET PROJECT_DIR=%~dp0
SET UNREAL_ENGINE_DIR=C:\UnrealEngine\5.4\Engine
SET BUILD_DIR=%PROJECT_DIR%Build

REM Clean previous build
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"

REM Build the project
"%UNREAL_ENGINE_DIR%\Build\BatchFiles\RunUAT.bat" BuildCookRun ^
    -project="%PROJECT_DIR%MyUnrealProject.uproject" ^
    -noP4 ^
    -clientconfig=Development ^
    -serverconfig=Development ^
    -platform=Win64 ^
    -targetplatform=Win64 ^
    -cook ^
    -build ^
    -stage ^
    -pak ^
    -archive ^
    -archivedirectory="%BUILD_DIR%"
