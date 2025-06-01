#include <algorithm>
#include <thread>
#include <chrono>
#include <string>

#include "ENIGMAsystem/SHELL/Widget_Systems/SDL/filedialogs.hpp"
#include "shared/apifilesystem/filesystem.hpp"
#include "shared/apiprocess/process.hpp"

static std::string filename_name(std::string fname) {
  std::size_t fp = fname.find_last_of("/\\");
  return fname.substr(fp + 1);
}

int main() {
  ngs::fs::environment_set_variable("IMGUI_DIALOG_CAPTION", "emake (Helper GUI)");
  ngs::fs::environment_set_variable("IMGUI_DIALOG_WIDTH", std::to_string(720));
  ngs::fs::environment_set_variable("IMGUI_DIALOG_HEIGHT", std::to_string(360));
  ngs::fs::environment_set_variable("IMGUI_DIALOG_THEME", std::to_string(2));
  ngs::fs::environment_set_variable("IMGUI_TEXT_COLOR_0", std::to_string(1));
  ngs::fs::environment_set_variable("IMGUI_TEXT_COLOR_1", std::to_string(1));
  ngs::fs::environment_set_variable("IMGUI_TEXT_COLOR_2", std::to_string(1));
  ngs::fs::environment_set_variable("IMGUI_HEAD_COLOR_0", std::to_string(0.35));
  ngs::fs::environment_set_variable("IMGUI_HEAD_COLOR_1", std::to_string(0.35));
  ngs::fs::environment_set_variable("IMGUI_HEAD_COLOR_2", std::to_string(0.35));
  ngs::fs::environment_set_variable("IMGUI_AREA_COLOR_0", std::to_string(0.05));
  ngs::fs::environment_set_variable("IMGUI_AREA_COLOR_1", std::to_string(0.05));
  ngs::fs::environment_set_variable("IMGUI_AREA_COLOR_2", std::to_string(0.05));
  ngs::fs::environment_set_variable("IMGUI_BODY_COLOR_0", std::to_string(1));
  ngs::fs::environment_set_variable("IMGUI_BODY_COLOR_1", std::to_string(1));
  ngs::fs::environment_set_variable("IMGUI_BODY_COLOR_2", std::to_string(1));
  ngs::fs::environment_set_variable("IMGUI_POPS_COLOR_0", std::to_string(0.07));
  ngs::fs::environment_set_variable("IMGUI_POPS_COLOR_1", std::to_string(0.07));
  ngs::fs::environment_set_variable("IMGUI_POPS_COLOR_2", std::to_string(0.07));
  ngs::fs::environment_set_variable("IMGUI_FONT_FILES",
    std::string("fonts/000-notosans-regular.ttf\n") +
    std::string("fonts/001-notokufiarabic-regular.ttf\n") +
    std::string("fonts/002-notomusic-regular.ttf\n") +
    std::string("fonts/003-notonaskharabic-regular.ttf\n") +
    std::string("fonts/004-notonaskharabicui-regular.ttf\n") +
    std::string("fonts/005-notonastaliqurdu-regular.ttf\n") +
    std::string("fonts/006-notosansadlam-regular.ttf\n") +
    std::string("fonts/007-notosansadlamunjoined-regular.ttf\n") +
    std::string("fonts/008-notosansanatolianhieroglyphs-regular.ttf\n") +
    std::string("fonts/009-notosansarabic-regular.ttf\n") +
    std::string("fonts/010-notosansarabicui-regular.ttf\n") +
    std::string("fonts/011-notosansarmenian-regular.ttf\n") +
    std::string("fonts/012-notosansavestan-regular.ttf\n") +
    std::string("fonts/013-notosansbamum-regular.ttf\n") +
    std::string("fonts/014-notosansbassavah-regular.ttf\n") +
    std::string("fonts/015-notosansbatak-regular.ttf\n") +
    std::string("fonts/016-notosansbengali-regular.ttf\n") +
    std::string("fonts/017-notosansbengaliui-regular.ttf\n") +
    std::string("fonts/018-notosansbhaiksuki-regular.ttf\n") +
    std::string("fonts/019-notosansbrahmi-regular.ttf\n") +
    std::string("fonts/020-notosansbuginese-regular.ttf\n") +
    std::string("fonts/021-notosansbuhid-regular.ttf\n") +
    std::string("fonts/022-notosanscanadianaboriginal-regular.ttf\n") +
    std::string("fonts/023-notosanscarian-regular.ttf\n") +
    std::string("fonts/024-notosanscaucasianalbanian-regular.ttf\n") +
    std::string("fonts/025-notosanschakma-regular.ttf\n") +
    std::string("fonts/026-notosanscham-regular.ttf\n") +
    std::string("fonts/027-notosanscherokee-regular.ttf\n") +
    std::string("fonts/028-notosanscoptic-regular.ttf\n") +
    std::string("fonts/029-notosanscuneiform-regular.ttf\n") +
    std::string("fonts/030-notosanscypriot-regular.ttf\n") +
    std::string("fonts/031-notosansdeseret-regular.ttf\n") +
    std::string("fonts/032-notosansdevanagari-regular.ttf\n") +
    std::string("fonts/033-notosansdevanagariui-regular.ttf\n") +
    std::string("fonts/034-notosansdisplay-regular.ttf\n") +
    std::string("fonts/035-notosansduployan-regular.ttf\n") +
    std::string("fonts/036-notosansegyptianhieroglyphs-regular.ttf\n") +
    std::string("fonts/037-notosanselbasan-regular.ttf\n") +
    std::string("fonts/038-notosansethiopic-regular.ttf\n") +
    std::string("fonts/039-notosansgeorgian-regular.ttf\n") +
    std::string("fonts/040-notosansglagolitic-regular.ttf\n") +
    std::string("fonts/041-notosansgothic-regular.ttf\n") +
    std::string("fonts/042-notosansgrantha-regular.ttf\n") +
    std::string("fonts/043-notosansgujarati-regular.ttf\n") +
    std::string("fonts/044-notosansgujaratiui-regular.ttf\n") +
    std::string("fonts/045-notosansgurmukhi-regular.ttf\n") +
    std::string("fonts/046-notosansgurmukhiui-regular.ttf\n") +
    std::string("fonts/047-notosanshanifirohingya-regular.ttf\n") +
    std::string("fonts/048-notosanshanunoo-regular.ttf\n") +
    std::string("fonts/049-notosanshatran-regular.ttf\n") +
    std::string("fonts/050-notosanshebrew-regular.ttf\n") +
    std::string("fonts/051-notosansimperialaramaic-regular.ttf\n") +
    std::string("fonts/052-notosansindicsiyaqnumbers-regular.ttf\n") +
    std::string("fonts/053-notosansinscriptionalpahlavi-regular.ttf\n") +
    std::string("fonts/054-notosansinscriptionalparthian-regular.ttf\n") +
    std::string("fonts/055-notosansjavanese-regular.ttf\n") +
    std::string("fonts/056-notosanskaithi-regular.ttf\n") +
    std::string("fonts/057-notosanskannada-regular.ttf\n") +
    std::string("fonts/058-notosanskannadaui-regular.ttf\n") +
    std::string("fonts/059-notosanskayahli-regular.ttf\n") +
    std::string("fonts/060-notosanskharoshthi-regular.ttf\n") +
    std::string("fonts/061-notosanskhmer-regular.ttf\n") +
    std::string("fonts/062-notosanskhmerui-regular.ttf\n") +
    std::string("fonts/063-notosanskhojki-regular.ttf\n") +
    std::string("fonts/064-notosanskhudawadi-regular.ttf\n") +
    std::string("fonts/065-notosanslao-regular.ttf\n") +
    std::string("fonts/066-notosanslaoui-regular.ttf\n") +
    std::string("fonts/067-notosanslepcha-regular.ttf\n") +
    std::string("fonts/068-notosanslimbu-regular.ttf\n") +
    std::string("fonts/069-notosanslineara-regular.ttf\n") +
    std::string("fonts/070-notosanslinearb-regular.ttf\n") +
    std::string("fonts/071-notosanslisu-regular.ttf\n") +
    std::string("fonts/072-notosanslycian-regular.ttf\n") +
    std::string("fonts/073-notosanslydian-regular.ttf\n") +
    std::string("fonts/074-notosansmahajani-regular.ttf\n") +
    std::string("fonts/075-notosansmalayalam-regular.ttf\n") +
    std::string("fonts/076-notosansmalayalamui-regular.ttf\n") +
    std::string("fonts/077-notosansmandaic-regular.ttf\n") +
    std::string("fonts/078-notosansmanichaean-regular.ttf\n") +
    std::string("fonts/079-notosansmarchen-regular.ttf\n") +
    std::string("fonts/080-notosansmath-regular.ttf\n") +
    std::string("fonts/081-notosansmayannumerals-regular.ttf\n") +
    std::string("fonts/082-notosansmeeteimayek-regular.ttf\n") +
    std::string("fonts/083-notosansmendekikakui-regular.ttf\n") +
    std::string("fonts/084-notosansmeroitic-regular.ttf\n") +
    std::string("fonts/085-notosansmiao-regular.ttf\n") +
    std::string("fonts/086-notosansmodi-regular.ttf\n") +
    std::string("fonts/087-notosansmongolian-regular.ttf\n") +
    std::string("fonts/088-notosansmono-regular.ttf\n") +
    std::string("fonts/089-notosansmro-regular.ttf\n") +
    std::string("fonts/090-notosansmultani-regular.ttf\n") +
    std::string("fonts/091-notosansmyanmar-regular.ttf\n") +
    std::string("fonts/092-notosansmyanmarui-regular.ttf\n") +
    std::string("fonts/093-notosansnabataean-regular.ttf\n") +
    std::string("fonts/094-notosansnewa-regular.ttf\n") +
    std::string("fonts/095-notosansnewtailue-regular.ttf\n") +
    std::string("fonts/096-notosansnko-regular.ttf\n") +
    std::string("fonts/097-notosansogham-regular.ttf\n") +
    std::string("fonts/098-notosansolchiki-regular.ttf\n") +
    std::string("fonts/099-notosansoldhungarian-regular.ttf\n") +
    std::string("fonts/100-notosansolditalic-regular.ttf\n") +
    std::string("fonts/101-notosansoldnortharabian-regular.ttf\n") +
    std::string("fonts/102-notosansoldpermic-regular.ttf\n") +
    std::string("fonts/103-notosansoldpersian-regular.ttf\n") +
    std::string("fonts/104-notosansoldsogdian-regular.ttf\n") +
    std::string("fonts/105-notosansoldsoutharabian-regular.ttf\n") +
    std::string("fonts/106-notosansoldturkic-regular.ttf\n") +
    std::string("fonts/107-notosansoriya-regular.ttf\n") +
    std::string("fonts/108-notosansoriyaui-regular.ttf\n") +
    std::string("fonts/109-notosansosage-regular.ttf\n") +
    std::string("fonts/110-notosansosmanya-regular.ttf\n") +
    std::string("fonts/111-notosanspahawhhmong-regular.ttf\n") +
    std::string("fonts/112-notosanspalmyrene-regular.ttf\n") +
    std::string("fonts/113-notosanspaucinhau-regular.ttf\n") +
    std::string("fonts/114-notosansphagspa-regular.ttf\n") +
    std::string("fonts/115-notosansphoenician-regular.ttf\n") +
    std::string("fonts/116-notosanspsalterpahlavi-regular.ttf\n") +
    std::string("fonts/117-notosansrejang-regular.ttf\n") +
    std::string("fonts/118-notosansrunic-regular.ttf\n") +
    std::string("fonts/119-notosanssamaritan-regular.ttf\n") +
    std::string("fonts/120-notosanssaurashtra-regular.ttf\n") +
    std::string("fonts/121-notosanssharada-regular.ttf\n") +
    std::string("fonts/122-notosansshavian-regular.ttf\n") +
    std::string("fonts/123-notosanssiddham-regular.ttf\n") +
    std::string("fonts/124-notosanssinhala-regular.ttf\n") +
    std::string("fonts/125-notosanssinhalaui-regular.ttf\n") +
    std::string("fonts/126-notosanssorasompeng-regular.ttf\n") +
    std::string("fonts/127-notosanssundanese-regular.ttf\n") +
    std::string("fonts/128-notosanssylotinagri-regular.ttf\n") +
    std::string("fonts/129-notosanssymbols2-regular.ttf\n") +
    std::string("fonts/130-notosanssymbols-regular.ttf\n") +
    std::string("fonts/131-notosanssyriac-regular.ttf\n") +
    std::string("fonts/132-notosanstagalog-regular.ttf\n") +
    std::string("fonts/133-notosanstagbanwa-regular.ttf\n") +
    std::string("fonts/134-notosanstaile-regular.ttf\n") +
    std::string("fonts/135-notosanstaitham-regular.ttf\n") +
    std::string("fonts/136-notosanstaiviet-regular.ttf\n") +
    std::string("fonts/137-notosanstakri-regular.ttf\n") +
    std::string("fonts/138-notosanstamil-regular.ttf\n") +
    std::string("fonts/139-notosanstamilsupplement-regular.ttf\n") +
    std::string("fonts/140-notosanstamilui-regular.ttf\n") +
    std::string("fonts/141-notosanstelugu-regular.ttf\n") +
    std::string("fonts/142-notosansteluguui-regular.ttf\n") +
    std::string("fonts/143-notosansthaana-regular.ttf\n") +
    std::string("fonts/144-notosansthai-regular.ttf\n") +
    std::string("fonts/145-notosansthaiui-regular.ttf\n") +
    std::string("fonts/146-notosanstibetan-regular.ttf\n") +
    std::string("fonts/147-notosanstifinagh-regular.ttf\n") +
    std::string("fonts/148-notosanstirhuta-regular.ttf\n") +
    std::string("fonts/149-notosansugaritic-regular.ttf\n") +
    std::string("fonts/150-notosansvai-regular.ttf\n") +
    std::string("fonts/151-notosanswarangciti-regular.ttf\n") +
    std::string("fonts/152-notosansyi-regular.ttf\n") +
    std::string("fonts/153-notosanstc-regular.otf\n") +
    std::string("fonts/154-notosansjp-regular.otf\n") +
    std::string("fonts/155-notosanskr-regular.otf\n") +
    std::string("fonts/156-notosanssc-regular.otf\n") +
    std::string("fonts/157-notosanshk-regular.otf"));
  ngs::ps::NGS_PROCID pid = 0;
  std::string dir = ngs::fs::executable_get_directory();
  #if defined(_WIN32)
  std::replace(dir.begin(), dir.end(), '\\', '/');
  #endif
  ngs::fs::directory_set_current_working(dir);
  std::string emake = filename_name(ngs::fs::directory_contents_first(dir, "*.*", false, false));
  while (emake.substr(0, 5) != "emake" || emake == ngs::fs::executable_get_filename()) {
    emake = filename_name(ngs::fs::directory_contents_next());
  }
  ngs::fs::directory_contents_close();
  if (emake == ngs::fs::executable_get_filename()) {
    emake.clear();
  }
  if (ngs::fs::file_exists(dir + emake)) {
    #if defined(__OpenBSD__)
    std::string params = "--output \"" + dir + "build/Key to Success\" --platform \"SDL\" --mode \"Compile\" --graphics \"OpenGL1\" --audio \"OpenAL\" --widgets \"None\" --collision \"Precise\" --extensions \"Paths,Alarms\" --run \"" +
    dir + "example/Key to Success.gmx/Key to Success.project.gmx\"";
    #else
    std::string params = "--output \"" + dir + "build/Key to Success\" --platform \"SDL\" --mode \"Compile\" --graphics \"OpenGL1\" --audio \"SDL\" --widgets \"None\" --collision \"Precise\" --extensions \"Paths,Alarms\" --run \"" +
    dir + "example/Key to Success.gmx/Key to Success.project.gmx\"";
    #endif
    params = ngs::imgui::get_string("Enter command line parameters you would like to pass to emake,\n(\"" + dir + emake + "\"), here:", params);
    if (params.empty()) {
      return 0;
    }
    ngs::imgui::show_message("Please wait patiently while the command processes. You will be notified when it is complete.");
    pid = ngs::ps::spawn_child_proc_id("\"" + dir + emake + "\" " + params + " > \"" + dir + "complile.log\"", false);
  }
  if (pid) {
    while (!ngs::ps::child_proc_id_is_complete(pid))
      std::this_thread::sleep_for(std::chrono::seconds(1));
    ngs::ps::free_stdout_for_child_proc_id(pid);
    ngs::ps::free_stdin_for_child_proc_id(pid);
  }
  ngs::imgui::show_message("Command processed successfully. You may now check the \"" + dir + "complile.log\" file to see if the build succeeded or not.");
  return 0;
}
