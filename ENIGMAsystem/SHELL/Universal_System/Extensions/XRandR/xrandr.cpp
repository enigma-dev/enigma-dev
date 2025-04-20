#include "include.h"

#include "Platforms/xlib/XLIBwindow.h"

#include <X11/extensions/Xrandr.h> // for display_set_size

namespace {

void query_size_id(int w, int h, SizeID &compatible_size_id) {
  int num_sizes;
  XRRScreenSize *xrrs;
  xrrs = XRRSizes(enigma::x11::disp, 0, &num_sizes);
  for (int i = 0; i < num_sizes; i++) {
    if (xrrs[i].width == w && xrrs[i].height == h) {
      compatible_size_id = i;
      return;
    }
  }
}

bool test_size_id(int &w, int &h, SizeID &compatible_size_id, SizeID original_size_id) {
  if (w == -1 && h == -1) {
    compatible_size_id = original_size_id;
  } else {
    if (w == -1) {
      w = XWidthOfScreen(enigma::x11::screen);
    }
    if (h == -1) {
      h = XHeightOfScreen(enigma::x11::screen);
    }
    query_size_id(w, h, compatible_size_id);
    if (compatible_size_id == -1)
      return false;
  }
  return true;
}

} // namespace anonymous

namespace enigma_user {

int display_get_frequency() {
  XRRScreenConfiguration *conf = XRRGetScreenInfo(enigma::x11::disp, enigma::x11::win);
  return XRRConfigCurrentRate(conf);
}

bool display_set_size(int w, int h) {
  XRRScreenConfiguration *conf = XRRGetScreenInfo(enigma::x11::disp, enigma::x11::win);
  Rotation original_rotation;
  SizeID original_size_id = XRRConfigCurrentConfiguration(conf, &original_rotation);
  SizeID compatible_size_id = -1;

  query_size_id(w, h, compatible_size_id);
  if (compatible_size_id == -1)
    return false;

  return XRRSetScreenConfig(enigma::x11::disp, conf, enigma::x11::win, compatible_size_id, original_rotation, CurrentTime);
}

bool display_set_frequency(int freq) {
  XRRScreenConfiguration *conf = XRRGetScreenInfo(enigma::x11::disp, enigma::x11::win);
  Rotation original_rotation;
  SizeID original_size_id = XRRConfigCurrentConfiguration(conf, &original_rotation);

  return XRRSetScreenConfigAndRate(enigma::x11::disp, conf, enigma::x11::win, original_size_id, original_rotation, freq, CurrentTime);
}

bool display_set_all(int w, int h, int freq, int bitdepth) {
  XRRScreenConfiguration *conf = XRRGetScreenInfo(enigma::x11::disp, enigma::x11::win);
  Rotation original_rotation;
  SizeID original_size_id = XRRConfigCurrentConfiguration(conf, &original_rotation);
  SizeID compatible_size_id = -1;

  if (!test_size_id(w, h, compatible_size_id, original_size_id)) return false;

  if (freq != -1) {
    return XRRSetScreenConfigAndRate(enigma::x11::disp, conf, enigma::x11::win, compatible_size_id, original_rotation, freq, CurrentTime);
  } else {
    return XRRSetScreenConfig(enigma::x11::disp, conf, enigma::x11::win, compatible_size_id, original_rotation, CurrentTime);
  }
}

bool display_test_all(int w, int h, int freq, int bitdepth) {
  XRRScreenConfiguration *conf = XRRGetScreenInfo(enigma::x11::disp, enigma::x11::win);
  Rotation original_rotation;
  SizeID original_size_id = XRRConfigCurrentConfiguration(conf, &original_rotation);
  SizeID compatible_size_id = -1;

  if (!test_size_id(w, h, compatible_size_id, original_size_id)) return false;

  if (freq != -1) {
    int num_rates;
    short *xrrr;
    xrrr = XRRRates(enigma::x11::disp, 0, compatible_size_id, &num_rates);
    bool rateSupported = false;
    for (int i = 0; i < num_rates; i++) {
      if (xrrr[i] == freq) {
        rateSupported = true;
        break;
      }
    }
    if (!rateSupported) return false;
  }

  return true;
}

} // namespace enigma_user
