:: This Windows Batch file script generate all the project files of this project
:: It should be called from the directory where is located the higher level
:: CMakeLists.txt passing as argument the folder where to generate the project
:: files
:: 
:: Usage: scripts\generate_project_files.bat <path_to_build_folder>
@echo off

if "%~1" == "" GOTO showUsage 

mkdir %1
cd %1
cmake ..
GOTO exit

:showUsage
	echo Usage:    scripts\generate_project_files.bat  ^<path_to_build_folder^>

:exit
