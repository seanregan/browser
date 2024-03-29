/*
 * Copyright 2006 John-Mark Bell <jmb@netsurf-browser.org>
 * Copyright 2009 Paul Blokus <paul_pl@users.sourceforge.net>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file
 * Single/Multi-line UTF-8 text area (implementation)
 */

#include <stdint.h>
#include <string.h>
#include "css/css.h"
#include "css/utils.h"
#include "desktop/mouse.h"
#include "desktop/textarea.h"
#include "desktop/textinput.h"
#include "desktop/plotters.h"
#include "render/font.h"
#include "utils/log.h"
#include "utils/utf8.h"
#include "utils/utils.h"

#define MARGIN_LEFT 4
#define MARGIN_RIGHT 4
#define CARET_COLOR 0x0000FF
/* background color for readonly textarea */
#define READONLY_BG 0xD9D9D9
#define BACKGROUND_COL 0xFFFFFF
#define BORDER_COLOR 0x000000
#define SELECTION_COL 0xFFDDDD

static plot_style_t pstyle_fill_selection = {
    .fill_type = PLOT_OP_TYPE_SOLID,
    .fill_colour = SELECTION_COL,
};

static plot_style_t pstyle_stroke_border = {
    .stroke_type = PLOT_OP_TYPE_SOLID,
    .stroke_colour = BORDER_COLOR,
    .stroke_width = 1,
};

static plot_style_t pstyle_stroke_caret = {
    .stroke_type = PLOT_OP_TYPE_SOLID,
    .stroke_colour = CARET_COLOR,
    .stroke_width = 1,
};

struct line_info {
	unsigned int b_start;		/**< Byte offset of line start */
	unsigned int b_length;		/**< Byte length of line */
};

struct text_area {

	int scroll_x, scroll_y;		/**< scroll offsets of the textarea
					 * content
					 */

	unsigned int flags;		/**< Textarea flags */
	int vis_width;			/**< Visible width, in pixels */
	int vis_height;			/**< Visible height, in pixels */

	char *text;			/**< UTF-8 text */
	unsigned int text_alloc;	/**< Size of allocated text */
	unsigned int text_len;		/**< Length of text, in bytes */
	unsigned int text_utf8_len;	/**< Length of text, in characters
					 * without the trailing NUL
					 */
	struct {
		int line;		/**< Line caret is on */
		int char_off;		/**< Character index of caret within the
					 * specified line
					 */
	} caret_pos;

	int caret_x;			/**< cached X coordinate of the caret */
	int caret_y;			/**< cached Y coordinate of the caret */

	int selection_start;	/**< Character index of sel start(inclusive) */
	int selection_end;	/**< Character index of sel end(exclusive) */

	plot_font_style_t fstyle;	/**< Text style */

	int line_count;			/**< Count of lines */
#define LINE_CHUNK_SIZE 16
	struct line_info *lines;	/**< Line info array */
	int line_height;		/**< Line height obtained from style */

	/** Callback function for a redraw request */
	textarea_redraw_request_callback redraw_request;

	void *data; /** < Callback data for both callback functions */

	int drag_start_char; /**< Character index at which the drag was
			      * started
			      */
};


static bool textarea_insert_text(struct text_area *ta, unsigned int index,
		const char *text);
static bool textarea_replace_text(struct text_area *ta, unsigned int start,
		unsigned int end, const char *text);
static bool textarea_reflow(struct text_area *ta, unsigned int line);
static unsigned int textarea_get_xy_offset(struct text_area *ta, int x, int y);
static bool textarea_set_caret_xy(struct text_area *ta, int x, int y);
static bool textarea_scroll_visible(struct text_area *ta);
static bool textarea_select(struct text_area *ta, int c_start, int c_end);
static void textarea_normalise_text(struct text_area *ta,
		unsigned int b_start, unsigned int b_len);

/**
 * Create a text area
 *
 * \param x X coordinate of left border
 * \param y Y coordinate of top border
 * \param width width of the text area
 * \param height width of the text area
 * \param flags text area flags
 * \param style font style
 * \param redraw_start_callback will be called when textarea wants to redraw
 * \param redraw_end_callback will be called when textarea finisjes redrawing
 * \param data user specified data which will be passed to redraw callbacks
 * \return Opaque handle for textarea or 0 on error
 */
struct text_area *textarea_create(int width, int height,
		unsigned int flags, const plot_font_style_t *style,
		textarea_redraw_request_callback redraw_request, void *data)
{
	struct text_area *ret;

	if (redraw_request == NULL) {
		LOG(("no callback provided"));
		return NULL;
	}

	ret = malloc(sizeof(struct text_area));
	if (ret == NULL) {
		LOG(("malloc failed"));
		return NULL;
	}

	ret->redraw_request = redraw_request;
	ret->data = data;
	ret->vis_width = width;
	ret->vis_height = height;
	ret->scroll_x = 0;
	ret->scroll_y = 0;
	ret->drag_start_char = 0;


	ret->flags = flags;
	ret->text = malloc(64);
	if (ret->text == NULL) {
		LOG(("malloc failed"));
		free(ret);
		return NULL;
	}
	ret->text[0] = '\0';
	ret->text_alloc = 64;
	ret->text_len = 1;
	ret->text_utf8_len = 0;

	ret->fstyle = *style;

	ret->line_height = FIXTOINT(FDIV((FMUL(FLTTOFIX(1.2), FMUL(nscss_screen_dpi, INTTOFIX((style->size / FONT_SIZE_SCALE))))), F_72));

	ret->caret_pos.line = ret->caret_pos.char_off = 0;
	ret->caret_x = MARGIN_LEFT;
	ret->caret_y = 0;
	ret->selection_start = -1;
	ret->selection_end = -1;

	ret->line_count = 0;
	ret->lines = NULL;

	return ret;
}


