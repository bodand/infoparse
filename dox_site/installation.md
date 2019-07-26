# Installation

If you wish to use InfoParse, logically, you need to acquire a copy of it.
This is achievable in some ways, two of which are presented here.

## conan

InfoParse uses [conan](conan.io) as its dependency manager a packager.
That means, we at InfoSoft gradually provide new InfoParse releases on our
conan server, the latest of which is always recommended to be used.
Our server is available at `conan.isdevnet.com` upon which InfoParse is
published as `InfoParse/1.3.5@bodand/stable`. 

More in-depth documentation on using conan to get InfoParse follows for
those not familiar with custom servers or with conan in general. 
\[Note: We assume you already have a working copy of conan installed.
To get conan look at their [website](conan.io) for documentation.]

### Remote

InfoSoft's conan package repository is `conan.isdevnet.com`. 
To tell conan to search in this repository in addition to `conan-central`
one needs to add it as a remote:

```shell
$ conan remote add InfoSoft-Conan-Server https://conan.isdevnet.com  
```

\[Note: The `InfoSoft-Conan-Server` is the server's local name thus it 
can be any arbitrary locally unique string.]

To see if it worked you can preform a listing on the current remotes
and search for InfoParse and even list all packages on our server.

```shell
$ conan remote list
$ conan search InfoParse
$ conan search -r InfoSoft-Conan-Server
```

### Requirement

If you searched for InfoParse in the last paragraph you saw what is
the name you need to add as a requirement.

Add `InfoParse/1.3.5@bodand/stable` to your conanfile.(py|txt) and conan
shall automagically install the package for you by running

```shell
$ conan install . -if <your_build_dir> -b missing
``` 

## Build it

You can build and install it on your local machine as described in the 
[appropriate documentation](/build).
