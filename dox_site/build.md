# Prerequisites

The following are required for the ability to build infoparse:
 
 - A C++17 compliant C++ compiler 
  \[Note: According to their docs this means GCC 7+, clang 3.9+ ]
  \[Note: Currently I cannot guarantee that MSVC works. On my system
  it shits itself quite badly.]
  \[Note: While GCC 7+ and clang 3.9+ can compile the library, 
  the tests require GCC 9+ or clang 5+.]
 - CMake 3.10+
 
A `make` implementation is most likely necessary as well.
Or maybe not if you use `ninja`, I don't know, but normally
it is.
 
# Downloading Sources

Our hopefully painless journey of building infoparse
starts with loading the required supplies aka downloading the source.
The followings describe the most common ways this can be done.

## git

The sources are available on github, so
you can pull them using `git`:
```bash
git clone https://github.com/isbodand/infoparse.git
```

## Tarball

If for whatever reason using git is not possible
you can resort to the good ol' `curl` and `tar` combo.

```bash
curl -Ls https://github.com/isbodand/infoparse/archive/master.tar.gz | tar xz
```

This will create a `infoparse-master` directory with the sources for info::parse. 
\[Note: Checksums are not provided by GitHub so don't need to be angry at me for
not checking them. If you don't want to download things without a checksum
use the [git](#git) method.]

## zip

If you happen to be stuck on a Windows PC without the required tooling to
get yourself `curl` and/or `tar`. 
You can try any of the following options to get yourself info::parse:
 
### PowerShell
  
```powershell
PS> Invoke-Webrequest https://github.com/isbodand/infoparse/archive/master.zip -OutFile infoparse.zip
PS> Expand-Archive infoparse.zip 
```
 
### cmd

If you are running and inferior version of Windows and don't have access to PS
or you broke it at some point in time:

```cmd
bitsadmin /transfer info::parseDownload /priority high https://github.com/isbodand/infoparse/archive/master.zip %home%/Downloads/infoparse.zip
```
If using cmd you have no build-in way of unzipping files, so figuring out this is left 
as an exercise to the reader, possible procedures range from `7z` to abusing `jar`. 

# Configuring CMake

After you got yourself the source, you need to tell cmake to configure it.
This is done it 2 steps:
 
 1) Creating a build directory
 
```bash
mkdir build && cd build
``` 

 2) Calling CMake
 
```bash
cmake ..
```

# Calling the build tool

The previous command created the required files for the build tool to build
infoparse, one just needs to call it. TIMTOWDI is in effect; here are your options:

## CMake

CMake is able to call the required build tool for the configuration
it generated. To do so just call:

```bash
cmake --build .
```

## Calling the tool itself
 
This command varies by platform, here are a few, however if you are not
sure which tool to call, just call through CMake as described above. 
 
  - `make` for *nix people 
  - `msbuild info::parse.sln` for MSVC
  - `mingw32-make` for MinGW 
