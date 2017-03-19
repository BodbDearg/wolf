@rem # Small script that compiles a Wolf Programming Language source file <.wolf>
@rem #
@rem # Args:
@rem # 	%1 - path to 'wolfc'
@rem #	%2 - folder for intermidate files <.ll and .s>
@rem #	%3 - folder for output executable
@rem #	%4 - file to compile

@rem # Setting up build paths
@set VS_DIR=C:\Program Files (x86)\Microsoft Visual Studio\2017
@set MSVC_DIR=%VS_DIR%\Community\VC
@set MSVC_LIB_DIR=%MSVC_DIR%\Tools\MSVC\14.10.25017\lib\onecore\x64
@set WIN_SDK_DIR=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0
@set WIN_LIB_DIR=%WIN_SDK_DIR%\um\x64
@set CRT_LIB_DIR=%WIN_SDK_DIR%\ucrt\x64
@set LIB=%MSVC_LIB_DIR%;%WIN_LIB_DIR%;%CRT_LIB_DIR%

@rem # Grab inputs
@set WOLFC_PATH=%1
@set INTERMEDIATE_OUTPUT_DIR=%2%
@set EXECUTABLE_OUTPUT_DIR=%3%

@if not exist "%INTERMEDIATE_OUTPUT_DIR%" (
	mkdir "%INTERMEDIATE_OUTPUT_DIR%"

	@if %ERRORLEVEL% neq 0 (
		@echo Failed to create intermidate output folder '%INTERMEDIATE_OUTPUT_DIR%'
		@exit /B 1
	)
)

@if not exist "%EXECUTABLE_OUTPUT_DIR%" (
	mkdir "%EXECUTABLE_OUTPUT_DIR%"

	@if %ERRORLEVEL% neq 0 (
		@echo Failed to create executable output folder '%${INTERMEDIATE_OUTPUT_DIR}%'
		@exit /B 1
	)
)

@set INPUT_FILE_PATH=%4%

@rem # Get the name of the input file without the extension
@for %%i in (%INPUT_FILE_PATH%) do @set INPUT_FILENAME=%%~ni

@rem # Generate the llvm ir code using wolfc
@set OUTPUT_LL_FILE_PATH=%INTERMEDIATE_OUTPUT_DIR%/%INPUT_FILENAME%.ll

@"%WOLFC_PATH%" ^
	"%INPUT_FILE_PATH%" ^
	1> "%OUTPUT_LL_FILE_PATH%" 2>&1

@if %ERRORLEVEL% neq 0 (
    @echo A compiler error occurred for file '%INPUT_FILE_PATH%'! Error details follow:
	@more "%OUTPUT_LL_FILE_PATH%"
	@exit /B 1
)

@rem # Generate assembly
@set OUTPUT_S_FILE_PATH=%INTERMEDIATE_OUTPUT_DIR%/%INPUT_FILENAME%.s

@llc ^
	-march=x86-64 ^
	-x86-asm-syntax=intel ^
	-o "%OUTPUT_S_FILE_PATH%" ^
	"%OUTPUT_LL_FILE_PATH%"

@if %ERRORLEVEL% neq 0 (
    @echo Assembly code generation failed for IR code in file '%OUTPUT_LL_FILE_PATH%'!
    @exit /B 1
)

@rem # Compile that assembly to an object file
@set OUTPUT_OBJ_FILE_PATH=%INTERMEDIATE_OUTPUT_DIR%/%INPUT_FILENAME%.obj

@llvm-mc ^
	-arch=x86-64 ^
	-filetype=obj ^
	-assemble "%OUTPUT_S_FILE_PATH%" ^
	-o="%OUTPUT_OBJ_FILE_PATH%"

@if %ERRORLEVEL% neq 0 (
    @echo Compiling failed for assembly file '%OUTPUT_S_FILE_PATH%'!
    @exit /B 1
)

@rem # Link the object file
@set OUTPUT_BIN_FILE_PATH=%EXECUTABLE_OUTPUT_DIR%/%INPUT_FILENAME%.exe

@lld-link ^
	/subsystem:console ^
	/defaultlib:libcmtd.lib ^
	/defaultlib:legacy_stdio_definitions.lib ^
	/out:"%OUTPUT_BIN_FILE_PATH%" "%OUTPUT_OBJ_FILE_PATH%"

@if %ERRORLEVEL% neq 0 (
	@echo Linking failed!
	@pause
	@exit /B 1
)

@rem # All went well!
@exit /B 0
