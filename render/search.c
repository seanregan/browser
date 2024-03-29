/*
 * Copyright 2004 John M Bell <jmb202@ecs.soton.ac.uk>
 * Copyright 2005 Adrian Lees <adrianl@users.sourceforge.net>
 * Copyright 2009 Mark Benjamin <netsurf-browser.org.MarkBenjamin@dfgh.net>
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
 * Free text search (core)
 */
#include "utils/config.h"

#include <ctype.h>
#include <string.h>
#include "content/content.h"
#include "content/hlcache.h"
#include "desktop/browser.h"
#include "desktop/gui.h"
#include "desktop/options.h"
#include "desktop/selection.h"
#include "render/box.h"
#include "render/html.h"
#include "render/html_internal.h"
#include "render/search.h"
#include "render/textplain.h"
#include "utils/config.h"
#include "utils/log.h"
#include "utils/messages.h"
#include "utils/url.h"
#include "utils/utils.h"


#ifndef NOF_ELEMENTS
#define NOF_ELEMENTS(array) (sizeof(array)/sizeof(*(array)))
#endif


struct list_entry {
	unsigned start_idx;	/* start position of match */
	unsigned end_idx;	/* end of match */

	struct box *start_box;	/* used only for html contents */
	struct box *end_box;

	struct selection *sel;

	struct list_entry *prev;
	struct list_entry *next;
};

struct search_context {
	struct content	 		*c;
	char 				*string;
	bool 				prev_case_sens;
	bool 				newsearch;
	bool				is_html;
	void 				*p; /* front-specific data */
	struct search_callbacks		*callbacks;
	struct list_entry 		*found;
	struct list_entry 		*current; /* first for select all */
};

static void search_text(const char *string, int string_len,
		struct search_context *context, search_flags_t flags);
static const char *find_pattern(const char *string, int s_len,
		const char *pattern, int p_len, bool case_sens, 
		unsigned int *m_len);
static bool find_occurrences_html(const char *pattern, int p_len,
		struct box *cur, bool case_sens,
		struct search_context *context);
static bool find_occurrences_text(const char *pattern, int p_len,
		struct content *c, bool case_sens,
		struct search_context *context);
static struct list_entry *add_entry(unsigned start_idx, unsigned end_idx,
		struct search_context *context);
static void free_matches(struct search_context *context);


/**
 * Find the browser window that contains the content associated with a search
 *
 * \param  search	search context
 * \return the browser window, or NULL if none
 */
static struct browser_window *search_get_browser_window(
		struct search_context *search)
{
	if (search->c != NULL) {
		if (search->is_html == true) {
			return html_get_browser_window(search->c);
		} else {
			return textplain_get_browser_window(search->c);
		}
	}
	return NULL;
}


/**
 * create a search_context
 * \param bw the browser_window the search_context is connected to
 * \param callbacks the callbacks to modify appearance according to results
 * \param p the pointer to send to the callbacks
 * \return true for success
 */
struct search_context * search_create_context(hlcache_handle *h,
		struct search_callbacks *callbacks, void *p)
{
	struct search_context *context;
	struct list_entry *search_head;
	struct content *c = hlcache_handle_get_content(h);

	if (h == NULL)
		return NULL;

	if (content_get_type(h) != CONTENT_HTML &&
			content_get_type(h) != CONTENT_TEXTPLAIN) {
		return NULL;
	}

	context = malloc(sizeof(struct search_context));
	if (context == NULL) {
		warn_user("NoMemory", 0);
		return NULL;
	}

	search_head = malloc(sizeof(struct list_entry));
	if (search_head == NULL) {
		warn_user("NoMemory", 0);
		free(context);
		return NULL;
	}

	search_head->start_idx = 0;
	search_head->end_idx = 0;
	search_head->start_box = NULL;
	search_head->end_box = NULL;
	search_head->sel = NULL;
	search_head->prev = NULL;
	search_head->next = NULL;

	context->found = search_head;
	context->current = NULL;
	context->string = NULL;
	context->prev_case_sens = false;
	context->newsearch = true;
	context->c = c;
	context->is_html = (content_get_type(h) == CONTENT_HTML) ? true : false;
	context->callbacks = callbacks;
	context->p = p;

	if (context->is_html) {
		html_set_search(context->c, context);
	} else {
		textplain_set_search(context->c, context);
	}

	return context;
}


/**
 * Begins/continues the search process
 * Note that this may be called many times for a single search.
 *
 * \param bw the browser_window to search in
 * \param flags the flags forward/back etc
 * \param string the string to match
 */

