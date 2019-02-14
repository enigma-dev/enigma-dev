
unsigned libpng_encode_memory(unsigned char** out, size_t* outsize,
                               const unsigned char* image, unsigned w, unsigned h,
                               int colortype, unsigned bitdepth);
unsigned libpng_encode32(unsigned char** out, size_t* outsize, const unsigned char* image, unsigned w, unsigned h);
unsigned libpng_decode32_file(unsigned char** out, unsigned* w, unsigned* h, const char* filename);

std::string libpng_error_text(unsigned error);
