#include "Platforms/General/PFmain.h"
#include <SDL2/SDL.h> //sdl does a #define main SDL_main...
#if defined(_WIN32)
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include <cstdio>
#include <windows.h>
#include "Win32/resources.h"
#include "apifilesystem/filesystem.hpp"
namespace {
  HRSRC        res[159];
  HGLOBAL      glb[159];
  DWORD        siz[159];
  void        *buf[159];
  std::string  pth[159] = {
    "assets\\fonts\\000-notosans-regular.ttf",
    "assets\\fonts\\001-notokufiarabic-regular.ttf",
    "assets\\fonts\\002-notomusic-regular.ttf",
    "assets\\fonts\\003-notonaskharabic-regular.ttf",
    "assets\\fonts\\004-notonaskharabicui-regular.ttf",
    "assets\\fonts\\005-notonastaliqurdu-regular.ttf",
    "assets\\fonts\\006-notosansadlam-regular.ttf",
    "assets\\fonts\\007-notosansadlamunjoined-regular.ttf",
    "assets\\fonts\\008-notosansanatolianhieroglyphs-regular.ttf",
    "assets\\fonts\\009-notosansarabic-regular.ttf",
    "assets\\fonts\\010-notosansarabicui-regular.ttf",
    "assets\\fonts\\011-notosansarmenian-regular.ttf",
    "assets\\fonts\\012-notosansavestan-regular.ttf",
    "assets\\fonts\\013-notosansbamum-regular.ttf",
    "assets\\fonts\\014-notosansbassavah-regular.ttf",
    "assets\\fonts\\015-notosansbatak-regular.ttf",
    "assets\\fonts\\016-notosansbengali-regular.ttf",
    "assets\\fonts\\017-notosansbengaliui-regular.ttf",
    "assets\\fonts\\018-notosansbhaiksuki-regular.ttf",
    "assets\\fonts\\019-notosansbrahmi-regular.ttf",
    "assets\\fonts\\020-notosansbuginese-regular.ttf",
    "assets\\fonts\\021-notosansbuhid-regular.ttf",
    "assets\\fonts\\022-notosanscanadianaboriginal-regular.ttf",
    "assets\\fonts\\023-notosanscarian-regular.ttf",
    "assets\\fonts\\024-notosanscaucasianalbanian-regular.ttf",
    "assets\\fonts\\025-notosanschakma-regular.ttf",
    "assets\\fonts\\026-notosanscham-regular.ttf",
    "assets\\fonts\\027-notosanscherokee-regular.ttf",
    "assets\\fonts\\028-notosanscoptic-regular.ttf",
    "assets\\fonts\\029-notosanscuneiform-regular.ttf",
    "assets\\fonts\\030-notosanscypriot-regular.ttf",
    "assets\\fonts\\031-notosansdeseret-regular.ttf",
    "assets\\fonts\\032-notosansdevanagari-regular.ttf",
    "assets\\fonts\\033-notosansdevanagariui-regular.ttf",
    "assets\\fonts\\034-notosansdisplay-regular.ttf",
    "assets\\fonts\\035-notosansduployan-regular.ttf",
    "assets\\fonts\\036-notosansegyptianhieroglyphs-regular.ttf",
    "assets\\fonts\\037-notosanselbasan-regular.ttf",
    "assets\\fonts\\038-notosansethiopic-regular.ttf",
    "assets\\fonts\\039-notosansgeorgian-regular.ttf",
    "assets\\fonts\\040-notosansglagolitic-regular.ttf",
    "assets\\fonts\\041-notosansgothic-regular.ttf",
    "assets\\fonts\\042-notosansgrantha-regular.ttf",
    "assets\\fonts\\043-notosansgujarati-regular.ttf",
    "assets\\fonts\\044-notosansgujaratiui-regular.ttf",
    "assets\\fonts\\045-notosansgurmukhi-regular.ttf",
    "assets\\fonts\\046-notosansgurmukhiui-regular.ttf",
    "assets\\fonts\\047-notosanshanifirohingya-regular.ttf",
    "assets\\fonts\\048-notosanshanunoo-regular.ttf",
    "assets\\fonts\\049-notosanshatran-regular.ttf",
    "assets\\fonts\\050-notosanshebrew-regular.ttf",
    "assets\\fonts\\051-notosansimperialaramaic-regular.ttf",
    "assets\\fonts\\052-notosansindicsiyaqnumbers-regular.ttf",
    "assets\\fonts\\053-notosansinscriptionalpahlavi-regular.ttf",
    "assets\\fonts\\054-notosansinscriptionalparthian-regular.ttf",
    "assets\\fonts\\055-notosansjavanese-regular.ttf",
    "assets\\fonts\\056-notosanskaithi-regular.ttf",
    "assets\\fonts\\057-notosanskannada-regular.ttf",
    "assets\\fonts\\058-notosanskannadaui-regular.ttf",
    "assets\\fonts\\059-notosanskayahli-regular.ttf",
    "assets\\fonts\\060-notosanskharoshthi-regular.ttf",
    "assets\\fonts\\061-notosanskhmer-regular.ttf",
    "assets\\fonts\\062-notosanskhmerui-regular.ttf",
    "assets\\fonts\\063-notosanskhojki-regular.ttf",
    "assets\\fonts\\064-notosanskhudawadi-regular.ttf",
    "assets\\fonts\\065-notosanslao-regular.ttf",
    "assets\\fonts\\066-notosanslaoui-regular.ttf",
    "assets\\fonts\\067-notosanslepcha-regular.ttf",
    "assets\\fonts\\068-notosanslimbu-regular.ttf",
    "assets\\fonts\\069-notosanslineara-regular.ttf",
    "assets\\fonts\\070-notosanslinearb-regular.ttf",
    "assets\\fonts\\071-notosanslisu-regular.ttf",
    "assets\\fonts\\072-notosanslycian-regular.ttf",
    "assets\\fonts\\073-notosanslydian-regular.ttf",
    "assets\\fonts\\074-notosansmahajani-regular.ttf",
    "assets\\fonts\\075-notosansmalayalam-regular.ttf",
    "assets\\fonts\\076-notosansmalayalamui-regular.ttf",
    "assets\\fonts\\077-notosansmandaic-regular.ttf",
    "assets\\fonts\\078-notosansmanichaean-regular.ttf",
    "assets\\fonts\\079-notosansmarchen-regular.ttf",
    "assets\\fonts\\080-notosansmath-regular.ttf",
    "assets\\fonts\\081-notosansmayannumerals-regular.ttf",
    "assets\\fonts\\082-notosansmeeteimayek-regular.ttf",
    "assets\\fonts\\083-notosansmendekikakui-regular.ttf",
    "assets\\fonts\\084-notosansmeroitic-regular.ttf",
    "assets\\fonts\\085-notosansmiao-regular.ttf",
    "assets\\fonts\\086-notosansmodi-regular.ttf",
    "assets\\fonts\\087-notosansmongolian-regular.ttf",
    "assets\\fonts\\088-notosansmono-regular.ttf",
    "assets\\fonts\\089-notosansmro-regular.ttf",
    "assets\\fonts\\090-notosansmultani-regular.ttf",
    "assets\\fonts\\091-notosansmyanmar-regular.ttf",
    "assets\\fonts\\092-notosansmyanmarui-regular.ttf",
    "assets\\fonts\\093-notosansnabataean-regular.ttf",
    "assets\\fonts\\094-notosansnewa-regular.ttf",
    "assets\\fonts\\095-notosansnewtailue-regular.ttf",
    "assets\\fonts\\096-notosansnko-regular.ttf",
    "assets\\fonts\\097-notosansogham-regular.ttf",
    "assets\\fonts\\098-notosansolchiki-regular.ttf",
    "assets\\fonts\\099-notosansoldhungarian-regular.ttf",
    "assets\\fonts\\100-notosansolditalic-regular.ttf",
    "assets\\fonts\\101-notosansoldnortharabian-regular.ttf",
    "assets\\fonts\\102-notosansoldpermic-regular.ttf",
    "assets\\fonts\\103-notosansoldpersian-regular.ttf",
    "assets\\fonts\\104-notosansoldsogdian-regular.ttf",
    "assets\\fonts\\105-notosansoldsoutharabian-regular.ttf",
    "assets\\fonts\\106-notosansoldturkic-regular.ttf",
    "assets\\fonts\\107-notosansoriya-regular.ttf",
    "assets\\fonts\\108-notosansoriyaui-regular.ttf",
    "assets\\fonts\\109-notosansosage-regular.ttf",
    "assets\\fonts\\110-notosansosmanya-regular.ttf",
    "assets\\fonts\\111-notosanspahawhhmong-regular.ttf",
    "assets\\fonts\\112-notosanspalmyrene-regular.ttf",
    "assets\\fonts\\113-notosanspaucinhau-regular.ttf",
    "assets\\fonts\\114-notosansphagspa-regular.ttf",
    "assets\\fonts\\115-notosansphoenician-regular.ttf",
    "assets\\fonts\\116-notosanspsalterpahlavi-regular.ttf",
    "assets\\fonts\\117-notosansrejang-regular.ttf",
    "assets\\fonts\\118-notosansrunic-regular.ttf",
    "assets\\fonts\\119-notosanssamaritan-regular.ttf",
    "assets\\fonts\\120-notosanssaurashtra-regular.ttf",
    "assets\\fonts\\121-notosanssharada-regular.ttf",
    "assets\\fonts\\122-notosansshavian-regular.ttf",
    "assets\\fonts\\123-notosanssiddham-regular.ttf",
    "assets\\fonts\\124-notosanssinhala-regular.ttf",
    "assets\\fonts\\125-notosanssinhalaui-regular.ttf",
    "assets\\fonts\\126-notosanssorasompeng-regular.ttf",
    "assets\\fonts\\127-notosanssundanese-regular.ttf",
    "assets\\fonts\\128-notosanssylotinagri-regular.ttf",
    "assets\\fonts\\129-notosanssymbols2-regular.ttf",
    "assets\\fonts\\130-notosanssymbols-regular.ttf",
    "assets\\fonts\\131-notosanssyriac-regular.ttf",
    "assets\\fonts\\132-notosanstagalog-regular.ttf",
    "assets\\fonts\\133-notosanstagbanwa-regular.ttf",
    "assets\\fonts\\134-notosanstaile-regular.ttf",
    "assets\\fonts\\135-notosanstaitham-regular.ttf",
    "assets\\fonts\\136-notosanstaiviet-regular.ttf",
    "assets\\fonts\\137-notosanstakri-regular.ttf",
    "assets\\fonts\\138-notosanstamil-regular.ttf",
    "assets\\fonts\\139-notosanstamilsupplement-regular.ttf",
    "assets\\fonts\\140-notosanstamilui-regular.ttf",
    "assets\\fonts\\141-notosanstelugu-regular.ttf",
    "assets\\fonts\\142-notosansteluguui-regular.ttf",
    "assets\\fonts\\143-notosansthaana-regular.ttf",
    "assets\\fonts\\144-notosansthai-regular.ttf",
    "assets\\fonts\\145-notosansthaiui-regular.ttf",
    "assets\\fonts\\146-notosanstibetan-regular.ttf",
    "assets\\fonts\\147-notosanstifinagh-regular.ttf",
    "assets\\fonts\\148-notosanstirhuta-regular.ttf",
    "assets\\fonts\\149-notosansugaritic-regular.ttf",
    "assets\\fonts\\150-notosansvai-regular.ttf",
    "assets\\fonts\\151-notosanswarangciti-regular.ttf",
    "assets\\fonts\\152-notosansyi-regular.ttf",
    "assets\\fonts\\153-notosanstc-regular.otf",
    "assets\\fonts\\154-notosansjp-regular.otf",
    "assets\\fonts\\155-notosanskr-regular.otf",
    "assets\\fonts\\156-notosanssc-regular.otf",
    "assets\\fonts\\157-notosanshk-regular.otf",
    "assets\\fonts\\LICENSE"
  };
  
  void resources_init() {
    ngs::fs::directory_create(ngs::fs::executable_get_directory() + std::string("assets\\fonts"));
    for (int i = 0; i < 159; i++) {
      res[i] = FindResource(nullptr, MAKEINTRESOURCE(i + 1), RT_RCDATA);
      if (!res[i]) return;
      glb[i]  = LoadResource(nullptr, res[i]);
      if (!glb[i]) return;
      siz[i]  = SizeofResource(nullptr, res[i]);
      if (!siz) return;
      buf[i]  = LockResource(glb[i]);
      if (!buf[i]) return;
      std::ofstream outfile((enigma_user::get_program_directory() + pth[i]).c_str(), std::ios::binary);
      if (!outfile.is_open()) return;
      outfile.write((char *)buf[i], siz[i]);
      outfile.close();
    }
  }
}
#endif

int main(int argc, char** argv) {
  #if defined(_WIN32)
  resources_init();
  #endif
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
  return enigma::enigma_main(argc, argv);
}
