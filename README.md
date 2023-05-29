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
usage: mingw-bundledlls exe_file [--copy]

positional arguments:
  exe_file    EXE or DLL file that you need to bundle dependencies for

optional arguments:
  No argument show this help message and exit
  --copy      In addition to printing out the dependencies, also copy them
              next to the exe_file
```

## Example
```
$ ./mingw-bundledlls ./scap-workbench/scap-workbench.exe --copy
/usr/i686-w64-mingw32/sys-root/mingw/bin/libssh2-1.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/QtNetwork4.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libcrypto-10.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libopenscap-8.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/QtCore4.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libpng16-16.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libintl-8.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/iconv.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libssl-10.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libxml2-2.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libwinpthread-1.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/QtXmlPatterns4.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libcurl-4.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libpcre-1.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libidn-11.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/QtGui4.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libxslt-1.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libstdc++-6.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libgcc_s_sjlj-1.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/libexslt-0.dll
/usr/i686-w64-mingw32/sys-root/mingw/bin/zlib1.dll

Copying enabled, will now copy all dependencies next to the exe_file.

Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libssh2-1.dll' to './scap-workbench/libssh2-1.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/QtNetwork4.dll' to './scap-workbench/QtNetwork4.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libcrypto-10.dll' to './scap-workbench/libcrypto-10.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libopenscap-8.dll' to './scap-workbench/libopenscap-8.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/QtCore4.dll' to './scap-workbench/QtCore4.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libpng16-16.dll' to './scap-workbench/libpng16-16.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libintl-8.dll' to './scap-workbench/libintl-8.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/iconv.dll' to './scap-workbench/iconv.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libssl-10.dll' to './scap-workbench/libssl-10.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libxml2-2.dll' to './scap-workbench/libxml2-2.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libwinpthread-1.dll' to './scap-workbench/libwinpthread-1.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/QtXmlPatterns4.dll' to './scap-workbench/QtXmlPatterns4.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libcurl-4.dll' to './scap-workbench/libcurl-4.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libpcre-1.dll' to './scap-workbench/libpcre-1.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libidn-11.dll' to './scap-workbench/libidn-11.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/QtGui4.dll' to './scap-workbench/QtGui4.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libxslt-1.dll' to './scap-workbench/libxslt-1.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libstdc++-6.dll' to './scap-workbench/libstdc++-6.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libgcc_s_sjlj-1.dll' to './scap-workbench/libgcc_s_sjlj-1.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/libexslt-0.dll' to './scap-workbench/libexslt-0.dll'
Copying '/usr/i686-w64-mingw32/sys-root/mingw/bin/zlib1.dll' to './scap-workbench/zlib1.dll'
```
