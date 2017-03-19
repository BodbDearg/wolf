@rem # Compiles and runs the 'wolfc-test.wolf' file
@rem # N.B: Assumes that 'wolfc.exe' has already been compiled!

@rem # Get the folder that this file is in
@set THIS_FILE_DIR=%~dp0

@rem # Path to the compile and run batch file
@set COMPILE_AND_RUN_SCRIPT=%THIS_FILE_DIR%compile_and_run_wolf_file.bat

@rem # Do the compile and run!
@%COMPILE_AND_RUN_SCRIPT% ^
	"%THIS_FILE_DIR%x64/Debug/wolfc.exe" ^
	"%THIS_FILE_DIR%x64/Debug" ^
	"%THIS_FILE_DIR%x64/Debug" ^
	"%THIS_FILE_DIR%../test_data/wolfc-test.wolf"

@rem # Return the error level from running that script
@exit /B %ERRORLEVEL%
