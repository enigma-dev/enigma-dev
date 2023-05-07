/*

MIT License

Copyright © 2016 Andre Leiradella

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

#include "imguial_msgbox.h"
#include "../filedialogs.hpp"
#if defined(_MSC_VER)
#include "../filesystem.hpp"
#else
#include "filesystem.hpp"
#endif
#include "../imgui.h"

extern SDL_Window *dialog;
ImGuiAl::MsgBox::~MsgBox() { }

void AlignForWidth(float width, float alignment = 0.5f) {
  ImGuiStyle& style = ImGui::GetStyle();
  float avail = ImGui::GetContentRegionAvail().x;
  float off = (avail - width) * alignment;
  if (off > 0.0f) {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
  }
  ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - (ImGui::GetFontSize() + (ImGui::GetFontSize() / 2)));
}

bool ImGuiAl::MsgBox::Init(const char *title, const char *text, std::vector<std::string> captions, bool input, const char *def) {
  m_Title = title;
  m_Text = text;
  m_Captions = captions;
  m_Input = input;
  if (m_Input) strcpy(m_Default, def);
  return true;
}

int ImGuiAl::MsgBox::Draw() {
  int index = 0;
  if (m_Input) InputBuffer[0] = 0;
  if (ImGui::BeginPopupModal(m_Title, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
    ImGui::TextWrapped(m_Text );
    int sw = 0, sh = 0;
    int dw = ImGui::CalcTextSize(m_Text, m_Text + strlen(m_Text), false, 100 * (0.25 * ImGui::GetFontSize())).x;
    if (dw < ImGui::GetWindowContentRegionMax().x * 0.75) dw = ImGui::GetWindowContentRegionMax().x * 0.75;
    int dh = ImGui::CalcTextSize(m_Text, m_Text + strlen(m_Text), false, 100 * (0.25 * ImGui::GetFontSize())).y + (4.875f * ImGui::GetFontSize());
    if (m_Input) dh += ((4.875f * ImGui::GetFontSize()) / 2);
    if (dialog) {
      SDL_GetWindowSize(dialog, &sw, &sh);
      SDL_SetWindowSize(dialog, dw, dh);
      if (ngs::fs::environment_get_variable("IMGUI_DIALOG_PARENT").empty()) {
        SDL_SetWindowPosition(dialog, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
      }
      if (m_Input) {
        strcpy(m_InputBuffer, m_Default);
      }
    }
    ImGui::Separator();
    ImVec2 size = ImVec2(4.875f * ImGui::GetFontSize(), 0.0f);
    int count;
    ImGuiStyle& style = ImGui::GetStyle();
    float width = 0.0f;
    for (count = 0; count < m_Captions.size(); count++) {
      width += size.x;
      width += style.ItemSpacing.x;
    }
    width -= style.ItemSpacing.x;
    if (m_Input) {
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
      ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - (2 * (ImGui::GetFontSize() + (ImGui::GetFontSize() / 2))));
      if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
        ImGui::SetKeyboardFocusHere(0);
      }
      if (ImGui::InputText("##inputBox", m_InputBuffer, 1024)) {
        if (m_Input) {
          InputBuffer[0] = 0;
          strcpy(InputBuffer, m_InputBuffer);
        }
        ImGui::CloseCurrentPopup();
      }
    }
    AlignForWidth(width);
    for (count = 0; count < m_Captions.size(); count++) {
      ImGui::PushID(count);
      if (ImGui::Button(m_Captions[ count ].c_str(), size)) {
        index = count + 1;
        if (m_Input) {
          InputBuffer[0] = 0;
          strcpy(InputBuffer, m_InputBuffer);
        }
        ImGui::CloseCurrentPopup();
        ImGui::PopID();
        break;
      }
      ImGui::SameLine();
      ImGui::PopID();
    }
    size = ImVec2((4 - count) * 4.875f * ImGui::GetFontSize(), ImGui::GetFontSize());
    ImGui::Dummy(size);
    ImGui::EndPopup();
  }
  return index;
}

void ImGuiAl::MsgBox::Open() {
  ImGui::OpenPopup(m_Title);
}
