@rem # This compiles and runs all the unit tests in the unit tests folder and makes sure they all pass.
@rem # N.B: Assumes that 'wolfc.exe' has already been compiled!

@rem # Get the folder that this file is in and clear output
@cls
@set THIS_FILE_DIR=%~dp0

@rem # Setup paths
@set COMPILE_AND_RUN_SCRIPT=%THIS_FILE_DIR%compile_and_run_wolf_file.bat
@set OUTPUT_DIR=%THIS_FILE_DIR%..\x64\Debug
@set INPUT_DIR=%THIS_FILE_DIR%..\..\unit_tests

@rem # Run through all unit tests in the unit tests folder
@for %%i in ("%INPUT_DIR%\*.wolf") do @(
	@rem # Do the compile and run!
	@cls
	@echo Executing unit test '%%i'...
	@call %COMPILE_AND_RUN_SCRIPT% ^
		%OUTPUT_DIR%\wolfc.exe ^
		%OUTPUT_DIR% ^
		%OUTPUT_DIR% ^
		%%i

	@rem # If that fails then exit
	@if %ERRORLEVEL% neq 0 (
		@echo Unit test failed for file '%%i'!
		@exit \b 1
	)
)

@rem # If we've got to here then all tests have passed
@cls
@echo All tests passed successfully!
@pause
