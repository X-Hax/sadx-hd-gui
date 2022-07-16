#pragma once
struct SAlMsgFontChar
{
	__int16 mCeid;
	__int16 mPosX;
	__int16 mPosY;
	unsigned __int8 mFlag;
	unsigned int mColor;
};

struct SAlMsgFontStr
{
	__int16 mNChar;
	__int16 mSizeX;
	__int16 mSizeY;
	SAlMsgFontChar* mChar;
};

struct SAlMsgTelop
{
	float mPosX;
	float mPosY;
	float mPosZ;
	float mSizeX;
	float mSizeY;
	SAlMsgFontStr** mStrArray;
	__int16 mStrNum;
	__int16 mStrIndex;
	float mSpeed;
	float mScroll;
};