/**
 * Destroy a text area
 *
 * \param ta Text area to destroy
 */
void textarea_destroy(struct text_area *ta)
{
	free(ta->text);
	free(ta->lines);
	free(ta);
}


/**
 * Set the text in a text area, discarding any current text
 *
 * \param ta Text area
 * \param text UTF-8 text to set text area's contents to
 * \return true on success, false on memory exhaustion
 */
bool textarea_set_text(struct text_area *ta, const char *text)
{
	unsigned int len = strlen(text) + 1;

	if (len >= ta->text_alloc) {
		char *temp = realloc(ta->text, len + 64);
		if (temp == NULL) {
			LOG(("realloc failed"));
			return false;
		}
		ta->text = temp;
		ta->text_alloc = len + 64;
	}

	memcpy(ta->text, text, len);
	ta->text_len = len;
	ta->text_utf8_len = utf8_length(ta->text);

	textarea_normalise_text(ta, 0, len);

	return textarea_reflow(ta, 0);
}


/**
 * Extract the text from a text area
 *
 * \param ta Text area
 * \param buf Pointer to buffer to receive data, or NULL
 *            to read length required
 * \param len Length (bytes) of buffer pointed to by buf, or 0 to read length
 * \return Length (bytes) written/required or -1 on error
 */
int textarea_get_text(struct text_area *ta, char *buf, unsigned int len)
{
	if (buf == NULL && len == 0) {
		/* want length */
		return ta->text_len;
	} else if (buf == NULL) {
		/* Can't write to NULL */
		return -1;
	}

	if (len < ta->text_len) {
		LOG(("buffer too small"));
		return -1;
	}

	memcpy(buf, ta->text, ta->text_len);

	return ta->text_len;
}


/**
 * Insert text into the text area
 *
 * \param ta Text area
 * \param index 0-based character index to insert at
 * \param text UTF-8 text to insert
 * \return false on memory exhaustion, true otherwise
 */
bool textarea_insert_text(struct text_area *ta, unsigned int index,
		const char *text)
{
	unsigned int b_len = strlen(text);
	size_t b_off;

	if (ta->flags & TEXTAREA_READONLY)
		return true;

	/* Find insertion point */
	if (index > ta->text_utf8_len)
		index = ta->text_utf8_len;

	/* find byte offset of insertion point */
	for (b_off = 0; index-- > 0;
			b_off = utf8_next(ta->text, ta->text_len, b_off))
		; /* do nothing */

	if (b_len + ta->text_len >= ta->text_alloc) {
		char *temp = realloc(ta->text, b_len + ta->text_len + 64);
		if (temp == NULL) {
			LOG(("realloc failed"));
			return false;
		}

		ta->text = temp;
		ta->text_alloc = b_len + ta->text_len + 64;
	}

	/* Shift text following up */
	memmove(ta->text + b_off + b_len, ta->text + b_off,
			ta->text_len - b_off);
	/* Insert new text */
	memcpy(ta->text + b_off, text, b_len);
	ta->text_len += b_len;
	ta->text_utf8_len += utf8_length(text);

	textarea_normalise_text(ta, b_off, b_len);

	/** \todo calculate line to reflow from */
	return textarea_reflow(ta, 0);

}


/**
 * Replace text in a text area
 *
 * \param ta Text area
 * \param start Start character index of replaced section (inclusive)
 * \param end End character index of replaced section (exclusive)
 * \param text UTF-8 text to insert
 * \return false on memory exhaustion, true otherwise
 */
bool textarea_replace_text(struct text_area *ta, unsigned int start,
		unsigned int end, const char *text)
{
	unsigned int b_len = strlen(text);
	size_t b_start, b_end, diff;

	if (ta->flags & TEXTAREA_READONLY)
		return true;

	if (start > ta->text_utf8_len)
		start = ta->text_utf8_len;
	if (end > ta->text_utf8_len)
		end = ta->text_utf8_len;

	if (start == end)
		return textarea_insert_text(ta, start, text);

	if (start > end)
		return false;

	diff = end - start;

	/* find byte offset of replace start */
	for (b_start = 0; start-- > 0;
			b_start = utf8_next(ta->text, ta->text_len, b_start))
		; /* do nothing */

	/* find byte length of replaced text */
	for (b_end = b_start; diff-- > 0;
			b_end = utf8_next(ta->text, ta->text_len, b_end))
		; /* do nothing */

	if (b_len + ta->text_len - (b_end - b_start) >= ta->text_alloc) {
		char *temp = realloc(ta->text,
			b_len + ta->text_len - (b_end - b_start) + 64);
		if (temp == NULL) {
			LOG(("realloc failed"));
			return false;
		}

		ta->text = temp;
		ta->text_alloc =
			b_len + ta->text_len - (b_end - b_start) + 64;
	}

	/* Shift text following to new position */
	memmove(ta->text + b_start + b_len, ta->text + b_end,
			ta->text_len - b_end);

	/* Insert new text */
	memcpy(ta->text + b_start, text, b_len);

	ta->text_len += b_len - (b_end - b_start);
	ta->text_utf8_len = utf8_length(ta->text);
	textarea_normalise_text(ta, b_start, b_len);

	/** \todo calculate line to reflow from */
	return textarea_reflow(ta, 0);
}


