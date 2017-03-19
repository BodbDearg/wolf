@rem # This script will install Wolf Language syntax for Sublime Text 3 on Windows.
@rem # The syntax file will be symlinked to the oriignal, meaning any updates to the syntax 
@rem # file in this folder will be automatically propagated to Sublime Text 3.

@set INSTALL_DIR=%APPDATA%\Sublime Text 3\Packages\User

@if not exist "%INSTALL_DIR%" (
	mkdir "%INSTALL_DIR%"
)

@set TARGET_FILE=%INSTALL_DIR%\Wolf.sublime-syntax

@if exist "%TARGET_FILE%" (
	del "%TARGET_FILE%"
)

ln Wolf.sublime-syntax "%TARGET_FILE%"
@pause