void search_step(struct search_context *context, search_flags_t flags,
		const char *string)
{
	int string_len;
	int i = 0;
	
	if ((context == NULL) || (context->callbacks == NULL)) {
		warn_user("SearchError", 0);
		return;
	}
	
	
	if (context->callbacks->add_recent != NULL)
		context->callbacks->add_recent(string, context->p);

	string_len = strlen(string);
	for(i = 0; i < string_len; i++)
		if (string[i] != '#' && string[i] != '*') break;
	if (i >= string_len) {
		struct browser_window *bw;
		free_matches(context);
		if (context->callbacks->status != NULL)
			context->callbacks->status(true, context->p);
		if (context->callbacks->back_state != NULL)
			context->callbacks->back_state(false, context->p);
		if (context->callbacks->forward_state != NULL)
			context->callbacks->forward_state(false, context->p);

		bw = search_get_browser_window(context);
		if (bw != NULL)
			browser_window_set_scroll(bw, 0, 0);
		return;
	}
	search_text(string, string_len, context, flags);
}

/**
 * Release the memory used by the list of matches,
 * deleting selection objects too
 */

void free_matches(struct search_context *context)
{
	struct list_entry *a;
	struct list_entry *b;
	
	a = context->found->next;

	/* empty the list before clearing and deleting the
	   selections because the the clearing updates the
	   screen immediately, causing nested accesses to the list */

	context->found->prev = NULL;
	context->found->next = NULL;

	for (; a; a = b) {
		b = a->next;
		if (a->sel) {
			selection_clear(a->sel, true);
			selection_destroy(a->sel);
		}
		free(a);
	}
}

/**
 * Search for a string in the box tree
 *
 * \param string the string to search for
 * \param string_len length of search string
 */
void search_text(const char *string, int string_len,
		struct search_context *context, search_flags_t flags)
{
	struct rect bounds;
	struct box *box = NULL;
	struct browser_window *bw;
	bool case_sensitive, forwards, showall;
	
	case_sensitive = ((flags & SEARCH_FLAG_CASE_SENSITIVE) != 0) ?
			true : false;
	forwards = ((flags & SEARCH_FLAG_FORWARDS) != 0) ? true : false;
	showall = ((flags & SEARCH_FLAG_SHOWALL) != 0) ? true : false;

	if (context->c == NULL)
		return;
        
	if (context->is_html == true) {
		html_content *html = (html_content *)context->c;

		box = html->layout;

		if (!box)
			return;
	}

	/* LOG(("do_search '%s' - '%s' (%p, %p) %p (%d, %d) %d",
		search_data.string, string, search_data.content, c, search_data.found->next,
		search_data.prev_case_sens, case_sens, forwards)); */

	/* check if we need to start a new search or continue an old one */
	if (context->newsearch) {
		bool res;

		if (context->string != NULL)
			free(context->string);
		context->current = NULL;
		free_matches(context);

		context->string = malloc(string_len + 1);
		if (context->string != NULL) {
			memcpy(context->string, string, string_len);
			context->string[string_len] = '\0';
		}

		if ((context->callbacks != NULL) && 
				(context->callbacks->hourglass != NULL))
			context->callbacks->hourglass(true, context->p);

		if (context->is_html == true) {
			res = find_occurrences_html(string, string_len,
					box, case_sensitive, context);
		} else {
			res = find_occurrences_text(string, string_len,
					context->c, case_sensitive, context);
		}

		if (!res) {
			free_matches(context);
			if ((context->callbacks != NULL) && 
					(context->callbacks->hourglass !=
					NULL))
				context->callbacks->hourglass(false,
						context->p);
			return;
		}
		if ((context->callbacks != NULL) && 
				(context->callbacks->hourglass != NULL))
			context->callbacks->hourglass(false, context->p);

		context->prev_case_sens = case_sensitive;
/* LOG(("%d %p %p (%p, %p)", new, search_data.found->next, search_data.current,
		search_data.current->prev, search_data.current->next)); */
		/* new search, beginning at the top of the page */
		context->current = context->found->next;
		context->newsearch = false;
	}
	else if (context->current != NULL) {
		/* continued search in the direction specified */
		if (forwards) {
			if (context->current->next)
				context->current = context->current->next;
		}
		else {
			if (context->current->prev)
				context->current = context->current->prev;
		}
	}

	if (context->callbacks == NULL)
		return;
	if (context->callbacks->status != NULL)
		context->callbacks->status((context->current != NULL), 
				context->p);
	search_show_all(showall, context);

	if (context->callbacks->back_state != NULL)
		context->callbacks->back_state((context->current != NULL) &&
				(context->current->prev != NULL),
				context->p);
	if (context->callbacks->forward_state != NULL)
		context->callbacks->forward_state((context->current != NULL) &&
				(context->current->next != NULL), context->p);

	if (context->current == NULL)
		return;

