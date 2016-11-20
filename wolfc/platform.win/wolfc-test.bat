@IF NOT EXIST "wolfc_test" (
	@mkdir wolfc_test
)

@"x64/Debug/wolfc.exe" "../test_data/wolfc-test.wolf" 2>"wolfc_test/wolf-test.ll"

@IF %ERRORLEVEL% NEQ 0 (
	@more "wolfc_test/wolf-test.ll"
	@pause
	@exit %ERRORLEVEL%
)

pause
