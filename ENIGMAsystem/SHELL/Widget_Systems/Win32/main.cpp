#include <string>
#define WINVER 9001
#include <windows.h>
#define _WIN32_IE 9001
#include <commctrl.h>

using namespace std;

#include "widgets.h"

/*  Declare Windows procedure  */
static LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "CodeBlocksWindowsApp";

HINSTANCE enigmaHinstance;
HWND enigmaHwnd;

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    
    INITCOMMONCONTROLSEX iccex;
    iccex.dwSize = sizeof(iccex);
    iccex.dwICC  = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&iccex);
    
    enigmaHinstance = hThisInstance;
    
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    enigmaHwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variatigtkl_widgeton */
           szClassName,         /* Classname */
           "Code::Blocks Template Windows App",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (enigmaHwnd, nCmdShow);
    
    int mywin = wgt_window_create(128,24);
    int layout = wgt_layout_create(mywin,
                                   "ooooooo\n"
                                   "puxywhs\n"
                                   "frggiis",
                                   4,4);
    wgt_layout_insert_widget(layout,"p",wgt_button_create("Pause"));
    wgt_layout_insert_widget(layout,"u",wgt_button_create("Undo"));
    wgt_layout_insert_widget(layout,"f",wgt_button_create("Freeze"));
    wgt_layout_insert_widget(layout,"r",wgt_button_create("Redo"));
    wgt_layout_insert_widget(layout,"s",wgt_button_create("Stop"));
    wgt_layout_insert_widget(layout,"x",wgt_textline_create("X",4));
    wgt_layout_insert_widget(layout,"y",wgt_textline_create("Y",4));
    wgt_layout_insert_widget(layout,"w",wgt_textline_create("W",4));
    wgt_layout_insert_widget(layout,"h",wgt_textline_create("H",4));
    wgt_layout_insert_widget(layout,"g",wgt_checkbox_create("Grid"));
    wgt_layout_insert_widget(layout,"i",wgt_checkbox_create("Iso"));
    wgt_layout_insert_widget(layout,"o",wgt_combobox_create("Item A|Item B|Item C"));
    wgt_window_show(mywin);
    
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
        if (!wgt_exists(mywin)) { messages.wParam = 0; break; }
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
