#include "GameLoop.h"

#include "Universal_System/loading.h" // initialize_everything()
#include "Universal_System/CallbackArrays.h" // input_push

#include <unistd.h> //usleep
#include <time.h>


namespace enigma
{

//TODO: Implement pause events
unsigned long current_time_mcs = 0; // microseconds since the start of the game
int game_return = 0;
void ENIGMA_events(void); //TODO: Synchronize this with Windows by putting these two in a single header.
bool gameWindowFocused = false;
unsigned int pausedSteps = 0;
bool game_isending = false;

std::string* parameters;
int parameterc;
int current_room_speed;
int cursorInt;

bool isVisible = true, isMinimized = false, isMaximized = false, stayOnTop = false, windowAdapt = true;
int regionWidth = 0, regionHeight = 0, windowWidth = 0, windowHeight = 0;
double scaledWidth = 0, scaledHeight = 0;


void windowsystem_write_exename(char* x)
{
  unsigned irx = 0;
  if (enigma::parameterc)
    for (irx = 0; enigma::parameters[0][irx] != 0; irx++)
      x[irx] = enigma::parameters[0][irx];
  x[irx] = 0;
}

#define hielem 9
void set_room_speed(int rs)
{
  current_room_speed = rs;
}

static inline long clamp(long value, long min, long max)
{
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

void Sleep(int ms)
{
	if(ms>=1000) sleep(ms/1000);
	if(ms>0)	usleep(ms%1000*1000);
}

void sleep(int ms) { Sleep(ms); }

int gameLoop(int argc,char** argv)
{
    // Copy our parameters
    enigma::parameters = new std::string[argc];
    enigma::parameterc = argc;
    for (int i=0; i<argc; i++)
        enigma::parameters[i]=argv[i];

    //Call ENIGMA system initializers; sprites, audio, and what have you
    enigma::initialize_everything();
    
    struct timespec time_offset;
    struct timespec time_offset_slowing;
    struct timespec time_current;
    clock_gettime(CLOCK_MONOTONIC, &time_offset);
    time_offset_slowing.tv_sec = time_offset.tv_sec;
    time_offset_slowing.tv_nsec = time_offset.tv_nsec;
    int frames_count = 0;

    while (!game_isending)
    {
        using enigma::current_room_speed;
        clock_gettime(CLOCK_MONOTONIC, &time_current);
        {
            long passed_mcs = (time_current.tv_sec - time_offset.tv_sec)*1000000 + (time_current.tv_nsec/1000 - + time_offset.tv_nsec/1000);
            passed_mcs = clamp(passed_mcs, 0, 1000000);
            if (passed_mcs >= 1000000) { // Handle resetting.

                enigma_user::fps = frames_count;
                frames_count = 0;
                time_offset.tv_sec += passed_mcs/1000000;
                time_offset_slowing.tv_sec = time_offset.tv_sec;
                time_offset_slowing.tv_nsec = time_offset.tv_nsec;
            }
        }
        long spent_mcs = 0;
        long last_mcs = 0;
        if (current_room_speed > 0) {
            spent_mcs = (time_current.tv_sec - time_offset_slowing.tv_sec)*1000000 + (time_current.tv_nsec/1000 - time_offset_slowing.tv_nsec/1000);
            spent_mcs = clamp(spent_mcs, 0, 1000000);
            long remaining_mcs = 1000000 - spent_mcs;
            long needed_mcs = long((1.0 - 1.0*frames_count/current_room_speed)*1e6);
            const int catchup_limit_ms = 50;
            if (needed_mcs > remaining_mcs + catchup_limit_ms*1000) {
                // If more than catchup_limit ms is needed than is remaining, we risk running too fast to catch up.
                // In order to avoid running too fast, we advance the offset, such that we are only at most catchup_limit ms behind.
                // Thus, if the load is consistently making the game slow, the game is still allowed to run as fast as possible
                // without any sleep.
                // And if there is very heavy load once in a while, the game will only run too fast for catchup_limit ms.
                time_offset_slowing.tv_nsec += 1000*(needed_mcs - (remaining_mcs + catchup_limit_ms*1000));
                spent_mcs = (time_current.tv_sec - time_offset_slowing.tv_sec)*1000000 + (time_current.tv_nsec/1000 - time_offset_slowing.tv_nsec/1000);
                spent_mcs = clamp(spent_mcs, 0, 1000000);
                remaining_mcs = 1000000 - spent_mcs;
                needed_mcs = long((1.0 - 1.0*frames_count/current_room_speed)*1e6);
            }
            if (remaining_mcs > needed_mcs) {
                const long sleeping_time = std::min((remaining_mcs - needed_mcs)/5, long(999999));
                usleep(std::max(long(1), sleeping_time));
                continue;
            }
        }

        //TODO: The placement of this code is inconsistent with Win32 because events are handled after, ask Josh.
        unsigned long dt = 0;
        if (spent_mcs > last_mcs) {
            dt = (spent_mcs - last_mcs);
        } else {
            //TODO: figure out what to do here this happens when the fps is reached and the timers start over
            dt = enigma_user::delta_time;
        }
        last_mcs = spent_mcs;
        enigma_user::delta_time = dt;
        current_time_mcs += enigma_user::delta_time;
        enigma_user::current_time += enigma_user::delta_time / 1000;

        if (!enigma::gameWindowFocused && enigma::freezeOnLoseFocus) {
          if (enigma::pausedSteps < 1) {
            enigma::pausedSteps += 1;
          } else {
            usleep(100000);
            continue;
          }
        }
        
        enigma::platformEvents();
        enigma::ENIGMA_events();
        enigma::input_push();

        frames_count++;
    }

    enigma::game_ending();
    return enigma::game_return;
}

}