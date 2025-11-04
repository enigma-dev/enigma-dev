/** Copyright (C) 2025 Samuel Venable
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

#include <vector>
#include <memory>
#include <unordered_map>

#include "wincapt.h"
#include "Universal_System/estring.h"

#include <windows.h>

namespace {

  int index = -1;
  int windex = -1;
  std::unordered_map<int, window_t>           capture_window;
  std::unordered_map<int, int>                capture_width;
  std::unordered_map<int, int>                capture_height;
  std::unordered_map<int, bool>               capture_fixedsize;
  std::unordered_map<int, std::vector<wid_t>> window_list;
  std::unordered_map<int, int>                window_list_length;

  void rgb_to_rgba(const unsigned char *rgb, unsigned char **rgba, int width, int height) {
    for (int y = 0; y < height; y++) {
      if (y > height) break;
      for (int x = 0; x < width; x++) {
        if (x > width) break;
        (*rgba)[(y * width + x) * 4 + 0] = rgb[(y * width + x) * 3 + 0];
        (*rgba)[(y * width + x) * 4 + 1] = rgb[(y * width + x) * 3 + 1];
        (*rgba)[(y * width + x) * 4 + 2] = rgb[(y * width + x) * 3 + 2];
        (*rgba)[(y * width + x) * 4 + 3] = 255;
      }
    }
  }

  void capture_window_pixels_and_size(int ind, HWND hwnd, unsigned char **pixels, int *width, int *height) {
    if (!capture_fixedsize[ind]) {
      (*width) = -1;
      (*height) = -1;
    }
    if (!IsWindow(hwnd)) return; 
    RECT rect; GetClientRect(hwnd, &rect);
    if ((rect.right - rect.left) > 16384 || (rect.bottom - rect.top) > 16384) return;
    if (!capture_fixedsize[ind]) {
      (*width) = (rect.right - rect.left);
      (*height) = (rect.bottom - rect.top);
    }
    if (pixels) {
      HDC hdc_window = GetDC(hwnd);
      HDC hdc_mem_dc = CreateCompatibleDC(hdc_window);
      if (!hdc_mem_dc) {
        ReleaseDC(hwnd, hdc_window);
        return;
      }
      HBITMAP hbm_screen = CreateCompatibleBitmap(hdc_window, (*width), (*height));
      if (!hbm_screen) {
        DeleteDC(hdc_mem_dc);
        ReleaseDC(hwnd, hdc_window);
        return;
      }
      SelectObject(hdc_mem_dc, hbm_screen);
      if (!BitBlt(hdc_mem_dc, 0, 0, (*width), (*height), hdc_window, 0, 0, SRCCOPY)) {
        return;
      }
      BITMAPINFO bmp_info;
      bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bmp_info.bmiHeader.biWidth = (*width);
      bmp_info.bmiHeader.biHeight = -(*height);
      bmp_info.bmiHeader.biPlanes = 1;
      bmp_info.bmiHeader.biBitCount = 24;
      bmp_info.bmiHeader.biCompression = BI_RGB;
      std::vector<unsigned char> src((*width) * (*height) * 3);
      if (!GetDIBits(hdc_mem_dc, hbm_screen, 0, (*height), src.data(), &bmp_info, DIB_RGB_COLORS)) {
        return;
      }
      rgb_to_rgba(src.data(), pixels, (*width), (*height));
      DeleteObject(hbm_screen);
      DeleteDC(hdc_mem_dc);
      ReleaseDC(hwnd, hdc_window);
    }
  }

  wid_t window_id_from_native_window(window_t window) {
    wid_t result;
    result = std::to_string((unsigned long long)(void *)window);
    return result;
  }

  window_t native_window_from_window_id(wid_t win_id) {
    return (window_t)(void *)strtoull(win_id.c_str(), nullptr, 10);
  }

  std::vector<wid_t> window_id_enumerate() {
    std::vector<wid_t> wid_vec;
    HWND hwnd = GetTopWindow(GetDesktopWindow());
    wid_t wid = window_id_from_native_window((window_t)hwnd);
    if (IsWindowVisible(hwnd) && !enigma_user::capture_get_window_caption(wid).empty()) {
      wid_vec.push_back(wid);
    }
    while ((hwnd = GetWindow(hwnd, GW_HWNDNEXT))) {
      wid_t wid = window_id_from_native_window((window_t)hwnd);
      if (IsWindowVisible(hwnd) && !enigma_user::capture_get_window_caption(wid).empty()) {
        wid_vec.push_back(wid);
      }
    }
    return wid_vec;
  }

  bool window_id_free(std::vector<wid_t> wid_vec) {
    if (!wid_vec.empty()) {
      wid_vec.clear();
      return true;
    }
    return false;
  }

} // anonymous namespace

namespace enigma_user {

  int capture_add(enigma::rvt window) {
    index++;
    capture_window[index] = (window_t)(void *)window.p;
    capture_window_pixels_and_size(index, (HWND)(void *)window.p, nullptr, &capture_width[index], &capture_height[index]);
    return index;
  }

  bool capture_exists(int ind) {
    if (capture_window.find(ind) == capture_window.end()) return false;
    return true;
  }

  bool capture_delete(int ind) {
    if (!capture_exists(ind)) return false;
    capture_window.erase(ind);
    capture_width.erase(ind);
    capture_height.erase(ind);
    capture_fixedsize.erase(ind);
    return true;
  }

  int capture_get_width(int ind) {
    if (!capture_exists(ind)) return -1;
    return capture_width.find(ind)->second;
  }

  int capture_get_height(int ind) {
    if (!capture_exists(ind)) return -1;
    return capture_height.find(ind)->second;
  }

  bool capture_grab_frame_buffer(int ind, void *buffer) {
    if (!capture_exists(ind)) return false;
    HWND hwnd = (HWND)(void *)capture_window.find(ind)->second;
    unsigned char *pixels = (unsigned char *)buffer;
    capture_window_pixels_and_size(ind, hwnd, &pixels, &capture_width[ind], &capture_height[ind]);
    return true;
  }

  bool capture_update(int ind) {
    if (!capture_exists(ind)) return false;
    HWND hwnd = (HWND)(void *)capture_window.find(ind)->second;
    capture_window_pixels_and_size(ind, hwnd, nullptr, &capture_width[ind], &capture_height[ind]);
    return true;
  }

  int capture_create_window_list() {
    windex++;
    window_list[windex] = window_id_enumerate();
    window_list_length[windex] = (int)window_list[windex].size();
    return windex;
  }

  bool capture_window_list_exists(int list) {
    if (window_list.find(list) == window_list.end()) return false;
    return true;
  }

  wid_t capture_get_window_id(int list, int ind) {
    if (!capture_window_list_exists(list)) return "0";
    return window_list[list][ind];
  }

  int capture_get_window_id_length(int list) {
    if (!capture_window_list_exists(list)) return 0;
    return window_list_length[list];
  }

  bool capture_destroy_window_list(int list) {
    if (!capture_window_list_exists(list)) return false;
    window_list.erase(list);
    window_list_length.erase(list);
    return window_id_free(window_list[list]);
  }

  std::string capture_get_window_caption(wid_t window) {
    std::string result;
    HWND hwnd = (HWND)(void *)(unsigned long long)native_window_from_window_id(window);
    if (!IsWindow(hwnd) || !IsWindowVisible(hwnd)) return result;
    std::size_t length = GetWindowTextLengthW(hwnd) + 1;
    wchar_t *buffer = new wchar_t[length]();
    if (buffer) {
      if (GetWindowTextW(hwnd, buffer, (int)length)) {
        result = shorten(buffer);
      }
      delete[] buffer;
    }
    return result;
  }

  bool capture_get_fixedsize(int ind) {
    if (!capture_exists(ind)) return false;
    return capture_fixedsize[ind];
  }

  bool capture_set_fixedsize(int ind, bool fixed) {
    if (!capture_exists(ind)) return false;
    capture_fixedsize[ind] = fixed;
    return true;
  }

  window_t capture_native_window_from_window_id(wid_t window) {
    return native_window_from_window_id(window);
  } 

  wid_t capture_window_id_from_native_window(enigma::rvt window) {
    return window_id_from_native_window((window_t)(void *)window.p);
  }

} // namespace enigma_user
