#include "PFmain.h"

#include "PFwindow.h"
#include "PFsystem.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/fileio.h"
#include "Universal_System/mathnc.h" // enigma_user::clamp

#include <chrono> // std::chrono::microseconds
#include <thread> // sleep_for

namespace enigma {

std::vector<std::function<void()> > extension_update_hooks;

bool game_isending = false;
int game_return = 0;
int pausedSteps = 0;
int current_room_speed;
std::string* parameters;
int parameterc;
int frames_count = 0;
// Monotic non-wall clock timer is required for accurate frame limiting.
// https://github.com/enigma-dev/enigma-dev/pull/2259
std::chrono::steady_clock::time_point timer_start;
std::chrono::steady_clock::time_point timer_offset;
std::chrono::steady_clock::time_point timer_offset_slowing;
std::chrono::steady_clock::time_point timer_current;
unsigned long current_time_mcs = 0;
bool game_window_focused = true;

void platform_focus_gained() {
  game_window_focused = true;
  pausedSteps = 0;
  input_initialize();
}

void platform_focus_lost() {
  game_window_focused = false;
  for (int i = 0; i < 255; i++) {
    last_keybdstatus[i] = keybdstatus[i];
    keybdstatus[i] = 0;
  }
  for (int i=0; i < 3; i++) {
    last_mousestatus[i] = mousestatus[i];
    mousestatus[i] = 0;
  }
}

int gameWait() {
  if (enigma_user::os_is_paused()) {
    if (pausedSteps < 1) {
      pausedSteps += 1;
    } else {
      std::this_thread::sleep_for(std::chrono::microseconds(100000));
      return -1;
    }
  }

  frames_count++;

  return 0;
}

void set_room_speed(int rs) { current_room_speed = rs; }

void set_program_args(int argc, char** argv) {
  parameters = new std::string[argc];
  parameterc = argc;
  for (int i = 0; i < argc; i++) parameters[i] = argv[i];
}

void initTimer() {
  timer_start = std::chrono::steady_clock::now();
  timer_offset = timer_start;
  timer_offset_slowing = timer_start;
  timer_current = timer_start;
}

void update_current_time() {
  timer_current = std::chrono::steady_clock::now();
}

long get_current_offset_difference_mcs() {
  auto delta = std::chrono::duration_cast<std::chrono::microseconds>(timer_current - timer_offset).count();
  return enigma_user::clamp(delta, 0, 1000000);
}

long get_current_offset_slowing_difference_mcs() {
  auto delta = std::chrono::duration_cast<std::chrono::microseconds>(timer_current - timer_offset_slowing).count();
  return enigma_user::clamp(delta, 0, 1000000);
}

void increase_offset_slowing(long increase_mcs) {
  timer_offset_slowing += std::chrono::microseconds(increase_mcs);
}

void offset_modulus_one_second() {
  long passed_mcs = get_current_offset_difference_mcs();
  // rounds towards 0
  timer_offset += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(passed_mcs));
  timer_offset_slowing = timer_offset;
}

long last_mcs = 0;
long spent_mcs = 0;
long remaining_mcs = 0;
long needed_mcs = 0;

