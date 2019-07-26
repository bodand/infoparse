# InfoParse

InfoParse is a CLI parameter parser influenced by
Perl's core Getopt::Long package.

The following two snippets are more or less equivalent:  

* Perl using Getopt::Long
```perl5
use Getopt::Long;
my $var1;
my $var2;

GetOptions(
    'b|bool'    => \$var1,
    's|string=s'=> \$var2
);

print 'Var1: ' . ($var1 ? 'True' : 'False') . "\n";
print "Var2: $var2\n"
```

* C++ using InfoParse
```objectivec
#include <iostream>
#include <string>
#include <infoparse/OptionsParser>

int main(int argc, char** argv) {
    
    bool var1;
    std::string var2;
    
    InfoParse::OptionsParser parser;
    parser.addOption("bool", &var1);
    parser.addOption("string", &var2);
    
    parser.parse(argc, argv);
    
    std::cout << std::boolaplha 
              << "Var1: " << var1 << "\n"
              << "Var2: " << var2 << std::endl;
    
    return 0;
} 
```
