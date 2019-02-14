#include <string>

#include <png.h>

unsigned libpng_encode_memory(unsigned char** out, size_t* outsize,
                               const unsigned char* image, unsigned w, unsigned h,
                               int colortype, unsigned bitdepth) {
  return -1;
}

unsigned libpng_encode32(unsigned char** out, size_t* outsize, const unsigned char* image, unsigned w, unsigned h) {
  return -1;
}

unsigned libpng_decode32_file(unsigned char** out, unsigned* w, unsigned* h, const char* filename) {
  (*w) = 0; (*h) = 0;
  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) return -1;
  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, NULL);
    return -2;
  }

  png_init_io(png, fp);
  png_read_info(png, info);

  png_byte color_type, bit_depth;
  (*w)       = png_get_image_width(png, info);
  (*h)       = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if (bit_depth == 16)
    png_set_strip_16(png);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if (color_type == PNG_COLOR_TYPE_RGB ||
      color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  png_bytep *rowptrs, image;
  size_t pitch = sizeof(png_byte) * 4 * (*w); // number of bytes in a row
  image = (png_bytep)malloc(pitch * (*h));
  rowptrs = (png_bytep*)malloc(sizeof(png_bytep) * (*h));
  for (size_t y = 0; y < (*h); y++) {
    rowptrs[y] = (png_bytep)&image[pitch * y];
  }

  png_read_image(png, rowptrs);

  png_destroy_read_struct(&png, &info, NULL);
  fclose(fp);
  free(rowptrs);

  (*out) = image;

  return 0;
}

std::string libpng_error_text(unsigned error) {
  return "";
}
