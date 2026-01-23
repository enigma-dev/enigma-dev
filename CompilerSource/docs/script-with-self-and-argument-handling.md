# Script `with(self)` Wrapper and `argument` Handling

## Overview

This document explains how ENIGMA handles scripts wrapped in `with(self)` and why the `argument` array is special - it's a local function variable, not an instance variable.

## How Scripts Are Parsed

When a script is parsed, it's automatically wrapped in a `with(self)` block:

```cpp
// In parse_and_link.cpp, line 73-74
std::string wrapped_code = 
    std::string("with (self) {\n") + 
    std::string(game.scripts[i]->code()) + 
    std::string("\n/* */}");
AST ast = AST::Parse(wrapped_code, &state.parse_context);
```

**Why?** This wrapper establishes the scope context during parsing. It tells the parser that unqualified variables like `x`, `y`, `speed`, etc. refer to instance variables of the calling object, not global variables.

### Example

If you have a script:
```gml
/// approach(current, target, amount)
var current = argument[0];
var target = argument[1];
var amount = argument[2];

if (current < target) {
    return min(current+amount, target); 
} else {
    return max(current-amount, target);
}
```

During parsing, it becomes:
```gml
with (self) {
    var current = argument[0];
    var target = argument[1];
    var amount = argument[2];
    
    if (current < target) {
        return min(current+amount, target); 
    } else {
        return max(current-amount, target);
    }
}
```

## How Scripts Are Code-Generated

During code generation, **only** the `with(self)` wrapper in scripts is **unwrapped** (see `ast.cpp` lines 41-127). The script body is written directly, and instance variables are accessed via `glaccess(int(self))->variable_name`.

**Important:** This unwrapping behavior is **specific to `with(self)` in scripts only**. Regular `with` statements in user code (like `with(other)`, `with(obj_enemy)`, etc.) are **NOT** unwrapped - they generate actual `with` statements in the C++ code, which are then expanded by the `with` macro (see `with.h`).

### Generated Code Structure

For a **global script** (standalone function):
```cpp
variant _SCR_approach(variant argument0, variant argument1, variant argument2) {
  // Local array declaration
  variant argument[16] = {argument0, argument1, argument2, ...};
  int argument_count = 3;
  
  // Script body (with(self) wrapper removed)
  var current = argument[0];  // Direct access to local array
  var target = argument[1];   // Direct access to local array
  
  // Instance variables accessed via glaccess
  // (if the script used x, y, etc., they'd be: glaccess(int(self))->x)
  
  return 0;
}
```

For an **object script** (method inside object class):
```cpp
variant enigma::OBJ_Player::_SCR_approach(variant argument0, variant argument1, variant argument2) {
  // Local array declaration
  variant argument[16] = {argument0, argument1, argument2, ...};
  int argument_count = 3;
  
  // Script body - instance variables accessed directly as members
  // (x, y, etc. are member variables, no glaccess needed)
  
  return 0;
}
```

## The `self` Variable

When a script is called, `self` refers to the calling instance:

```cpp
// When obj_player calls approach(10, 20, 5):
_SCR_approach(10, 20, 5);
// Inside the script, self refers to the obj_player instance
```

The `self` variable is available in the script's scope, allowing `glaccess(int(self))->x` to access the correct instance's variables.

## Why `argument` is Special

The `argument` array is **NOT** an instance variable. It's a **local array** declared inside each script function:

```cpp
variant _SCR_approach(...) {
  variant argument[16] = {argument0, argument1, ...};  // <-- LOCAL to function
  // ...
}
```

### Isolation and Thread Safety

**Each script call has its own `argument` array** - it's completely isolated to that function call's stack frame. This means:

1. **No cross-instance contamination**: If `obj_player` calls `approach(10, 20, 5)` and `obj_enemy` calls `approach(100, 200, 50)` simultaneously, each call has its own separate `argument` array. They cannot interfere with each other.

2. **Stack frame isolation**: Each function call creates a new stack frame with its own local variables. The `argument` array lives in that stack frame and is automatically cleaned up when the function returns.

3. **No shared state**: Unlike instance variables (which are stored on the instance object and could theoretically be accessed by multiple threads), `argument` is purely local to the function call.

