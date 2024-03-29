/*
 * Copyright 2008-2010 Chris Young <chris@unsatisfactorysoftware.co.uk>
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

#ifndef AMIGA_GUI_H
#define AMIGA_GUI_H
#include <graphics/rastport.h>
#include "amiga/object.h"
#include <intuition/classusr.h>
#include "desktop/browser.h"
#include <dos/dos.h>
#include <devices/inputevent.h>
#include "desktop/gui.h"
#include "amiga/os3support.h"
#include "amiga/plotters.h"
#include "amiga/menu.h"

enum
{
    OID_MAIN = 0,
	OID_VSCROLL,
	OID_HSCROLL,
	OID_LAST, /* for compatibility */
	GID_MAIN,
	GID_TABLAYOUT,
	GID_BROWSER,
	GID_STATUS,
	GID_URL,
	GID_ICON,
	GID_STOP,
	GID_RELOAD,
	GID_HOME,
	GID_BACK,
	GID_FORWARD,
	GID_THROBBER,
	GID_SEARCH_ICON,
	GID_CLOSETAB,
	GID_CLOSETAB_BM,
	GID_ADDTAB,
	GID_ADDTAB_BM,
	GID_TABS,
	GID_TABS_FLAG,
	GID_USER,
	GID_PASS,
	GID_LOGIN,
	GID_CANCEL,
	GID_NEXT,
	GID_PREV,
	GID_SEARCHSTRING,
	GID_SHOWALL,
	GID_CASE,
	GID_TOOLBARLAYOUT,
	GID_HSCROLL,
	GID_LAST
};

struct find_window;
struct history_window;

struct gui_window_2 {
	struct nsObject *node;
	struct Window *win;
	Object *objects[GID_LAST];
	struct browser_window *bw;
	bool redraw_required;
	int throbber_frame;
	struct List tab_list;
	ULONG tabs;
	ULONG next_tab;
	struct Hook scrollerhook;
	struct form_control *control;
	browser_mouse_state mouse_state;
	browser_mouse_state key_state;
	ULONG throbber_update_count;
	struct find_window *searchwin;
	ULONG oldh;
	ULONG oldv;
	int temp;
	bool redraw_scroll;
	bool new_content;
	char *menulab[AMI_MENU_AREXX_MAX + 1];
	char menukey[AMI_MENU_AREXX_MAX + 1];
	UBYTE *menutype;
	struct NewMenu *menu;
	ULONG hotlist_items;
	char *svbuffer;
	char *status;
	char *wintitle;
	char *helphints[GID_LAST];
	struct timeval lastclick;
	BOOL rmbtrapped;
	struct AppIcon *appicon; /* iconify appicon */
	struct DiskObject *dobj; /* iconify appicon */
	struct Hook search_ico_hook;
	gui_drag_type drag_op;
	struct IBox *ptr_lock;
};

struct gui_window
{
	struct gui_window_2 *shared;
	int tab;
	struct Node *tab_node;
	struct Node *last_new_tab;
	int c_x; /* Caret X posn */
	int c_y; /* Caret Y posn */
	int c_w; /* Caret width */
	int c_h; /* Caret height */
	int c_h_temp;
	int scrollx;
	int scrolly;
	struct history_window *hw;
	struct List dllist;
	hlcache_handle *favicon;
	bool throbbing;
	char *tabtitle;
};

void ami_get_msg(void);
void ami_close_all_tabs(struct gui_window_2 *gwin);
void ami_quit_netsurf(void);
void ami_do_redraw(struct gui_window_2 *g);
void ami_do_redraw_limits(struct gui_window *g, struct browser_window *bw,
		int x0, int y0, int x1, int y1);
STRPTR ami_locale_langs(void);
int ami_key_to_nskey(ULONG keycode, struct InputEvent *ie);
struct box *ami_text_box_at_point(struct gui_window_2 *gwin, ULONG *x, ULONG *y);
BOOL ami_gadget_hit(Object *obj, int x, int y);
void ami_gui_history(struct gui_window_2 *gwin, bool back);

struct TextFont *origrpfont;
struct MinList *window_list;
struct Screen *scrn;
STRPTR nsscreentitle;
struct MsgPort *sport;
bool win_destroyed;
struct browser_window *curbw;
struct gui_globals browserglob;
uint32 ami_appid;
BOOL ami_autoscroll;
#endif
