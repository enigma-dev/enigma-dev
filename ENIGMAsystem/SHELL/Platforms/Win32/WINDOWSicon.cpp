/** Copyright (C) 2020 Samuel Venable
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

// Josh will probably gaze at this code lazily and think he has a better way to do it
// but not realize we are dependent on converting the char array to an HICON datatype

namespace enigma {

void SetIconFromSprite(HWND window, int ind, unsigned subimg) {
  sprite *spr;
  if (!get_sprite_mtx(spr, ind))
    return;

  unsigned char *data = nullptr;
  unsigned pngwidth, pngheight;
  data = graphics_copy_texture_pixels(spr->texturearray[subimg], &pngwidth, &pngheight);

  const unsigned
    widfull = nlpo2dc(pngwidth) + 1,
    hgtfull = nlpo2dc(pngheight) + 1;
  const int bitmap_size = widfull * hgtfull * 4;

  HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, bitmap_size);
  BYTE *pmem = (BYTE *)GlobalLock(m_hMem);
  memcpy(pmem, data, bitmap_size);

  IStream *pstm;
  CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);

  Gdiplus::Bitmap *bitmap = Gdiplus::Bitmap::FromStream(pstm, FALSE);
  pstm->Release();

  GlobalUnlock(m_hMem);

  bitmap->RotateFlip(RotateNoneFlipX);
  HICON hIcon = bitmap->GetHICON(&hIcon);
  PostMessage(window, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

  DeleteObject(hIcon);
  delete bitmap;
  GlobalFree(m_hMem);
}

} // namespace enigma
