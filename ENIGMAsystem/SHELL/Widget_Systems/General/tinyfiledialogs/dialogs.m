#import <Cocoa/Cocoa.h>

void cocoa_window_activate()
{
    [[NSApplication sharedApplication] activateIgnoringOtherApps : YES];
}