int updateTimer() {
  // Update current time.
  update_current_time();
  {
    // Find diff between current and offset.

    long passed_mcs = enigma::get_current_offset_difference_mcs();
    if (passed_mcs >= 1000000) {  // Handle resetting.
      // If more than one second has passed, update fps variable, reset frames count,
      // and advance offset by difference in seconds, rounded down.

      enigma_user::fps = frames_count;
      frames_count = 0;
      enigma::offset_modulus_one_second();
    }
  }

  if (current_room_speed > 0) {
    spent_mcs = enigma::get_current_offset_slowing_difference_mcs();

    remaining_mcs = 1000000 - spent_mcs;
    needed_mcs = long((1.0 - 1.0 * frames_count / current_room_speed) * 1e6);
    const int catchup_limit_ms = 50;
    if (needed_mcs > remaining_mcs + catchup_limit_ms * 1000) {
      // If more than catchup_limit ms is needed than is remaining, we risk running too fast to catch up.
      // In order to avoid running too fast, we advance the offset, such that we are only at most catchup_limit ms behind.
      // Thus, if the load is consistently making the game slow, the game is still allowed to run as fast as possible
      // without any sleep.
      // And if there is very heavy load once in a while, the game will only run too fast for catchup_limit ms.
      enigma::increase_offset_slowing(needed_mcs - (remaining_mcs + catchup_limit_ms * 1000));

      spent_mcs = enigma::get_current_offset_slowing_difference_mcs();
      remaining_mcs = 1000000 - spent_mcs;
      needed_mcs = long((1.0 - 1.0 * frames_count / current_room_speed) * 1e6);
    }
    if (remaining_mcs > needed_mcs) {
      const long sleeping_time = std::min((remaining_mcs - needed_mcs) / 5, long(999999));
      std::this_thread::sleep_for(std::chrono::microseconds(std::max(long(1), sleeping_time)));
      return -1;
    }
  }

  //TODO: The placement of this code is inconsistent with XLIB because events are handled before, ask Josh.
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

  return 0;
}

