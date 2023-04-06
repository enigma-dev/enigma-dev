/** Copyright (C) 2023-2024 Saif Kandil
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

#include "include.h"

/*
	Starting point of the extension.
    This function is called in get_password() function which implements GUI in the game so that players can enter-
    their credentials.
*/

void handle_error( const char* str )
{
        if (str)
        {
                DEBUG_MESSAGE(std::string("Steamworks Error: cannot create ") + str + std::string("file"), MESSAGE_TYPE::M_ERROR);
                exit( EXIT_FAILURE );
        }
}

extern "C" void __cdecl SteamAPIDebugTextHook( int nSeverity, const char *pchDebugText )
{
	// ::OutputDebugStringA( pchDebugText );

    DEBUG_MESSAGE(pchDebugText, M_INFO);

	if ( nSeverity >= 1 )
	{
		int x = 3;
		(void)x;
	}
}

namespace {
    void create_appid_file() {
        ofstream appID_file;
        std::string filename = fs::temp_directory_path().string() + "steam_appid.txt";
        std::string appID;
        appID_file.open (filename.c_str());

        if (appID_file.is_open()) {

            appID_file << "480"; // appID for spacewar game which is the default game for steamworks SDK
            appID_file.close();
        }
        else 
            handle_error(filename.c_str());
    }
}

namespace enigma {
    void init() {
        extension_update_hooks.push_back(create_appid_file);
    }

    class GeneralAPI
    {
    public:
        void steam_init_test();
        void periodic_test();
        GeneralAPI() = default;
        ~GeneralAPI();
    };
    
    GeneralAPI::~GeneralAPI()
    {
    }

    void GeneralAPI::steam_init_test() {
        if ( SteamAPI_RestartAppIfNecessary( k_uAppIdInvalid ) ) // Replace "k_uAppIdInvalid" with your app id
	    {
		    cout << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n" << endl;
	    }
        else {
            cout << "SteamAPI_RestartAppIfNecessary completed successfully.\n" << endl;
        }

        // return SteamAPI_Init();

	    if ( !SteamAPI_Init() )
	    {
		    cout << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n" << endl;
	    }
        else {
            cout << "SteamAPI_Init completed successfully.\n" << endl;

            SteamClient()->SetWarningMessageHook( &SteamAPIDebugTextHook );
        }
    }  // steam_initialised() function

    void GeneralAPI::periodic_test() {
        SteamAPI_RunCallbacks();
        cout << "Working...\n" << endl;
    }
    
}

namespace enigma_user {

    void test() {
        create_appid_file();
    }

    void steam_init() {
        if ( SteamAPI_RestartAppIfNecessary( k_uAppIdInvalid ) ) // Replace "k_uAppIdInvalid" with your app id
            return;

	    if ( !SteamAPI_Init() ) return;
    }

    void steam_update() {
        SteamAPI_RunCallbacks();
    }

    void steam_shutdown() {
        SteamAPI_Shutdown();
    }

    void steam_initialised() {
        enigma::GeneralAPI generalAPI;

        generalAPI.steam_init_test();

    }  // steam_initialised() function

    void steam_periodic_test() {
        enigma::GeneralAPI generalAPI;

        generalAPI.periodic_test();
    } // steam_periodic_test() function

    // Ensure that the user has logged into Steam. This will always return true if the game is launched
    // from Steam, but if Steam is at the login prompt when you run your game from the debugger, it
    // will return false.
    void steam_is_user_logged_on() {
        if (!SteamUser()->BLoggedOn()) {
            cout << "Fatal Error - Steam user must be logged in to play this game (SteamUser()->BLoggedOn() returned false).\n" << endl;
        } else {
            cout << "Steam user is logged (SteamUser()->BLoggedOn() returned true).\n" << endl;
        }
    }  // steam_is_user_logged_on() function

}  // namespace enigma

