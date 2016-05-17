#include <SDL/SDL.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
struct SDL_SaveJPEG
{
	JSAMPLE *image_buffer;
	int image_height;
	int image_width;
	int write_JPEG_file(const char *filename, int quality)
	{
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		FILE *outfile;			/* 目标文件 */
		JSAMPROW row_pointer[1];	/* 指向 JSAMPLE row[s] */
		int row_stride;			/* physical row width in image buffer */
		  cinfo.err = jpeg_std_error(&jerr);
		  jpeg_create_compress(&cinfo);
		if ((outfile = fopen(filename, "wb")) == NULL)
		{
			fprintf(stderr, "can't open %s\n", filename);
			return -1;
		}
		jpeg_stdio_dest(&cinfo, outfile);
		cinfo.image_width = image_width;
		cinfo.image_height = image_height;
		cinfo.input_components = 3;
		cinfo.in_color_space = JCS_RGB;
		jpeg_set_defaults(&cinfo);

		jpeg_set_quality(&cinfo, quality, TRUE);
		jpeg_start_compress(&cinfo, TRUE);
		row_stride = image_width * 3;	/* JSAMPLEs per row in image_buffer */
		while (cinfo.next_scanline < cinfo.image_height)
		{
			row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
			(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}
		jpeg_finish_compress(&cinfo);
		fclose(outfile);
		jpeg_destroy_compress(&cinfo);
		return 0;
	}
	int save( SDL_Surface * s,const char *f)
	{
		if (SDL_MUSTLOCK(s))
			if (SDL_LockSurface(s) < 0)
				return -1;
		int i, j;
		this->image_width = s->w;
		this->image_height = s->h;
		this->image_buffer = (JSAMPLE *) malloc(image_width * image_height * 3 * sizeof(JSAMPLE));
		// assert(image_buffer);

		JSAMPLE *p = this->image_buffer;
		for (i = 0; i < s->h; i++)
			for (j = 0; j < s->w; j++)
			{
				int bpp = s->format->BytesPerPixel;
				if (bpp == 1)
				{
					Uint8 *pix = (Uint8 *) s->pixels + i * s->pitch + j * bpp;
					*(p++) = pix[0];
					*(p++) = pix[0];
					*(p++) = pix[0];
				}
				else if (bpp == 3)
				{
					Uint8 *pix = (Uint8 *) s->pixels + i * s->pitch + j * bpp;
					if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
					{
						*(p++) = pix[0];
						*(p++) = pix[1];
						*(p++) = pix[2];
					}
					else
					{
						*(p++) = pix[2];
						*(p++) = pix[1];
						*(p++) = pix[0];
					}
				}
				else if (bpp == 4)
				{

					Uint8 r, g, b;
					Uint32 *pix = (Uint32 *) s->pixels + i * this->image_width + j;
					SDL_GetRGB(*pix, s->format, &r, &g, &b);
					*(p++) = r;
					*(p++) = g;
					*(p++) = b;
				}
			}
		if (SDL_MUSTLOCK(s))
			SDL_UnlockSurface(s);
		write_JPEG_file(f, 90);
		free(image_buffer);
		return 0;
	}
};
