# mingw-bundledlls

A convenient C native (Mingw-w64 or GNU/Linux gcc) app that copies all dependency DLLs next to the executable.

Suitable for creating a ready-to-go application bundle for Windows.

The application assumes Fedora 21 mingw32 install paths & Windows(mingw64), but that is easy to change.

This C native application is a port/rewrite of original Martin Preisler Python3 script https://github.com/mpreisler/mingw-bundledlls

[![Build](https://github.com/bvernoux/mingw-bundledlls/actions/workflows/Build.yml/badge.svg)](https://github.com/bvernoux/mingw-bundledlls/actions/workflows/Build.yml)

## Features
  - find all dependencies of an EXE or DLL file (recursively)
  - copy all dependencies next to the EXE or DLL

## Environment Variables
  - `$MINGW_BUNDLEDLLS_SEARCH_PATH`: Colon-separated list of paths to search for DLLs
     - Note if MINGW_BUNDLEDLLS_SEARCH_PATH is not found the application use DEFAULT_PATH_PREFIXES see source code for more details

## Usage

```
usage: mingw-bundledlls exe_file [--copy] [--verbose]

positional arguments:
  exe_file    EXE or DLL file that you need to bundle dependencies for

optional arguments:
  No argument show this help message and exit
  --copy      In addition to printing out the dependencies, also copy them
              next to the exe_file
  --verbose   Add full details on parameters and dependencies checks with recursion level ...
```

## Example 1 MinGW64 glscopeclient windows copy dependencies
```
$ cd ~/scopehal-apps/build/dist/windows_x64/
$ export MINGW_BUNDLEDLLS_SEARCH_PATH="C:\msys64\mingw64\bin;../../lib/graphwidget;../../lib/log;../../lib/scopeexports;../../lib/scopehal;../../lib/scopeprotocols;../../lib/xptools"
$ ~/mingw-bundledlls/mingw-bundledlls ~/scopehal-apps/build/dist/windows_x64/glscopeclient.exe
mingw-bundledlls v0.2.1 30/05/2023 B.VERNOUX
Dependencies(55):
C:\msys64\mingw64\bin\libatkmm-1.6-1.dll
C:\msys64\mingw64\bin\libgcc_s_seh-1.dll
C:\msys64\mingw64\bin\libwinpthread-1.dll
C:\msys64\mingw64\bin\libstdc++-6.dll
C:\msys64\mingw64\bin\libatk-1.0-0.dll
C:\msys64\mingw64\bin\libglib-2.0-0.dll
C:\msys64\mingw64\bin\libintl-8.dll
C:\msys64\mingw64\bin\libiconv-2.dll
C:\msys64\mingw64\bin\libpcre2-8-0.dll
C:\msys64\mingw64\bin\libgobject-2.0-0.dll
C:\msys64\mingw64\bin\libffi-8.dll
C:\msys64\mingw64\bin\libglibmm-2.4-1.dll
C:\msys64\mingw64\bin\libgmodule-2.0-0.dll
C:\msys64\mingw64\bin\libsigc-2.0-0.dll
C:\msys64\mingw64\bin\libcairomm-1.0-1.dll
C:\msys64\mingw64\bin\libcairo-2.dll
C:\msys64\mingw64\bin\libfontconfig-1.dll
C:\msys64\mingw64\bin\libexpat-1.dll
C:\msys64\mingw64\bin\libfreetype-6.dll
C:\msys64\mingw64\bin\libbz2-1.dll
C:\msys64\mingw64\bin\libbrotlidec.dll
C:\msys64\mingw64\bin\libbrotlicommon.dll
C:\msys64\mingw64\bin\libharfbuzz-0.dll
C:\msys64\mingw64\bin\libgraphite2.dll
C:\msys64\mingw64\bin\libpng16-16.dll
C:\msys64\mingw64\bin\zlib1.dll
C:\msys64\mingw64\bin\libpixman-1-0.dll
C:\msys64\mingw64\bin\libgdkmm-3.0-1.dll
C:\msys64\mingw64\bin\libgdk-3-0.dll
C:\msys64\mingw64\bin\libcairo-gobject-2.dll
C:\msys64\mingw64\bin\libepoxy-0.dll
C:\msys64\mingw64\bin\libfribidi-0.dll
C:\msys64\mingw64\bin\libgdk_pixbuf-2.0-0.dll
C:\msys64\mingw64\bin\libgio-2.0-0.dll
C:\msys64\mingw64\bin\libpango-1.0-0.dll
C:\msys64\mingw64\bin\libthai-0.dll
C:\msys64\mingw64\bin\libdatrie-1.dll
C:\msys64\mingw64\bin\libpangocairo-1.0-0.dll
C:\msys64\mingw64\bin\libpangoft2-1.0-0.dll
C:\msys64\mingw64\bin\libpangowin32-1.0-0.dll
C:\msys64\mingw64\bin\libgiomm-2.4-1.dll
C:\msys64\mingw64\bin\libgtk-3-0.dll
C:\msys64\mingw64\bin\libgtkmm-3.0-1.dll
C:\msys64\mingw64\bin\libpangomm-1.4-1.dll
C:\msys64\mingw64\bin\libyaml-cpp.dll
C:\msys64\mingw64\bin\glew32.dll
C:\msys64\mingw64\bin\libgomp-1.dll
../../lib/graphwidget/libgraphwidget.dll
../../lib/log/liblog.dll
../../lib/scopeexports/libscopeexports.dll
../../lib/scopehal/libscopehal.dll
C:\msys64\mingw64\bin\libffts.dll
C:\msys64\mingw64\bin\glfw3.dll
C:\msys64\mingw64\bin\vulkan-1.dll
../../lib/scopeprotocols/libscopeprotocols.dll

Copying enabled, will now copy all dependencies next to the exe_file.

$ ~/mingw-bundledlls/mingw-bundledlls ~/scopehal-apps/build/dist/windows_x64/glscopeclient.exe --copy
mingw-bundledlls v0.2.1 30/05/2023 B.VERNOUX
Dependencies(55):
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libatkmm-1.6-1.dll' target directory already exist, skipping copy...
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgcc_s_seh-1.dll' target directory already exist, skipping copy...
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libwinpthread-1.dll' target directory already exist, skipping copy...
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libstdc++-6.dll' target directory already exist, skipping copy...
Copying 'C:\msys64\mingw64\bin\libatk-1.0-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libatk-1.0-0.dll'
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libglib-2.0-0.dll' target directory already exist, skipping copy...
Copying 'C:\msys64\mingw64\bin\libintl-8.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libintl-8.dll'
Copying 'C:\msys64\mingw64\bin\libiconv-2.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libiconv-2.dll'
Copying 'C:\msys64\mingw64\bin\libpcre2-8-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libpcre2-8-0.dll'
Copying 'C:\msys64\mingw64\bin\libgobject-2.0-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgobject-2.0-0.dll'
Copying 'C:\msys64\mingw64\bin\libffi-8.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libffi-8.dll'
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libglibmm-2.4-1.dll' target directory already exist, skipping copy...
Copying 'C:\msys64\mingw64\bin\libgmodule-2.0-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgmodule-2.0-0.dll'
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libsigc-2.0-0.dll' target directory already exist, skipping copy...
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libcairomm-1.0-1.dll' target directory already exist, skipping copy...
Copying 'C:\msys64\mingw64\bin\libcairo-2.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libcairo-2.dll'
Copying 'C:\msys64\mingw64\bin\libfontconfig-1.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libfontconfig-1.dll'
Copying 'C:\msys64\mingw64\bin\libexpat-1.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libexpat-1.dll'
Copying 'C:\msys64\mingw64\bin\libfreetype-6.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libfreetype-6.dll'
Copying 'C:\msys64\mingw64\bin\libbz2-1.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libbz2-1.dll'
Copying 'C:\msys64\mingw64\bin\libbrotlidec.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libbrotlidec.dll'
Copying 'C:\msys64\mingw64\bin\libbrotlicommon.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libbrotlicommon.dll'
Copying 'C:\msys64\mingw64\bin\libharfbuzz-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libharfbuzz-0.dll'
Copying 'C:\msys64\mingw64\bin\libgraphite2.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgraphite2.dll'
Copying 'C:\msys64\mingw64\bin\libpng16-16.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libpng16-16.dll'
Copying 'C:\msys64\mingw64\bin\zlib1.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/zlib1.dll'
Copying 'C:\msys64\mingw64\bin\libpixman-1-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libpixman-1-0.dll'
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgdkmm-3.0-1.dll' target directory already exist, skipping copy...
Copying 'C:\msys64\mingw64\bin\libgdk-3-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgdk-3-0.dll'
Copying 'C:\msys64\mingw64\bin\libcairo-gobject-2.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libcairo-gobject-2.dll'
Copying 'C:\msys64\mingw64\bin\libepoxy-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libepoxy-0.dll'
Copying 'C:\msys64\mingw64\bin\libfribidi-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libfribidi-0.dll'
Copying 'C:\msys64\mingw64\bin\libgdk_pixbuf-2.0-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgdk_pixbuf-2.0-0.dll'
Copying 'C:\msys64\mingw64\bin\libgio-2.0-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgio-2.0-0.dll'
Copying 'C:\msys64\mingw64\bin\libpango-1.0-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libpango-1.0-0.dll'
Copying 'C:\msys64\mingw64\bin\libthai-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libthai-0.dll'
Copying 'C:\msys64\mingw64\bin\libdatrie-1.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libdatrie-1.dll'
Copying 'C:\msys64\mingw64\bin\libpangocairo-1.0-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libpangocairo-1.0-0.dll'
Copying 'C:\msys64\mingw64\bin\libpangoft2-1.0-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libpangoft2-1.0-0.dll'
Copying 'C:\msys64\mingw64\bin\libpangowin32-1.0-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libpangowin32-1.0-0.dll'
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgiomm-2.4-1.dll' target directory already exist, skipping copy...
Copying 'C:\msys64\mingw64\bin\libgtk-3-0.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgtk-3-0.dll'
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgtkmm-3.0-1.dll' target directory already exist, skipping copy...
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libpangomm-1.4-1.dll' target directory already exist, skipping copy...
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libyaml-cpp.dll' target directory already exist, skipping copy...
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/glew32.dll' target directory already exist, skipping copy...
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgomp-1.dll' target directory already exist, skipping copy...
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libgraphwidget.dll' target directory already exist, skipping copy...
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/liblog.dll' target directory already exist, skipping copy...
Copying '../../lib/scopeexports/libscopeexports.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libscopeexports.dll'
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libscopehal.dll' target directory already exist, skipping copy...
Copying 'C:\msys64\mingw64\bin\libffts.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libffts.dll'
Copying 'C:\msys64\mingw64\bin\glfw3.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/glfw3.dll'
Copying 'C:\msys64\mingw64\bin\vulkan-1.dll' to 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/vulkan-1.dll'
Dependency 'C:/msys64/home/bvern/scopehal-apps/build/dist/windows_x64/libscopeprotocols.dll' target directory already exist, skipping copy...
```

## Example2 MinGW64 ImHex.exe 1.29.0 check dependencies
```
$ export MINGW_BUNDLEDLLS_SEARCH_PATH="C:\msys64\mingw64\bin;C:\Program Files\ImHex"
$ ./mingw-bundledlls 'C:\Program Files\ImHex\ImHex.exe'
mingw-bundledlls v0.2.1 30/05/2023 B.VERNOUX
Dependencies(28):
C:\msys64\mingw64\bin\libgcc_s_seh-1.dll
C:\msys64\mingw64\bin\libwinpthread-1.dll
C:\msys64\mingw64\bin\glfw3.dll
C:\msys64\mingw64\bin\libstdc++-6.dll
C:\Program Files\ImHex\libimhex.dll
C:\msys64\mingw64\bin\libfreetype-6.dll
C:\msys64\mingw64\bin\libbz2-1.dll
C:\msys64\mingw64\bin\libbrotlidec.dll
C:\msys64\mingw64\bin\libbrotlicommon.dll
C:\msys64\mingw64\bin\libharfbuzz-0.dll
C:\msys64\mingw64\bin\libglib-2.0-0.dll
C:\msys64\mingw64\bin\libintl-8.dll
C:\msys64\mingw64\bin\libiconv-2.dll
C:\msys64\mingw64\bin\libpcre2-8-0.dll
C:\msys64\mingw64\bin\libgraphite2.dll
C:\msys64\mingw64\bin\libpng16-16.dll
C:\msys64\mingw64\bin\zlib1.dll
C:\msys64\mingw64\bin\libidn2-0.dll
C:\msys64\mingw64\bin\libunistring-5.dll
C:\Program Files\ImHex\libmagic-1.dll
C:\msys64\mingw64\bin\libsystre-0.dll
C:\msys64\mingw64\bin\libtre-5.dll
C:\Program Files\ImHex\libmbedcrypto.dll
C:\Program Files\ImHex\libmbedtls.dll
C:\Program Files\ImHex\libmbedx509.dll
C:\msys64\mingw64\bin\libpsl-5.dll
C:\msys64\mingw64\bin\libssh2-1.dll
C:\msys64\mingw64\bin\libcrypto-3-x64.dll

Verbose mode to check what dependencies are not found

$ ./mingw-bundledlls 'C:\Program Files\ImHex\ImHex.exe' --verbose
mingw-bundledlls v0.2.1 30/05/2023 B.VERNOUX
verbose
--copy=0
exe_file="C:\Program Files\ImHex\ImHex.exe"
path_prefixes(2):
"C:\msys64\mingw64\bin"
"C:\Program Files\ImHex"

dep find(0-11 deps): C:\Program Files\ImHex\ImHex.exe
dep(0-0): dwmapi.dll blacklisted
dep(0-1) found full path: C:\msys64\mingw64\bin\libgcc_s_seh-1.dll
dep find(1-3 deps): C:\msys64\mingw64\bin\libgcc_s_seh-1.dll
dep(1-0): kernel32.dll blacklisted
dep(1-1): msvcrt.dll blacklisted
dep(1-2) found full path: C:\msys64\mingw64\bin\libwinpthread-1.dll
dep find(2-2 deps): C:\msys64\mingw64\bin\libwinpthread-1.dll
dep(2-0): kernel32.dll already seen
dep(2-1): msvcrt.dll already seen
dep(0-2) found full path: C:\msys64\mingw64\bin\glfw3.dll
dep find(1-5 deps): C:\msys64\mingw64\bin\glfw3.dll
dep(1-0): gdi32.dll blacklisted
dep(1-1): kernel32.dll already seen
dep(1-2): msvcrt.dll already seen
dep(1-3): shell32.dll blacklisted
dep(1-4): user32.dll blacklisted
dep(0-3): kernel32.dll already seen
dep(0-4): msvcrt.dll already seen
dep(0-5): ole32.dll blacklisted
dep(0-6): opengl32.dll blacklisted
dep(0-7): libwinpthread-1.dll already seen
dep(0-8) found full path: C:\msys64\mingw64\bin\libstdc++-6.dll
dep find(1-4 deps): C:\msys64\mingw64\bin\libstdc++-6.dll
dep(1-0): libgcc_s_seh-1.dll already seen
dep(1-1): kernel32.dll already seen
dep(1-2): msvcrt.dll already seen
dep(1-3): libwinpthread-1.dll already seen
dep(0-9): user32.dll already seen
findFullPath: C:\msys64\mingw64\bin\libimhex.dll (Not Found)
dep(0-10) found full path: C:\Program Files\ImHex\libimhex.dll
dep find(1-23 deps): C:\Program Files\ImHex\libimhex.dll
findFullPath: C:\msys64\mingw64\bin\dbghelp.dll (Not Found)
findFullPath: C:\Program Files\ImHex\dbghelp.dll (Not Found)
dep(1-0): NOT FOUND in full path: dbghelp.dll
dep(1-1): advapi32.dll blacklisted
dep(1-2): bcrypt.dll blacklisted
dep(1-3) found full path: C:\msys64\mingw64\bin\libfreetype-6.dll
dep find(2-7 deps): C:\msys64\mingw64\bin\libfreetype-6.dll
dep(2-0) found full path: C:\msys64\mingw64\bin\libbz2-1.dll
dep find(3-2 deps): C:\msys64\mingw64\bin\libbz2-1.dll
dep(3-0): kernel32.dll already seen
dep(3-1): msvcrt.dll already seen
dep(2-1): kernel32.dll already seen
dep(2-2): msvcrt.dll already seen
dep(2-3) found full path: C:\msys64\mingw64\bin\libbrotlidec.dll
dep find(3-3 deps): C:\msys64\mingw64\bin\libbrotlidec.dll
dep(3-0): kernel32.dll already seen
dep(3-1): msvcrt.dll already seen
dep(3-2) found full path: C:\msys64\mingw64\bin\libbrotlicommon.dll
dep find(4-2 deps): C:\msys64\mingw64\bin\libbrotlicommon.dll
dep(4-0): kernel32.dll already seen
dep(4-1): msvcrt.dll already seen
dep(2-4) found full path: C:\msys64\mingw64\bin\libharfbuzz-0.dll
dep find(3-12 deps): C:\msys64\mingw64\bin\libharfbuzz-0.dll
dep(3-0): libgcc_s_seh-1.dll already seen
dep(3-1): gdi32.dll already seen
dep(3-2): kernel32.dll already seen
dep(3-3): msvcrt.dll already seen
dep(3-4): rpcrt4.dll blacklisted
dep(3-5): libstdc++-6.dll already seen
dep(3-6): user32.dll already seen
dep(3-7): usp10.dll blacklisted
dep(3-8): libfreetype-6.dll already seen
dep(3-9) found full path: C:\msys64\mingw64\bin\libglib-2.0-0.dll
dep find(4-9 deps): C:\msys64\mingw64\bin\libglib-2.0-0.dll
dep(4-0): advapi32.dll already seen
dep(4-1) found full path: C:\msys64\mingw64\bin\libintl-8.dll
dep find(5-4 deps): C:\msys64\mingw64\bin\libintl-8.dll
dep(5-0): advapi32.dll already seen
dep(5-1) found full path: C:\msys64\mingw64\bin\libiconv-2.dll
dep find(6-2 deps): C:\msys64\mingw64\bin\libiconv-2.dll
dep(6-0): kernel32.dll already seen
dep(6-1): msvcrt.dll already seen
dep(5-2): kernel32.dll already seen
dep(5-3): msvcrt.dll already seen
dep(4-2): kernel32.dll already seen
dep(4-3): msvcrt.dll already seen
dep(4-4): ole32.dll already seen
dep(4-5): shell32.dll already seen
dep(4-6): user32.dll already seen
dep(4-7): ws2_32.dll blacklisted
dep(4-8) found full path: C:\msys64\mingw64\bin\libpcre2-8-0.dll
dep find(5-2 deps): C:\msys64\mingw64\bin\libpcre2-8-0.dll
dep(5-0): kernel32.dll already seen
dep(5-1): msvcrt.dll already seen
dep(3-10) found full path: C:\msys64\mingw64\bin\libgraphite2.dll
dep find(4-4 deps): C:\msys64\mingw64\bin\libgraphite2.dll
dep(4-0): kernel32.dll already seen
dep(4-1): msvcrt.dll already seen
dep(4-2): libgcc_s_seh-1.dll already seen
dep(4-3): libstdc++-6.dll already seen
dep(3-11): libintl-8.dll already seen
dep(2-5) found full path: C:\msys64\mingw64\bin\libpng16-16.dll
dep find(3-3 deps): C:\msys64\mingw64\bin\libpng16-16.dll
dep(3-0): kernel32.dll already seen
dep(3-1): msvcrt.dll already seen
dep(3-2) found full path: C:\msys64\mingw64\bin\zlib1.dll
dep find(4-2 deps): C:\msys64\mingw64\bin\zlib1.dll
dep(4-0): kernel32.dll already seen
dep(4-1): msvcrt.dll already seen
dep(2-6): zlib1.dll already seen
dep(1-4): libgcc_s_seh-1.dll already seen
dep(1-5): glfw3.dll already seen
dep(1-6) found full path: C:\msys64\mingw64\bin\libidn2-0.dll
dep find(2-5 deps): C:\msys64\mingw64\bin\libidn2-0.dll
dep(2-0): libiconv-2.dll already seen
dep(2-1): libintl-8.dll already seen
dep(2-2): kernel32.dll already seen
dep(2-3): msvcrt.dll already seen
dep(2-4) found full path: C:\msys64\mingw64\bin\libunistring-5.dll
dep find(3-3 deps): C:\msys64\mingw64\bin\libunistring-5.dll
dep(3-0): libiconv-2.dll already seen
dep(3-1): kernel32.dll already seen
dep(3-2): msvcrt.dll already seen
dep(1-7): kernel32.dll already seen
findFullPath: C:\msys64\mingw64\bin\libmagic-1.dll (Not Found)
dep(1-8) found full path: C:\Program Files\ImHex\libmagic-1.dll
dep find(2-4 deps): C:\Program Files\ImHex\libmagic-1.dll
dep(2-0) found full path: C:\msys64\mingw64\bin\libsystre-0.dll
dep find(3-3 deps): C:\msys64\mingw64\bin\libsystre-0.dll
dep(3-0) found full path: C:\msys64\mingw64\bin\libtre-5.dll
dep find(4-3 deps): C:\msys64\mingw64\bin\libtre-5.dll
dep(4-0): libintl-8.dll already seen
dep(4-1): kernel32.dll already seen
dep(4-2): msvcrt.dll already seen
dep(3-1): kernel32.dll already seen
dep(3-2): msvcrt.dll already seen
dep(2-1): kernel32.dll already seen
dep(2-2): msvcrt.dll already seen
dep(2-3): shlwapi.dll blacklisted
findFullPath: C:\msys64\mingw64\bin\libmbedcrypto.dll (Not Found)
dep(1-9) found full path: C:\Program Files\ImHex\libmbedcrypto.dll
dep find(2-5 deps): C:\Program Files\ImHex\libmbedcrypto.dll
dep(2-0): advapi32.dll already seen
dep(2-1): libgcc_s_seh-1.dll already seen
dep(2-2): kernel32.dll already seen
dep(2-3): msvcrt.dll already seen
dep(2-4): libwinpthread-1.dll already seen
findFullPath: C:\msys64\mingw64\bin\libmbedtls.dll (Not Found)
dep(1-10) found full path: C:\Program Files\ImHex\libmbedtls.dll
dep find(2-5 deps): C:\Program Files\ImHex\libmbedtls.dll
dep(2-0): kernel32.dll already seen
dep(2-1): msvcrt.dll already seen
dep(2-2): ws2_32.dll already seen
dep(2-3): libmbedcrypto.dll already seen
findFullPath: C:\msys64\mingw64\bin\libmbedx509.dll (Not Found)
dep(2-4) found full path: C:\Program Files\ImHex\libmbedx509.dll
dep find(3-3 deps): C:\Program Files\ImHex\libmbedx509.dll
dep(3-0): kernel32.dll already seen
dep(3-1): msvcrt.dll already seen
dep(3-2): libmbedcrypto.dll already seen
dep(1-11): libmbedx509.dll already seen
dep(1-12): msvcrt.dll already seen
dep(1-13): ole32.dll already seen
dep(1-14) found full path: C:\msys64\mingw64\bin\libpsl-5.dll
dep find(2-6 deps): C:\msys64\mingw64\bin\libpsl-5.dll
dep(2-0): libiconv-2.dll already seen
dep(2-1): kernel32.dll already seen
dep(2-2): msvcrt.dll already seen
dep(2-3): libunistring-5.dll already seen
dep(2-4): ws2_32.dll already seen
dep(2-5): libidn2-0.dll already seen
dep(1-15): libwinpthread-1.dll already seen
dep(1-16): shell32.dll already seen
dep(1-17) found full path: C:\msys64\mingw64\bin\libssh2-1.dll
dep find(2-6 deps): C:\msys64\mingw64\bin\libssh2-1.dll
dep(2-0) found full path: C:\msys64\mingw64\bin\libcrypto-3-x64.dll
dep find(3-5 deps): C:\msys64\mingw64\bin\libcrypto-3-x64.dll
dep(3-0): advapi32.dll already seen
dep(3-1): kernel32.dll already seen
dep(3-2): msvcrt.dll already seen
dep(3-3): user32.dll already seen
dep(3-4): ws2_32.dll already seen
dep(2-1): kernel32.dll already seen
dep(2-2): msvcrt.dll already seen
dep(2-3): user32.dll already seen
dep(2-4): ws2_32.dll already seen
dep(2-5): zlib1.dll already seen
dep(1-18): libstdc++-6.dll already seen
dep(1-19): user32.dll already seen
dep(1-20): wldap32.dll blacklisted
dep(1-21): ws2_32.dll already seen
dep(1-22): zlib1.dll already seen
Dependencies(28):
C:\msys64\mingw64\bin\libgcc_s_seh-1.dll
C:\msys64\mingw64\bin\libwinpthread-1.dll
C:\msys64\mingw64\bin\glfw3.dll
C:\msys64\mingw64\bin\libstdc++-6.dll
C:\Program Files\ImHex\libimhex.dll
C:\msys64\mingw64\bin\libfreetype-6.dll
C:\msys64\mingw64\bin\libbz2-1.dll
C:\msys64\mingw64\bin\libbrotlidec.dll
C:\msys64\mingw64\bin\libbrotlicommon.dll
C:\msys64\mingw64\bin\libharfbuzz-0.dll
C:\msys64\mingw64\bin\libglib-2.0-0.dll
C:\msys64\mingw64\bin\libintl-8.dll
C:\msys64\mingw64\bin\libiconv-2.dll
C:\msys64\mingw64\bin\libpcre2-8-0.dll
C:\msys64\mingw64\bin\libgraphite2.dll
C:\msys64\mingw64\bin\libpng16-16.dll
C:\msys64\mingw64\bin\zlib1.dll
C:\msys64\mingw64\bin\libidn2-0.dll
C:\msys64\mingw64\bin\libunistring-5.dll
C:\Program Files\ImHex\libmagic-1.dll
C:\msys64\mingw64\bin\libsystre-0.dll
C:\msys64\mingw64\bin\libtre-5.dll
C:\Program Files\ImHex\libmbedcrypto.dll
C:\Program Files\ImHex\libmbedtls.dll
C:\Program Files\ImHex\libmbedx509.dll
C:\msys64\mingw64\bin\libpsl-5.dll
C:\msys64\mingw64\bin\libssh2-1.dll
C:\msys64\mingw64\bin\libcrypto-3-x64.dll
```
