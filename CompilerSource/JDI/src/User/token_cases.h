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
handle_user_token(TT_IF,        "`if' token")
handle_user_token(TT_THEN,      "`then' token")
handle_user_token(TT_ELSE,      "`else' token")
handle_user_token(TT_REPEAT,    "`repeat' token")
handle_user_token(TT_DO,        "`do' token")
handle_user_token(TT_WHILE,     "`while' token")
handle_user_token(TT_UNTIL,     "`until' token")
handle_user_token(TT_FOR,       "`for' token")
handle_user_token(TT_SWITCH,    "`switch' token")
handle_user_token(TT_CASE,      "`case' token")
handle_user_token(TT_DEFAULT,   "`default' token")
handle_user_token(TT_BREAK,     "`break' token")
handle_user_token(TT_CONTINUE,  "`continue' token")
handle_user_token(TT_RETURN,    "`return' token")
handle_user_token(TT_WITH,      "`with' token")
handle_user_token(TT_TRY,       "`try' token")
handle_user_token(TT_CATCH,     "`catch' token")
handle_user_token(TT_LOCAL,     "`local' keyword")
handle_user_token(TT_GLOBAL,    "`global' keyword")

// Do not remove this line
#undef handle_user_token
