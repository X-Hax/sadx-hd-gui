#include <SADXModLoader.h>
#include "TitleScreen.h"

static float xoffset = 0;
static float sonic_xoffset = 832;
static float logo_xoffset = 800;
static bool logoback = false;

void DrawTitleScreenShit_cdecl(TitleScreenData* a1)
{
	NJS_COLOR StartColor = { 0x00FFFFFF };
	unsigned int v1; // ecx
	float xpos = 0;
	float ypos = 0;
	float vscale = 1.0f;
	float hzscale = 1.0f;
	float center_x = (float)HorizontalResolution / 2.0f;
	float center_y = (float)VerticalResolution / 2.0f;
	unsigned char PressStartTransparency;
	NJS_COLOR color1 = { 0xFF0000EE };
	NJS_COLOR color2 = { 0xFF0000EE };
	NJS_COLOR color3 = { 0xFF310072 };
	NJS_COLOR color4 = { 0xFF310072 };

	vscale = (float)VerticalResolution / 480.0f;
	hzscale = (float)HorizontalResolution / 640.0f;
	v1 = a1->dword30;
	if (v1 < 2) xoffset = 0;
	if (v1 >= 0x3C)
	{
		SetVtxColorA(0xFFFFFFFF);
	}
	else
	{
		SetVtxColorA(((char)(255 * v1 / 0x3C) << 24) + 0xFFFFFF);
		sonic_xoffset = ((float)HorizontalResolution / vscale) + 256.0f;
		logo_xoffset = ((float)HorizontalResolution / vscale) + 256.0f;
		logoback = false;
	}
	SetVtxColorB(0xFFFFFFFF);
	njSetTexture(&ava_title_cmn_TEXLIST);
	njTextureShadingMode(1); //Disable alpha rejection
	if (!MissedFrames)
	{
		//The full scrolling clouds image is 2560 (1280 at 1x) pixels wide
		float scale = 1280 * vscale;
		int delta = HorizontalResolution - (int)scale;
		int times = 2 + ceil(delta / scale);
		//PrintDebug("Vscale: %f, Scale: %f\n", vscale, times);
		//Draw the scrolling clouds
		ypos = -10 * vscale;
		//Draw the four rows of clouds
		xpos = 0;
		for (int q = 0; q < times + 1; q++)
		{
			for (int i = 0; i < 10; i++)
			{
				xpos = 256.0f * vscale * 0.5f * (i + 10 * q) - xoffset;
				//if (xpos <= -2560.0f/vscale) xoffset = 0;
				//PrintDebug("xpos: %f\n", xpos);
				DrawBG(30 + i, xpos, ypos, 2.0f, vscale * 0.5f, vscale * 0.5f);
				DrawBG(40 + i, xpos, ypos + 256.0f * vscale * 0.5f, 2.0f, vscale * 0.5f, vscale * 0.5f);
				DrawBG(50 + i, xpos, ypos + 512.0f * vscale * 0.5f, 2.0f, vscale * 0.5f, vscale * 0.5f);
				DrawBG(60 + i, xpos, ypos + 768.0f * vscale * 0.5f, 2.0f, vscale * 0.5f, vscale * 0.5f);
			}
		}
		if (vscale >= 0.5f) xoffset += 2 * vscale;
		else xoffset = 0;
		//PrintDebug("v1: %f\n", xoffset*vscale* 0.5f);
		//Draw the circles around Sonic (front)
		xpos = center_x - 320.0f * vscale + 280 * vscale;
		ypos = center_y - 240.0f * vscale + 52 * vscale;
		DrawBG(2, xpos, ypos, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(3, xpos + 256.0f * vscale * 0.5f, ypos, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(4, xpos, ypos + 256.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(5, xpos + 256.0f * vscale * 0.5f, ypos + 256.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(6, xpos, ypos + 512.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(7, xpos + 256.0f * vscale * 0.5f, ypos + 512.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		//Draw the circles around Sonic (back)
		xpos = center_x - 320.0f * vscale - 90.0f * vscale;
		ypos = center_y - 240.0f * vscale + 52 * vscale;
		DrawBG(8, xpos, ypos, 1.3f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(9, xpos, ypos + 256.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(10, xpos, ypos + 512.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		//Draw the gradient overlay/border thing
		//Left
		if (v1 <= 60)
		{
			color1.argb.a = int(((float)v1 / 60.0f) * 255);
			color2.argb.a = int(((float)v1 / 60.0f) * 255);
			color3.argb.a = int(((float)v1 / 60.0f) * 255);
			color4.argb.a = int(((float)v1 / 60.0f) * 255);
		}
		else
		{
			color1.argb.a = 255;
			color2.argb.a = 255;
			color3.argb.a = 255;
			color4.argb.a = 255;
		}
		ScreenTextureVertices[0].col = color1.color;
		ScreenTextureVertices[1].col = color2.color;
		//Right
		ScreenTextureVertices[2].col = color3.color;
		ScreenTextureVertices[3].col = color4.color;
		DrawBG(1, 0, 0, 1.3f, (float)HorizontalResolution / 256.0f, vscale * 0.5f);
		ypos = (float)VerticalResolution - 256.0f * vscale * 0.5f;
		DrawBG(0x40000001, 0, ypos, 1.3f, (float)HorizontalResolution / 256.0f, vscale * 0.5f);
		SetVtxColorB(0xFFFFFFFF);
		//Draw copyright text
		xpos = center_x;
		ypos = center_y + 140.0f * vscale;
		DrawBG(11, xpos, ypos, 1.3f, vscale * 0.4f, vscale * 0.4f);
		DrawBG(12, xpos + 256.0f * vscale * 0.4f, ypos, 1.3f, vscale * 0.4f, vscale * 0.4f);
		DrawBG(13, xpos + 512.0f * vscale * 0.4f, ypos, 1.3f, vscale * 0.4f, vscale * 0.4f);
		//Draw the lines above and below the gradients
		DrawBG(0, 0, 0, 1.3f, (float)HorizontalResolution / 256.0f, vscale * 0.5f);
		ypos = (float)VerticalResolution - 256.0f * vscale * 0.5f;
		DrawBG(0x40000000, 0, ypos, 1.3f, (float)HorizontalResolution / 256.0f, vscale * 0.5f);
		//Draw Sonic
		xpos = center_x - 320.0f * vscale - (128.0f - sonic_xoffset) * vscale;
		ypos = center_y - 260.0f * vscale;
		if (v1 >= 60)
		{
			if (sonic_xoffset > 63) sonic_xoffset -= 64; else sonic_xoffset = 0;
		}
		int sonictex = 0;
		for (int v = 0; v < 4; v++)
		{
			for (int h = 0; h < 4; h++)
			{
				float finalx = xpos + 256.0f * h * vscale * 0.5f;
				float finaly = ypos + 256.0f * v * vscale * 0.5f;
				DrawBG(14 + sonictex, finalx, finaly, 1.2f, vscale * 0.5f, vscale * 0.5f);
				sonictex++;
			}
		}
		//Draw logo
		njSetTexture(&ava_gtitle0_e_TEXLIST);
		xpos = center_x - (171.0f + logo_xoffset) * vscale;
		ypos = center_y - 143.0f * vscale;
		DrawBG(0, xpos, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(1, xpos + 256.0f * vscale * 0.5f, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(2, xpos + 512.0f * vscale * 0.5f, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(3, xpos + 768.0f * vscale * 0.5f, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(4, xpos, ypos + 256.0f * vscale * 0.5f, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(5, xpos + 256.0f * vscale * 0.5f, ypos + 256.0f * vscale * 0.5f, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(6, xpos + 512.0f * vscale * 0.5f, ypos + 256.0f * vscale * 0.5f, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(7, xpos + 768.0f * vscale * 0.5f, ypos + 256.0f * vscale * 0.5f, 1.1f, vscale * 0.5f, vscale * 0.5f);
		//PrintDebug("logo: %f\n", logo_xoffset);
		//PrintDebug("Start: %d\n", a1->dword28);
		if (v1 >= 80)
		{
			if (!logoback)
			{
				if (logo_xoffset > -128) logo_xoffset -= 64;
				else
				{
					logoback = true;
				}
			}
			else
			{
				if (logo_xoffset < -63) logo_xoffset += 64; else logo_xoffset = 0;
			}
		}
		//Increase timer
		a1->dword30 += MissedFrames_B;
		//Draw Press Start
		if (a1->dword24 >= 150)
		{
			PressStartTransparency = a1->dword28;
			if ((a1->dword28 & 0x1FFu) >= 0x100)
			{
				PressStartTransparency = -1 - a1->dword28;
			}
			StartColor.argb.a = PressStartTransparency;
			SetVtxColorB(StartColor.color);
			xpos = center_x + 32.0f * vscale;
			ypos = center_y + 84 * vscale;
			njTextureShadingMode(1);
			njSetTexture(&ava_gtitle0_e_TEXLIST);
			DrawBG(8, xpos, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
			DrawBG(9, xpos + 128.0f * vscale, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
			njTextureShadingMode(2);
			SetVtxColorB(0xFFFFFFFF);
		}
	}
}

static void __declspec(naked) DrawTitleScreenShit_asm()
{
	__asm
	{
		push esi // a1

		// Call your __cdecl function here:
		call DrawTitleScreenShit_cdecl

		pop esi // a1
		retn
	}
}

void TitleScreen_Init()
{
	WriteCall((void*)0x51037E, DrawTitleScreenShit_asm);
}

void TitleScreen_OnReset()
{
	xoffset = 0;
}