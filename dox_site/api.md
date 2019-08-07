# API

The API is pretty easy to handle, this combined with
my madness complicated the implementation, but never mind that.
Only one class is present in the `InfoParse` namespace, others are 
`InfoParse::Internals` namespace which, you'd never guess it, is for
internal use. 

## Include

The library - if installed correctly and not butchered like how I usually
install stuff - has its directory called `infoparse`, where
all headers reside. Therefrom do you need to include 

## Construction

This only class is `OptionsParser`. As of `1.4.0`, this class
is default constructed, so you can easily just pop it wherever you want:
\[Note: For the followings `IP` is an alias to namespace `InfoParse`.]
```objectivec
IP::OptionsParser parser;
```
And with that your parser is constructed, but not much happened, not 
even internally. 

## Options

To actually get something going we need to input the options which are
to be parsed. This can be done in the two following ways, and a third 
one is to arrive in a following update soon(tm). 
The first and original way is to add the options by hand. This is a bit 
tedious but compared to the migraine of writing JS it is nothing.

### Single

```objectivec
int i = 0; // Be good and initialize your variables
parser.addOption("amazingly-great-option-name", &i);
bool silent = false;
bool chain = false;
parser.addOption("silent|quiet|s|q", &silent)
    ->addOption("calls-can-be-chained", &wget);
```

This method adds options one-by-one as the above example presents.
`addOption` takes a `OptionString` (which is implicitly constructed from 
`std::string`s or string literals, mind you) which parses the input string
into the names of the parameters, by splitting the string on the pipes 
`'|'`. Also note that `addOption` returns a pointer to the 
`IP::OptionsParser` which you called it from, so as shown on the second 
and third call they can be chain called. Note this is the original and 
was the only behavior until `1.3.x`, however plans are to only return
a const reference in some future version, when is not yet specified.

### Multiple

```objectivec
int i = 0; 
bool silent = false;
bool chain = false;
parser.addOptions()
    ("easy|fast", &i)
    ("sleek", &silent)
    ("boost-like", &wget)
;
```

As found in some Boost libraries (such as Boost.ProgramOptions)
this method of adding multiple options is a new and shiny feature since
`1.4.0`. Their behaviour is exactly the same as if it were `addOption`
called `n` times separately, and thus all documentation is 
presented [there](#single) .

### Functions

However mighty variables are, they are boring. Having functions as possible
output variables definitely is not, however they are highly dysfunctional (haha)
at the moment: for some ungodly reason the standard doesn't specify a type
for lambdas so I cannot go Template Wizard on their(the lambdas') asses 
to check if the provided type `T` is a lambda, so currently only function pointers
work. This would be fine, but C++ is limited in the regard of not allowing 
function definitions inside other functions, and thus to have some kind
of side-effect performed that affects the state of the current caller
function is impossible, unless using some kind of global callback hack, 
like tie-ing a local stream to `std::cout` and have a function put stuff
to the currently tie-d stream of `std::cout` \[Note: **Warning** 
This is not a tried and working method, just something I ass-pulled while
writing these docs. Usage is highly unadvised unless tested extensively 
for possible failures. In case of not finding any, please notify me so 
I can remove this warning.].

See this example of using function callbacks.  

```objectivec
#include <iostream>
#include <cstdlib>

#include <infoparse/OptionsParser.hpp>

[[noreturn]] void help();
[[noreturn]] void version();

int main(int argc, char** argv) { // K&R All the way
    // Setup
    InfoParse::OptionsParser parser;
    parser.addOptions()
        ("help|h|?", help)
        ("version|v", version)
    ;
    
    // Parsing
    auto rem = parser.parse(argc, argv);
    
    // Whatever
    std::cout << "Parameters: " << rem << std::endl;
    
    return 0;
}

void help() {
    std::cout << "Example v1\n"
                 "Usage: xmpl [options]\n"
                 "  -h --help -?\tShow this message and exit\n"
                 "  -v --version\tShow version and exit"
              << std::endl;
    std::exit(0);
}

void version() {
    std::cout << "Example v1" << std::endl;
    std::exit(0);
}

```
