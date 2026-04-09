#include "display.h"
#include "spi.h"
#include "max7219.h"
#include "WaitTicks.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

static const uint8_t FontSmall8x8[15][8] =
{
	{0x00,0x1C,0x22,0x22,0x22,0x22,0x1C,0x00}, // 0
	{0x00,0x08,0x18,0x08,0x08,0x08,0x1C,0x00}, // 1
	{0x00,0x1C,0x02,0x04,0x08,0x10,0x1E,0x00}, // 2
	{0x00,0x1C,0x02,0x0C,0x02,0x02,0x1C,0x00}, // 3
	{0x00,0x04,0x0C,0x14,0x1E,0x04,0x04,0x00}, // 4
	{0x00,0x1E,0x10,0x1C,0x02,0x02,0x1C,0x00}, // 5
	{0x00,0x0C,0x10,0x1C,0x12,0x12,0x0C,0x00}, // 6
	{0x00,0x1E,0x02,0x04,0x08,0x08,0x08,0x00}, // 7
	{0x00,0x1C,0x12,0x0C,0x12,0x12,0x1C,0x00}, // 8
	{0x00,0x1C,0x12,0x12,0x0E,0x02,0x0C,0x00}, // 9
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // 10: pusty
	{0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x00}, // 11: minus
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03}, // 12: kropka
	{0x00,0x0C,0x12,0x0C,0x00,0x00,0x00,0x00}, // 13: stopnie
	{0x00,0x0E,0x10,0x10,0x10,0x10,0x0E,0x00}  // 14: C
};

static const uint8_t Glyph_DegreeC[8] = {0x00,0x0C,0x12,0x0C,0x0E,0x10,0x10,0x0E};
static const uint8_t Glyph_Sun[8]     = {0x42,0x24,0x18,0xFF,0xFF,0x18,0x24,0x42};
static const uint8_t Glyph_Moon[8]    = {0x00,0x0E,0x1C,0x38,0x38,0x1C,0x0F,0x00};

static const uint8_t* GetGlyph(uint8_t code) {
	if (code <= 14) return FontSmall8x8[code];
	return FontSmall8x8[10];
}

static void CopyGlyph(uint8_t* dst, const uint8_t* src) {
	for (uint8_t r = 0; r < 8; r++) dst[r] = src[r];
}

static void MakeDigitWithDot(uint8_t digitCode, uint8_t* out) {
	CopyGlyph(out, GetGlyph(digitCode));
	out[7] |= 0x03;
}

static void GenerateTargetRowsFromGlyphs(const uint8_t* A, const uint8_t* B,
const uint8_t* C, const uint8_t* D,
uint32_t* target) {
	for (uint8_t r = 0; r < 8; r++) {
		target[r] = ((uint32_t)A[r] << 24) | ((uint32_t)B[r] << 16)
		| ((uint32_t)C[r] <<  8) |  (uint32_t)D[r];
	}
}

static void GenerateTargetRows(uint8_t g0, uint8_t g1, uint8_t g2, uint8_t g3,
uint32_t* target) {
	GenerateTargetRowsFromGlyphs(GetGlyph(g0), GetGlyph(g1),
	GetGlyph(g2), GetGlyph(g3), target);
}

static void DrawFrame32(uint32_t* targetRows, uint8_t shift_x, uint8_t shift_y) {
	uint8_t LocVAR_Array[8];
	for (uint8_t r = 0; r < 8; r++) {
		uint32_t screen = 0;
		if (r + shift_y < 8)  screen = targetRows[r + shift_y];
		if (shift_x < 32)     screen = (screen >> (32 - shift_x));

		LocVAR_Array[0] = r+1; LocVAR_Array[1] = (screen >> 24) & 0xFF;
		LocVAR_Array[2] = r+1; LocVAR_Array[3] = (screen >> 16) & 0xFF;
		LocVAR_Array[4] = r+1; LocVAR_Array[5] = (screen >>  8) & 0xFF;
		LocVAR_Array[6] = r+1; LocVAR_Array[7] =  screen        & 0xFF;
		Fun_SPI_SendNudes(LocVAR_Array, 8);
	}
}

static void PlayAnimation(uint32_t* targetRows) {
	for (uint8_t x = 1; x <= 32; x++) { DrawFrame32(targetRows, x, 0); WaitTicks(150); }
	for (uint8_t i = 0; i < 50;  i++) { WaitTicks(50); }
	for (uint8_t y = 1; y <= 8;  y++) { DrawFrame32(targetRows, 32, y); WaitTicks(200); }
	for (uint8_t i = 0; i < 10;  i++) { WaitTicks(50); }
}

void AnimateTemperatureWithUnit(float x) {
	uint32_t target[8];
	uint8_t glyphDot[8];
	const uint8_t *g0, *g1, *g2;
	const uint8_t* g3 = Glyph_DegreeC;

	if (!(x == x) || x > 99.9f || x < -9.9f) {
		GenerateTargetRows(11, 11, 11, 11, target);
		PlayAnimation(target);
		return;
	}
	if (x < 0.0f) {
		uint16_t v = (uint16_t)((-x) * 10.0f + 0.5f);
		g0 = GetGlyph(11);
		MakeDigitWithDot((v / 10) % 10, glyphDot); g1 = glyphDot;
		g2 = GetGlyph(v % 10);
		} else {
		uint16_t v = (uint16_t)(x * 10.0f + 0.5f);
		uint8_t tens = (v / 100) % 10, ones = (v / 10) % 10, tenths = v % 10;
		g0 = (v >= 100) ? GetGlyph(tens) : GetGlyph(10);
		MakeDigitWithDot(ones, glyphDot); g1 = glyphDot;
		g2 = GetGlyph(tenths);
	}
	GenerateTargetRowsFromGlyphs(g0, g1, g2, g3, target);
	PlayAnimation(target);
}

void AnimateSun(void) {
	uint32_t target[8];
	GenerateTargetRowsFromGlyphs(GetGlyph(10), Glyph_Sun,
	GetGlyph(10), GetGlyph(10), target);
	PlayAnimation(target);
}

void AnimateMoon(void) {
	uint32_t target[8];
	GenerateTargetRowsFromGlyphs(GetGlyph(10), Glyph_Moon,
	GetGlyph(10), GetGlyph(10), target);
	PlayAnimation(target);
}