/**
 * Set the caret's position
 *
 * \param ta 		Text area
 * \param caret 	0-based character index to place caret at, -1 removes
 * 			the caret
 * \return true on success false otherwise
 */
bool textarea_set_caret(struct text_area *ta, int caret)
{
	unsigned int c_len;
	unsigned int b_off;
	int i;
	int index;
	int x, y;
	int x0, y0, x1, y1;
	int text_y_offset;

	if (ta->flags & TEXTAREA_READONLY)
		return true;

	c_len = ta->text_utf8_len;

	if (caret != -1 && (unsigned)caret > c_len)
		caret = c_len;

	if (ta->flags & TEXTAREA_MULTILINE) {
		/* Multiline textarea */
		text_y_offset = 0;
	} else {
		/* Single line text area; text is vertically centered */
		text_y_offset = (ta->vis_height - ta->line_height + 1) / 2;
	}

	/* Delete the old caret */
	if (ta->caret_pos.char_off != -1) {
		index = textarea_get_caret(ta);
		if (index == -1)
			return false;

		/* the redraw might happen in response to a text-change and
		the caret position might be beyond the current text */
		if ((unsigned)index > c_len)
			index = c_len;

		/* find byte offset of caret position */
		for (b_off = 0; index-- > 0;
				b_off = utf8_next(ta->text,
				ta->text_len, b_off))
			; /* do nothing */

		nsfont.font_width(&ta->fstyle,
				ta->text +
				ta->lines[ta->caret_pos.line].b_start,
				b_off - ta->lines[ta->caret_pos.line].b_start,
    				&x);

		x += MARGIN_LEFT - ta->scroll_x;

		y = ta->line_height * ta->caret_pos.line - ta->scroll_y;

		/* set the caret coordinate beyond the redraw rectangle */
		ta->caret_x = x - 2;

		ta->redraw_request(ta->data, x - 1, y + text_y_offset, x + 1,
				y + ta->line_height + text_y_offset);
	}

	/* check if the caret has to be drawn at all */
	if (caret != -1) {
		/* Find byte offset of caret position */
		for (b_off = 0; caret > 0; caret--)
			b_off = utf8_next(ta->text, ta->text_len, b_off);

		/* Now find line in which byte offset appears */
		for (i = 0; i < ta->line_count - 1; i++)
			if (ta->lines[i + 1].b_start > b_off)
				break;

		ta->caret_pos.line = i;

		/* Now calculate the char. offset of the caret in this line */
		for (c_len = 0, ta->caret_pos.char_off = 0;
				c_len < b_off - ta->lines[i].b_start;
				c_len = utf8_next(ta->text +
						ta->lines[i].b_start,
						ta->lines[i].b_length, c_len))
			ta->caret_pos.char_off++;

		/* Finally, redraw the caret */
		index = textarea_get_caret(ta);
		if (index == -1)
			return false;

		/* find byte offset of caret position */
		for (b_off = 0; index-- > 0;
				b_off = utf8_next(ta->text,
						ta->text_len, b_off))
			; /* do nothing */

		nsfont.font_width(&ta->fstyle,
				ta->text +
				ta->lines[ta->caret_pos.line].b_start,
				b_off - ta->lines[ta->caret_pos.line].b_start,
    				&x);

		x += MARGIN_LEFT - ta->scroll_x;
		ta->caret_x = x;
		y = ta->line_height * ta->caret_pos.line - ta->scroll_y;
		ta->caret_y = y;

		if (textarea_scroll_visible(ta))
			ta->redraw_request(ta->data, 0, 0,
					ta->vis_width,
					ta->vis_height);
		else {
			x0 = max(x - 1, MARGIN_LEFT);
			y0 = max(y + text_y_offset, 0);
			x1 = min(x + 1, ta->vis_width - MARGIN_RIGHT);
			y1 = min(y + ta->line_height + text_y_offset, ta->vis_height);
			ta->redraw_request(ta->data, x0, y0, x1, y1);
		}
	}

	return true;
}


/**
 * get character offset from the beginning of the text for some coordinates
 *
 * \param ta		Text area
 * \param x		X coordinate
 * \param y		Y coordinate
 * \return		character offset
 */
unsigned int textarea_get_xy_offset(struct text_area *ta, int x, int y)
{
	size_t b_off, temp;
	unsigned int c_off;
	int line;

	if (!ta->line_count)
		return 0;

	x = x - MARGIN_LEFT + ta->scroll_x;
	y = y + ta->scroll_y;

	if (x < 0)
		x = 0;

	line = y / ta->line_height;

	if (line < 0)
		line = 0;
	if (ta->line_count - 1 < line)
		line = ta->line_count - 1;

	nsfont.font_position_in_string(&ta->fstyle,
			ta->text + ta->lines[line].b_start,
	   		ta->lines[line].b_length, x, &b_off, &x);

	/* If the calculated byte offset corresponds with the number of bytes
	 * in the line, and the line has been soft-wrapped, then ensure the
	 * caret offset is before the trailing space character, rather than
	 * after it. Otherwise, the caret will be placed at the start of the
	 * following line, which is undesirable.
	 */
	if (b_off == (unsigned)ta->lines[line].b_length &&
			ta->text[ta->lines[line].b_start +
			ta->lines[line].b_length - 1] == ' ')
		b_off--;

	for (temp = 0, c_off = 0; temp < b_off + ta->lines[line].b_start;
			temp = utf8_next(ta->text, ta->text_len, temp))
		c_off++;

	return c_off;
}


