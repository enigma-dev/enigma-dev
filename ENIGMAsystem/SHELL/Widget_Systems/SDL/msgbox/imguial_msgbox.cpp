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
#include "../imgui.h"

ImGuiAl::MsgBox::~MsgBox() {}

void AlignForWidth(float width, float alignment = 0.5f)
{
  ImGuiStyle& style = ImGui::GetStyle();
  float avail = ImGui::GetContentRegionAvail().x;
  float off = (avail - width) * alignment;
  if (off > 0.0f)
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
  ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - (ImGui::GetFontSize() + (ImGui::GetFontSize() / 2)));
}

bool ImGuiAl::MsgBox::Init( const char* title, const char* text, std::vector<std::string> captions )
{
  m_Title = title;
  m_Text = text;
  m_Captions = captions;
  return true;
}

int ImGuiAl::MsgBox::Draw()
{
  int index = 0;
  if ( ImGui::BeginPopupModal( m_Title, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove ) )
  {
    ImGui::TextWrapped( "%s", m_Text );
      
    ImGui::Separator();

    ImVec2 size = ImVec2( 4.75f * ImGui::GetFontSize(), 0.0f );
    int count;

    ImGuiStyle& style = ImGui::GetStyle();
    float width = 0.0f;
    for ( count = 0; count < m_Captions.size(); count++ )
    {
      width += size.x;
      width += style.ItemSpacing.x;
    }
    width -= style.ItemSpacing.x;
    AlignForWidth(width);
    for ( count = 0; count < m_Captions.size(); count++ )
    {
      ImGui::PushID(count);
      if ( ImGui::Button( m_Captions[ count ].c_str(), size ) )
      {
        index = count + 1;
        ImGui::CloseCurrentPopup();
        ImGui::PopID();
        break;
      }
      ImGui::SameLine();
      ImGui::PopID();
    }
      
    size = ImVec2( ( 4 - count ) * 4.75f * ImGui::GetFontSize(), ImGui::GetFontSize() );
    ImGui::Dummy( size );
    
    ImGui::EndPopup();
  }
  return index;
}

void ImGuiAl::MsgBox::Open()
{
  ImGui::OpenPopup( m_Title );
}