	if (context->is_html == true) {
		/* get box position and jump to it */
		box_coords(context->current->start_box, &bounds.x0, &bounds.y0);
		/* \todo: move x0 in by correct idx */
		box_coords(context->current->end_box, &bounds.x1, &bounds.y1);
		/* \todo: move x1 in by correct idx */
		bounds.x1 += context->current->end_box->width;
		bounds.y1 += context->current->end_box->height;
	} else {
		textplain_coords_from_range(context->c,
				context->current->start_idx,
				context->current->end_idx, &bounds);
	}

	bw = search_get_browser_window(context);
	if (bw != NULL)
		browser_window_scroll_visible(bw, &bounds);
}

/**
 * Find the first occurrence of 'match' in 'string' and return its index
 *
 * \param  string     the string to be searched (unterminated)
 * \param  s_len      length of the string to be searched
 * \param  pattern    the pattern for which we are searching (unterminated)
 * \param  p_len      length of pattern
 * \param  case_sens  true iff case sensitive match required
 * \param  m_len      accepts length of match in bytes
 * \return pointer to first match, NULL if none
 */

const char *find_pattern(const char *string, int s_len, const char *pattern,
		int p_len, bool case_sens, unsigned int *m_len)
{
	struct { const char *ss, *s, *p; bool first; } context[16];
	const char *ep = pattern + p_len;
	const char *es = string  + s_len;
	const char *p = pattern - 1;  /* a virtual '*' before the pattern */
	const char *ss = string;
	const char *s = string;
	bool first = true;
	int top = 0;

	while (p < ep) {
		bool matches;
		if (p < pattern || *p == '*') {
			char ch;

			/* skip any further asterisks; one is the same as many 
			*/
			do p++; while (p < ep && *p == '*');

			/* if we're at the end of the pattern, yes, it matches 
			*/
			if (p >= ep) break;

			/* anything matches a # so continue matching from
			   here, and stack a context that will try to match
			   the wildcard against the next character */

			ch = *p;
			if (ch != '#') {
				/* scan forwards until we find a match for 
				   this char */
				if (!case_sens) ch = toupper(ch);
				while (s < es) {
					if (case_sens) {
						if (*s == ch) break;
					} else if (toupper(*s) == ch)
						break;
					s++;
				}
			}

			if (s < es) {
				/* remember where we are in case the match 
				   fails; we may then resume */
				if (top < (int)NOF_ELEMENTS(context)) {
					context[top].ss = ss;
					context[top].s  = s + 1;
					context[top].p  = p - 1;
					/* ptr to last asterisk */
					context[top].first = first;
					top++;
				}

				if (first) {
					ss = s;
					/* remember first non-'*' char */
					first = false;
				}

				matches = true;
			}
			else
				matches = false;
		}
		else if (s < es) {
			char ch = *p;
			if (ch == '#')
				matches = true;
			else {
				if (case_sens)
					matches = (*s == ch);
				else
					matches = (toupper(*s) == toupper(ch));
			}
			if (matches && first) {
				ss = s;  /* remember first non-'*' char */
				first = false;
			}
		}
		else
			matches = false;

		if (matches) {
			p++; s++;
		}
		else {
			/* doesn't match, resume with stacked context if we have one */
			if (--top < 0) return NULL;  /* no match, give up */

			ss = context[top].ss;
			s  = context[top].s;
			p  = context[top].p;
			first = context[top].first;
		}
	}

	/* end of pattern reached */
	*m_len = max(s - ss, 1);
	return ss;
}

/**
 * Finds all occurrences of a given string in the html box tree
 *
 * \param pattern   the string pattern to search for
 * \param p_len     pattern length
 * \param cur       pointer to the current box
 * \param case_sens whether to perform a case sensitive search
 * \return true on success, false on memory allocation failure
 */
bool find_occurrences_html(const char *pattern, int p_len, struct box *cur,
		bool case_sens, struct search_context *context)
{
	struct box *a;

	/* ignore this box, if there's no visible text */
	if (!cur->object && cur->text) {
		const char *text = cur->text;
		unsigned length = cur->length;

		while (length > 0) {
			struct list_entry *entry;
			unsigned match_length;
			unsigned match_offset;
			const char *new_text;
			const char *pos = find_pattern(text, length,
					pattern, p_len, case_sens,
					&match_length);
			if (!pos) break;

			/* found string in box => add to list */
			match_offset = pos - cur->text;

			entry = add_entry(cur->byte_offset + match_offset,
						cur->byte_offset +
							match_offset +
							match_length, context);
			if (!entry)
				return false;

			entry->start_box = cur;
			entry->end_box = cur;

			new_text = pos + match_length;
			length -= (new_text - text);
			text = new_text;
		}
	}

	/* and recurse */
	for (a = cur->children; a; a = a->next) {
		if (!find_occurrences_html(pattern, p_len, a, case_sens,
				context))
			return false;
	}

	return true;
}