/**
 * Set the caret's position
 *
 * \param ta Text area
 * \param x X position of caret in a window relative to text area top left
 * \param y Y position of caret in a window relative to text area top left
 * \return true on success false otherwise
 */
bool textarea_set_caret_xy(struct text_area *ta, int x, int y)
{
	unsigned int c_off;

	if (ta->flags & TEXTAREA_READONLY)
		return true;

	c_off = textarea_get_xy_offset(ta, x, y);
	return textarea_set_caret(ta, c_off);
}


/**
 * Get the caret's position
 *
 * \param ta Text area
 * \return 0-based character index of caret location, or -1 on error
 */
int textarea_get_caret(struct text_area *ta)
{
	unsigned int c_off = 0, b_off;


	/* if the text is a trailing NUL only */
	if (ta->text_utf8_len == 0)
		return 0;

	/* Calculate character offset of this line's start */
	for (b_off = 0; b_off < ta->lines[ta->caret_pos.line].b_start;
			b_off = utf8_next(ta->text, ta->text_len, b_off))
		c_off++;

	return c_off + ta->caret_pos.char_off;
}

/**
 * Reflow a text area from the given line onwards
 *
 * \param ta Text area to reflow
 * \param line Line number to begin reflow on
 * \return true on success false otherwise
 */
bool textarea_reflow(struct text_area *ta, unsigned int line)
{
	char *text;
	unsigned int len;
	size_t b_off;
	int x;
	char *space;
	unsigned int line_count = 0;

	/** \todo pay attention to line parameter */
	/** \todo create horizontal scrollbar if needed */

	ta->line_count = 0;

	if (ta->lines == NULL) {
		ta->lines =
			malloc(LINE_CHUNK_SIZE * sizeof(struct line_info));
		if (ta->lines == NULL) {
			LOG(("malloc failed"));
			return false;
		}
	}

	if (!(ta->flags & TEXTAREA_MULTILINE)) {
		/* Single line */
		ta->lines[line_count].b_start = 0;
		ta->lines[line_count++].b_length = ta->text_len - 1;

		ta->line_count = line_count;

		return true;
	}

	for (len = ta->text_len - 1, text = ta->text; len > 0;
			len -= b_off, text += b_off) {

		nsfont.font_split(&ta->fstyle, text, len,
				ta->vis_width - MARGIN_LEFT - MARGIN_RIGHT,
				&b_off, &x);

		if (line_count > 0 && line_count % LINE_CHUNK_SIZE == 0) {
			struct line_info *temp = realloc(ta->lines,
					(line_count + LINE_CHUNK_SIZE) *
					sizeof(struct line_info));
			if (temp == NULL) {
				LOG(("realloc failed"));
				return false;
			}

			ta->lines = temp;
		}

		/* handle LF */
		for (space = text; space <= text + b_off; space++) {
			if (*space == '\n')
				break;
		}

		if (space <= text + b_off) {
			/* Found newline; use it */
			ta->lines[line_count].b_start = text - ta->text;
			ta->lines[line_count++].b_length = space - text;

			b_off = space + 1 - text;

			if (len - b_off == 0) {
				/* reached end of input => add last line */
				ta->lines[line_count].b_start =
						text + b_off - ta->text;
				ta->lines[line_count++].b_length = 0;
			}

			continue;
		}

		if (len - b_off > 0) {
			/* find last space (if any) */
			for (space = text + b_off; space > text; space--)
				if (*space == ' ')
					break;

			if (space != text)
				b_off = space + 1 - text;
		}

		ta->lines[line_count].b_start = text - ta->text;
		ta->lines[line_count++].b_length = b_off;
	}

	ta->line_count = line_count;

	return true;
}


/**
 * Handle redraw requests for text areas
 *
 * \param redraw Redraw request block
 * \param x0	left X coordinate of redraw area
 * \param y0	top Y coordinate of redraw area
 * \param x1	right X coordinate of redraw area
 * \param y1	bottom Y coordinate of redraw area
 * \param ctx	current redraw context
 */