This is a fundamental property of how C++ function calls work - each call gets its own stack frame with its own local variables. Even if the same script is called recursively or from multiple instances, each call has its own isolated `argument` array.

### Key Differences

| Aspect | Instance Variables (x, y, speed) | `argument` Array |
|--------|----------------------------------|------------------|
| **Storage** | Stored on the instance object | Stored in function's stack frame |
| **Scope** | Instance scope | Function scope |
| **Access** | Via `glaccess(int(self))->x` | Direct: `argument[N]` |
| **Lifetime** | Exists as long as instance exists | Exists only during function execution (stack frame) |
| **Isolation** | Shared across all code accessing that instance | **Completely isolated per function call** |
| **Needs `varaccess_*`?** | Yes (for global scripts) | **NO** - it's local |

### Why This Matters

Because `argument` is a local array:
- It doesn't need `varaccess_argument` function
- It doesn't need to go through `self`
- It should be accessed directly as `argument[N]`
- It's the same for both global scripts and object scripts

## Current Bug

The current codegen incorrectly treats `argument` as if it needs instance lookup:

**Incorrect (current codegen for global scripts):**
```cpp
// pretty_printer.cpp line 526
print("enigma::varaccess_argument(int(self))[int(");
```

**Correct (what it should be):**
```cpp
// Should be direct access, same as object scripts
print("argument[int(");
```

The bug also:
1. Adds `argument` to `dot_accessed_locals` in `collect_variables.cpp` (lines 295, 464)
2. Generates a dummy `varaccess_argument` function in `write_object_access.cpp` (lines 124-133)

## The Fix

1. **Remove `argument` from `dot_accessed_locals`** - it's not an instance variable
2. **Remove special case codegen** - always use `argument[int(N)]` directly
3. **Remove `varaccess_argument` generation** - it shouldn't exist

Both global scripts and object scripts should access `argument[N]` the same way: directly, as a local array.

## Regular `with` Statements vs Script `with(self)`

### Regular `with` Statements (User Code)

When you write `with(other)` or `with(obj_enemy)` in your GML code:

```gml
with (obj_enemy) {
    x = 100;
    y = 200;
}
```

This generates actual `with` statements in C++:
```cpp
with (obj_enemy) {
    x = 100;
    y = 200;
}
```

The `with` macro (defined in `write_object_data.cpp` line 1095) expands this to:
```cpp
for (enigma::iterator::with with(enigma::fetch_inst_iter_by_int(obj_enemy)); 
     enigma::instance_event_iterator; 
     enigma::instance_event_iterator = enigma::instance_event_iterator->next) {
    x = 100;
    y = 200;
}
```

**Key Point:** Inside a `with` block, `instance_event_iterator` is set to point to the object(s) in the `with` statement. When you access `x` or `y` inside the block, they should be accessed through `instance_event_iterator->inst->x`, **NOT** through `glaccess(int(self))->x`.

The `instance_event_iterator` is a global variable that tracks the "current instance" in the context. Inside a `with` block, it points to the object(s) being iterated over, not `self`.

**Important:** The codegen should track when we're inside a `with` block and access variables through `instance_event_iterator->inst` instead of `glaccess(int(self))`. Currently, the codegen may not properly distinguish this context.

### Script `with(self)` (Special Case)

Scripts are wrapped in `with(self)` during parsing, but this wrapper is **unwrapped** during codegen because:
- Scripts are standalone functions, not inside a `with` block
- The `self` variable is available in the function scope
- Instance variables are accessed via `glaccess(int(self))->x` instead

This unwrapping is **only** done for `with(self)` in scripts, as detected by checking if the `with` statement's object is the identifier `"self"` (see `ast.cpp` lines 47-60, 102-115).

## Summary

- `with(self)` wrapper is used during parsing to establish scope context for scripts
- **Only** `with(self)` in scripts is unwrapped during codegen; regular `with` statements are kept as-is
- After unwrapping, instance variables are accessed via `glaccess(int(self))`
- `argument` is a **local array** in the function, not an instance variable
- `argument[N]` should always be accessed directly, never via `varaccess_argument`
- The current codegen incorrectly treats `argument` as an instance variable
