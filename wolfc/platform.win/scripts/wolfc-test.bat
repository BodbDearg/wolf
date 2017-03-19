@rem # Compiles and runs the 'wolfc-test.wolf' file
@rem # N.B: Assumes that 'wolfc.exe' has already been compiled!

@rem # Get the folder that this file is in and clear output
@cls
@set THIS_FILE_DIR=%~dp0

@rem # Setup paths
@set COMPILE_AND_RUN_SCRIPT=%THIS_FILE_DIR%compile_and_run_wolf_file.bat
@set OUTPUT_DIR=%THIS_FILE_DIR%..\x64\Debug
@set INPUT_DIR=%THIS_FILE_DIR%..\..\test_data
@set INPUT_FILE=%INPUT_DIR%\wolfc-test.wolf

@rem # Do the compile and run!
@%COMPILE_AND_RUN_SCRIPT% ^
	%OUTPUT_DIR%\wolfc.exe ^
	%OUTPUT_DIR% ^
	%OUTPUT_DIR% ^
	%INPUT_FILE%
