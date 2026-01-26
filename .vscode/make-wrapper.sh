#!/bin/bash
# Wrapper script to normalize file paths in make output for VSCode problem matcher

# Run make and capture output
make "$@" 2>&1 | while IFS= read -r line; do
    # Check if line matches error/warning pattern: file:line:column: severity: message
    if [[ $line =~ ^([^:]+):([0-9]+):([0-9]+):\ (warning|error|fatal\ error):\ (.*)$ ]] || \
       [[ $line =~ ^([^:]+):([0-9]+):\ (warning|error|fatal\ error):\ (.*)$ ]]; then
        filepath="${BASH_REMATCH[1]}"
        
        # Skip external library files (Homebrew, system headers, etc.)
        if [[ "$filepath" =~ ^/opt/homebrew ]] || \
           [[ "$filepath" =~ ^/usr/local ]] || \
           [[ "$filepath" =~ ^/Library/Developer ]] || \
           [[ "$filepath" =~ ^/Applications/Xcode ]]; then
            # Skip this line (don't output it)
            continue
        fi
        
        # Remove leading ./ if present
        filepath="${filepath#./}"
        
        # If path doesn't start with / and doesn't exist, try common subdirectories
        if [[ ! "$filepath" =~ ^/ ]] && [[ ! -f "$filepath" ]]; then
            # Try CompilerSource/ prefix
            if [[ -f "CompilerSource/$filepath" ]]; then
                filepath="CompilerSource/$filepath"
            # Try CommandLine/ prefix
            elif [[ -f "CommandLine/$filepath" ]]; then
                filepath="CommandLine/$filepath"
            # Try shared/ prefix
            elif [[ -f "shared/$filepath" ]]; then
                filepath="shared/$filepath"
            # Try ENIGMAsystem/ prefix
            elif [[ -f "ENIGMAsystem/$filepath" ]]; then
                filepath="ENIGMAsystem/$filepath"
            fi
        fi
        
        # Reconstruct the line with the corrected path
        if [[ $line =~ ^([^:]+):([0-9]+):([0-9]+):\ (warning|error|fatal\ error):\ (.*)$ ]]; then
            echo "$filepath:${BASH_REMATCH[2]}:${BASH_REMATCH[3]}: ${BASH_REMATCH[4]}: ${BASH_REMATCH[5]}"
        elif [[ $line =~ ^([^:]+):([0-9]+):\ (warning|error|fatal\ error):\ (.*)$ ]]; then
            echo "$filepath:${BASH_REMATCH[2]}: ${BASH_REMATCH[3]}: ${BASH_REMATCH[4]}"
        else
            echo "$line"
        fi
    else
        # Not an error line, pass through
        echo "$line"
    fi
done

# Preserve exit code
exit ${PIPESTATUS[0]}