int enigma_main(int argc, char** argv) {
  #if defined(BUILDMODE)
  enigma_user::directory_set_current_working(enigma_user::executable_get_directory());
  #endif
  
  // Copy our parameters
  set_program_args(argc, argv);
  
  #ifdef SDL_VIDEO_DRIVER_X11
  enigma_user::environment_set_variable("SDL_VIDEODRIVER", "x11");
  #endif

  if (!initGameWindow()) {
    DEBUG_MESSAGE("Failed to create game window", MESSAGE_TYPE::M_FATAL_ERROR);
    return -4;
  }

  enigma_user::environment_set_variable("IMGUI_DIALOG_WIDTH", std::to_string(720));
  enigma_user::environment_set_variable("IMGUI_DIALOG_HEIGHT", std::to_string(360));
  enigma_user::environment_set_variable("IMGUI_DIALOG_THEME", std::to_string(2));
  enigma_user::environment_set_variable("IMGUI_TEXT_COLOR_0", std::to_string(1));
  enigma_user::environment_set_variable("IMGUI_TEXT_COLOR_1", std::to_string(1));
  enigma_user::environment_set_variable("IMGUI_TEXT_COLOR_2", std::to_string(1));
  enigma_user::environment_set_variable("IMGUI_HEAD_COLOR_0", std::to_string(0.35));
  enigma_user::environment_set_variable("IMGUI_HEAD_COLOR_1", std::to_string(0.35));
  enigma_user::environment_set_variable("IMGUI_HEAD_COLOR_2", std::to_string(0.35));
  enigma_user::environment_set_variable("IMGUI_AREA_COLOR_0", std::to_string(0.05));
  enigma_user::environment_set_variable("IMGUI_AREA_COLOR_1", std::to_string(0.05));
  enigma_user::environment_set_variable("IMGUI_AREA_COLOR_2", std::to_string(0.05));
  enigma_user::environment_set_variable("IMGUI_BODY_COLOR_0", std::to_string(1));
  enigma_user::environment_set_variable("IMGUI_BODY_COLOR_1", std::to_string(1));
  enigma_user::environment_set_variable("IMGUI_BODY_COLOR_2", std::to_string(1));
  enigma_user::environment_set_variable("IMGUI_POPS_COLOR_0", std::to_string(0.07));
  enigma_user::environment_set_variable("IMGUI_POPS_COLOR_1", std::to_string(0.07));
  enigma_user::environment_set_variable("IMGUI_POPS_COLOR_2", std::to_string(0.07));
  enigma_user::environment_set_variable("IMGUI_FONT_FILES",
    std::string("assets/fonts/000-notosans-regular.ttf\n") +
    std::string("assets/fonts/001-notokufiarabic-regular.ttf\n") +
    std::string("assets/fonts/002-notomusic-regular.ttf\n") +
    std::string("assets/fonts/003-notonaskharabic-regular.ttf\n") +
    std::string("assets/fonts/004-notonaskharabicui-regular.ttf\n") +
    std::string("assets/fonts/005-notonastaliqurdu-regular.ttf\n") +
    std::string("assets/fonts/006-notosansadlam-regular.ttf\n") +
    std::string("assets/fonts/007-notosansadlamunjoined-regular.ttf\n") +
    std::string("assets/fonts/008-notosansanatolianhieroglyphs-regular.ttf\n") +
    std::string("assets/fonts/009-notosansarabic-regular.ttf\n") +
    std::string("assets/fonts/010-notosansarabicui-regular.ttf\n") +
    std::string("assets/fonts/011-notosansarmenian-regular.ttf\n") +
    std::string("assets/fonts/012-notosansavestan-regular.ttf\n") +
    std::string("assets/fonts/013-notosansbamum-regular.ttf\n") +
    std::string("assets/fonts/014-notosansbassavah-regular.ttf\n") +
    std::string("assets/fonts/015-notosansbatak-regular.ttf\n") +
    std::string("assets/fonts/016-notosansbengali-regular.ttf\n") +
    std::string("assets/fonts/017-notosansbengaliui-regular.ttf\n") +
    std::string("assets/fonts/018-notosansbhaiksuki-regular.ttf\n") +
    std::string("assets/fonts/019-notosansbrahmi-regular.ttf\n") +
    std::string("assets/fonts/020-notosansbuginese-regular.ttf\n") +
    std::string("assets/fonts/021-notosansbuhid-regular.ttf\n") +
    std::string("assets/fonts/022-notosanscanadianaboriginal-regular.ttf\n") +
    std::string("assets/fonts/023-notosanscarian-regular.ttf\n") +
    std::string("assets/fonts/024-notosanscaucasianalbanian-regular.ttf\n") +
    std::string("assets/fonts/025-notosanschakma-regular.ttf\n") +
    std::string("assets/fonts/026-notosanscham-regular.ttf\n") +
    std::string("assets/fonts/027-notosanscherokee-regular.ttf\n") +
    std::string("assets/fonts/028-notosanscoptic-regular.ttf\n") +
    std::string("assets/fonts/029-notosanscuneiform-regular.ttf\n") +
    std::string("assets/fonts/030-notosanscypriot-regular.ttf\n") +
    std::string("assets/fonts/031-notosansdeseret-regular.ttf\n") +
    std::string("assets/fonts/032-notosansdevanagari-regular.ttf\n") +
    std::string("assets/fonts/033-notosansdevanagariui-regular.ttf\n") +
    std::string("assets/fonts/034-notosansdisplay-regular.ttf\n") +
    std::string("assets/fonts/035-notosansduployan-regular.ttf\n") +
    std::string("assets/fonts/036-notosansegyptianhieroglyphs-regular.ttf\n") +
    std::string("assets/fonts/037-notosanselbasan-regular.ttf\n") +
    std::string("assets/fonts/038-notosansethiopic-regular.ttf\n") +
    std::string("assets/fonts/039-notosansgeorgian-regular.ttf\n") +
    std::string("assets/fonts/040-notosansglagolitic-regular.ttf\n") +
    std::string("assets/fonts/041-notosansgothic-regular.ttf\n") +
    std::string("assets/fonts/042-notosansgrantha-regular.ttf\n") +
    std::string("assets/fonts/043-notosansgujarati-regular.ttf\n") +
    std::string("assets/fonts/044-notosansgujaratiui-regular.ttf\n") +
    std::string("assets/fonts/045-notosansgurmukhi-regular.ttf\n") +
    std::string("assets/fonts/046-notosansgurmukhiui-regular.ttf\n") +
    std::string("assets/fonts/047-notosanshanifirohingya-regular.ttf\n") +
    std::string("assets/fonts/048-notosanshanunoo-regular.ttf\n") +
    std::string("assets/fonts/049-notosanshatran-regular.ttf\n") +
    std::string("assets/fonts/050-notosanshebrew-regular.ttf\n") +
    std::string("assets/fonts/051-notosansimperialaramaic-regular.ttf\n") +
    std::string("assets/fonts/052-notosansindicsiyaqnumbers-regular.ttf\n") +
    std::string("assets/fonts/053-notosansinscriptionalpahlavi-regular.ttf\n") +
    std::string("assets/fonts/054-notosansinscriptionalparthian-regular.ttf\n") +
    std::string("assets/fonts/055-notosansjavanese-regular.ttf\n") +
    std::string("assets/fonts/056-notosanskaithi-regular.ttf\n") +
    std::string("assets/fonts/057-notosanskannada-regular.ttf\n") +
    std::string("assets/fonts/058-notosanskannadaui-regular.ttf\n") +
    std::string("assets/fonts/059-notosanskayahli-regular.ttf\n") +
    std::string("assets/fonts/060-notosanskharoshthi-regular.ttf\n") +
    std::string("assets/fonts/061-notosanskhmer-regular.ttf\n") +
    std::string("assets/fonts/062-notosanskhmerui-regular.ttf\n") +
    std::string("assets/fonts/063-notosanskhojki-regular.ttf\n") +
    std::string("assets/fonts/064-notosanskhudawadi-regular.ttf\n") +
    std::string("assets/fonts/065-notosanslao-regular.ttf\n") +
    std::string("assets/fonts/066-notosanslaoui-regular.ttf\n") +
    std::string("assets/fonts/067-notosanslepcha-regular.ttf\n") +
    std::string("assets/fonts/068-notosanslimbu-regular.ttf\n") +
    std::string("assets/fonts/069-notosanslineara-regular.ttf\n") +
    std::string("assets/fonts/070-notosanslinearb-regular.ttf\n") +
    std::string("assets/fonts/071-notosanslisu-regular.ttf\n") +
    std::string("assets/fonts/072-notosanslycian-regular.ttf\n") +
    std::string("assets/fonts/073-notosanslydian-regular.ttf\n") +
    std::string("assets/fonts/074-notosansmahajani-regular.ttf\n") +
    std::string("assets/fonts/075-notosansmalayalam-regular.ttf\n") +
    std::string("assets/fonts/076-notosansmalayalamui-regular.ttf\n") +
    std::string("assets/fonts/077-notosansmandaic-regular.ttf\n") +
    std::string("assets/fonts/078-notosansmanichaean-regular.ttf\n") +
    std::string("assets/fonts/079-notosansmarchen-regular.ttf\n") +
    std::string("assets/fonts/080-notosansmath-regular.ttf\n") +
    std::string("assets/fonts/081-notosansmayannumerals-regular.ttf\n") +
    std::string("assets/fonts/082-notosansmeeteimayek-regular.ttf\n") +
    std::string("assets/fonts/083-notosansmendekikakui-regular.ttf\n") +
    std::string("assets/fonts/084-notosansmeroitic-regular.ttf\n") +
    std::string("assets/fonts/085-notosansmiao-regular.ttf\n") +
    std::string("assets/fonts/086-notosansmodi-regular.ttf\n") +
    std::string("assets/fonts/087-notosansmongolian-regular.ttf\n") +
    std::string("assets/fonts/088-notosansmono-regular.ttf\n") +
    std::string("assets/fonts/089-notosansmro-regular.ttf\n") +
    std::string("assets/fonts/090-notosansmultani-regular.ttf\n") +
    std::string("assets/fonts/091-notosansmyanmar-regular.ttf\n") +
    std::string("assets/fonts/092-notosansmyanmarui-regular.ttf\n") +
    std::string("assets/fonts/093-notosansnabataean-regular.ttf\n") +
    std::string("assets/fonts/094-notosansnewa-regular.ttf\n") +
    std::string("assets/fonts/095-notosansnewtailue-regular.ttf\n") +
    std::string("assets/fonts/096-notosansnko-regular.ttf\n") +
    std::string("assets/fonts/097-notosansogham-regular.ttf\n") +
    std::string("assets/fonts/098-notosansolchiki-regular.ttf\n") +
    std::string("assets/fonts/099-notosansoldhungarian-regular.ttf\n") +
    std::string("assets/fonts/100-notosansolditalic-regular.ttf\n") +
    std::string("assets/fonts/101-notosansoldnortharabian-regular.ttf\n") +
    std::string("assets/fonts/102-notosansoldpermic-regular.ttf\n") +
    std::string("assets/fonts/103-notosansoldpersian-regular.ttf\n") +
    std::string("assets/fonts/104-notosansoldsogdian-regular.ttf\n") +
    std::string("assets/fonts/105-notosansoldsoutharabian-regular.ttf\n") +
    std::string("assets/fonts/106-notosansoldturkic-regular.ttf\n") +
    std::string("assets/fonts/107-notosansoriya-regular.ttf\n") +
    std::string("assets/fonts/108-notosansoriyaui-regular.ttf\n") +
    std::string("assets/fonts/109-notosansosage-regular.ttf\n") +
    std::string("assets/fonts/110-notosansosmanya-regular.ttf\n") +
    std::string("assets/fonts/111-notosanspahawhhmong-regular.ttf\n") +
    std::string("assets/fonts/112-notosanspalmyrene-regular.ttf\n") +
    std::string("assets/fonts/113-notosanspaucinhau-regular.ttf\n") +
    std::string("assets/fonts/114-notosansphagspa-regular.ttf\n") +
    std::string("assets/fonts/115-notosansphoenician-regular.ttf\n") +
    std::string("assets/fonts/116-notosanspsalterpahlavi-regular.ttf\n") +
    std::string("assets/fonts/117-notosansrejang-regular.ttf\n") +
    std::string("assets/fonts/118-notosansrunic-regular.ttf\n") +
    std::string("assets/fonts/119-notosanssamaritan-regular.ttf\n") +
    std::string("assets/fonts/120-notosanssaurashtra-regular.ttf\n") +
    std::string("assets/fonts/121-notosanssharada-regular.ttf\n") +
    std::string("assets/fonts/122-notosansshavian-regular.ttf\n") +
    std::string("assets/fonts/123-notosanssiddham-regular.ttf\n") +
    std::string("assets/fonts/124-notosanssinhala-regular.ttf\n") +
    std::string("assets/fonts/125-notosanssinhalaui-regular.ttf\n") +
    std::string("assets/fonts/126-notosanssorasompeng-regular.ttf\n") +
    std::string("assets/fonts/127-notosanssundanese-regular.ttf\n") +
    std::string("assets/fonts/128-notosanssylotinagri-regular.ttf\n") +
    std::string("assets/fonts/129-notosanssymbols2-regular.ttf\n") +
    std::string("assets/fonts/130-notosanssymbols-regular.ttf\n") +
    std::string("assets/fonts/131-notosanssyriac-regular.ttf\n") +
    std::string("assets/fonts/132-notosanstagalog-regular.ttf\n") +
    std::string("assets/fonts/133-notosanstagbanwa-regular.ttf\n") +
    std::string("assets/fonts/134-notosanstaile-regular.ttf\n") +
    std::string("assets/fonts/135-notosanstaitham-regular.ttf\n") +
    std::string("assets/fonts/136-notosanstaiviet-regular.ttf\n") +
    std::string("assets/fonts/137-notosanstakri-regular.ttf\n") +
    std::string("assets/fonts/138-notosanstamil-regular.ttf\n") +
    std::string("assets/fonts/139-notosanstamilsupplement-regular.ttf\n") +
    std::string("assets/fonts/140-notosanstamilui-regular.ttf\n") +
    std::string("assets/fonts/141-notosanstelugu-regular.ttf\n") +
    std::string("assets/fonts/142-notosansteluguui-regular.ttf\n") +
    std::string("assets/fonts/143-notosansthaana-regular.ttf\n") +
    std::string("assets/fonts/144-notosansthai-regular.ttf\n") +
    std::string("assets/fonts/145-notosansthaiui-regular.ttf\n") +
    std::string("assets/fonts/146-notosanstibetan-regular.ttf\n") +
    std::string("assets/fonts/147-notosanstifinagh-regular.ttf\n") +
    std::string("assets/fonts/148-notosanstirhuta-regular.ttf\n") +
    std::string("assets/fonts/149-notosansugaritic-regular.ttf\n") +
    std::string("assets/fonts/150-notosansvai-regular.ttf\n") +
    std::string("assets/fonts/151-notosanswarangciti-regular.ttf\n") +
    std::string("assets/fonts/152-notosansyi-regular.ttf\n") +
    std::string("assets/fonts/153-notosanstc-regular.otf\n") +
    std::string("assets/fonts/154-notosansjp-regular.otf\n") +
    std::string("assets/fonts/155-notosanskr-regular.otf\n") +
    std::string("assets/fonts/156-notosanssc-regular.otf\n") +
    std::string("assets/fonts/157-notosanshk-regular.otf"));

  initTimer();
  initInput();

  EnableDrawing(nullptr);

  // Call ENIGMA system initializers; sprites, audio, and what have you
  initialize_everything();

  while (!game_isending) {

    if (!((std::string)enigma_user::room_caption).empty() && 
      enigma_user::window_get_caption() != enigma_user::room_caption)
      enigma_user::window_set_caption(enigma_user::room_caption);
    update_mouse_variables();

    if (updateTimer() != 0) continue;
    if (handleEvents() != 0) break;
    if (gameWait() != 0) continue;

    // if any extensions need updated, update them now
    // just before we fire off user events like step
    for (auto update_hook : extension_update_hooks)
      update_hook();

    ENIGMA_events();
    handleInput();
  }

  game_ending();
  DisableDrawing(nullptr);
  destroyWindow();
  return game_return;
}

}  //namespace enigma

