# Parsing Rules

Of course the most important factor of a parameter parser library is
how it parses the parameters: which options are allowed?; does it support
archaic stuff like the `+name` notation?; how customizable is it?;
DOS-like `/name` support? This section defines how the currently, opposing
my general ideology of the most general code being the best, quite
python-y parsing engine works, which really needs heavy rework in
regard to configuration the interface provides. 
(The answers to the above asked questions if you happen to suffer in
notansweredquestionphobia:
short POSIX `-f` and long parameters `--facilitate-destruction-of-humanity`; 
No.; Not much.; No. - As you can see, quite a bad record.)

# Boolean Flags

Boolean flags return either true or false. They do not take meaningful
parameters, or the value they take is coerced into true or false according
to a very perl-y truthyness table. They are initiated by creating an 
option with a non-nullptr `bool` pointer for the value spit back parameter.

## Raw flags

As boolean flags are the easiest thing to parse, as they do (= should) not 
take a value to be parsed. As all options both have a short and a long name,
flags do as well; `--<flag>` and `-<f>` are parsed and the spit back value 
is a bool whether they were found in the input string or not.

That's easy to reason about. This is where the shady parts come in.

## Negated flags

All long flags are also possible to be parsed in their negated form,
which is `--no-<flag>`. If this flag is found the option spits back a false value
into the pointer. Each flag or parameter is only parsed once, unless
specified otherwise, so the first flag wins and the others will be
left there as is, since they were not to be removed by anything.

## Flags with values ?!

Of course a library has to do something with high levels of bodandness
to become an InfoSoft solution. So boolean can take values as described 
later and the parsed value is checked for truthyness according to the followings:

1) Literals: `Yes` & `True` are true, `No` & `False` are false.
\[Note: Literals are case insensitive.]

2) Numbers: True unless equals to zero.
0 is false, while -12, 69 and 420 are true.

3) Strings: True unless only consists of whitespace or is the empty string.
"", "   ", "\n\t\n" are false while "a", "text text text", "bodand" are true.

Literals are tried first, if those do not match, 
then numbers are checked; if a string begins with at least one digit,
`atoi` does its job on it and then numeric truthyness check if performed.
So the string "0text" is false however true it is according to stringic (?)
truthyness.

A good example for this behaviour, invocation-wise, is using this in
a shell script for optional flags without needing branching.
```shell
which wget 2>/dev/null
```
This snipped prints the path at which `wget` is found or the empty string.
Using this as a parameter for a flag can make an optional parameter if 
`wget` is found.
```shell
./prog --use-wget=$(which wget 2>/dev/null)
```
This will set `--use-wget` is `wget` was found, and to false otherwise.

# Value taking options

There exist, of course, values whose job is to take actual meaningful
parameters and pass them into the program. These values compared to
the values taken by boolean flags do actually show up in the library's
caller program, unless, of course, an option is defined to dump into `nullptr`.
\[Note: **Warning** This behavior is not yet implemented.]

## Direct value

Options take the value that immediately succeed them, so care must be
taken when deciding which option to add before the other. For example:
Our options are `--text` and `--text-overlay`. If `--text` is registered
first `--text-overlay` will never match:
```plain
Params -> "text text text --text-overlay=Cocaine text text"
--text matches there      ^^^^^^
--text will use the value       ^^^^^^^^^^^^^^^^ 
```
However, if `--text-overlay` was registered first, the wanted behavior
would happen, and `--text-overlay` would be set to `Cocaine`.

The value absorbed equals to the value split up by the local shell,
so if using quotes or apostrophes, then spaces are viable, otherwise
the value spans to the next whitespace character, or the end of string.

## Optional space

Compared to the quite barbaric looking way of gluing together the option
and its value, a more aesthetic way of doing things is by splitting them
up with a space. This space when parsing happens is disregarded and the
value is taken as if the flags name were to include that space. So
`--textAbsorption` and `--text Absorption` are equal.

## Assignments

Assignments look cool, so they are regarded as the most important 
part of parameter parsing and using options with in InfoParse.
Compared to this rich (although confusing at first) behavior presented
no other implementation offers such freedom of supplying parameters.

### Equals sign

This is the common behavior on most programs thus the common
notation is bestowed to it: `--<option>=<val>`, without allowing
spaces before the `<val>`. Short options allow for this as well, in 
the form of `-<short>=<val>`. 

After finding an option's name; if the following character is an equals sign,
the following are parsed as a value for the flag until the next space.
This means `--<option>= ` parses the value as the empty string.

### Colon

While having an equals sign or a colon might seem like a trivial difference
for InfoParse it is not. While an equals sign is lazy to do any more work
than necessary, the colon searches for the match with her extreme energy
reserves, and disregards the amount of whitespace between her and her prey.

The parser after finding an option's name followed by a colon (':'), seeks out 
the first non-whitespace character then the next whitespace character,
between those is the value to use for whatever.

\[Note: The colon can still return the empty string if she is at the end 
of the string as there is hardly anything to seek. The idea that is should
loop around was discarded for it would be too confusing]

The differences are presented here:
```plain
"text --wordval?"   -> "val?" & "text " (1)
"text --word val?"  -> "val?" & "text "
"text --word=val?"  -> "val?" & "text "
"text --word= val?" -> ""     & "text val?"
"text --word="      -> ""     & "text "
"text --word:val?"  -> "val?" & "text "
"text --word: val?" -> "val?" & "text "
"text --word:"      -> ""     & "text "
```
\[Note: Boolean flags do not take the value that directly follows
them so `(1)` is not legal if `--word` takes a boolean.]
