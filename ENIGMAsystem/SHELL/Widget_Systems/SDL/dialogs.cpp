/*

 MIT License

 Copyright © 2021-2022 Samuel Venable

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*/

#include <string>
#include <cstdlib>
#include <cstdio>

#include "Widget_Systems/SDL/filedialogs.hpp"
#include "Widget_Systems/SDL/dialogs.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Universal_System/directoryglobals.h"
#include "Universal_System/estring.h"
#include "Universal_System/fileio.h"
#include "Platforms/SDL/Window.h"
#include "OpenGLHeaders.h"

using std::string;

namespace enigma {

  bool widget_system_initialize() {
    enigma_user::environment_set_variable("IMGUI_DIALOG_PARENT", std::to_string((std::uint64_t)(void *)enigma_user::window_handle()));
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
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/000-notosans-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/001-notokufiarabic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/002-notomusic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/003-notonaskharabic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/004-notonaskharabicui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/005-notonastaliqurdu-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/006-notosansadlam-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/007-notosansadlamunjoined-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/008-notosansanatolianhieroglyphs-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/009-notosansarabic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/010-notosansarabicui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/011-notosansarmenian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/012-notosansavestan-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/013-notosansbamum-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/014-notosansbassavah-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/015-notosansbatak-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/016-notosansbengali-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/017-notosansbengaliui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/018-notosansbhaiksuki-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/019-notosansbrahmi-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/020-notosansbuginese-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/021-notosansbuhid-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/022-notosanscanadianaboriginal-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/023-notosanscarian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/024-notosanscaucasianalbanian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/025-notosanschakma-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/026-notosanscham-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/027-notosanscherokee-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/028-notosanscoptic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/029-notosanscuneiform-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/030-notosanscypriot-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/031-notosansdeseret-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/032-notosansdevanagari-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/033-notosansdevanagariui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/034-notosansdisplay-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/035-notosansduployan-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/036-notosansegyptianhieroglyphs-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/037-notosanselbasan-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/038-notosansethiopic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/039-notosansgeorgian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/040-notosansglagolitic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/041-notosansgothic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/042-notosansgrantha-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/043-notosansgujarati-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/044-notosansgujaratiui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/045-notosansgurmukhi-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/046-notosansgurmukhiui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/047-notosanshanifirohingya-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/048-notosanshanunoo-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/049-notosanshatran-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/050-notosanshebrew-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/051-notosansimperialaramaic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/052-notosansindicsiyaqnumbers-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/053-notosansinscriptionalpahlavi-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/054-notosansinscriptionalparthian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/055-notosansjavanese-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/056-notosanskaithi-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/057-notosanskannada-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/058-notosanskannadaui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/059-notosanskayahli-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/060-notosanskharoshthi-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/061-notosanskhmer-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/062-notosanskhmerui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/063-notosanskhojki-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/064-notosanskhudawadi-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/065-notosanslao-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/066-notosanslaoui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/067-notosanslepcha-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/068-notosanslimbu-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/069-notosanslineara-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/070-notosanslinearb-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/071-notosanslisu-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/072-notosanslycian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/073-notosanslydian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/074-notosansmahajani-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/075-notosansmalayalam-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/076-notosansmalayalamui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/077-notosansmandaic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/078-notosansmanichaean-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/079-notosansmarchen-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/080-notosansmath-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/081-notosansmayannumerals-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/082-notosansmeeteimayek-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/083-notosansmendekikakui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/084-notosansmeroitic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/085-notosansmiao-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/086-notosansmodi-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/087-notosansmongolian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/088-notosansmono-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/089-notosansmro-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/090-notosansmultani-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/091-notosansmyanmar-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/092-notosansmyanmarui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/093-notosansnabataean-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/094-notosansnewa-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/095-notosansnewtailue-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/096-notosansnko-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/097-notosansogham-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/098-notosansolchiki-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/099-notosansoldhungarian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/100-notosansolditalic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/101-notosansoldnortharabian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/102-notosansoldpermic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/103-notosansoldpersian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/104-notosansoldsogdian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/105-notosansoldsoutharabian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/106-notosansoldturkic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/107-notosansoriya-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/108-notosansoriyaui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/109-notosansosage-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/110-notosansosmanya-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/111-notosanspahawhhmong-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/112-notosanspalmyrene-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/113-notosanspaucinhau-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/114-notosansphagspa-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/115-notosansphoenician-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/116-notosanspsalterpahlavi-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/117-notosansrejang-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/118-notosansrunic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/119-notosanssamaritan-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/120-notosanssaurashtra-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/121-notosanssharada-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/122-notosansshavian-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/123-notosanssiddham-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/124-notosanssinhala-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/125-notosanssinhalaui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/126-notosanssorasompeng-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/127-notosanssundanese-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/128-notosanssylotinagri-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/129-notosanssymbols2-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/130-notosanssymbols-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/131-notosanssyriac-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/132-notosanstagalog-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/133-notosanstagbanwa-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/134-notosanstaile-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/135-notosanstaitham-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/136-notosanstaiviet-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/137-notosanstakri-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/138-notosanstamil-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/139-notosanstamilsupplement-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/140-notosanstamilui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/141-notosanstelugu-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/142-notosansteluguui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/143-notosansthaana-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/144-notosansthai-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/145-notosansthaiui-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/146-notosanstibetan-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/147-notosanstifinagh-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/148-notosanstirhuta-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/149-notosansugaritic-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/150-notosansvai-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/151-notosanswarangciti-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/152-notosansyi-regular.ttf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/153-notosanstc-regular.otf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/154-notosansjp-regular.otf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/155-notosanskr-regular.otf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/156-notosanssc-regular.otf\n") +
    enigma_user::filename_change_ext(enigma_user::executable_get_pathname(), "") + std::string("_files/fonts/157-notosanshk-regular.otf"));
    return true;
  }

}

