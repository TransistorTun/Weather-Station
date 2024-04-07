#ifndef FONT_H_
#define FONT_H_
#define FontxGlyphBufSize (32*32/8)

typedef struct {
	const char *path;
	char  fxname[10];
	bool  opened;
	bool  valid;
	bool  is_ank;
	uint8_t w;
	uint8_t h;
	uint16_t fsz;
	uint8_t bc;
	FILE *file;
} FontxFile;

void AaddFontx(FontxFile *fx, const char *path);
void InitFontx(FontxFile *fxs, const char *f0, const char *f1);
bool GetFontx(FontxFile *fxs, uint8_t ascii , uint8_t *pGlyph, uint8_t *pw, uint8_t *ph);

#endif