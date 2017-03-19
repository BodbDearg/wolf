@rem # Small script that compiles and runs a Wolf Programming Language source file <.wolf>
@rem # Returns the exit code of the program run, or '1' if the build fails.
@rem # 
@rem # Args:
@rem # 	%1 - path to 'wolfc.exe'
@rem # 	%2 - folder for intermidate files <.ll and .s>
@rem # 	%3 - folder for output executable
@rem # 	%4 - file to compile and run

@rem # Clear any errors firstly
@set ERRORLEVEL=0

@rem # Get the folder that this file is in
@set THIS_FILE_DIR=%~dp0

@rem # Try to compile the file first, if compile fails then bail out
@"%THIS_FILE_DIR%compile_wolf_file.bat" "%1%" "%2%" "%3%" "%4%"

@if %ERRORLEVEL% neq 0 (
	@exit /B %ERRORLEVEL%
)

@rem # Get the output dir and input file path
@set EXECUTABLE_OUTPUT_DIR=%3%
@set INPUT_FILE_PATH=%4%

@rem # Get the name of the input file without the extension
@for %%i in (%1) do @set INPUT_FILENAME="%%~ni"

@rem # This is the path to the output executable
@set OUTPUT_BIN_FILE_PATH=%EXECUTABLE_OUTPUT_DIR%\%INPUT_FILENAME%.exe

@rem # Run the executable
@"%OUTPUT_BIN_FILE_PATH%"

@rem # Return the return code from that program
@exit /B %ERRORLEVEL%