namespace enigma_user {

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) { }

void show_info() { }

void show_debug_message(string errortext, MESSAGE_TYPE type) {
  #ifndef DEBUG_MODE
  errortext += "\n";
  fputs(errortext.c_str(), stderr);
  fflush(stderr);
  #endif
  if (type == MESSAGE_TYPE::M_FATAL_ERROR || 
    type == MESSAGE_TYPE::M_FATAL_USER_ERROR) {
    string ok = environment_get_variable("IMGUI_YES");
    string caption = environment_get_variable("IMGUI_DIALOG_CAPTION");
    environment_set_variable("IMGUI_DIALOG_CAPTION", "Fatal Error");
    environment_set_variable("IMGUI_OK", "Abort");
    show_message(errortext);
    environment_set_variable("IMGUI_DIALOG_CAPTION", caption);
    environment_set_variable("IMGUI_OK", ok);
    abort();
  } else if (type == MESSAGE_TYPE::M_ERROR || 
      type == MESSAGE_TYPE::M_USER_ERROR) {
    string yes = environment_get_variable("IMGUI_YES");
    string no = environment_get_variable("IMGUI_NO");
    string caption = environment_get_variable("IMGUI_DIALOG_CAPTION");
    environment_set_variable("IMGUI_DIALOG_CAPTION", "Error");
    environment_set_variable("IMGUI_YES", "Abort");
    environment_set_variable("IMGUI_NO", "Ignore");
    bool question = show_question(errortext);
    environment_set_variable("IMGUI_DIALOG_CAPTION", caption);
    environment_set_variable("IMGUI_YES", yes);
    environment_set_variable("IMGUI_NO", no);
    if (question) abort();
  }
}

int show_message(const string &message) {
  ::show_message(message.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return 1;
}

bool show_question(string message) {
  string result = ::show_question(message.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  string yes = (environment_get_variable("IMGUI_YES").empty() ? "Yes" : environment_get_variable("IMGUI_YES"));
  return ((result == yes) ? true : false);
}

int show_question_ext(string message) {
  string result = ::show_question_ext(message.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  string yes = (environment_get_variable("IMGUI_YES").empty() ? "Yes" : environment_get_variable("IMGUI_YES"));
  string no = (environment_get_variable("IMGUI_NO").empty() ? "No" : environment_get_variable("IMGUI_YES"));
  return ((result == yes) ? 1 : ((result == no) ? 0 : -1));
}

std::string get_string(std::string message, std::string defstr) {
  std::string result = ::get_string(message.c_str(), defstr.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return result;
}

double get_number(std::string message, double defnum) {
  double result = ::get_number(message.c_str(), defnum);
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return result;
}

double get_integer(std::string message, double defint) {
  return get_number(message, defint);
}

string get_open_filename(string filter, string fname) {
  string result = ::get_open_filename(filter.c_str(), fname.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return result;
}

string get_open_filename_ext(string filter, string fname, string title, string dir) {
  string result = ::get_open_filename_ext(filter.c_str(), fname.c_str(), title.c_str(), dir.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return result;
}

string get_open_filenames(string filter, string fname) {
  string result = ::get_open_filenames(filter.c_str(), fname.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return result;
}

string get_open_filenames_ext(string filter, string fname, string title, string dir) {
  string result = ::get_open_filenames_ext(filter.c_str(), fname.c_str(), title.c_str(), dir.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return result;
}

string get_save_filename(string filter, string fname) { 
  string result = ::get_save_filename(filter.c_str(), fname.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return result;
}

string get_save_filename_ext(string filter, string fname, string title, string dir) {
  string result = ::get_save_filename_ext(filter.c_str(), fname.c_str(), title.c_str(), dir.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return result;
}

string get_directory(string dname) {
  string result = ::get_directory(dname.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return result;
}

string get_directory_alt(string capt, string root) {
  string result = ::get_directory_alt(capt.c_str(), root.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  SDL_RaiseWindow(enigma::windowHandle);
  return result;
}

} // namespace enigma_user
