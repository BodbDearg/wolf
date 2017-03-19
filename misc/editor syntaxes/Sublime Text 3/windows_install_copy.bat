@rem # This script will install Wolf Language syntax for Sublime Text 3 on Windows.
@rem # The syntax file will be copied across, meaning any updates to the syntax file in
@rem # this folder will NOT be automatically propagated to Sublime Text 3.

@set INSTALL_DIR=%APPDATA%\Sublime Text 3\Packages\User

@if not exist "%INSTALL_DIR%" (
	mkdir "%INSTALL_DIR%"
)

copy Wolf.sublime-syntax "%INSTALL_DIR%"
@pause
