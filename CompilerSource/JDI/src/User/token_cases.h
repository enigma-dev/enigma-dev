/// @file token_cases.h
/// This file is included by JDI as a means of handling additional tokens in JDI switch blocks.
/// It will almost certainly not be modified in JDI. In the event that it is modified, an auto-merge will succeed.
/// To add tokens to be handled, simply use the handle_user_token defined below for each token, one per line.
/// The macro takes the identifier of the token, followed by a string description of the token. For example:
///
/// handle_user_token(TT_NONSENSE, "nonsense token")
///
/// JDI will use this information in error reporting, but will not attempt to work with your custom tokens.
/// If you need to add an operator token, simply use the generic TT_OPERATOR type.

// Do not remove this block
#ifndef handle_user_token
#  define handle_user_token(tname, tdesc) case tname:
#endif

// Place your tokens here:

// Do not remove this line
#undef handle_user_token
