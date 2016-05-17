#include "SDL.h"
#include "SDL/SDL_ttf.h"
 #include "SDL_SavePNG.cpp"
SDL_SavePNG worker;
void ApplySurface(int x, int y, SDL_Surface * pSrc, SDL_Surface * pDest)
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = pSrc->w;
	rect.h = pSrc->h;

	SDL_BlitSurface(pSrc, NULL, pDest, &rect);
}

int main()
{
	char str[] = "HeMiao";
	TTF_Font *font;
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
		return 0;
	/* 初始化字体库 */
	if (TTF_Init() == -1)
		return 0;


	/* 打开字库，设字体大小 */

//	 font = TTF_OpenFont("Times New Roman.ttf", 30);
	font = TTF_OpenFont("/system/fonts/DroidSansFallback.ttf", 30);
	if (font == NULL)
	{
		return 0;
	}
	TTF_SetFontStyle(font, TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
	SDL_Surface *pText = TTF_RenderText_Solid(font, str, (SDL_Color) { 0, 255, 255 }
	);
	if(pText==NULL)
	return 0;
	SDL_Surface *s = SDL_SetVideoMode(pText->w, pText->h, 32,
									  SDL_SWSURFACE);

	freopen("sdl.log", "w", stdout);
	printf("%d\n", s->format->BytesPerPixel);
	printf("%d\n", pText->w);
	fflush(stdout);
	// ApplySurface(80, 120, pText, s);
	/* SDL_Rect rect; rect.x=100,rect.y=100,rect.w=pText->w,rect.h=pText->h; */
	SDL_BlitSurface(pText, NULL, s, NULL);
	SDL_FreeSurface(pText);

	worker.save(s, "hello.jpg");
	 SDL_Flip(s);
	SDL_Delay(1000);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}