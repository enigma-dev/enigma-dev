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
  if (off > 0.0f);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
  int text_height = ImGui::GetFontSize();
  float hgt = (text_height + style.FramePadding.y) * 0.5;
  ImGui::SetCursorPosY(ImGui::GetContentRegionAvail().y - hgt);
}

bool ImGuiAl::MsgBox::Init( const char* title, const char* icon, const char* text, std::vector<std::string> captions, bool show_checkbox )
{
  m_Title = title;
  m_Icon = icon;
  m_Text = text;
  m_Captions = captions;
  m_ShowCheckbox = show_checkbox;
  m_DontAskAgain = false;
  m_Selected = 0;
  return true;
}

int ImGuiAl::MsgBox::Draw()
{
  int index = 0;
  if ( ImGui::BeginPopupModal( m_Title, nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove ) )
  {
    if ( m_DontAskAgain && m_Selected != 0 )
    {
      ImGui::CloseCurrentPopup();
      index = m_Selected;
    }
    else
    {
      if ( m_Icon != nullptr )
      {
        ImVec2 size = ImGui::CalcTextSize( m_Icon );
        ImVec2 pos = ImGui::GetCursorPos();
        float save_y = pos.y;
        pos.y += size.y;
        
        ImGui::SetCursorPos( pos );
        ImGui::Text( "%s", m_Icon );
        
        pos.x += size.x + pos.x;
        pos.y = save_y;
        
        ImGui::SetCursorPos( pos );
        ImGui::TextWrapped( "%s", m_Text );
      }
      else
      {
        ImGui::TextWrapped( "%s", m_Text );
      }
      
      ImGui::Separator();
      
      if ( m_ShowCheckbox )
      {
        ImGui::Checkbox( "Don't ask me again", &m_DontAskAgain );
      }
      
      ImVec2 size = ImVec2( 10.0f * ImGui::GetFontSize(), 0.0f );
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
      
      size = ImVec2( ( 4 - count ) * 10.0f * ImGui::GetFontSize(), 1.0f );
      ImGui::Dummy( size );
      
      if ( m_DontAskAgain )
      {
        m_Selected = index;
      }
    }
    
    ImGui::EndPopup();
  }
  return index;
}

void ImGuiAl::MsgBox::Open()
{
  ImGui::OpenPopup( m_Title );
}
