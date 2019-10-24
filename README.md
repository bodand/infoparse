# InfoParse
C++ 17 parameter parser like Perl's Getopt::Long.

## Synopsis

```objectivec
#include <iostream>
#include <string>

#include <infoparse/OptionsParser>

int main(int argc, char** argv) {
    // Variables
    bool var1;
    std::string var2;
    
    // Parsing
    info::parse::OptionsParser parser;
    parser.addOptions()
        ("b|bool", &var1)
        ("s|string", &var2)
    ;
    
    parser.parse(argc, argv);
    
    // Output
    std::cout << std::boolaplha 
              << "Var1: " << var1 << "\n"
              << "Var2: \"" << var2 << '"' << std::endl;
    
    return 0;
} 
```

## Usage

### Include
First include the necessary header, wherever it may be hiding
on your hard-drive.
```objectivec2
#include "infoparse/OptionsParser.hpp"
```
With including that one header you have all you *need* to work with
InfoParse, more headers as described in the documentation are available,
however messing with them is highly unadvised. 

### Instantiate
In the scope in which you plan to use info::parse in you need to instantiate
an `info::parse::OptionsParse`. 
\[Note: For the following parts of the documentation
`namespace IP = info::parse` is in place, as I'm too lazy to write that much.\]
```objectivec
IP::OptionsParser parser;
```
`IP::OptionsParser` is default-constructable and doesn't offer any other 
constructor.

### Optionize
Having created the parser object you need to add the options you wish it to parse.
The results are spit back into already user-owned resources using 
a pointer stored when adding the option.
Option addition is achievable using the following method calls:
```objectivec

// Creates a "--string-long-opt" option with implicit "-s" short variant
// The string following the appearance of the option according to the
// local shell's partitioning will be stored in resultString. If
// option is not present, the empty string.
std::string resultString;
parser.addOption("string-long-opt", &resultString); 

// Creates a "--long-bool-opt" option with "-b" short variant
// resultBool contains true if option present false otherwise.
bool resultBool;
parser.addOption("long-bool-opt", 'b', &resultBool);

// addOption accepts all types that overload operator>> for std::istream
// Otherwise a nice SFINAE error will greet you
MyType resultMyType;
parser.addOption("funky-option", 'f', &resultMyType);
```

### Parse
After adding the options you may call parse any number of times you wish to
be parsed according to those options.
The parse method accepts either an std::string to be parsed which contains 
the options and parameters separated by whitespace as in a shell, or 
a char** and int parameters which work exactly as the ones passed into the
main function.
```objectivec
int main(int argc, char** argv) {
    //...
    std::string rem = parser.parse(argc, argv);
}
```
```objectivec
std::string args;
std::string rem = parser.parse(args);
```
The `rem` string in both of these snippets contains the text remaining after 
removing the parsed text from the passed string (or c-array of c-strings). 

## Documentation

Full documentation is available [here](https://isbodand.github.io/infoparsed).

## License

This project and repository is licensed under the BSD 3-Clause license.
For more information check the provided `LICENSE` file.

