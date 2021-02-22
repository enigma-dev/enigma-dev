/*

 MIT License

 Copyright © 2021 Samuel Venable

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

#include <cmath>
#include <cstring>
#include <climits>

#include <string>
#include <thread>
#include <chrono>

#include "panoview.h"

#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFwindow.h"

#include "libpng-util/libpng-util.h"

#include <sys/types.h>
#include <unistd.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace {

GLuint tex;
void LoadPanorama(const char *fname) {
  unsigned char *data = nullptr;
  unsigned pngwidth, pngheight;
  unsigned error = libpng_decode32_file(&data, &pngwidth, &pngheight, fname);
  if (error) { return; }
  const int size = pngwidth * pngheight * 4;
  unsigned char *buffer = new unsigned char[size]();
  for (unsigned i = 0; i < pngheight; i++) {
    for (unsigned j = 0; j < pngwidth; j++) {
      unsigned oldPos = (pngheight - i - 1) * (pngwidth * 4) + 4 * j;
      unsigned newPos = i * (pngwidth * 4) + 4 * j;
      buffer[newPos + 0] = data[oldPos + 0];
      buffer[newPos + 1] = data[oldPos + 1];
      buffer[newPos + 2] = data[oldPos + 2];
      buffer[newPos + 3] = data[oldPos + 3];
    }
  }
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pngwidth, pngheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
  delete[] buffer;
  delete[] data;
}

float xr     = 0.0f;
float angleX = 0.0f;
void DrawPanorama() {
  const double PI = 3.141592653589793;
  double i, resolution  = 0.3141592653589793;
  double height = 3.5;
  double radius = 0.5;
  glPushMatrix();
  glRotatef(90 + xr, 0, 90 + 0.5, 0);
  glTranslatef(0, -1.75, 0);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0.5, 1);
  glVertex3f(0, height, 0);
  for (i = 2 * PI; i >= 0; i -= resolution) {
    glTexCoord2f(0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f);
    glVertex3f(radius * cos(i), height, radius * sin(i));
  }
  glTexCoord2f(0.5, 0.5);
  glVertex3f(radius, height, 0);
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0.5, 0.5);
  glVertex3f(0, 0, 0);
  for (i = 0; i <= 2 * PI; i += resolution) {
    glTexCoord2f(0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f);
    glVertex3f(radius * cos(i), 0, radius * sin(i));
  }
  glEnd();
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= 2 * PI; i += resolution) {
    const float tc = (i / (float)(2 * PI));
    glTexCoord2f(tc, 0.0);
    glVertex3f(radius * cos(i), 0, radius * sin(i));
    glTexCoord2f(tc, 1.0);
    glVertex3f(radius * cos(i), height, radius * sin(i));
  }
  glTexCoord2f(0.0, 0.0);
  glVertex3f(radius, 0, 0);
  glTexCoord2f(0.0, 1.0);
  glVertex3f(radius, height, 0);
  glEnd();
  glPopMatrix();
}

GLuint cur;
void LoadCursor(const char *fname) {
  unsigned char *data = nullptr;
  unsigned pngwidth, pngheight;
  unsigned error = libpng_decode32_file(&data, &pngwidth, &pngheight, fname);
  if (error) { return; }
  const int size = pngwidth * pngheight * 4;
  unsigned char *buffer = new unsigned char[size]();
  for (unsigned i = 0; i < pngheight; i++) {
    for (unsigned j = 0; j < pngwidth; j++) {
      unsigned oldPos = (pngheight - i - 1) * (pngwidth * 4) + 4 * j;
      unsigned newPos = i * (pngwidth * 4) + 4 * j;
      buffer[newPos + 0] = data[oldPos + 0];
      buffer[newPos + 1] = data[oldPos + 1];
      buffer[newPos + 2] = data[oldPos + 2];
      buffer[newPos + 3] = data[oldPos + 3];
    }
  }
  glGenTextures(1, &cur);
  glBindTexture(GL_TEXTURE_2D, cur);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pngwidth, pngheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
  delete[] buffer;
  delete[] data;
}

void DrawCursor(GLuint id, int x, int y, int width, int height) {
  glBindTexture(GL_TEXTURE_2D, id);
  glEnable(GL_TEXTURE_2D);
  glColor4f(1, 1, 1, 1);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 1); glVertex2f(x, y);
  glTexCoord2f(0, 0); glVertex2f(x, y + height);
  glTexCoord2f(1, 0); glVertex2f(x + width, y + height);
  glTexCoord2f(1, 1); glVertex2f(x + width, y);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void display() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0f, 16 / 9, 0.1f, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, 0);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();
  glRotatef(angleX, 1, 0, 0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, tex);
  DrawPanorama();
  glFlush();
  glClear(GL_DEPTH_BITS);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, enigma_user::display_get_width(),
    enigma_user::display_get_height(), 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, cur);
  DrawCursor(cur, (enigma_user::display_get_width() / 2) - 16,
    (enigma_user::display_get_height() / 2) - 16, 32, 32);
  glBlendFunc(GL_ONE, GL_ZERO);
  glutSwapBuffers();
}

void timer(int i) {
  int mx  = enigma_user::display_mouse_get_x();
  int my  = enigma_user::display_mouse_get_y();
  int hdw = enigma_user::display_get_x() + (enigma_user::display_get_width()  / 2);
  int hdh = enigma_user::display_get_y() + (enigma_user::display_get_height() / 2);
  enigma_user::display_mouse_set(hdw, hdh);
  enigma_user::panorama_set_hangle((hdw - mx) / 20);
  enigma_user::panorama_set_vangle((hdh - my) / 20);
  glutPostRedisplay();
  glutTimerFunc(5, timer, 0);
}

int window = 0;
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27: glutDestroyWindow(window); exit(0); break;
  }
  glutPostRedisplay();
}

} // anonymous namespace

namespace enigma {

void panoview_init() {
  int i = 0; glutInit(&i, NULL);
  glutInitDisplayMode(GLUT_DOUBLE);
}

} // namespace enigma

namespace enigma_user {

void panorama_create(std::string panorama, std::string defcursor) {
  window_set_fullscreen(true);
  window_set_visible(false);
  int hdw = display_get_x() + (display_get_width()  / 2);
  int hdh = display_get_y() + (display_get_height() / 2);
  display_mouse_set(hdw, hdh);
  std::this_thread::sleep_for (std::chrono::milliseconds(5));
  if (window != 0) glutDestroyWindow(window);
  window = glutCreateWindow(window_get_caption().c_str());
  glutDisplayFunc(display);
  glutHideWindow();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  LoadPanorama(panorama.c_str());
  LoadCursor(defcursor.c_str());
  glutSetCursor(GLUT_CURSOR_NONE);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(0, timer, 0);
  glutShowWindow();
  glutFullScreen();
  glutMainLoop();
}

double panorama_get_hangle() {
  return xr;
}

void panorama_set_hangle(double hangle) {
  xr -= hangle;
}

double panorama_get_vangle() {
  return angleX;
}

void panorama_set_vangle(double vangle) {
  angleX -= vangle;
  angleX  = ((angleX >= -28) ? angleX : -28);
  angleX  = ((angleX <=  28) ? angleX :  28);
}

} // namespace enigma_user
