
SET "ACTION=%~1"
SET "WORKSPACE=%~2..\.."
SET "PACKAGES_PATH=%WORKSPACE%\edk2;%WORKSPACE%"
SET "PATH=%PATH%;%WORKSPACE%;%PACKAGES_PATH%"
SET "OUTPUT_PATH=%~3"

cd ../../edk2

REM Set Visual Studio version
IF "%~4" == "15.0" SET TAG_NAME=VS2017
IF "%~4" == "16.0" SET TAG_NAME=VS2019

REM Get build configuration
IF /I "%~5" == "RELEASE" (
	SET BUILD_TARGET=RELEASE
) ELSE (
	SET BUILD_TARGET=DEBUG
)

REM Prepare EDK2 build environment
IF EXIST "%PACKAGES_PATH%\edk2\BaseTools\Bin\Win32\" (
	@CALL EDKSETUP.BAT %TAG_NAME%
) ELSE (
	@CALL EDKSETUP.BAT %TAG_NAME% rebuild
)
IF NOT ERRORLEVEL 0 GOTO :END

REM Check for CLEAN action
IF /I "%ACTION%" == "CLEAN" (
	PUSHD "%WORKSPACE%"
	CALL :CLEAN
	POPD
	GOTO :END
)

REM Check for BUILD action
IF /I "%ACTION%" == "BUILD" (
	PUSHD "%WORKSPACE%"
	CALL :BUILD
	POPD
	GOTO :END
)

REM Check for REBUILD action
IF /I "%ACTION%" == "REBUILD" (
	PUSHD "%WORKSPACE%"
	CALL :CLEAN
	CALL :BUILD
	POPD
	GOTO :END
)

REM Unsupported action
GOTO :END

:CLEAN
	CALL BUILD cleanall --arch X64 --buildtarget %BUILD_TARGET% -D WIN_SEC_BUILD --tagname %TAG_NAME% --platform EmulatorPkg\EmulatorPkg.dsc
	GOTO :END

:BUILD
	CALL BUILD all --arch X64 --define WIN_SEC_BUILD --buildtarget %BUILD_TARGET% -D WIN_SEC_BUILD --tagname %TAG_NAME% --platform EmulatorPkg\EmulatorPkg.dsc
	copy "%WORKSPACE%\utils\Emulator\startup.nsh" "%OUTPUT_PATH%\X64\startup.nsh"

	GOTO :END

:END