/**
 * Finds all occurrences of a given string in a textplain content
 *
 * \param pattern   the string pattern to search for
 * \param p_len     pattern length
 * \param c         the content to be searched
 * \param case_sens wheteher to perform a case sensitive search
 * \return true on success, false on memory allocation failure
 */

bool find_occurrences_text(const char *pattern, int p_len,
		struct content *c, bool case_sens,
		struct search_context *context)
{
	int nlines = textplain_line_count(c);
	int line;

	for(line = 0; line < nlines; line++) {
		size_t offset, length;
		const char *text = textplain_get_line(c, line,
				&offset, &length);
		if (text) {
			while (length > 0) {
				struct list_entry *entry;
				unsigned match_length;
				size_t start_idx;
				const char *new_text;
				const char *pos = find_pattern(text, length,
						pattern, p_len, case_sens,
						&match_length);
				if (!pos) break;

				/* found string in line => add to list */
				start_idx = offset + (pos - text);
				entry = add_entry(start_idx, start_idx +
						match_length, context);
				if (!entry)
					return false;

				new_text = pos + match_length;
				offset += (new_text - text);
				length -= (new_text - text);
				text = new_text;
			}
		}
	}

	return true;
}

/**
 * Determines whether any portion of the given text box should be
 * selected because it matches the current search string.
 *
 * \param  bw            browser window
 * \param  start_offset  byte offset within text of string to be checked
 * \param  end_offset    byte offset within text
 * \param  start_idx     byte offset within string of highlight start
 * \param  end_idx       byte offset of highlight end
 * \return true iff part of the box should be highlighted
 */

bool search_term_highlighted(struct content *c,
		unsigned start_offset, unsigned end_offset,
		unsigned *start_idx, unsigned *end_idx,
		struct search_context *context)
{
	if (c == context->c) {
		struct list_entry *a;
		for(a = context->found->next; a; a = a->next)
			if (a->sel && selection_defined(a->sel) &&
				selection_highlighted(a->sel,
					start_offset, end_offset,
					start_idx, end_idx))
				return true;
	}

	return false;
}

/**
 * Specifies whether all matches or just the current match should
 * be highlighted in the search text.
 */

void search_show_all(bool all, struct search_context *context)
{
	struct list_entry *a;

	for (a = context->found->next; a; a = a->next) {
		bool add = true;
		if (!all && a != context->current) {
			add = false;
			if (a->sel) {
				selection_clear(a->sel, true);
				selection_destroy(a->sel);
				a->sel = NULL;
			}
		}
		if (add && !a->sel) {

			if (context->is_html == true) {
				html_content *html = (html_content *)context->c;
				a->sel = selection_create(context->c, true);
				if (!a->sel)
					continue;

				selection_init(a->sel, html->layout);
			} else {
				a->sel = selection_create(context->c, false);
				if (!a->sel)
					continue;

				selection_init(a->sel, NULL);
			}

			selection_set_start(a->sel, a->start_idx);
			selection_set_end(a->sel, a->end_idx);
		}
	}
}

/**
 * Add a new entry to the list of matches
 *
 * \param  start_idx  offset of match start within textual representation
 * \param  end_idx    offset of match end
 * \return pointer to added entry, NULL iff failed
 */

struct list_entry *add_entry(unsigned start_idx, unsigned end_idx,
		struct search_context *context)
{
	struct list_entry *entry;

	/* found string in box => add to list */
	entry = calloc(1, sizeof(*entry));
	if (!entry) {
		warn_user("NoMemory", 0);
		return NULL;
	}

	entry->start_idx = start_idx;
	entry->end_idx = end_idx;
	entry->sel = NULL;

	entry->next = 0;
	entry->prev = context->found->prev;
	if (context->found->prev == NULL)
		context->found->next = entry;
	else
		context->found->prev->next = entry;
	context->found->prev = entry;

	return entry;
}

/**
 * Ends the search process, invalidating all state
 * freeing the list of found boxes
 */
void search_destroy_context(struct search_context *context)
{
	assert(context != NULL);

	if (context->c != NULL) {
		struct browser_window *bw = search_get_browser_window(context);

		/* TODO: don't poke inside bw */
		if (bw->cur_search == context) {
			bw->cur_search = NULL;
		}

		if (context->is_html)
			html_set_search(context->c, NULL);
		else
			textplain_set_search(context->c, NULL);
	}
	if ((context->string != NULL) && (context->callbacks != NULL) &&
			(context->callbacks->add_recent != NULL)) {
		context->callbacks->add_recent(context->string, context->p);
		free(context->string);
	}
	free_matches(context);
	free(context);
}