namespace enigma_user {

int os_unknown   = -1;
int os_win32     =  0;
int os_win64     =  1;
int os_macosx    =  2;
int os_linux     =  3;
int os_freebsd   =  4;
int os_dragonfly =  5;
int os_netbsd    =  6;
int os_openbsd   =  7;
int os_sunos     =  8;
#if (defined(_WIN32) && !defined(_WIN64))
int os_windows = os_win32;
#elif (defined(_WIN32) && defined(_WIN64))
int os_windows = os_win64;
#else
int os_windows = os_unknown;
#endif
#if (defined(_WIN32) && !defined(_WIN64))
int os_type = os_win32;
#elif (defined(_WIN32) && defined(_WIN64))
int os_type = os_win64;
#elif (defined(__APPLE__) && defined(__MACH__))
int os_type = os_macosx;
#elif defined(__linux__)
int os_type = os_linux;
#elif defined(__FreeBSD__)
int os_type = os_freebsd;
#elif defined(__DragonFly__)
int os_type = os_dragonfly;
#elif defined(__NetBSD__)
int os_type = os_netbsd;
#elif defined(__OpenBSD__)
int os_type = os_openbsd;
#elif defined(__sun)
int os_type = os_sunos;
#else
int os_type = os_unknown;
#endif

std::string game_save_id = "";
std::string keyboard_string = "";
double fps = 0;
unsigned long delta_time = 0;
unsigned long current_time = 0;

bool os_is_paused() { return !enigma::game_window_focused && enigma::freezeOnLoseFocus; }

std::string parameter_string(int num) { return num < enigma::parameterc ? enigma::parameters[num] : ""; }

int parameter_count() { return enigma::parameterc; }

void sleep(int ms) { enigma::Sleep(ms); }

unsigned long get_timer() {
  enigma::update_current_time();
  return std::chrono::duration_cast<std::chrono::microseconds>(enigma::timer_current - enigma::timer_start).count();
}

void game_end(int ret) {
  enigma::game_isending = true;
  enigma::game_return = ret;
}

void game_end() {
  enigma::game_isending = true;
}

void action_end_game() { return game_end(); }

}  //namespace enigma_user