void textarea_redraw(struct text_area *ta, int x, int y,
		const struct rect *clip, const struct redraw_context *ctx)
{
	const struct plotter_table *plot = ctx->plot;
	int line0, line1, line;
	int chars, offset, text_y_offset, text_y_offset_baseline;
	unsigned int c_pos, c_len, b_start, b_end, line_len;
	char *line_text;
	struct rect r;
        plot_style_t plot_style_fill_bg = {
            .fill_type = PLOT_OP_TYPE_SOLID,
            .fill_colour = BACKGROUND_COL,
        };

	r = *clip;

	if (r.x1 < x || r.x0 > x + ta->vis_width || r.y1 < y ||
		   r.y0 > y + ta->vis_height)
		/* Textarea outside the clipping rectangle */
		return;

	if (ta->lines == NULL)
		/* Nothing to redraw */
		return;

        if (ta->flags & TEXTAREA_READONLY)
            plot_style_fill_bg.fill_colour = READONLY_BG;

	line0 = (r.y0 - y + ta->scroll_y) / ta->line_height - 1;
	line1 = (r.y1 - y + ta->scroll_y) / ta->line_height + 1;

	if (line0 < 0)
		line0 = 0;
	if (line1 < 0)
		line1 = 0;
	if (ta->line_count - 1 < line0)
		line0 = ta->line_count - 1;
	if (ta->line_count - 1 < line1)
		line1 = ta->line_count - 1;
	if (line1 < line0)
		line1 = line0;

	if (r.x0 < x)
		r.x0 = x;
	if (r.y0 < y)
		r.y0 = y;
	if (r.x1 > x + ta->vis_width)
		r.x1 = x + ta->vis_width;
	if (r.y1 > y + ta->vis_height)
		r.y1 = y + ta->vis_height;

	plot->clip(&r);
	plot->rectangle(r.x0, r.y0, r.x1, r.y1, &plot_style_fill_bg);
	plot->rectangle(x, y,
		       x + ta->vis_width - 1, y + ta->vis_height - 1,
		       &pstyle_stroke_border);

	if (r.x0 < x + MARGIN_LEFT)
		r.x0 = x + MARGIN_LEFT;
	if (r.x1 > x + ta->vis_width - MARGIN_RIGHT)
		r.x1 = x + ta->vis_width - MARGIN_RIGHT;
	plot->clip(&r);

	if (line0 > 0)
		c_pos = utf8_bounded_length(ta->text,
				ta->lines[line0].b_start - 1);
	else
		c_pos = 0;

	if (ta->flags & TEXTAREA_MULTILINE) {
		/* Multiline textarea */
		text_y_offset = 0;
		text_y_offset_baseline = (ta->line_height * 3 + 2) / 4;
	} else {
		/* Single line text area; text is vertically centered */
		text_y_offset = (ta->vis_height - ta->line_height + 1) / 2;
		text_y_offset_baseline = (ta->vis_height * 3 + 2) / 4;
	}

	for (line = line0; (line <= line1) &&
			(y + line * ta->line_height <= r.y1 + ta->scroll_y);
			line++) {
		if (ta->lines[line].b_length == 0)
			continue;

		c_len = utf8_bounded_length(
				&(ta->text[ta->lines[line].b_start]),
				ta->lines[line].b_length);

		/* if there is a newline between the lines count it too */
		if (line < ta->line_count - 1 && ta->lines[line + 1].b_start !=
				ta->lines[line].b_start +
				ta->lines[line].b_length)
			c_len++;

		/* check if a part of the line is selected, won't happen if no
		  selection (ta->selection_end = -1) */
		if (ta->selection_end != -1 &&
				c_pos < (unsigned)ta->selection_end &&
				c_pos + c_len > (unsigned)ta->selection_start) {

			/* offset from the beginning of the line */
			offset = ta->selection_start - c_pos;
			chars = ta->selection_end - c_pos -
					(offset > 0 ? offset:0);

			line_text = &(ta->text[ta->lines[line].b_start]);
			line_len = ta->lines[line].b_length;

			if (offset > 0) {

				/* find byte start of the selected part */
				for (b_start = 0; offset > 0; offset--)
					b_start = utf8_next(line_text,
							line_len,
       							b_start);
				nsfont.font_width(&ta->fstyle, line_text,
						b_start, &r.x0);
				r.x0 += x + MARGIN_LEFT;
			}
			else {
				r.x0 = x + MARGIN_LEFT;
				b_start = 0;
			}


			if (chars >= 0) {

				/* find byte end of the selected part */
				for (b_end = b_start; chars > 0 &&
						b_end < line_len;
						chars--) {
					b_end = utf8_next(line_text, line_len,
       							b_end);
				}
			}
			else
				b_end = ta->lines[line].b_length;

			b_end -= b_start;
			nsfont.font_width(&ta->fstyle,
					&(ta->text[ta->lines[line].b_start +
					b_start]),
					b_end, &r.x1);
			r.x1 += r.x0;
			plot->rectangle(r.x0 - ta->scroll_x, y +
				       line * ta->line_height +
				       1 - ta->scroll_y + text_y_offset,
				       r.x1 - ta->scroll_x,
				       y + (line + 1) * ta->line_height -
				       1 - ta->scroll_y + text_y_offset,
				       &pstyle_fill_selection);

		}

		c_pos += c_len;

		r.y0 = y + line * ta->line_height + text_y_offset_baseline;

		ta->fstyle.background =
			   	(ta->flags & TEXTAREA_READONLY) ?
					READONLY_BG : BACKGROUND_COL,

		plot->text(x + MARGIN_LEFT - ta->scroll_x,
				r.y0 - ta->scroll_y,
			  	ta->text + ta->lines[line].b_start,
			   	ta->lines[line].b_length,
				&ta->fstyle);
	}

	if ((ta->selection_end == -1 ||
			ta->selection_start == ta->selection_end) &&
			x + ta->caret_x >= clip->x0 &&
			x + ta->caret_x <= clip->x1) {
		/* There is no selection and caret is in horizontal
		 * clip range. */
		int caret_height = ta->line_height - 1;
		y += ta->caret_y + text_y_offset;
		if (y + caret_height >= clip->y0 && y <= clip->y1)
			/* Caret in vertical clip range; plot */
			plot->line(x + ta->caret_x, y + ta->caret_y,
				  	x + ta->caret_x,
					y + ta->caret_y + ta->line_height,
					&pstyle_stroke_caret);
	}
}

