# Configurations

While InfoParse is not the most configurable of libraries; it 
still offers some ability to be configured. 

Configuration is done by setting or defining specific macros before including
any library header. The macros are listed in the followings, by
describing the library's behavior in both cases whether or not the macro
is defined. Parameters define the value it is supposed to be set.
"Parameters: `none`" means only defined-ness(?) is checked. 

## INFO_RETRY_FAILED_CALLBACK_FUNCTION
Parameters: `none`  
If defined the library once retries running a callback function, in
case it is deemed to have failed. Failure conditions are described 
[here](/infoparsed/api#failure-and-success-conditions). 

## INFO_PARSER_FAIL_BAD_FUNCTION_SILENTLY
Parameters: `none`  
If the callback takes more that 2 parameters the library upon realizing
this when trying to call the function, throws a `bad_function_callback`
exception. Note this is hardly required as OptionsParser static
asserts less than or equal to 2 on it's input of addOption, so
only needed if instantiating Option_ manually, which you shouldn't 
be doing in the first place, so bear the consequences.

## INFO_DELETE_RETURN_VALUE_OF_CALLBACK
Parameters: `none`  
If the callback function returns a pointer which is supposed to be
deleted, for some ungodly reason, with this option the library
will call `delete ptr`, where `ptr` is the value returned by the callback.
Note that this is then called for every returned pointer.

## INFO_USE_BUILD_TIME_IN_VERSION
Parameters: `1` or `0` whether yes or no; default is `1`  
Decides whether build time is to be shown in the version number of
the library.
