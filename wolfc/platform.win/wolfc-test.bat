@REM ** Setting up build paths ** 
@SET VS_DIR=C:\Program Files (x86)\Microsoft Visual Studio 14.0
@SET MSVC_DIR=%VS_DIR%\VC
@SET WIN_SDK_DIR=C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0\um\x64
@SET INCLUDE=%MSVC_DIR%\INCLUDE;%MSVC_DIR%\VC\ATLMFC\INCLUDE;C:\Program Files (x86)\Windows Kits\10\include\10.0.14393.0\ucrt;C:\Program Files (x86)\Windows Kits\NETFXSDK\4.6.1\include\um;C:\Program Files (x86)\Windows Kits\10\include\10.0.14393.0\shared;C:\Program Files (x86)\Windows Kits\10\include\10.0.14393.0\um;C:\Program Files (x86)\Windows Kits\10\include\10.0.14393.0\winrt;
@SET LIB=%MSVC_DIR%\LIB\AMD64;%MSVC_DIR%\ATLMFC\LIB;C:\Program Files (x86)\Windows Kits\10\lib\10.0.14393.0\ucrt\x64;C:\Program Files (x86)\Windows Kits\NETFXSDK\4.6.1\lib\um\x64;C:\Program Files (x86)\Windows Kits\10\lib\10.0.14393.0\um\x64;
@SET LIBPATH=C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\LIB;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\ATLMFC\LIB;C:\Program Files (x86)\Windows Kits\10\UnionMetadata;C:\Program Files (x86)\Windows Kits\10\References;C:\Program Files (x86)\Microsoft SDKs\Windows Kits\10\ExtensionSDKs\Microsoft.VCLibs\14.0\References\CommonConfiguration\neutral;

@REM ** Make the output folder if it does not exist ** 
@IF NOT EXIST "wolfc-test" (
	@MKDIR wolfc-test
)

@REM ** Run the compiler ** 
@"x64\Debug\wolfc.exe" "..\test_data\wolfc-test.wolf" 2>"wolfc-test\wolfc-test.ll"

@IF %ERRORLEVEL% NEQ 0 (
	@echo wolfc compile failed! Error details follow:
	@MORE "wolfc_test\wolf-test.ll"
	@PAUSE
	@EXIT /B %ERRORLEVEL%
)

@REM ** Convert IR code to assembly code ** 
@llc -march=x86-64 -x86-asm-syntax=intel -o "wolfc-test\wolfc-test.s" "wolfc-test\wolfc-test.ll"

@IF %ERRORLEVEL% NEQ 0 (
	@echo Converting IR to assembly code failed!
	@PAUSE
	@EXIT /B %ERRORLEVEL%
)

@REM ** Assemble assembly code to an object file **
@llvm-mc -arch=x86-64 -filetype=obj -assemble "wolfc-test\wolfc-test.s" -o="wolfc-test\wolfc-test.o"

@IF %ERRORLEVEL% NEQ 0 (
	@echo Assembling to object file failed!
	@pause
	@EXIT /B %ERRORLEVEL%
)

@REM ** Link the object file **
@lld-link /subsystem:console /defaultlib:libcmtd.lib /out:"wolfc-test\wolfc-test.exe" "wolfc-test\wolfc-test.o"

@IF %ERRORLEVEL% NEQ 0 (
	@echo Linking failed!
	@pause
	@EXIT /B %ERRORLEVEL%
)

@REM ** Run the actual program **
"wolfc-test\wolfc-test"
@ECHO Program exited with status %ERRORLEVEL%.
@PAUSE
@CLS
