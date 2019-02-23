/** Copyright (C) 2019 Samuel Venable
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#import <Cocoa/Cocoa.h>

int cocoa_show_message(const char *title, const char *str, bool error)
{
    NSString *myTitle = [NSString stringWithUTF8String:title];
    NSString *myStr = [NSString stringWithUTF8String:str];
    NSAlert *alert = [[NSAlert alloc] init];
    [[alert window] setTitle:myTitle];
    [alert setMessageText:myStr];
    [alert addButtonWithTitle:@"OK"];
    [alert setAlertStyle:1];
    if (error) [alert setAlertStyle:2];
    [alert runModal];
    [alert release];
    
    return 1;
}

int cocoa_show_question(const char *title, const char *str, bool error)
{
    NSString *myTitle = [NSString stringWithUTF8String:title];
    NSString *myStr = [NSString stringWithUTF8String:str];
    NSAlert *alert = [[NSAlert alloc] init];
    [[alert window] setTitle:myTitle];
    [alert setMessageText:myStr];
    [alert addButtonWithTitle:@"Yes"];
    [alert addButtonWithTitle:@"No"];
    [alert setAlertStyle:1];
    if (error) [alert setAlertStyle:2];
    NSModalResponse responseTag = [alert runModal];
    [alert release];
    
    if (responseTag == NSAlertFirstButtonReturn)
        return 1;
    
    return 0;
}

int cocoa_show_error(const char *str, bool abort)
{
    NSString *myStr;
    NSString *myMsg;
    
    if (abort)
    {
        myStr = [NSString stringWithUTF8String:str];
        myMsg = @"\n\nClick 'OK' to abort the application.";
        myStr = [myStr stringByAppendingString:myMsg];
        const char *cstrStr = [myStr UTF8String];
        
        return cocoa_show_message("Error", cstrStr, true);
    }
    else
    {
        myStr = [NSString stringWithUTF8String:str];
        myMsg = @"\n\nDo you want to abort the application?";
        myStr = [myStr  stringByAppendingString:myMsg];
        const char *cstrStr = [myStr UTF8String];
        
        return cocoa_show_question("Error", cstrStr, true);
    }
}

const char *cocoa_input_box(const char *title, const char *str, const char *def)
{
    NSString *myTitle = [NSString stringWithUTF8String:title];
    NSString *myStr = [NSString stringWithUTF8String:str];
    NSString *myDef = [NSString stringWithUTF8String:def];
    NSAlert *alert = [[NSAlert alloc] init];
    [[alert window] setTitle:myTitle];
    [alert setMessageText:myStr];
    [alert addButtonWithTitle:@"OK"];
    [alert addButtonWithTitle:@"Cancel"];
    
    NSTextField *input = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 22)];
    [[input cell] setWraps:NO];
    [[input cell] setScrollable:YES];
    [input setMaximumNumberOfLines:1];
    [input setStringValue:myDef];
    [alert setAccessoryView:input];
    NSView *myAccessoryView = [alert accessoryView];
    [[alert window] setInitialFirstResponder:myAccessoryView];
    NSModalResponse responseTag = [alert runModal];
    const char *result;
    
    if (responseTag == NSAlertFirstButtonReturn)
    {
        [input validateEditing];
        result = [[input stringValue] UTF8String];
    }
    else
        result = "";
    
    [input release];
    [alert release];
    
    return result;
}

const char *cocoa_password_box(const char *title, const char *str, const char *def)
{
    NSString *myTitle = [NSString stringWithUTF8String:title];
    NSString *myStr = [NSString stringWithUTF8String:str];
    NSString *myDef = [NSString stringWithUTF8String:def];
    NSAlert *alert = [[NSAlert alloc] init];
    [[alert window] setTitle:myTitle];
    [alert setMessageText:myStr];
    [alert addButtonWithTitle:@"OK"];
    [alert addButtonWithTitle:@"Cancel"];
    
    NSSecureTextField *input = [[NSSecureTextField alloc] initWithFrame:NSMakeRect(0, 0, 300, 22)];
    [[input cell] setWraps:NO];
    [[input cell] setScrollable:YES];
    [input setMaximumNumberOfLines:1];
    [input setStringValue:myDef];
    [alert setAccessoryView:input];
    NSView *myAccessoryView = [alert accessoryView];
    [[alert window] setInitialFirstResponder:myAccessoryView];
    NSModalResponse responseTag = [alert runModal];
    const char *result;
    
    if (responseTag == NSAlertFirstButtonReturn)
    {
        [input validateEditing];
        result = [[input stringValue] UTF8String];
    }
    else
        result = "";
    
    [input release];
    [alert release];
    
    return result;
}

const char *cocoa_get_open_filename(const char *title, const char *filter, const char *fname, const char *dir, const bool mselect)
{
    NSOpenPanel *oFilePanel = [NSOpenPanel openPanel];
    [oFilePanel setMessage:[NSString stringWithUTF8String:title]];
    [oFilePanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dir]]];
    [oFilePanel setCanChooseFiles:YES];
    [oFilePanel setCanChooseDirectories:NO];
    [oFilePanel setCanCreateDirectories:NO];
    [oFilePanel setResolvesAliases:YES];
    
    if (mselect)
        [oFilePanel setAllowsMultipleSelection:YES];
    else
        [oFilePanel setAllowsMultipleSelection:NO];
    
    NSView *openView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 431, 21)];
    NSTextField *openLabel = [[NSTextField alloc] initWithFrame:NSMakeRect(57, 0, 69, 22)];
    NSPopUpButton *openPop = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(104, 0, 255, 22)];
    NSString *openFilter = [NSString stringWithUTF8String:filter];
    bool openShowAccessory = true;
    bool openAllowAllFiles = false;
    
    if ([openFilter length] == 0 ||
        ([openFilter rangeOfString:@"|"].location != NSNotFound &&
         [openFilter rangeOfString:@"|"].location == 0))
        openShowAccessory = false;
    
    if ([openFilter length] == 0 ||
        [openFilter rangeOfString:@"|"].location == NSNotFound)
    {
        openFilter = [openFilter stringByAppendingString:@"|"];
        openAllowAllFiles = true;
    }
    
    int openIndex = 0;
    int openCount = 0, openLength = [openFilter length];
    NSRange openRange = NSMakeRange(0, openLength);
    
    while (openRange.location != NSNotFound)
    {
        openRange = [openFilter rangeOfString: @"|" options:0 range:openRange];
        
        if (openRange.location != NSNotFound)
        {
            openRange = NSMakeRange(openRange.location + openRange.length, openLength - (openRange.location + openRange.length));
            openCount += 1;
        }
    }
    
    NSString *openPattern = openFilter;
    openPattern = [openPattern stringByReplacingOccurrencesOfString:@"*." withString:@""];
    openPattern = [openPattern stringByReplacingOccurrencesOfString:@" " withString:@""];
    
    NSArray *openArray1 = [openFilter componentsSeparatedByString:@"|"];
    NSArray *openArray2 = [openPattern componentsSeparatedByString:@"|"];
    NSMutableArray *openPatternArray = [[NSMutableArray alloc] init];
    NSMutableArray *openDescrArray = [[NSMutableArray alloc] init];
    
    for (openIndex = 0; openIndex <= openCount; openIndex += 1)
    {
        if (openIndex % 2)
        {
            [openPatternArray addObject:[openArray2 objectAtIndex:openIndex]];
        }
        else
        {
            [openDescrArray addObject:[openArray1 objectAtIndex:openIndex]];
        }
    }
    
    NSString *selectedOpenPattern = [openPatternArray objectAtIndex:0];
    NSArray *openPatternItems = [selectedOpenPattern componentsSeparatedByString:@";"];
    [oFilePanel setAllowedFileTypes:openPatternItems];
    
    if ([openPatternItems containsObject:@"*"] || openAllowAllFiles || !openShowAccessory)
        [oFilePanel setAllowedFileTypes:nil];
    
    [openLabel setStringValue:@"Enable: "];
    [openLabel setBezeled:NO];
    [openLabel setDrawsBackground:NO];
    [openLabel setEditable:NO];
    [openLabel setSelectable:NO];
    [openView addSubview:openLabel];
    [openPop addItemsWithTitles:openDescrArray];
    [openPop selectItemWithTitle:[openDescrArray objectAtIndex:0]];
    [openView addSubview:openPop];
    [oFilePanel setAccessoryView:openView];
    
    if (!openShowAccessory)
        [oFilePanel setAccessoryView:nil];
    
    const char *theOpenResult = "";
    bool initOpenAccessory = false;
    int openPopIndex = 0;
    
    NSModalSession openSession = [NSApp beginModalSessionForWindow:oFilePanel];
    
    for (;;)
    {
        if ([NSApp runModalSession:openSession] == NSModalResponseOK)
        {
            NSURL *theOpenURL;
            NSString *theOpenFile;
            int openURLSize = [[oFilePanel URLs] count];
            
            if (openURLSize > 1)
            {
                NSMutableArray *openFileArray = [[NSMutableArray alloc] init];
                
                for (int openURLIndex = 0; openURLIndex < openURLSize; openURLIndex += 1)
                {
                    [openFileArray addObject:[[[oFilePanel URLs] objectAtIndex:openURLIndex] path]];
                }
                
                theOpenFile = [openFileArray componentsJoinedByString:@"\n"];
                [openFileArray release];
            }
            else
                theOpenFile = [[[oFilePanel URLs] objectAtIndex:0] path];
            
            theOpenResult = [theOpenFile UTF8String];
            break;
        }
        
        if ([NSApp runModalSession:openSession] == NSModalResponseCancel)
            break;
        
        if (![oFilePanel isAccessoryViewDisclosed] && !initOpenAccessory)
        {
            [oFilePanel setAccessoryViewDisclosed:YES];
            initOpenAccessory = true;
        }
        
        if (openShowAccessory)
        {
            if ([openPop indexOfSelectedItem] != openPopIndex)
            {
                selectedOpenPattern = [openPatternArray objectAtIndex:[openPop indexOfSelectedItem]];
                openPatternItems = [selectedOpenPattern componentsSeparatedByString:@";"];
                
                if ([openPatternItems containsObject:@"*"])
                    [oFilePanel setAllowedFileTypes:nil];
                else
                    [oFilePanel setAllowedFileTypes:openPatternItems];
                
                openPopIndex = [openPop indexOfSelectedItem];
            }
        }
    }
    
    [NSApp endModalSession:openSession];
    [oFilePanel close];
    
    [openPatternArray release];
    [openDescrArray release];
    [openLabel release];
    [openPop release];
    [openView release];
    
    return theOpenResult;
}

const char *cocoa_get_save_filename(const char *title, const char *filter, const char *fname, const char *dir)
{
    NSSavePanel *sFilePanel = [NSSavePanel savePanel];
    [sFilePanel setMessage:[NSString stringWithUTF8String:title]];
    [sFilePanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dir]]];
    [sFilePanel setNameFieldStringValue:[[[NSString stringWithUTF8String:fname] lastPathComponent] stringByDeletingPathExtension]];
    [sFilePanel setCanCreateDirectories:YES];
    
    NSView *saveView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 433, 21)];
    NSTextField *saveLabel = [[NSTextField alloc] initWithFrame:NSMakeRect(56, 0, 69, 22)];
    NSPopUpButton *savePop = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(105, 0, 255, 22)];
    NSString *saveFilter = [NSString stringWithUTF8String:filter];
    bool saveShowAccessory = true;
    bool saveAllowAllFiles = false;
    
    if ([saveFilter length] == 0 ||
        ([saveFilter rangeOfString:@"|"].location != NSNotFound &&
         [saveFilter rangeOfString:@"|"].location == 0))
        saveShowAccessory = false;
    
    if ([saveFilter length] == 0 ||
        [saveFilter rangeOfString:@"|"].location == NSNotFound)
    {
        saveFilter = [saveFilter stringByAppendingString:@"|"];
        saveAllowAllFiles = true;
    }
    
    int saveIndex = 0;
    int saveCount = 0, saveLength = [saveFilter length];
    NSRange saveRange = NSMakeRange(0, saveLength);
    
    while (saveRange.location != NSNotFound)
    {
        saveRange = [saveFilter rangeOfString: @"|" options:0 range:saveRange];
        
        if (saveRange.location != NSNotFound)
        {
            saveRange = NSMakeRange(saveRange.location + saveRange.length, saveLength - (saveRange.location + saveRange.length));
            saveCount += 1;
        }
    }
    
    NSString *savePattern = saveFilter;
    savePattern = [savePattern stringByReplacingOccurrencesOfString:@"*." withString:@""];
    savePattern = [savePattern stringByReplacingOccurrencesOfString:@" " withString:@""];
    
    NSArray *saveArray1 = [saveFilter componentsSeparatedByString:@"|"];
    NSArray *saveArray2 = [savePattern componentsSeparatedByString:@"|"];
    NSMutableArray *savePatternArray = [[NSMutableArray alloc] init];
    NSMutableArray *saveDescrArray = [[NSMutableArray alloc] init];
    
    for (saveIndex = 0; saveIndex <= saveCount; saveIndex += 1)
    {
        if (saveIndex % 2)
        {
            [savePatternArray addObject:[saveArray2 objectAtIndex:saveIndex]];
        }
        else
        {
            [saveDescrArray addObject:[saveArray1 objectAtIndex:saveIndex]];
        }
    }
    
    NSString *selectedSavePattern = [savePatternArray objectAtIndex:0];
    NSArray *savePatternItems = [selectedSavePattern componentsSeparatedByString:@";"];
    [sFilePanel setAllowedFileTypes:savePatternItems];
    
    [sFilePanel setAllowedFileTypes:savePatternItems];
    
    if ([savePatternItems containsObject:@"*"] || saveAllowAllFiles || !saveShowAccessory)
        [sFilePanel setAllowedFileTypes:nil];
    
    [saveLabel setStringValue:@"Format: "];
    [saveLabel setBezeled:NO];
    [saveLabel setDrawsBackground:NO];
    [saveLabel setEditable:NO];
    [saveLabel setSelectable:NO];
    [saveView addSubview:saveLabel];
    [savePop addItemsWithTitles:saveDescrArray];
    [savePop selectItemWithTitle:[saveDescrArray objectAtIndex:0]];
    [saveView addSubview:savePop];
    [sFilePanel setAccessoryView:saveView];
    
    if (!saveShowAccessory)
        [sFilePanel setAccessoryView:nil];
    
    const char *theSaveResult = "";
    int savePopIndex = 0;
    
    NSModalSession saveSession = [NSApp beginModalSessionForWindow:sFilePanel];
    
    for (;;)
    {
        if ([NSApp runModalSession:saveSession] == NSModalResponseOK)
        {
            NSURL *theSaveURL = [sFilePanel URL];
            NSString *theSaveFile = [theSaveURL path];
            theSaveResult = [theSaveFile UTF8String];
            break;
        }
        
        if ([NSApp runModalSession:saveSession] == NSModalResponseCancel)
            break;
        
        if (saveShowAccessory)
        {
            if ([savePop indexOfSelectedItem] != savePopIndex)
            {
                selectedSavePattern = [savePatternArray objectAtIndex:[savePop indexOfSelectedItem]];
                savePatternItems = [selectedSavePattern componentsSeparatedByString:@";"];
                
                if ([savePatternItems containsObject:@"*"])
                    [sFilePanel setAllowedFileTypes:nil];
                else
                    [sFilePanel setAllowedFileTypes:savePatternItems];
                
                savePopIndex = [savePop indexOfSelectedItem];
            }
        }
    }
    
    [NSApp endModalSession:saveSession];
    [sFilePanel close];
    
    [savePatternArray release];
    [saveDescrArray release];
    [saveLabel release];
    [savePop release];
    [saveView release];
    
    return theSaveResult;
}

const char *cocoa_get_directory(const char *title, const char *dname)
{
    NSOpenPanel* dirPanel = [NSOpenPanel openPanel];
    [dirPanel setMessage:[NSString stringWithUTF8String:title]];
    [dirPanel setDirectoryURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:dname]]];
    [dirPanel setPrompt:@"Choose"];
    [dirPanel setCanChooseFiles:NO];
    [dirPanel setCanChooseDirectories:YES];
    [dirPanel setCanCreateDirectories:YES];
    [dirPanel setAllowsMultipleSelection:NO];
    [dirPanel setResolvesAliases:YES];
    const char *theFolderResult = "";
    
    if ([dirPanel runModal] == NSModalResponseOK)
    {
        NSURL *theFolderURL = [[dirPanel URLs] objectAtIndex:0];
        NSString *theFolderPath = [[theFolderURL path] stringByAppendingString:@"/"];
        theFolderResult = [theFolderPath UTF8String];
    }
    
    return theFolderResult;
}

int cocoa_get_color(const char *title, int defcol)
{
    int redValue = (int)defcol & 0xFF;
    int greenValue = ((int)defcol >> 8) & 0xFF;
    int blueValue = ((int)defcol >> 16) & 0xFF;
    
    NSInteger buttonWidth = 82, buttonHeight = 30, rightofButtons = 8;
    NSString *myColorTitle = [NSString stringWithUTF8String:title];
    NSColor *myDefCol = [NSColor colorWithCalibratedRed:redValue green:greenValue blue:blueValue alpha:1.0f];
    NSColorPanel *myColorPanel = [NSColorPanel sharedColorPanel];
    
    [myColorPanel setFrame:NSMakeRect(0, 0, 229, 399) display:YES animate:NO];
    if ([myColorTitle length] > 0) [myColorPanel setTitle:myColorTitle];
    NSSize myColorSize = [[myColorPanel contentView] frame].size;
    NSView *myViewView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, buttonHeight)];
    NSView *myButtonView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, buttonHeight)];
    NSButton *myOKButton = [[NSButton alloc] initWithFrame:NSMakeRect(myColorSize.width - buttonWidth - rightofButtons, 0, buttonWidth, buttonHeight)];
    NSButton *myCancelButton = [[NSButton alloc] initWithFrame:NSMakeRect(myColorSize.width - (buttonWidth * 2) - rightofButtons, 0, buttonWidth, buttonHeight)];
    
    [myOKButton setTitle:@"OK"];
    [myOKButton setAlternateTitle:@"OK"];
    [myOKButton setBezelStyle:NSRoundedBezelStyle];
    [myCancelButton setTitle:@"Cancel"];
    [myCancelButton setAlternateTitle:@"Cancel"];
    [myCancelButton setBezelStyle:NSRoundedBezelStyle];
    [myButtonView addSubview:myOKButton];
    [myButtonView addSubview:myCancelButton];
    [myOKButton setKeyEquivalent:@"\r"];
    [myOKButton setNeedsDisplay:YES];
    [myViewView addSubview:myButtonView];
    [myColorPanel setColor:myDefCol];
    NSView *oldView = [myColorPanel contentView];
    [oldView retain];
    [myColorPanel setContentView:0];
    NSView *colorView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, myColorSize.height)];
    NSView *parentView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, myColorSize.width, myColorSize.height)];
    [colorView addSubview:oldView];
    [parentView addSubview:colorView];
    [parentView addSubview:myViewView];
    [myColorPanel setContentView:parentView];
    [[myColorPanel standardWindowButton:NSWindowCloseButton] setHidden:YES];
    [[myColorPanel standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
    [[myColorPanel standardWindowButton:NSWindowZoomButton] setHidden:YES];
    myColorPanel.styleMask &= ~NSWindowStyleMaskResizable;
    colorView.autoresizingMask =  NSViewMinYMargin;
    parentView.autoresizingMask = NSViewMaxYMargin;
    myViewView.autoresizingMask = NSViewWidthSizable | NSViewMinXMargin;
    myButtonView.autoresizingMask = NSViewMinXMargin;
    [myColorPanel setFrame:NSMakeRect(0, 0, 229, 399 + buttonHeight) display:YES animate:NO];
    [myColorPanel center];
    
    NSModalSession colorSession = [NSApp beginModalSessionForWindow:myColorPanel];
    NSColor *myColor = myDefCol;
    
    CGFloat r, g, b, a;
    int redIntValue, greenIntValue, blueIntValue;
    NSString *redHexValue, *greenHexValue, *blueHexValue;
    NSColor *convertedColor;
    
    int rescol = -1;
    bool colorOKPressed = false;
    
    for (;;)
    {
        if ([NSApp runModalSession:colorSession] != NSModalResponseContinue)
            break;
        
        if ([myOKButton state] == NSControlStateValueOn)
        {
            colorOKPressed = true;
            break;
        }
        
        if ([myCancelButton state] == NSControlStateValueOn)
            break;
    }
    
    if (colorOKPressed)
    {
        myColor = [myColorPanel color];
        convertedColor = [myColor colorUsingColorSpaceName:NSCalibratedRGBColorSpace];
        
        if (convertedColor)
        {
            [convertedColor getRed:&r green:&g blue:&b alpha:&a];
            redIntValue = (int)(r * 255);
            greenIntValue = (int)(g * 255);
            blueIntValue = (int)(b * 255);
            
            rescol = (int)(redIntValue & 0xff) + ((greenIntValue & 0xff) << 8) + ((blueIntValue & 0xff) << 16);
        }
    }
    
    [myColorPanel close];
    [NSApp endModalSession:colorSession];
    [myColorPanel setContentView:oldView];
    
    [oldView release];
    [colorView release];
    [myOKButton release];
    [myCancelButton release];
    [myButtonView release];
    [myViewView release];
    [parentView release];
    
    return rescol;
}
