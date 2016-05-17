#include <SDL/SDL.h>
#include "SDL_SaveJPEG.cpp"
SaveJPEG worker;
#define print_surface_info(surface,info) printf(#surface"->"#info"=%d\n",surface->info);
int main()
{
	SDL_Init(SDL_INIT_VIDEO);
SDL_Surface *bmp = SDL_LoadBMP("lena.bmp");
	if (bmp == NULL)
		return -1;
	SDL_Surface *s =
		SDL_SetVideoMode(bmp->w, bmp->h, bmp->format->BitsPerPixel, SDL_SWSURFACE | SDL_DOUBLEBUF);
	if (s == NULL)
		return -1;
//	freopen("sdl.log", "w", stdout);
//	printf("BMP BitsPerPixel=%d\n", bmp->format->BitsPerPixel);

print_surface_info(bmp,w);
print_surface_info(bmp,h);
print_surface_info(bmp,format->BitsPerPixel);
print_surface_info(bmp,pitch);

//print_surface_info(s,w);
//print_surface_info(s,h);
//print_surface_info(s,format->BitsPerPixel);
	SDL_BlitSurface(bmp, NULL, s, NULL);
	SDL_FreeSurface(bmp);
	SDL_Flip(s);
	clock_t t=clock();
	worker.save(s,"lena.jpg");
	printf("jpeg save time: %ld\n",clock()-t);
	fflush(stdout);
	fclose(stdout);
	SDL_Quit();
	return 0;
}