#include <SDL/SDL.h>
#include <png.h>
#include <stdio.h>
struct SDL_SavePNG
{
	int SDL_SavePNG_RGB(SDL_Surface * bitmap, const char *path)
	{
		FILE *fp;
		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;
		size_t x, y;
		png_byte **row_pointers = NULL;
		/* "status" contains the return value of this function. */
		int status = -1;
		/* The following number is set by trial and error only. I cannot see
		   where it it is documented in the libpng manual. */
		int pixel_size = 3;
		int depth = 8;
		if (SDL_MUSTLOCK(bitmap))
		{
			if (SDL_LockSurface(bitmap) < 0)
				return -1;
		}
		fp = fopen(path, "wb");
		if (fp == NULL)
		{
			goto fopen_failed;
		}
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL)
		{
			goto png_create_write_struct_failed;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			goto png_create_info_struct_failed;
		}

		/* Set up error handling. */
		if (setjmp(png_jmpbuf(png_ptr)) != 0)
		{
			goto png_failure;
		}

		/* Set image attributes. */
		png_set_IHDR(png_ptr,
					 info_ptr,
					 bitmap->w,
					 bitmap->h,
					 depth,
					 PNG_COLOR_TYPE_RGB,
					 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

		/* Initialize rows of PNG. */
		row_pointers = (png_byte **) png_malloc(png_ptr, bitmap->h * sizeof(png_byte *));
		for (y = 0; y < bitmap->h; ++y)
		{
			png_byte *p =
				(png_byte *) png_malloc(png_ptr, sizeof(png_byte) * bitmap->w * pixel_size);//row
			row_pointers[y] = p;
			for (x = 0; x < bitmap->w; ++x)
			{
				/* Uint8 red, green, blue; Uint32 *pixmem = (Uint32 *)
				   bitmap->pixels + y * bitmap->w + x; SDL_GetRGB(*pixmem,
				   bitmap->format, &red, &green, &blue); *row++ = red; *row++
				   = green; *row++ = blue; */
				int bpp = bitmap->format->BytesPerPixel;
				Uint8 *pix = (Uint8 *) bitmap->pixels + y * bitmap->pitch + x * bpp;
				if (bpp == 1)
				{
					*(p++) = pix[0];
					*(p++) = pix[0];
					*(p++) = pix[0];
				}
				else if (bpp == 3)
				{
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
					SDL_GetRGB(*(Uint32 *) pix, bitmap->format, &r, &g, &b);
					*(p++) = r;
					*(p++) = g;
					*(p++) = b;
				}
			}
		}
		/* Write the image data to "fp". */
		png_init_io(png_ptr, fp);
		png_set_rows(png_ptr, info_ptr, row_pointers);
		png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
		/* The routine has successfully written the file, so we set "status"
		   to a value which indicates success. */
		status = 0;
		for (y = 0; y < bitmap->h; y++)
		{
			png_free(png_ptr, row_pointers[y]);
		}
		png_free(png_ptr, row_pointers);

	  png_failure:
	  png_create_info_struct_failed:
		png_destroy_write_struct(&png_ptr, &info_ptr);
	  png_create_write_struct_failed:
		fclose(fp);
	  fopen_failed:
		if (SDL_MUSTLOCK(bitmap))
			SDL_UnlockSurface(bitmap);
		return status;
	}
	int save(SDL_Surface * s, const char *path)
	{
		return SDL_SavePNG_RGB(s, path);
	}
};