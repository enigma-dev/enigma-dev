/** Copyright (C) 2013 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY {
} without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "DSvideo.h"
#include <windows.h>
#include <dshow.h>

namespace enigma_user {

void action_show_video(string fname, bool windowed, bool loop) {
    IGraphBuilder *pGraph = NULL;
    IMediaControl *pControl = NULL;
    IMediaEvent   *pEvent = NULL;

    // Initialize the COM library.
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
	   MessageBox(NULL, "Failed to initialize COM library.", "ERROR", MB_ICONERROR | MB_OK);
        return;
    }

    // Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                        IID_IGraphBuilder, (void **)&pGraph);
    if (FAILED(hr))
    {
		MessageBox(NULL, "Failed to create the Filter Graph Manager.", "ERROR", MB_ICONERROR | MB_OK);
        return;
    }

    hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

	// Build the graph.
    hr = pGraph->RenderFile((LPCWSTR)fname.c_str(), NULL);
	MessageBox(NULL, fname.c_str(), "ERROR", MB_ICONERROR | MB_OK);
    if (SUCCEEDED(hr))
    {
        // Run the graph.
        hr = pControl->Run();
        if (SUCCEEDED(hr))
        {
            // Wait for completion.
            long evCode;
            pEvent->WaitForCompletion(INFINITE, &evCode);
        } else {
			MessageBox(NULL, "Failed to run the COM control.", "ERROR", MB_ICONERROR | MB_OK);
			// Don't return so pointers can be released.
		}
    } else {
		MessageBox(NULL, "Failed to render the Filter Graph Manager.", "ERROR", MB_ICONERROR | MB_OK);
		// Don't return so pointers can be released.
    }

    pControl->Release();
    pEvent->Release();
    pGraph->Release();
    CoUninitialize();
}

}