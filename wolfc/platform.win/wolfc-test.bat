@REM ** Clear the screen when starting ** 
@CLS

@REM ** Setting up build paths ** 

@SET VS_DIR=C:\Program Files (x86)\Microsoft Visual Studio 14.0
@SET MSVC_DIR=%VS_DIR%\VC
@SET WIN_SDK_DIR=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0

@SET MSVC_LIB_DIR=%MSVC_DIR%\LIB\AMD64
@SET WIN_LIB_DIR=%WIN_SDK_DIR%\um\x64
@SET CRT_LIB_DIR=%WIN_SDK_DIR%\ucrt\x64

@SET LIB=%MSVC_LIB_DIR%;%WIN_LIB_DIR%;%CRT_LIB_DIR%

@REM ** Make the output folder if it does not exist ** 
@IF NOT EXIST "wolfc-test" (
	@MKDIR wolfc-test
)

@REM ** Run the compiler ** 
@"x64\Debug\wolfc.exe" ^
	"..\test_data\wolfc-test.wolf" ^
	1> "wolfc-test\wolfc-test.ll" 2>&1

@IF %ERRORLEVEL% NEQ 0 (
	@echo wolfc compile failed! Error details follow:
	@MORE "wolfc-test\wolfc-test.ll"
	@PAUSE
	@EXIT /B %ERRORLEVEL%
)

@REM ** Convert IR code to assembly code ** 
@llc ^
	-march=x86-64 ^
	-x86-asm-syntax=intel ^
	-o "wolfc-test\wolfc-test.s" ^
	"wolfc-test\wolfc-test.ll"

@IF %ERRORLEVEL% NEQ 0 (
	@echo Converting IR to assembly code failed!
	@PAUSE
	@EXIT /B %ERRORLEVEL%
)

@REM ** Assemble assembly code to an object file **
@llvm-mc ^
	-arch=x86-64 ^
	-filetype=obj ^
	-assemble "wolfc-test\wolfc-test.s" ^
	-o="wolfc-test\wolfc-test.o"

@IF %ERRORLEVEL% NEQ 0 (
	@echo Assembling to object file failed!
	@pause
	@EXIT /B %ERRORLEVEL%
)

@REM ** Link the object file **
@lld-link ^
	/subsystem:console ^
	/defaultlib:libcmtd.lib ^
	/defaultlib:legacy_stdio_definitions.lib ^
	/out:"wolfc-test\wolfc-test.exe" "wolfc-test\wolfc-test.o"

@IF %ERRORLEVEL% NEQ 0 (
	@echo Linking failed!
	@pause
	@EXIT /B %ERRORLEVEL%
)

@REM ** Run the actual program **
"wolfc-test\wolfc-test"
@ECHO Program exited with status %ERRORLEVEL%.
@PAUSE