/**
 * Key press handling for text areas.
 *
 * \param ta	The text area which got the keypress
 * \param key	The ucs4 character codepoint
 * \return     	true if the keypress is dealt with, false otherwise.
 */
bool textarea_keypress(struct text_area *ta, uint32_t key)
{
	char utf8[6];
	unsigned int caret, caret_init, length, l_len, b_off, b_len;
	int c_line, c_chars, line;
	bool redraw = false;
	bool readonly;

	caret_init = caret = textarea_get_caret(ta);
	line = ta->caret_pos.line;
	readonly = (ta->flags & TEXTAREA_READONLY ? true:false);


	if (!(key <= 0x001F || (0x007F <= key && key <= 0x009F))) {
		/* normal character insertion */
		length = utf8_from_ucs4(key, utf8);
		utf8[length] = '\0';

		if (!textarea_insert_text(ta, caret, utf8))
			return false;
		caret++;
		redraw = true;

	} else switch (key) {
		case KEY_SELECT_ALL:
			caret = ta->text_utf8_len;

 			ta->selection_start = 0;
			ta->selection_end = ta->text_utf8_len;
 			redraw = true;
 			break;
		case KEY_COPY_SELECTION:
			break;
		case KEY_DELETE_LEFT:
			if (readonly)
				break;
			if (ta->selection_start != -1) {
				if (!textarea_replace_text(ta,
				     		ta->selection_start,
						ta->selection_end, ""))
					return false;

				caret = ta->selection_start;
				ta->selection_start = ta->selection_end = -1;
				redraw = true;
			} else {
				if (caret) {
					if (!textarea_replace_text(ta,
					     		caret - 1,
							caret, ""))
						return false;
					caret--;
					redraw = true;
				}
			}
			break;
		case KEY_NL:
			if (readonly)
				break;
			if(!textarea_insert_text(ta, caret, "\n"))
				return false;
			caret++;
			ta->selection_start = ta->selection_end = -1;
			redraw = true;
			break;
		case KEY_CUT_LINE:
		case KEY_PASTE:
		case KEY_CUT_SELECTION:
			break;
		case KEY_ESCAPE:
		case KEY_CLEAR_SELECTION:
			ta->selection_start = -1;
			ta->selection_end = -1;
			redraw = true;
			break;
		case KEY_LEFT:
			if (readonly)
				break;
			if (caret)
				caret--;
			if (ta->selection_start != -1) {
				ta->selection_start = ta->selection_end = -1;
				redraw = true;
			}
			break;
		case KEY_RIGHT:
			if (readonly)
				break;
			if (caret < ta->text_utf8_len)
				caret++;
			if (ta->selection_start != -1) {
				ta->selection_start = ta->selection_end = -1;
				redraw = true;
			}
			break;
		case KEY_PAGE_UP:
			if (readonly)
				break;
			if (ta->flags & TEXTAREA_MULTILINE) {
				/* +1 because one line is subtracted in
				   KEY_UP */
				line = ta->caret_pos.line - (ta->vis_height +
						ta->line_height - 1) /
						ta->line_height
						+ 1;
			}
			/* fall through */
		case KEY_UP:
			if (readonly)
				break;
			if (ta->selection_start != -1) {
				ta->selection_start = ta->selection_end = -1;
				redraw = true;
			}
			if (ta->flags & TEXTAREA_MULTILINE) {
				line--;
				if (line < 0)
					line = 0;
				if (line == ta->caret_pos.line)
					break;

				b_off = ta->lines[line].b_start;
				b_len = ta->lines[line].b_length;

				c_line = ta->caret_pos.line;
				c_chars = ta->caret_pos.char_off;

				if (ta->text[b_off + b_len - 1] == ' '
						&& line < ta->line_count - 1)
					b_len--;

				l_len = utf8_bounded_length(&(ta->text[b_off]),
						b_len);


				ta->caret_pos.line = line;
				ta->caret_pos.char_off = min(l_len,
						(unsigned)
						ta->caret_pos.char_off);

				caret = textarea_get_caret(ta);

				ta->caret_pos.line = c_line;
				ta->caret_pos.char_off = c_chars;
			}
			break;
		case KEY_PAGE_DOWN:
			if (readonly)
				break;
			if (ta->flags & TEXTAREA_MULTILINE) {
				/* -1 because one line is added in KEY_DOWN */
				line = ta->caret_pos.line + (ta->vis_height +
						ta->line_height - 1) /
						ta->line_height
						- 1;
			}
			/* fall through */
		case KEY_DOWN:
			if (readonly)
				break;
			if (ta->selection_start != -1) {
				ta->selection_start = ta->selection_end = -1;
				redraw = true;
			}
			if (ta->flags & TEXTAREA_MULTILINE) {
				line++;
				if (line > ta->line_count - 1)
					line = ta->line_count - 1;
				if (line == ta->caret_pos.line)
					break;

				b_off = ta->lines[line].b_start;
				b_len = ta->lines[line].b_length;

				c_line = ta->caret_pos.line;
				c_chars = ta->caret_pos.char_off;

				if (ta->text[b_off + b_len - 1] == ' '
						&& line < ta->line_count - 1)
					b_len--;

				l_len = utf8_bounded_length(&(ta->text[b_off]),
						b_len);


				ta->caret_pos.line = line;
				ta->caret_pos.char_off = min(l_len,
						(unsigned)
						ta->caret_pos.char_off);

				caret = textarea_get_caret(ta);

				ta->caret_pos.line = c_line;
				ta->caret_pos.char_off = c_chars;
			}
			break;
		case KEY_DELETE_RIGHT:
			if (readonly)
				break;
			if (ta->selection_start != -1) {
				if (!textarea_replace_text(ta,
				     		ta->selection_start,
						ta->selection_end, ""))
					return false;

				caret = ta->selection_start;
				ta->selection_start = ta->selection_end = -1;
				redraw = true;
			} else {
				if (caret < ta->text_utf8_len) {
					if (!textarea_replace_text(ta, caret,
							caret + 1, ""))
						return false;
					redraw = true;
				}
			}
			break;
		case KEY_LINE_START:
			if (readonly)
				break;
			caret -= ta->caret_pos.char_off;
			if (ta->selection_start != -1) {
				ta->selection_start = ta->selection_end = -1;
				redraw = true;
			}
			break;
		case KEY_LINE_END:
			if (readonly)
				break;

			caret = utf8_bounded_length(ta->text,
					ta->lines[ta->caret_pos.line].b_start +
					ta->lines[ta->caret_pos.line].b_length);
			if (ta->text[ta->lines[ta->caret_pos.line].b_start +
					ta->lines[ta->caret_pos.line].b_length
					- 1] == ' ')
				caret--;
			if (ta->selection_start != -1) {
				ta->selection_start = ta->selection_end = -1;
				redraw = true;
			}
			break;
		case KEY_TEXT_START:
			if (readonly)
				break;
			caret = 0;
			if (ta->selection_start != -1) {
				ta->selection_start = ta->selection_end = -1;
				redraw = true;
			}
			break;
		case KEY_TEXT_END:
			if (readonly)
				break;
			caret = ta->text_utf8_len;
			if (ta->selection_start != -1) {
				ta->selection_start = ta->selection_end = -1;
				redraw = true;
			}
			break;
		case KEY_WORD_LEFT:
		case KEY_WORD_RIGHT:
			break;
		case KEY_DELETE_LINE_END:
			if (readonly)
				break;
			if (ta->selection_start != -1) {
				if (!textarea_replace_text(ta,
				     		ta->selection_start,
						ta->selection_end, ""))
					return false;
				ta->selection_start = ta->selection_end = -1;
			} else {
				b_off = ta->lines[ta->caret_pos.line].b_start;
				b_len = ta->lines[ta->caret_pos.line].b_length;
				l_len = utf8_bounded_length(&(ta->text[b_off]),
						b_len);
				if (!textarea_replace_text(ta, caret,
						caret + l_len, ""))
					return false;
			}
			redraw = true;
			break;
		case KEY_DELETE_LINE_START:
			if (readonly)
				break;
			if (ta->selection_start != -1) {
				if (!textarea_replace_text(ta,
				     		ta->selection_start,
						ta->selection_end, ""))
					return false;
				ta->selection_start = ta->selection_end = -1;
			} else {
				if (!textarea_replace_text(ta,
						caret - ta->caret_pos.char_off,
      						caret,
						""))
					return false;
				caret -= ta->caret_pos.char_off;
			}
			redraw = true;
			break;
		default:
			return false;
	}


	if (caret != caret_init)
		textarea_set_caret(ta, caret);
	//TODO:redraw only the important part
	if (redraw) {
		ta->redraw_request(ta->data, 0, 0,
				ta->vis_width, ta->vis_height);
	}

	return true;
}

