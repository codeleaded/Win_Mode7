#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"

#define MAPSIZE		1024

Sprite map;
float fWorldX;
float fWorldY;
float fWorldA;
float fNear;
float fFar;
float fFoVHalf;

void Setup(AlxWindow* w){
	map = Sprite_New(MAPSIZE,MAPSIZE);
	memset(map.img,0,sizeof(Pixel) * MAPSIZE * MAPSIZE);

	fWorldX = 0.0f;
	fWorldY = 0.0f;
	fWorldA = 0.0f;
	
	fNear = 0.0f;
	fFar = 0.0f;
	fFoVHalf = F32_PI / 4.0f;

	for(int i = 0;i<MAPSIZE;i+=32){
		for(int j = 0;j<MAPSIZE;j+=1){
			Sprite_Set(&map,i,j,MAGENTA);
			Sprite_Set(&map,i - 1,j,MAGENTA);
			Sprite_Set(&map,i + 1,j,MAGENTA);

			Sprite_Set(&map,j,i,BLUE);
			Sprite_Set(&map,j - 1,i,BLUE);
			Sprite_Set(&map,j + 1,i,BLUE);
		}
	}
}
void Update(AlxWindow* w){
	
	if (Stroke(ALX_KEY_Q).DOWN) fNear += 0.1f * w->ElapsedTime;
	if (Stroke(ALX_KEY_A).DOWN) fNear -= 0.1f * w->ElapsedTime;
	if (Stroke(ALX_KEY_W).DOWN) fFar += 0.1f * w->ElapsedTime;
	if (Stroke(ALX_KEY_S).DOWN) fFar -= 0.1f * w->ElapsedTime;
	if (Stroke(ALX_KEY_Z).DOWN) fFoVHalf += 0.1f * w->ElapsedTime;
	if (Stroke(ALX_KEY_X).DOWN) fFoVHalf -= 0.1f * w->ElapsedTime;
	
	float fFarX1 = fWorldX + cosf(fWorldA - fFoVHalf) * fFar;
	float fFarY1 = fWorldY + sinf(fWorldA - fFoVHalf) * fFar;
	float fNearX1 = fWorldX + cosf(fWorldA - fFoVHalf) * fNear;
	float fNearY1 = fWorldY + sinf(fWorldA - fFoVHalf) * fNear;
	float fFarX2 = fWorldX + cosf(fWorldA + fFoVHalf) * fFar;
	float fFarY2 = fWorldY + sinf(fWorldA + fFoVHalf) * fFar;
	float fNearX2 = fWorldX + cosf(fWorldA + fFoVHalf) * fNear;
	float fNearY2 = fWorldY + sinf(fWorldA + fFoVHalf) * fNear;
	
	for (int y = 0; y < ScreenHeight() / 2; y++){
		float fSampleDepth = (float)y / ((float)ScreenHeight() / 2.0f);		
		
		float fStartX = (fFarX1 - fNearX1) / (fSampleDepth) + fNearX1;
		float fStartY = (fFarY1 - fNearY1) / (fSampleDepth) + fNearY1;
		float fEndX = (fFarX2 - fNearX2) / (fSampleDepth) + fNearX2;
		float fEndY = (fFarY2 - fNearY2) / (fSampleDepth) + fNearY2;
		
		for (int x = 0; x < ScreenWidth(); x++){
			float fSampleWidth = (float)x / (float)ScreenWidth();
			float fSampleX = (fEndX - fStartX) * fSampleWidth + fStartX;
			float fSampleY = (fEndY - fStartY) * fSampleWidth + fStartY;
			
			fSampleX = fmod(fSampleX, 1.0f);
			fSampleY = fmod(fSampleY, 1.0f);
			
			Pixel colground = Sprite_Get(&map,fSampleX,fSampleY);
			RenderPixel(x, y + (ScreenHeight() / 2), colground);		
			
			//Pixel colsky = Sprite_Get(&sky,fSampleX,fSampleY);
			//RenderPixel(x, (ScreenHeight() / 2) - y, colsky);
		}
	}
	RenderLine(((Vec2){ 0,(float)GetHeight() / 2.0f }),((Vec2){ (float)GetWidth(),(float)GetHeight() / 2 }),CYAN,1.0f);
	if (Stroke(ALX_KEY_LEFT).DOWN)
		fWorldA -= 1.0f * w->ElapsedTime;
	if (Stroke(ALX_KEY_RIGHT).DOWN)
		fWorldA += 1.0f * w->ElapsedTime;
	if (Stroke(ALX_KEY_UP).DOWN)
	{
		fWorldX += cosf(fWorldA) * 0.2f * w->ElapsedTime;
		fWorldY += sinf(fWorldA) * 0.2f * w->ElapsedTime;
	}
	if (Stroke(ALX_KEY_DOWN).DOWN)
	{
		fWorldX -= cosf(fWorldA) * 0.2f * w->ElapsedTime;
		fWorldY -= sinf(fWorldA) * 0.2f * w->ElapsedTime;
	}

	// String str = String_Format("MI:%d",nMaxIterations);
	// CStr_RenderSizeAlxFont(WINDOW_STD_ARGS,&window.AlxFont,str.Memory,str.size,0.0f,0.0f,WHITE);
	// String_Free(&str);
}
void Delete(AlxWindow* w){
	Sprite_Free(&map);
	//Sprite_Free(&sky);
}

int main(){
    if(Create("Mode7",320,240,4,4,Setup,Update,Delete))
        Start();
    return 0;
}