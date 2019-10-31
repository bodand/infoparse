# API

The API is pretty easy to handle, this combined with
my madness complicated the implementation, but never mind that.
Only one class is present in the `info::parse` namespace, others are 
`info::parse::Internals` namespace which, you'd never guess it, is for
internal use. 

## Include

The library - if installed correctly and not butchered like how I usually
install stuff - has its directory called `info/parse`, where
all headers reside. Therefrom do you need to include 

## Construction

This only class is `OptionsParser`. As of `1.4.0`, this class
is default constructed, so you can easily just pop it wherever you want:
\[Note: For the followings `IP` is an alias to namespace `info::parse`.]
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
    .addOption("calls-can-be-chained", &wget);
```

This method adds options one-by-one as the above example presents.
`addOption` takes a `OptionString` (which is implicitly constructed from 
`std::string`s or string literals, mind you) which parses the input string
into the names of the parameters, by splitting the string on the pipes 
`'|'`. Also note that `addOption` returns a reference to the 
`IP::OptionsParser` which you called it from, so as shown on the second 
and third call they can be chain called. \[Note: Up to and including 
`1.4.x` the return value was a pointer to the parser object.]

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
output variables definitely is not. 
Since the glorious release of version `1.5.0`, using any kind of callable is
a great way to specify callbacks to the parser system. 
Works with function pointers, function objects and even lambdas.
While values specify the type of the program parameter by their own type,
lambdas, and function pointers do so with
their parameter type.
Callables may have some type `T` as their parameter iff `T` implements
the streaming `operator>>`, or `void`:
If their type is `T`, they will be called with an argument of type `T`,
parsed from the program arguments; 
if their type is `void` they will be called without any value being passed to them,
as if the boolean value were to be encoded in the 
fact that whether or not they are called.

See this example of using function callbacks.  

```objectivec
#include <iostream>
#include <cstdlib>

#include <infoparse/OptionsParser.hpp>

[[noreturn]] void help();
[[noreturn]] void version();

int main(int argc, char** argv) { // K&R braces All the way
    // Setup
    info::parse::OptionsParser parser;
    parser.addOptions()
        ("help|h|?", help)
        ("version|v", version)
        ("lambda|l", [&](const std::string& str) {
          std::cout << str << std::endl;
        })
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

#### Failure and success conditions

Depending on the [configuration](/infoparsed/config) and on the
function's return value, a function may be deemed to have succeeded or
to have failed by the library upon calling the callback-function. 
The failure and success conditions are as of the followings.

 1) A `void` function is always successful.  
 2) A function returning a pointer is successful if the returned
     pointer is not `nullptr`. \[Note: See 
     [configs](/infoparsed/config#info_delete_return_value_of_callback) 
     for returned pointers that should be deleted.]  
 3) If the returned value is convertible to `int` the following
    expression determines success: `((int) f(args...)) == 0` where
    `f` is the callback function and `args...` are the parameters.  
 4) A function whose return value is convertible to `bool`, that equals
    whether the function failed or not.  
 5) Any other case the function is hoped to have succeeded.  