/**
 * Scrolls a textarea to make the caret visible (doesn't perform a redraw)
 *
 * \param ta	The text area to be scrolled
 * \return 	true if textarea was scrolled false otherwise
 */
bool textarea_scroll_visible(struct text_area *ta)
{
	int x0, x1, y0, y1, x, y;
	int index, b_off;
	bool scrolled = false;

	if (ta->caret_pos.char_off == -1)
		return false;

	x0 = MARGIN_LEFT;
	x1 = ta->vis_width - MARGIN_RIGHT;
	y0 = 0;
	y1 = ta->vis_height;

	index = textarea_get_caret(ta);

	/* find byte offset of caret position */
	for (b_off = 0; index-- > 0;
			b_off = utf8_next(ta->text, ta->text_len, b_off))
		; /* do nothing */

	nsfont.font_width(&ta->fstyle,
			ta->text + ta->lines[ta->caret_pos.line].b_start,
			b_off - ta->lines[ta->caret_pos.line].b_start,
			&x);

	/* top-left of caret */
	x += MARGIN_LEFT - ta->scroll_x;
	y = ta->line_height * ta->caret_pos.line - ta->scroll_y;

	/* check and change vertical scroll */
	if (y < y0) {
		ta->scroll_y -= y0 - y;
		scrolled = true;
	} else if (y + ta->line_height > y1) {
		ta->scroll_y += y + ta->line_height - y1;
		scrolled = true;
	}


	/* check and change horizontal scroll */
	if (x < x0) {
		ta->scroll_x -= x0 - x ;
		scrolled = true;
	} else if (x > x1 - 1) {
		ta->scroll_x += x - (x1 - 1);
		scrolled = true;
	}

	return scrolled;
}


