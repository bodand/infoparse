# Conventions

The info::parse source uses some standard-ish conventions. These
are not that intrusive after getting used them. 
The followings describe what these are and what they are doing, as
some are a bit cryptic on first sight for those not necessarily 
well versed in the C++ language's abusability. 

# Naming

 - Variable names are `camelCase`
 - Normal macros are `SCREAMING_SNAKE_CASE`
 - Macros that are applied to something like a function are `_lowercase`
 - Class names are `PascalCase`
 - Classes that behave like interfaces are `PascalCase` and are an adjective, 
 like the Java convention for interfaces.
 - Member variables are `_camelCase`, while prefering single-word names,
 if possible, of course.
 - Accessor member functions are the name of the member variable without
 the leading underscore
 - Accessor member functions are the name of the member variable without
 the leading underscore, while taking a parameter of the approriate type
 - static and/or non-member functions are `PascalCase` 
 - Namespace of the project is `info::parse`, implementation details inside
 the `detail` subnamespace.

# Macros

The following preprocessor macros are found inside the source code
upon closer inspection. While these would mostly not be required for 
the program to be functional or even get compiled I use them because I can. 
Their names may or may not (they do) conflict with the language standard.

## `_pure`

### Usage
The `_pure` macro is used to declare functions pure in a sense they 
shall always return the same value on the same input and never disturb
state which is visible by other parts of the same program. 
The return value of these functions is always supposed to be used 
for something as that is their only option for output. 
Discarding their return value is equivalent of them not being called,
and while compilers are not going to be aggressive enough to just
optimize the function call away, they will warn if you waste resources,
by calling these function without using their return value.

### Implementation
If applicable this applies the C++17 `[[nodiscard]]` attribute,
and iff using GCC or clang `[[gnu::const]]`.
On a GCC compiler before C++11 this applies `__attribute__((const))`.

## `_retpure`

### Usage
The `_retpure` macro provides a weaker guarantee compared to the `_pure`
macro in such a way it need not be deterministic. It's return value
is pure so it doesn't write any other piece of memory, but it may return
different values on sequential calls. It also suggests the requirement
of use of the returned value, as that can be the sole reason for 
getting called since no other state changes in the program.
The best example is a getter; chances are the return value is why
the getter got called, yet if calling them in such a way the object's 
internal state changes they do not behave deterministically. 
See this example:
```objectivec
#include <iostream>

#define _retpure "<see actual code in utils.hpp>";

class Foo {
    /// Interface
public:
    _retpure int getBar() const {
        return bar;
    }
    void changeState() {
        bar += (bar + 1) * (bar + 1); 
    }
    
    /// Fields
private:
    int bar = 0;
};

int main() {
    Foo foo;
    std::cout << foo.getBar() << std::endl; // 0
    foo.changeState();
    std::cout << foo.getBar() << std::endl; // 1
    foo.changeState();
    std::cout << foo.getBar() << std::endl; // 5
    
    return 0;
}
```
The `Foo#getBar()` doesn't change any other value apart it's return,
yet even calling it on the same thread is not guaranteed to return the same.  

### Implementation
If applicable this applies the C++17 `[[nodiscard]]` attribute,
and iff using GCC or clang `[[gnu::pure]]`.
On a GCC compiler before C++11 this applies `__attribute__((pure))`.

## `_retval`

### Usage
The `_retval` macro is not as radical as the `_pure` or `_retpure` but is
on the same alignment. It means the returned value is most likely 
supposed to be used, however, the function itself is not `_pure` or even
`_retpure`. Example is a member method which changes state in the object,
yet returns some useful value. Most non-void functions that are not
either `_pure` or `_retpure` are `_retval`.

### Implementation
If using C++17 or greater `[[nodiscard]]` otherwise nothing.

## `unless(x)`
`if (!(x))`, no (diagnostic) explanation needed.

## `until(x)`
`while (!(x))`, no (diagnostic) explanation needed. 

# Class declaration layout

All classes try to follow the following convention of class 
layouts. Parts may be missing according to required usage of the class,
for example a trivially constructable & destructable classes
may leave out the `Contstructors & destructor` and `Operators` parts.
```objectivec
class Name : public GreatClass,
             public virtual Abolishable {
    /// Interface
public:
    /* Public and/or static methods */
    
    /// Lifecycle
public:
    Name() = default;
    Name(const Name& cp) = default;
    Name(Name&& mv) noexcept = default;
    
    virtual ~Name() = default;
    
    /// Operators
public:
    Name& operator=(const Name& cp) = default;
    Name& operator=(Name&& mv) noexcept = default;
    
    /// Fields
private:
    /* Fields */
    
    /// Methods
private: 
    /* Internal methods */
};
``` 
Classes before this schema was finalized currently as `InfoSoft CodeStyle C++ 2.1`,
may not use any kind of noticeable schema, or use `InfoSoft CodeStyle C++ 1.x`.
Differences to `1.x` are:

 - Fields are not denoted explicitly until `1.3`
 - Fields are the first thing in the class in `1.x`
 - Until `1.2` different groups were created for static, public, inherited, implemented, etc
   methods
 - Group names were using only two slashes and were put after the accesiblity modifier on the same line
 like so: `public: // Implemented public methods`

Difference to `2.0` is:
 
 - `/// Lifecycle` was `/// Constructor(s?) (& destructor)?`

Transitioning old schemas into the new one is curently in 
