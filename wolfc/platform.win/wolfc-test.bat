@IF NOT EXIST "wolfc_test" (
	@mkdir wolfc_test
)

@"x64\Debug\wolfc.exe" "..\test_data\wolfc-test.wolf" 2>"wolfc_test\wolfc-test.ll"

@IF %ERRORLEVEL% NEQ 0 (
	@more "wolfc_test\wolf-test.ll"
	@pause
	@exit %ERRORLEVEL%
)

@llc -march=x86-64 -x86-asm-syntax=intel -o "wolfc_test\wolfc-test.s" "wolfc_test\wolfc-test.ll"

@IF %ERRORLEVEL% NEQ 0 (
	@pause
	@exit %ERRORLEVEL%
)

@clang "wolfc_test\wolfc-test.s" -o "wolfc_test\wolfc-test"

@IF %ERRORLEVEL% NEQ 0 (
	@pause
	@exit %ERRORLEVEL%
)

"wolfc_test\wolfc-test"
@echo Program exited with status %ERRORLEVEL%.
pause