/**
 * Handles all kinds of mouse action
 *
 * \param ta	Text area
 * \param mouse	the mouse state at action moment
 * \param x	X coordinate
 * \param y	Y coordinate
 * \return true if action was handled false otherwise
 */
bool textarea_mouse_action(struct text_area *ta, browser_mouse_state mouse,
		int x, int y)
{
	int c_start, c_end;

	/* mouse button pressed above the text area, move caret */
	if (mouse & BROWSER_MOUSE_PRESS_1) {
		if (!(ta->flags & TEXTAREA_READONLY))
			textarea_set_caret_xy(ta, x, y);
		if (ta->selection_start != -1) {
			ta->selection_start = ta->selection_end = -1;
			ta->redraw_request(ta->data, 0, 0,
					ta->vis_width,
					ta->vis_height);
		}
	}
	else if (mouse & BROWSER_MOUSE_DRAG_1) {
		ta->drag_start_char = textarea_get_xy_offset(ta, x, y);
		if (!(ta->flags & TEXTAREA_READONLY))
			return textarea_set_caret(ta, -1);
	}
	else if (mouse & BROWSER_MOUSE_HOLDING_1) {
		c_start = ta->drag_start_char;
		c_end = textarea_get_xy_offset(ta, x, y);
		return textarea_select(ta, c_start, c_end);

	}

	return true;
}


/**
 * Handles the end of a drag operation
 *
 * \param ta	Text area
 * \param mouse	the mouse state at drag end moment
 * \param x	X coordinate
 * \param y	Y coordinate
 * \return true if drag end was handled false otherwise
 */
bool textarea_drag_end(struct text_area *ta, browser_mouse_state mouse,
		int x, int y)
{
	int c_end;

	c_end = textarea_get_xy_offset(ta, x, y);
	return textarea_select(ta, ta->drag_start_char, c_end);
}

/**
 * Selects a character range in the textarea and redraws it
 *
 * \param ta		Text area
 * \param c_start	First character (inclusive)
 * \param c_end		Last character (exclusive)
 * \return 		true on success false otherwise
 */
bool textarea_select(struct text_area *ta, int c_start, int c_end)
{
	int swap = -1;

	/* if start is after end they get swapped, start won't and end will
	   be selected this way */
	if (c_start > c_end) {
		swap = c_start;
		c_start = c_end;
		c_end = swap;
	}

	ta->selection_start = c_start;
	ta->selection_end = c_end;

	if (!(ta->flags & TEXTAREA_READONLY)) {
		if (swap == -1)
			return textarea_set_caret(ta, c_end);
		else
			return textarea_set_caret(ta, c_start);
	}

	ta->redraw_request(ta->data, 0, 0, ta->vis_width, ta->vis_height);

	return true;
}


/**
 * Normalises any line endings within the text, replacing CRLF or CR with
 * LF as necessary. If the textarea is single line, then all linebreaks are
 * converted into spaces.
 *
 * \param ta		Text area
 * \param b_start	Byte offset to start at
 * \param b_len		Byte length to check
 */
void textarea_normalise_text(struct text_area *ta,
		unsigned int b_start, unsigned int b_len)
{
	bool multi = (ta->flags & TEXTAREA_MULTILINE) ? true:false;
	unsigned int index;

	/* Remove CR characters. If it's a CRLF pair delete it ot replace it
	 * with LF otherwise.
	 */
	for (index = 0; index < b_len; index++) {
		if (ta->text[b_start + index] == '\r') {
			if (b_start + index + 1 <= ta->text_len &&
					ta->text[b_start + index + 1] == '\n') {
				ta->text_len--;
				ta->text_utf8_len--;
				memmove(ta->text + b_start + index,
						ta->text + b_start + index + 1,
						ta->text_len - b_start - index);
			}
			else
				ta->text[b_start + index] = '\n';
		}
		/* Replace newlines with spaces if this is a single line
		 * textarea.
		 */
		if (!multi && (ta->text[b_start + index] == '\n'))
			ta->text[b_start + index] = ' ';
	}

}


/**
 * Gets the dimensions of a textarea
 *
 * \param width		if not NULL, gets updated to the width of the textarea
 * \param height	if not NULL, gets updated to the height of the textarea
 */
void textarea_get_dimensions(struct text_area *ta, int *width, int *height)
{
	if (width != NULL)
		*width = ta->vis_width;
	if (height != NULL)
		*height = ta->vis_height;
}

/**
 * Set the dimensions of a textarea, causing a reflow and
 * emitting a redraw request.
 *
 * \param width 	the new width of the textarea
 * \param height	the new height of the textarea
 */
void textarea_set_dimensions(struct text_area *ta, int width, int height)
{
	ta->vis_width = width;
	ta->vis_height = height;
	textarea_reflow(ta, 0);
	ta->redraw_request(ta->data, 0, 0, ta->vis_width, ta->vis_height);
}
