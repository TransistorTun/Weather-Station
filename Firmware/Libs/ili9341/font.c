#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"

#include "font.h"


static void AddFontx(FontxFile *fx, const char *path)
{
	memset(fx, 0, sizeof(FontxFile));
	fx->path = path;
	fx->opened = false;
}

void InitFontx(FontxFile *fxs, const char *f0, const char *f1)
{
	AddFontx(&fxs[0], f0);
	AddFontx(&fxs[1], f1);
}


static bool OpenFontx(FontxFile *fx)
{
	FILE *file;
	if(!fx->opened){
		file = fopen(fx->path, "r");
		
		if (file == NULL) 
		{
			fx->valid = false;
			printf("Fontx:%s not found.\n",fx->path);
			return fx->valid ;
		}

		fx->opened = true;
		fx->file = file;
		char buf[18];

		if (fread(buf, 1, sizeof(buf), fx->file) != sizeof(buf)) 
		{
			fx->valid = false; 
			printf("Fontx:%s not FONTX format.\n",fx->path);
			fclose(fx->file);
			return fx->valid ;
		}

		memcpy(fx->fxname, &buf[6], 8);
		fx->w = buf[14];
		fx->h = buf[15];
		fx->is_ank = (buf[16] == 0);
		fx->bc = buf[17];
		fx->fsz = (fx->w + 7) / 8 * fx->h;

		if(fx->fsz > FontxGlyphBufSize)
		{
			printf("Fontx:%s is too big font size.\n",fx->path);
			fx->valid = false;
			fclose(fx->file);
			return fx->valid ;
		}
		fx->valid = true;
	}
	return fx->valid;
}

bool GetFontx(FontxFile *fxs, uint8_t ascii , uint8_t *pGlyph, uint8_t *pw, uint8_t *ph)
{
	uint32_t offset;
	for(uint8_t i = 0; i < 2; i++)
	{
	//for(i=0; i<1; i++){
		if(!OpenFontx(&fxs[i])) continue;
	
		//if(ascii < 0xFF){
			if(fxs[i].is_ank)
			{
				offset = 17 + ascii * fxs[i].fsz;

				if(fseek(fxs[i].file, offset, SEEK_SET)) 
				{
					printf("Fontx:seek(%"PRIu32") failed.\n",offset);
					return false;
				}

				if(fread(pGlyph, 1, fxs[i].fsz, fxs[i].file) != fxs[i].fsz) 
				{
					printf("Fontx:fread failed.\n");
					return false;
				}

				if(pw) *pw = fxs[i].w;
				if(ph) *ph = fxs[i].h;
				return true;
			}
		//}
	}
	return false;
}