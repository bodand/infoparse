# Parsing Rules

Of course the most important factor of a parameter parser library is
how it parses the parameters: which options are allowed?; does it support
archaic stuff like the `+name` notation?; how customizable it is?;
DOS-like `/name` support? This section defines how the currently, opposing
my general ideology of the most general code being the best, quite
python-y parsing engine works, which really needs heavy rework in
regard to configuration the interface provides. 
(The answers to the above asked questions if you happen to suffer in
notansweredquestionphobia:
short POSIX `-f` and long parameters `--facilitate-destruction-of-humanity`; 
No.; Not much.; No. As you can see, quite a bad record.)

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

## eXtended Flags (XF) 

As the first paragraph said flags *should* not take values, you could 
have guessed (and probably did, how great you are doing today) 
they actually do, and they do it in 2 ways: 1) Lazy eXtended Flags (LXF); 
and 2) Seeking eXtended Flags (SXF).

### Lazy eXtended Flags (LXF)

They are, as their name suggests, lazy. The take the form
`--<flag>=<val>` or `-<f>=<val>` and they require both the flag and
the value touch directly on both sides of the equals sign.

After finding a flag; if the following character is an equals sign,
the following are parsed as a value for the flag until the next space.
This means `--<flag>= ` parses the value as the empty string. The parsed
value is then coerced into a boolean value according to the followings:

1) Literals: `Yes` & `True` are true, `No` & `False` are false.

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

### Seeking eXtended Flags (SXF)

While seeking and lazy extended flags work similarly, the subtle difference
is that seeking extended flags can only return the empty string if they
are at the end of everything and there is nothing to seek for. The parser
after finding a flag followed by a colon (':'), it seeks out the first
non-whitespace character then the next whitespace character, between them
is the value of the flag interpreted for truthyness as described above.

To see the difference between LXF and SXF see this example:
```plain
"text --flag text"  -> true  & "text text"
"text --flag=text"  -> true  & "text "
"text --flag= text" -> false & "text text"
"text --flag="      -> false & "text "
"text --flag:text"  -> true  & "text "
"text --flag: text" -> true  & "text "
"text --flag:"      -> false & "text "
```

# Value taking options

There exist, of course, values whose job is to take actual meaningful
parameters and pass them into the program.
