/*
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 *
 * Copyright Tim Tyler
 * Copyright Michael Drake <tlsa@netsurf-browser.org>
 *
 * This font is based on the "Default" font with Zap 1.47 [TEST], which
 * was created for Zap by Tim Tyler.  It was converted to a plain text
 * format and many glyphs added for use in the NetSurf project.
 *
 * Plain text font data:
 *     http://source.netsurf-browser.org/trunk/art/fonts/netsurf/glyph_data
 *
 *     Zap: http://zap.tartarus.org/
 * NetSurf: http://www.netsurf-browser.org/
 *
 * Thanks to Tim Tyler for the original font and his permission to use it.
 * Thanks to James Aylett for helping track down Tim.
 * Thanks to Christian Ludlam for helping with Zap font info.
 *
 * Please contact Michael Drake if you want to contribute gylphs to
 * this font.
 */

/* Don't edit this file, it was generated from the plain text source data. */

#include "desktop/plotters.h"
#include "utils/utf8.h"

#include "framebuffer/font_internal.h"

#define FONTDATAMAX 4096

static const uint32_t fontdata_italic_bold[FONTDATAMAX] = {
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEE, 0xAA, 0xAA, 0xAA, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xE4, 0xAC, 0xA4, 0xA4, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEC, 0xA2, 0xA4, 0xA8, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEC, 0xA2, 0xAC, 0xA2, 0xEC, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEA, 0xAA, 0xAE, 0xA2, 0xE2, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEE, 0xA8, 0xAE, 0xA2, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xE6, 0xA8, 0xAE, 0xAA, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEE, 0xA2, 0xA4, 0xA4, 0xE4, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEE, 0xAA, 0xAE, 0xAA, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEE, 0xAA, 0xAE, 0xA2, 0xE2, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xE4, 0xAA, 0xAE, 0xAA, 0xEA, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEC, 0xAA, 0xAC, 0xAA, 0xEC, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xE4, 0xAA, 0xA8, 0xAA, 0xE4, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEC, 0xAA, 0xAA, 0xAA, 0xEC, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEE, 0xA8, 0xAC, 0xA8, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEE, 0xA8, 0xAC, 0xA8, 0xE8, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4E, 0xCA, 0x4A, 0x4A, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x44, 0xCC, 0x44, 0x44, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4C, 0xC2, 0x44, 0x48, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4C, 0xC2, 0x4C, 0x42, 0xEC, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4A, 0xCA, 0x4E, 0x42, 0xE2, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4E, 0xC8, 0x4E, 0x42, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x46, 0xC8, 0x4E, 0x4A, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4E, 0xC2, 0x44, 0x44, 0xE4, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4E, 0xCA, 0x4E, 0x4A, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4E, 0xCA, 0x4E, 0x42, 0xE2, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x44, 0xCA, 0x4E, 0x4A, 0xEA, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4C, 0xCA, 0x4C, 0x4A, 0xEC, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x44, 0xCA, 0x48, 0x4A, 0xE4, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4C, 0xCA, 0x4A, 0x4A, 0xEC, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4E, 0xC8, 0x4C, 0x48, 0xEE, 0x00, 0xFE,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0x4E, 0xC8, 0x4C, 0x48, 0xE8, 0x00, 0xFE,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x0E, 0x0E, 0x0E, 0x0E, 0x1C, 0x1C, 0x1C,
	0x1C, 0x00, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x77, 0x77, 0x77, 0x77, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x36, 0x36, 0x7F, 0x7F, 0x7F, 0x36, 0x6C,
	0xFE, 0xFE, 0xFE, 0x6C, 0x6C, 0x00, 0x00, 0x00,
	0x00, 0x08, 0x3E, 0x7F, 0x7B, 0x78, 0x7C, 0x7C,
	0x3E, 0x1E, 0xDE, 0xFE, 0x7C, 0x10, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x70, 0x73, 0x77, 0x0E,
	0x3C, 0x70, 0xEE, 0xCE, 0x0E, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x3E, 0x7E, 0x7C, 0x7C, 0x3B, 0xFE,
	0xFE, 0xEC, 0xEE, 0xFE, 0x74, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x06, 0x0E, 0x1C, 0x1C, 0x38, 0x38, 0x70,
	0x70, 0x70, 0x38, 0x38, 0x1C, 0x0C, 0x06, 0x00,
	0x60, 0x30, 0x38, 0x1C, 0x1C, 0x0E, 0x0E, 0x0E,
	0x1C, 0x1C, 0x38, 0x38, 0x70, 0x60, 0xC0, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x2A, 0x7F, 0x3E, 0x18,
	0x7C, 0xFE, 0x54, 0x10, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x7E,
	0x7E, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x38, 0x78, 0x70, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE,
	0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0F, 0x3C,
	0x78, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x3E, 0x77, 0x77, 0x77, 0x77, 0xEE,
	0xEE, 0xEE, 0xEE, 0x7C, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x0C, 0x1C, 0x3C, 0x3C, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x07, 0x07, 0x3E, 0x7C,
	0xE0, 0xE0, 0xE0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x07, 0x07, 0x1E, 0x3C,
	0x0E, 0x0E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x02, 0x06, 0x0E, 0x1E, 0x3E, 0x76, 0xEC,
	0xFE, 0xFE, 0x1C, 0x1C, 0x1C, 0x00, 0x00, 0x00,
	0x00, 0x7F, 0x7F, 0x70, 0x70, 0x7C, 0x7E, 0x0E,
	0x0E, 0x0E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7E, 0x70, 0x70, 0x7C, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x7F, 0x7F, 0x07, 0x07, 0x0E, 0x0E, 0x1C,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x77, 0x7F, 0x3E, 0x7E,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x77, 0x77, 0x7F, 0x3E,
	0x0E, 0x0E, 0x0E, 0x7C, 0x78, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x00,
	0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x00,
	0x00, 0x18, 0x38, 0x38, 0x30, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x07, 0x0E, 0x1C, 0x70,
	0x70, 0x38, 0x1C, 0x0E, 0x06, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00,
	0x00, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x30, 0x38, 0x1C, 0x0E, 0x0E,
	0x0E, 0x1C, 0x38, 0x70, 0x60, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x07, 0x07, 0x1E, 0x7C,
	0x70, 0x00, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x77, 0x77, 0x7F, 0xFE,
	0xEC, 0xE0, 0xE0, 0xFC, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x77, 0x77, 0x76, 0xFE,
	0xFE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x7E, 0x7F, 0x77, 0x77, 0x77, 0x7E, 0xFC,
	0xEE, 0xEE, 0xEE, 0xFE, 0xFC, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x77, 0x70, 0x70, 0xE0,
	0xE0, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x7C, 0x7E, 0x77, 0x77, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFC, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x7F, 0x7F, 0x70, 0x70, 0x70, 0x7C, 0xF8,
	0xE0, 0xE0, 0xE0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x7F, 0x7F, 0x70, 0x70, 0x70, 0x7C, 0xF8,
	0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x77, 0x70, 0x70, 0xFE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x7E, 0xFE,
	0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x7F, 0x7F, 0x1C, 0x1C, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x06, 0x0E,
	0x0E, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x70, 0x70, 0x72, 0x77, 0x7F, 0x7E, 0xF8,
	0xF8, 0xFC, 0xFE, 0xEE, 0xE6, 0x00, 0x00, 0x00,
	0x00, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0xE0,
	0xE0, 0xE0, 0xE0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x63, 0x77, 0x7F, 0x7F, 0x7F, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x67, 0x77, 0x77, 0x7F, 0x7F, 0x7E, 0xFE,
	0xFE, 0xFE, 0xEE, 0xEE, 0xE6, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x77, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x7E, 0x7F, 0x77, 0x77, 0x77, 0x77, 0xFE,
	0xFC, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x77, 0x77, 0x76, 0xEE,
	0xEE, 0xFE, 0xFE, 0xFE, 0x7E, 0x06, 0x00, 0x00,
	0x00, 0x7E, 0x7F, 0x77, 0x77, 0x77, 0x7E, 0xFC,
	0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x70, 0x70, 0x7C, 0x3E,
	0x0E, 0x0E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x7F, 0x7F, 0x1C, 0x1C, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x76, 0xEE,
	0x6C, 0x7C, 0x38, 0x38, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x76, 0xEE,
	0xFE, 0xFE, 0xFE, 0xEE, 0x44, 0x00, 0x00, 0x00,
	0x00, 0x77, 0x77, 0x77, 0x77, 0x3E, 0x1C, 0x38,
	0x7C, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x77, 0x77, 0x77, 0x77, 0x77, 0x3E, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x7F, 0x7F, 0x07, 0x07, 0x0E, 0x1C, 0x38,
	0x70, 0xE0, 0xE0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x1F, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x3E, 0x3E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0x78, 0x38,
	0x1C, 0x1E, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x7E, 0x7E, 0x0E, 0x0E, 0x0E, 0x0E, 0x1C,
	0x1C, 0x1C, 0x1C, 0xFC, 0xFC, 0x00, 0x00, 0x00,
	0x00, 0x08, 0x1C, 0x3E, 0x77, 0x63, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x1C, 0x0C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x3E, 0x0E,
	0x3E, 0x7E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x38, 0x38, 0x70, 0x7C, 0x7E, 0x6E,
	0xEE, 0xEE, 0xEE, 0xFC, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x7E, 0xEE,
	0xE0, 0xE0, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x07, 0x07, 0x07, 0x07, 0x3F, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x7E, 0xEE,
	0xFE, 0xFC, 0xE0, 0xFC, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x1F, 0x3F, 0x38, 0x38, 0x70, 0x7C, 0x7C,
	0x70, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x1C, 0xFC, 0xF8,
	0x00, 0x00, 0x70, 0x70, 0x70, 0x7C, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0E, 0x0E, 0x00, 0x1E, 0x3E, 0x1C,
	0x1C, 0x38, 0x38, 0x7C, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0E, 0x0E, 0x00, 0x1E, 0x3E, 0x0E,
	0x1C, 0x1C, 0x1C, 0x1C, 0x38, 0x38, 0xF8, 0xF0,
	0x00, 0x38, 0x38, 0x38, 0x3A, 0x7F, 0x7E, 0x7C,
	0x70, 0xF8, 0xFC, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x3C, 0x3C, 0x1C, 0x1C, 0x38, 0x38, 0x38,
	0x38, 0x70, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x74, 0x7E, 0xFE,
	0xFE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFC, 0x78, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFC, 0xF8, 0xE0, 0xE0, 0xE0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFC, 0x7C, 0x1C, 0x1E, 0x1E,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x7E, 0x70,
	0x70, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x3E, 0x70,
	0x7C, 0x3E, 0x0E, 0x7C, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x38, 0x38, 0x70, 0x7C, 0x7C, 0x70,
	0xE0, 0xE0, 0xE0, 0xFC, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0x7C, 0x38, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x76, 0xEE,
	0xFE, 0xFE, 0xFE, 0xEE, 0x44, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x76, 0x7E,
	0x38, 0x7C, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xFE, 0x7E, 0x1C, 0x1C, 0xFC, 0xF8,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7E, 0x1E,
	0x3C, 0x78, 0xF0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x0F, 0x1F, 0x1C, 0x1C, 0x1C, 0x78, 0xF0,
	0x38, 0x38, 0x38, 0x3E, 0x1E, 0x00, 0x00, 0x00,
	0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x00,
	0x00, 0x78, 0x7C, 0x1C, 0x1C, 0x1C, 0x0F, 0x1E,
	0x38, 0x38, 0x38, 0xF8, 0xF0, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x33, 0x7F, 0x7F, 0x66, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x7F, 0x00, 0x77, 0x55, 0x55, 0x55, 0x77,
	0x00, 0xEE, 0x28, 0x4C, 0x48, 0x48, 0x00, 0xFE,
	0x00, 0x06, 0x1F, 0x3B, 0x70, 0xFE, 0x70, 0xF8,
	0xE0, 0xE0, 0xEE, 0xFC, 0x70, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x38, 0x38, 0x6C, 0x54, 0xF6, 0xEE,
	0x6C, 0x7C, 0x28, 0x38, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x38, 0x38, 0x30, 0x60, 0x00,
	0x00, 0x1F, 0x3F, 0x38, 0x38, 0x70, 0x7C, 0x7C,
	0x70, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xEE, 0xEE, 0xCC, 0x98, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xDB, 0xDB, 0xDB, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x7F, 0x7F, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00,
	0x00, 0x1C, 0x1C, 0x1C, 0x7F, 0x7F, 0x1C, 0xFE,
	0xFE, 0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00,
	0x00, 0x3E, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x67, 0x1C,
	0x38, 0x70, 0xE0, 0xDB, 0x1B, 0x00, 0x00, 0x00,
	0x77, 0x3E, 0x00, 0x3E, 0x7F, 0x77, 0x70, 0x7C,
	0x3E, 0x0E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x1C,
	0x38, 0x70, 0x38, 0x1C, 0x0E, 0x06, 0x00, 0x00,
	0x00, 0x3F, 0x7F, 0x77, 0x77, 0x77, 0x77, 0xEF,
	0xEE, 0xEE, 0xEE, 0xFF, 0x7F, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x38, 0x38, 0x6C, 0x54, 0xF6, 0xEE,
	0x6C, 0x7C, 0x28, 0x38, 0x10, 0x00, 0x00, 0x00,
	0x77, 0x3E, 0x00, 0x7F, 0x7F, 0x07, 0x0E, 0x1C,
	0x38, 0x70, 0xE0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x38, 0x38, 0x6C, 0x54, 0xF6, 0xEE,
	0x6C, 0x7C, 0x28, 0x38, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x38, 0x38, 0x6C, 0x54, 0xF6, 0xEE,
	0x6C, 0x7C, 0x28, 0x38, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0C, 0x18, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1B, 0x36, 0x36, 0x36, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1B, 0x1B, 0x1B, 0x36, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x7E,
	0x7E, 0x7E, 0x7E, 0x3C, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x3C, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3B, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x79, 0x7F, 0x2F, 0x29, 0x29, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x77, 0x3E, 0x1C, 0x00, 0x1F, 0x3E, 0x70,
	0x7C, 0x3E, 0x0E, 0x7C, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x38, 0x38,
	0x1C, 0x0E, 0x1C, 0x38, 0x70, 0x60, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x7F, 0xFA,
	0xDE, 0xDE, 0xFC, 0xFE, 0x6E, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x38, 0x38, 0x6C, 0x54, 0xF6, 0xEE,
	0x6C, 0x7C, 0x28, 0x38, 0x10, 0x00, 0x00, 0x00,
	0x00, 0x77, 0x3E, 0x1C, 0x00, 0x7F, 0x7E, 0x1E,
	0x3C, 0x78, 0xF0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x3B, 0x00, 0x77, 0x77, 0x77, 0x77, 0x3E,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x00,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x7F, 0xD6,
	0xD0, 0xD0, 0xD6, 0xFE, 0x7C, 0x10, 0x10, 0x00,
	0x00, 0x1E, 0x3F, 0x3B, 0x38, 0x7E, 0x7E, 0xFC,
	0x70, 0x76, 0xFE, 0xFE, 0xDC, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x3E, 0xFE,
	0xFE, 0xFE, 0xFE, 0x7C, 0xEE, 0xEE, 0x00, 0x00,
	0x00, 0x00, 0x77, 0x77, 0x77, 0x3E, 0x1C, 0xFE,
	0x38, 0xFE, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x00,
	0x00, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x1E, 0x3E, 0x3E, 0x38, 0x1E, 0x3F, 0x66,
	0x7E, 0x3C, 0x0E, 0x3E, 0x3E, 0x3C, 0x00, 0x00,
	0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x1E, 0x3F, 0x61, 0x4D, 0x5D, 0x51, 0xA1,
	0xA5, 0xBD, 0x99, 0xC3, 0x7E, 0x3C, 0x00, 0x00,
	0x00, 0x3C, 0x06, 0x3E, 0x66, 0x3E, 0x00, 0xFC,
	0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E,
	0x7C, 0xF8, 0xF8, 0x7C, 0x3E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x03, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x7E, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x1E, 0x3F, 0x61, 0x59, 0x5D, 0x55, 0xB9,
	0xB9, 0xA5, 0xA5, 0xC3, 0x7E, 0x3C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x3C, 0x7E, 0x66, 0x7E, 0x3C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1C, 0x1C, 0x7F, 0x7F, 0x7F, 0x38,
	0x38, 0x00, 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x78, 0x7C, 0x0C, 0x3C, 0x78, 0xC0,
	0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x78, 0x7C, 0x0C, 0x18, 0x0C, 0xF8,
	0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1C, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0xFC, 0xE0, 0xE0, 0xC0,
	0x00, 0x00, 0x00, 0x3F, 0x6D, 0x6D, 0x6D, 0x7A,
	0x36, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x38,
	0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x18, 0x1C, 0x0C, 0x1C, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x30, 0x70, 0x30, 0x30, 0x30, 0xF0,
	0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x7C, 0x44, 0x7C, 0x38, 0x00, 0xF8,
	0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD8,
	0x6C, 0x36, 0x36, 0x6C, 0xD8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x30, 0x70, 0x31, 0x33, 0x7F, 0x1C,
	0x38, 0x76, 0xEE, 0xD6, 0x3E, 0x3E, 0x06, 0x00,
	0x00, 0x00, 0x30, 0x70, 0x31, 0x33, 0x7F, 0x1C,
	0x38, 0x70, 0xFC, 0xC6, 0x0C, 0x18, 0x1E, 0x00,
	0x00, 0x00, 0x70, 0x18, 0x31, 0x1B, 0x77, 0x1C,
	0x3A, 0x76, 0xEE, 0xD6, 0x3E, 0x3E, 0x06, 0x00,
	0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x00, 0x0E, 0x1C,
	0x7C, 0xF8, 0xE0, 0xEE, 0xEE, 0xFE, 0x7C, 0x00,
	0x00, 0x70, 0x38, 0x00, 0x08, 0x1C, 0x3E, 0xEE,
	0xFE, 0xFE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x07, 0x0E, 0x00, 0x08, 0x1C, 0x3E, 0xEE,
	0xFE, 0xFE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x36, 0x63, 0x08, 0x1C, 0x3E, 0xEE,
	0xFE, 0xFE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x3B, 0x6E, 0x00, 0x08, 0x1C, 0x3E, 0xEE,
	0xFE, 0xFE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x77, 0x77, 0x08, 0x1C, 0x3E, 0x76, 0xFE,
	0xFE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x36, 0x1C, 0x1C, 0x3E, 0x76, 0xFE,
	0xFE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x0F, 0x1F, 0x3F, 0x7F, 0x77, 0x7F, 0xFF,
	0xFE, 0xEE, 0xEF, 0xEF, 0xEF, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x70, 0x70, 0x70, 0xE0,
	0xE0, 0xE0, 0xEE, 0xFE, 0x7C, 0x0E, 0x6E, 0x7C,
	0x00, 0x38, 0x1C, 0x00, 0x7F, 0x7F, 0x70, 0xE0,
	0xF8, 0xE0, 0xE0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x0E, 0x1C, 0x00, 0x7F, 0x7F, 0x70, 0xE0,
	0xF8, 0xE0, 0xE0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x77, 0x00, 0x7F, 0x7F, 0x70, 0xE0,
	0xF8, 0xE0, 0xE0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x77, 0x77, 0x00, 0x7F, 0x7F, 0x70, 0x70, 0xF8,
	0xF8, 0xE0, 0xE0, 0xFE, 0xFE, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x1C, 0x00, 0x1C, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x0E, 0x1C, 0x00, 0x1C, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x36, 0x1C, 0x1C, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x77, 0x77, 0x00, 0x1C, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x7C, 0x7E, 0x77, 0x77, 0x76, 0xFE,
	0xEE, 0xEE, 0xEE, 0xFC, 0xF8, 0x00, 0x00, 0x00,
	0x00, 0x3B, 0x6E, 0x00, 0x77, 0x77, 0x76, 0xF6,
	0xFE, 0xDE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x1C, 0x00, 0x3E, 0x7F, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x0E, 0x1C, 0x00, 0x3E, 0x7F, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x77, 0x00, 0x3E, 0x7F, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x3B, 0x6E, 0x00, 0x3E, 0x7F, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x77, 0x77, 0x00, 0x3E, 0x7F, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x76, 0x7C,
	0x38, 0x38, 0x7C, 0xEE, 0xC6, 0x00, 0x00, 0x00,
	0x00, 0x3D, 0x7E, 0x77, 0x77, 0x77, 0x7E, 0xFE,
	0xFE, 0xEE, 0xEE, 0xFE, 0x7C, 0x80, 0x00, 0x00,
	0x00, 0x38, 0x1C, 0x00, 0x77, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x0E, 0x1C, 0x00, 0x77, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x77, 0x00, 0x77, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x77, 0x77, 0x00, 0x77, 0x77, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x0E, 0x1C, 0x00, 0x77, 0x77, 0x76, 0xEE,
	0x7C, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x70, 0x70, 0x7E, 0x7F, 0x76, 0xEE,
	0xEE, 0xFE, 0xFC, 0xE0, 0xE0, 0x00, 0x00, 0x00,
	0x00, 0x3E, 0x7F, 0x77, 0x77, 0x77, 0x7E, 0xFC,
	0xEE, 0xEE, 0xEE, 0xFE, 0xFC, 0xE0, 0xC0, 0x00,
	0x00, 0x00, 0x70, 0x38, 0x00, 0x3C, 0x3E, 0x0E,
	0x3E, 0x7E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1C, 0x38, 0x00, 0x3C, 0x3E, 0x0E,
	0x3E, 0x7E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x7C, 0xEE, 0x00, 0x3C, 0x3E, 0x0E,
	0x3E, 0x7E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x76, 0xDC, 0x00, 0x3C, 0x3E, 0x0E,
	0x3E, 0x7E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xEE, 0xEE, 0x00, 0x3C, 0x3E, 0x0E,
	0x3E, 0x7E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x38, 0x6C, 0x38, 0x00, 0x3C, 0x3E, 0x0E,
	0x3E, 0x7E, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x3A, 0x3F, 0x3F, 0x1A,
	0x7E, 0xFE, 0xD8, 0xFE, 0x7E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0xFE,
	0xEE, 0xE0, 0xEE, 0xFE, 0x7C, 0x1C, 0x7E, 0x7C,
	0x00, 0x00, 0x38, 0x1C, 0x00, 0x3E, 0x7F, 0xEE,
	0xFE, 0xFE, 0xE0, 0xFE, 0x7E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1C, 0x38, 0x00, 0x3E, 0x7F, 0xEE,
	0xFE, 0xFE, 0xE0, 0xFE, 0x7E, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x3E, 0x77, 0x00, 0x3E, 0x7F, 0xEE,
	0xFE, 0xFE, 0xE0, 0xFE, 0x7E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x77, 0x77, 0x00, 0x3E, 0x7F, 0xEE,
	0xFE, 0xFE, 0xE0, 0xFE, 0x7E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x38, 0x1C, 0x00, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0E, 0x1C, 0x00, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x3E, 0x77, 0x00, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x77, 0x77, 0x00, 0x1C, 0x1C, 0x38,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00,
	0x00, 0x76, 0x3E, 0x7C, 0x7E, 0x3F, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x00, 0x00, 0x00,
	0x00, 0x3B, 0x7F, 0x6E, 0x00, 0x7E, 0x7F, 0xEE,
	0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x70, 0x38, 0x00, 0x3C, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFC, 0x78, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1C, 0x38, 0x00, 0x3C, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFC, 0x78, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x3E, 0x77, 0x00, 0x3C, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFC, 0x78, 0x00, 0x00, 0x00,
	0x00, 0x3B, 0x7F, 0x6E, 0x00, 0x3C, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFC, 0x78, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x77, 0x77, 0x00, 0x3C, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFC, 0x78, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x1C, 0x1C, 0x00, 0xFE,
	0xFE, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x3E, 0x7E, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7C, 0x80, 0x00, 0x00,
	0x00, 0x00, 0x70, 0x38, 0x00, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x1C, 0x38, 0x00, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7E, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x3E, 0x77, 0x00, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x77, 0x77, 0x00, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7E, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x0E, 0x1C, 0x00, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7E, 0x0E, 0xFE, 0xFC,
	0x00, 0x00, 0x70, 0x70, 0x7E, 0x7F, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xEE, 0xFE, 0xFC, 0xE0, 0xE0,
	0x00, 0x00, 0x77, 0x77, 0x00, 0x77, 0x76, 0xEE,
	0xEE, 0xEE, 0xEE, 0xFE, 0x7E, 0x0E, 0xFE, 0xFC,
};

const struct fb_font_desc font_italic_bold = {
	.name = "NetSurf Italic Bold",
	.width = 8,
	.height = 16,
	.encoding = "CP1252",
	.data = fontdata_italic_bold,
};
