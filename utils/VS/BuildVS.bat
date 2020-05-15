set WORKSPACE=%~dp0..\..
set PACKAGES_PATH=%~dp0..\..\edk2;%~dp0..\..
cd ..\..\edk2
@call edksetup.bat
build -p ..\utils\utils.dsc -t VS2019 %*
