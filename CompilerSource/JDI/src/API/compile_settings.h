
/// True if comments ending in backslashes should continue onto the next line.
#define ALLOW_MULTILINE_COMMENTS 1

/// True if errors are to be treated as fatal (end the parse)
#define FATAL_ERRORS 0
/// Optimized verion of (FATAL_ERRORS? x : y)
#define FATAL_ERRORS_T(x,y) (x)
/// Conditional return: Return if ( FATAL_ERRORS ).
#define FATAL_RETURN(ret_value) { return ret_value; }
/// Conditional return: Return if ( FATAL_ERRORS and ( expression ) ).
#define FATAL_RETURN_IF(expression,ret_value) { if (expression) return ret_value; }
#if !FATAL_ERRORS
  #undef  FATAL_ERRORS_T
  #undef  FATAL_RETURN
  #undef  FATAL_RETURN_IF
  #define FATAL_ERRORS_T(x,y) (y)
  #define FATAL_RETURN(ret_value) {}
  #define FATAL_RETURN_IF(expression,ret_value) expression
#endif
