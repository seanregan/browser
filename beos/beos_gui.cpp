/*
 * Copyright 2008 François Revol <mmu_man@users.sourceforge.net>
 * Copyright 2005 James Bursa <bursa@users.sourceforge.net>
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

#define __STDBOOL_H__	1
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <curl/curl.h>

#include <Alert.h>
#include <Application.h>
#include <BeBuild.h>
#include <FindDirectory.h>
#include <Mime.h>
#include <Path.h>
#include <Roster.h>
#include <String.h>

extern "C" {

#include "content/content.h"
#include "content/content_protected.h"
#include "content/fetch.h"
#include "content/fetchers/curl.h"
#include "content/fetchers/resource.h"
#include "content/urldb.h"
#include "desktop/401login.h"
#include "desktop/browser.h"
#include "desktop/cookies.h"
#include "desktop/gui.h"
#include "desktop/netsurf.h"
#include "desktop/options.h"

#include "render/box.h"
#include "render/form.h"
#include "render/html.h"
#include "utils/filename.h"
#include "utils/log.h"
#include "utils/messages.h"
#include "utils/url.h"
#include "utils/utf8.h"
#include "utils/utils.h"
}

#include "beos/beos_gui.h"

#include "beos/beos_options.h"
//#include "beos/beos_completion.h"
#include "beos/beos_window.h"
#include "beos/options.h"
#include "beos/beos_throbber.h"
#include "beos/beos_filetype.h"
//#include "beos/beos_download.h"
#include "beos/beos_schedule.h"
#include "beos/beos_fetch_rsrc.h"
#include "beos/beos_scaffolding.h"


static void *myrealloc(void *ptr, size_t len, void *pw);
void gui_init(int argc, char** argv);


/* Where to search for shared resources.  Must have trailing / */
#define RESPATH "/boot/apps/netsurf/res/"

//TODO: use resources
// enable using resources instead of files
#define USE_RESOURCES 1

bool replicated = false; /**< if we are running as a replicant */

char *options_file_location;
char *glade_file_location;

struct gui_window *search_current_window = 0;

BWindow *wndAbout;
BWindow *wndWarning;
//GladeXML *gladeWindows;
BWindow *wndTooltip;
//beosLabel *labelTooltip;
BFilePanel *wndOpenFile;

//static beosWidget *select_menu;
static struct browser_window *select_menu_bw;
static struct form_control *select_menu_control;

static thread_id sBAppThreadID;

static BMessage *gFirstRefsReceived = NULL;

static int sEventPipe[2];

#if 0 /* GTK */
static void nsbeos_create_ssl_verify_window(struct browser_window *bw,
		hlcache_handle *c, const struct ssl_cert_info *certs,
		unsigned long num);
static void nsbeos_ssl_accept(BButton *w, gpointer data);
static void nsbeos_ssl_reject(BButton *w, gpointer data);
static void nsbeos_select_menu_clicked(BCheckMenuItem *checkmenuitem,
					gpointer user_data);
#endif

// #pragma mark - class NSBrowserFrameView


NSBrowserApplication::NSBrowserApplication()
	: BApplication("application/x-vnd.NetSurf")
{
}


NSBrowserApplication::~NSBrowserApplication()
{
}


void
NSBrowserApplication::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case B_REFS_RECEIVED:
		// messages for top-level
		// we'll just send them to the first window
		case 'back':
		case 'forw':
		case 'stop':
		case 'relo':
		case 'home':
		case 'urlc':
		case 'urle':
		case 'menu':
		// NetPositive messages
		case B_NETPOSITIVE_OPEN_URL:
		case B_NETPOSITIVE_BACK:
		case B_NETPOSITIVE_FORWARD:
		case B_NETPOSITIVE_HOME:
		case B_NETPOSITIVE_RELOAD:
		case B_NETPOSITIVE_STOP:
		case B_NETPOSITIVE_DOWN:
		case B_NETPOSITIVE_UP:
			//DetachCurrentMessage();
			//nsbeos_pipe_message(message, this, fGuiWindow);
			break;
		default:
			BApplication::MessageReceived(message);
	}
}


void
NSBrowserApplication::ArgvReceived(int32 argc, char **argv)
{
	CALLED();
	NSBrowserWindow *win = nsbeos_find_last_window();
	if (!win) {
		return;
	}
	win->Unlock();
	BMessage *message = DetachCurrentMessage();
	nsbeos_pipe_message_top(message, win, win->Scaffolding());
}


void
NSBrowserApplication::RefsReceived(BMessage *message)
{
	CALLED();
	DetachCurrentMessage();
	NSBrowserWindow *win = nsbeos_find_last_window();
	if (!win) {
		gFirstRefsReceived = message;
		return;
	}
	win->Unlock();
	nsbeos_pipe_message_top(message, win, win->Scaffolding());
}


void
NSBrowserApplication::AboutRequested()
{
	nsbeos_pipe_message(new BMessage(B_ABOUT_REQUESTED), NULL, NULL);
}


bool
NSBrowserApplication::QuitRequested()
{
	// let it notice it
	nsbeos_pipe_message(new BMessage(B_QUIT_REQUESTED), NULL, NULL);
	// we'll let the main thread Quit() ourselves when it's done.
	return false;
}


// #pragma mark - implementation


// XXX doesn't work
#if 0
static char *generate_default_css()
{
	BString text;
	rgb_color colBg = { 255, 255, 255, 255 };
	rgb_color colFg = { 0, 0, 0, 255 };
	rgb_color colControlBg = { 255, 255, 255, 255 };
	rgb_color colControlFg = { 0, 0, 0, 255 };
	const char *url = "file://beosdefault.css";

	text << "/*\n";
	text << " * This file is part of NetSurf, http://netsurf-browser.org/\n";
	text << " */\n";
	text << "\n";
	text << "/* Load base stylesheet. */\n";
	text << "\n";
	text << "@import \"default.css\";\n";
	text << "\n";
	text << "/* Apply BeOS specific rules. */\n";
	text << "\n";
	text << "\n";
	text << "\n";
	text << "\n";

	text << "input { font-size: 95%; border: medium inset #ddd; }\n";
	text << "input[type=button], input[type=reset], input[type=submit], button {\n";
	text << "	background-color: #ddd; border: medium outset #ddd; }\n";
	text << "input[type=checkbox], input[type=radio] { font-size: 105%; }\n";
	text << "input[type=file] { background-color: #ddd; border: medium inset #ddd; }\n";
	text << "\n";
	text << "select { background-color: #ddd; border: medium inset #ddd; font-size: 95%; }\n";
	text << "select:after { border-left:4px ridge #ddd; }\n";
	text << "\n";
	text << "textarea { font-size: 95%; border: medium inset #ddd; }\n";

	struct content *c;
	c = content_create(url);
	if (c == NULL)
		return NULL;

	const char *params[] = { 0 };
	if (!content_set_type(c, CONTENT_CSS, "text/css", params, NULL))
		return NULL;

	if (!content_process_data(c, text.String(), text.Length()))
		return NULL;

	content_set_done(c);

	return strdup(url);
}
#endif

/* realpath fallback on R5 */
#if !defined(__HAIKU__) && !defined(B_BEOS_VERSION_DANO)
extern "C" char *realpath(const char *f, char *buf);
char *realpath(const char *f, char *buf)
{
	BPath path(f, NULL, true);
	if (path.InitCheck() < 0) {
		strncpy(buf, f, MAXPATHLEN);
		return NULL;
	}
	//printf("RP: '%s'\n", path.Path());
	strncpy(buf, path.Path(), MAXPATHLEN);
	return buf;
}
#endif

/* finds the NetSurf binary image ID and path
 * 
 */
image_id nsbeos_find_app_path(char *path)
{
	image_info info;
	int32 cookie = 0;
	while (get_next_image_info(0, &cookie, &info) == B_OK) {
//fprintf(stderr, "%p <> %p, %p\n", (char *)&find_app_resources, (char *)info.text, (char *)info.text + info.text_size);
		if (((char *)&nsbeos_find_app_path >= (char *)info.text)
		 && ((char *)&nsbeos_find_app_path < (char *)info.text + info.text_size)) {
//fprintf(stderr, "match\n");
			if (path) {
				memset(path, 0, B_PATH_NAME_LENGTH);
				strncpy(path, info.name, B_PATH_NAME_LENGTH-1);
			}
			return info.id;
		}
	}
	return B_ERROR;
}

/**
 * Locate a shared resource file by searching known places in order.
 *
 * \param  buf      buffer to write to.  must be at least PATH_MAX chars
 * \param  filename file to look for
 * \param  def      default to return if file not found
 * \return buf
 *
 * Search order is: ~/config/settings/NetSurf/, ~/.netsurf/, $NETSURFRES/
 * (where NETSURFRES is an environment variable), and finally the path
 * specified by the #define at the top of this file.
 */

static char *find_resource(char *buf, const char *filename, const char *def)
{
	CALLED();
	const char *cdir = NULL;
	status_t err;
	BPath path;
	char t[PATH_MAX];

	err = find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.Append("NetSurf");
	if (err >= B_OK)
		cdir = path.Path();
	if (cdir != NULL) {
		strcpy(t, cdir);
		strcat(t, "/");
		strcat(t, filename);
		realpath(t, buf);
		if (access(buf, R_OK) == 0)
			return buf;
	}

	cdir = getenv("HOME");
	if (cdir != NULL) {
		strcpy(t, cdir);
		strcat(t, "/.netsurf/");
		strcat(t, filename);
		realpath(t, buf);
		if (access(buf, R_OK) == 0)
			return buf;
	}

	cdir = getenv("NETSURFRES");

	if (cdir != NULL) {
		realpath(cdir, buf);
		strcat(buf, "/");
		strcat(buf, filename);
		if (access(buf, R_OK) == 0)
			return buf;
	}

	strcpy(t, RESPATH);
	strcat(t, filename);
	realpath(t, buf);
	if (access(buf, R_OK) == 0)
		return buf;

	if (def[0] == '%') {
		snprintf(t, PATH_MAX, "%s%s", path.Path(), def + 1);
		realpath(t, buf);
	} else if (def[0] == '~') {
		snprintf(t, PATH_MAX, "%s%s", getenv("HOME"), def + 1);
		realpath(t, buf);
	} else {
		realpath(def, buf);
	}

	return buf;
}

/**
 * Check that ~/.netsurf/ exists, and if it doesn't, create it.
 */
static void check_homedir(void)
{
	CALLED();
	status_t err;

	BPath path;
	err = find_directory(B_USER_SETTINGS_DIRECTORY, &path, true);

	if (err < B_OK) {
		/* we really can't continue without a home directory. */
		LOG(("Can't find user settings directory - nowhere to store state!"));
		die("NetSurf needs to find the user settings directory in order to run.\n");
	}

	path.Append("NetSurf");
	err = create_directory(path.Path(), 0644); 
	if (err < B_OK) {
		LOG(("Unable to create %s", path.Path()));
		die("NetSurf could not create its settings directory.\n");
	}
}

static int32 bapp_thread(void *arg)
{
	be_app->Lock();
	be_app->Run();
	return 0;
}

nsurl *gui_get_resource_url(const char *path)
{
	nsurl *url = NULL;
	BString u("rsrc:/");
	if (strcmp(path, "default.css") == 0)
		u << "beosdefault.css";
	else
		u << path;
	fprintf(stderr, "%s(%s) -> '%s'\n", __FUNCTION__, path, u.String());
	nsurl_create(u.String(), &url);
	return url;
}

static void gui_init2(int argc, char** argv)
{
	CALLED();
	const char *addr = NETSURF_HOMEPAGE;

	if (option_homepage_url != NULL && option_homepage_url[0] != '\0')
		addr = option_homepage_url;

	if (argc > 1) addr = argv[1];
	if (gFirstRefsReceived) addr = NULL;
	browser_window_create(addr, 0, 0, true, false);
	if (gFirstRefsReceived) {
		// resend the refs we got before having a window to send them to
		be_app_messenger.SendMessage(gFirstRefsReceived);
		delete gFirstRefsReceived;
		gFirstRefsReceived = NULL;
	}
}

/** Normal entry point from OS */
int main(int argc, char** argv)
{
	char buf[PATH_MAX];
	setbuf(stderr, NULL);

	BPath options;
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &options, true) == B_OK) {
		options.Append("x-vnd.NetSurf");
	}

	find_resource(buf, "messages", "./beos/res/messages");
	LOG(("Using '%s' as Messages file", buf));
	//messages_load(buf);

	/* initialise netsurf */
	netsurf_init(&argc, &argv, options.Path(), buf);

    gui_init(argc, argv);
    gui_init2(argc, argv);

	netsurf_main_loop();

	netsurf_exit();

	return 0;
}


void gui_init(int argc, char** argv)
{
	char buf[PATH_MAX];
	CALLED();

	if (pipe(sEventPipe) < 0)
		return;
	if (!replicated) {
		new NSBrowserApplication;
		sBAppThreadID = spawn_thread(bapp_thread, "BApplication(NetSurf)", B_NORMAL_PRIORITY, (void *)find_thread(NULL));
		if (sBAppThreadID < B_OK)
			return; /* #### handle errors */
		if (resume_thread(sBAppThreadID) < B_OK)
			return;
	}

	// ui_color() gives hardcoded values before BApplication is created.
	nsbeos_update_system_ui_colors();

	fetch_rsrc_register();

	check_homedir();

	// make sure the cache dir exists
	create_directory(TEMP_FILENAME_PREFIX, 0600);

	//nsbeos_completion_init();


	/* This is an ugly hack to just get the new-style throbber going.
	 * It, along with the PNG throbber loader, need making more generic.
	 */
	{
#define STROF(n) #n
#define FIND_THROB(n) filenames[(n)] = \
				"throbber" STROF(n) ".png";
		char *filenames[9];
		FIND_THROB(0);
		FIND_THROB(1);
		FIND_THROB(2);
		FIND_THROB(3);
		FIND_THROB(4);
		FIND_THROB(5);
		FIND_THROB(6);
		FIND_THROB(7);
		FIND_THROB(8);
		nsbeos_throbber_initialise_from_png(9,
			filenames[0], filenames[1], filenames[2], filenames[3],
			filenames[4], filenames[5], filenames[6], filenames[7], 
			filenames[8]);
#undef FIND_THROB
#undef STROF
	}

#if 0
	find_resource(buf, "throbber.gif", "./beos/res/throbber.gif");
	nsbeos_throbber_initialise_from_gif(buf);
#endif

	if (nsbeos_throbber == NULL)
		die("Unable to load throbber image.\n");

	find_resource(buf, "Choices", "%/Choices");
	LOG(("Using '%s' as Preferences file", buf));
	options_file_location = strdup(buf);
	options_read(buf);


	/* check what the font settings are, setting them to a default font
	 * if they're not set - stops Pango whinging
	 */
#define SETFONTDEFAULT(x,y) (x) = ((x) != NULL) ? (x) : strdup((y))
	//XXX: use be_plain_font & friends, when we can check if font is serif or not.
/*
	font_family family;
	font_style style;
	be_plain_font->GetFamilyAndStyle(&family, &style);
	SETFONTDEFAULT(option_font_sans, family);
	SETFONTDEFAULT(option_font_serif, family);
	SETFONTDEFAULT(option_font_mono, family);
	SETFONTDEFAULT(option_font_cursive, family);
	SETFONTDEFAULT(option_font_fantasy, family);
*/
#ifdef __HAIKU__
	SETFONTDEFAULT(option_font_sans, "DejaVu Sans");
	SETFONTDEFAULT(option_font_serif, "DejaVu Serif");
	SETFONTDEFAULT(option_font_mono, "DejaVu Mono");
	SETFONTDEFAULT(option_font_cursive, "DejaVu Sans");
	SETFONTDEFAULT(option_font_fantasy, "DejaVu Sans");
#else
	SETFONTDEFAULT(option_font_sans, "Bitstream Vera Sans");
	SETFONTDEFAULT(option_font_serif, "Bitstream Vera Serif");
	SETFONTDEFAULT(option_font_mono, "Bitstream Vera Sans Mono");
	SETFONTDEFAULT(option_font_cursive, "Bitstream Vera Serif");
	SETFONTDEFAULT(option_font_fantasy, "Bitstream Vera Serif");
#if 0
	SETFONTDEFAULT(option_font_sans, "Swis721 BT");
	SETFONTDEFAULT(option_font_serif, "Dutch801 Rm BT");
	//SETFONTDEFAULT(option_font_mono, "Monospac821 BT");
	SETFONTDEFAULT(option_font_mono, "Courier10 BT");
	SETFONTDEFAULT(option_font_cursive, "Swis721 BT");
	SETFONTDEFAULT(option_font_fantasy, "Swis721 BT");
#endif
#endif

	nsbeos_options_init();

	if (!option_cookie_file) {
		find_resource(buf, "Cookies", "%/Cookies");
		LOG(("Using '%s' as Cookies file", buf));
		option_cookie_file = strdup(buf);
	}
	if (!option_cookie_jar) {
		find_resource(buf, "Cookies", "%/Cookies");
		LOG(("Using '%s' as Cookie Jar file", buf));
		option_cookie_jar = strdup(buf);
	}
	if (!option_cookie_file || !option_cookie_jar)
		die("Failed initialising cookie options");

	if (!option_url_file) {
		find_resource(buf, "URLs", "%/URLs");
		LOG(("Using '%s' as URL file", buf));
		option_url_file = strdup(buf);
	}

        if (!option_ca_path) {
                find_resource(buf, "certs", "/etc/ssl/certs");
                LOG(("Using '%s' as certificate path", buf));
                option_ca_path = strdup(buf);
        }

	//find_resource(buf, "mime.types", "/etc/mime.types");
	beos_fetch_filetype_init();

	urldb_load(option_url_file);
	urldb_load_cookies(option_cookie_file);

	//nsbeos_download_initialise();

	if (!replicated)
		be_app->Unlock();

#if 0 /* GTK */
	wndAbout = beos_WINDOW(glade_xml_get_widget(gladeWindows, "wndAbout"));
	beos_label_set_text(beos_LABEL(
		glade_xml_get_widget(gladeWindows, "labelVersion")),
		netsurf_version);
	beos_image_set_from_file(beos_IMAGE(
		glade_xml_get_widget(gladeWindows, "imageLogo")),
		find_resource(buf, "netsurf-logo.png", "netsurf-logo.png"));
	fontdesc = pango_font_description_from_string("Monospace 8");
	beos_widget_modify_font(beos_WIDGET(
		glade_xml_get_widget(gladeWindows, "textviewGPL")), fontdesc);

	wndWarning = beos_WINDOW(glade_xml_get_widget(gladeWindows, "wndWarning"));
	wndOpenFile = beos_DIALOG(glade_xml_get_widget(gladeWindows, "wndOpenFile"));
#endif
}




void nsbeos_pipe_message(BMessage *message, BView *_this, struct gui_window *gui)
{
	if (message == NULL) {
		fprintf(stderr, "%s(NULL)!\n", __FUNCTION__);
		return;
	}
	if (_this)
		message->AddPointer("View", _this);
	if (gui)
		message->AddPointer("gui_window", gui);
	int len = write(sEventPipe[1], &message, sizeof(void *));
	//LOG(("nsbeos_pipe_message: %d written", len));
	//printf("nsbeos_pipe_message: %d written\n", len);
}


void nsbeos_pipe_message_top(BMessage *message, BWindow *_this, struct beos_scaffolding *scaffold)
{
	if (message == NULL) {
		fprintf(stderr, "%s(NULL)!\n", __FUNCTION__);
		return;
	}
	if (_this)
		message->AddPointer("Window", _this);
	if (scaffold)
		message->AddPointer("scaffolding", scaffold);
	int len = write(sEventPipe[1], &message, sizeof(void *));
	//LOG(("nsbeos_pipe_message: %d written", len));
	//printf("nsbeos_pipe_message: %d written\n", len);
}


void gui_poll(bool active)
{
	//CALLED();
	CURLMcode code;

	fd_set read_fd_set, write_fd_set, exc_fd_set;
	int max_fd = 0;
	struct timeval timeout;
	unsigned int fd_count = 0;
	bool block = true;

	if (browser_reformat_pending)
		block = false;

	FD_ZERO(&read_fd_set);
	FD_ZERO(&write_fd_set);
	FD_ZERO(&exc_fd_set);

	if (active) {
		code = curl_multi_fdset(fetch_curl_multi,
				&read_fd_set,
				&write_fd_set,
				&exc_fd_set,
				&max_fd);
		assert(code == CURLM_OK);
	}

	// our own event pipe
	FD_SET(sEventPipe[0], &read_fd_set);
	max_fd = MAX(max_fd, sEventPipe[0] + 1);


	bigtime_t next_schedule = earliest_callback_timeout - system_time();
	if (!block)
		next_schedule = 0LL; // now
	if (block && earliest_callback_timeout != B_INFINITE_TIMEOUT)
		block = false;
	timeout.tv_sec = (long)(next_schedule / 1000000LL);
	timeout.tv_usec = (long)(next_schedule % 1000000LL);
	LOG(("gui_poll: select(%d, ..., %Ldus", max_fd, next_schedule));

	fd_count = select(max_fd, &read_fd_set, &write_fd_set, &exc_fd_set, 
		block ? NULL : &timeout);

	if (fd_count > 0 && FD_ISSET(sEventPipe[0], &read_fd_set)) {
		BMessage *message;
		int len = read(sEventPipe[0], &message, sizeof(void *));
		LOG(("gui_poll: BMessage ? %d read", len));
		if (len == sizeof(void *))
			nsbeos_dispatch_event(message);
	}

	schedule_run();

	if (browser_reformat_pending)
		nsbeos_window_process_reformats();
}


void gui_quit(void)
{
	CALLED();
	urldb_save_cookies(option_cookie_jar);
	urldb_save(option_url_file);
	//options_save_tree(hotlist,option_hotlist_file,messages_get("TreeHotlist"));

	free(option_cookie_file);
	free(option_cookie_jar);
	beos_fetch_filetype_fin();
	fetch_rsrc_unregister();
}



struct gui_download_window *gui_download_window_create(download_context *ctx,
		struct gui_window *gui)
{
	return NULL;
}


nserror gui_download_window_data(struct gui_download_window *dw, 
		const char *data, unsigned int size)
{
	return NSERROR_OK;
}


void gui_download_window_error(struct gui_download_window *dw,
		const char *error_msg)
{
}


void gui_download_window_done(struct gui_download_window *dw)
{
}

#if 0 /* GTK */
static void nsbeos_select_menu_clicked(BCheckMenuItem *checkmenuitem,
					gpointer user_data) 
{
	form_select_process_selection(select_menu_bw->current_content,
			select_menu_control, (intptr_t)user_data);
}
#endif

void gui_create_form_select_menu(struct browser_window *bw,
		struct form_control *control)
{
	CALLED();
#if 0 /* GTK */

	intptr_t i;
	struct form_option *option;
	
	beosWidget *menu_item;

	/* control->data.select.multiple is true if multiple selections
	 * are allowable.  We ignore this, as the core handles it for us.
	 * Yay. \o/
	 */
	
	if (select_menu != NULL)
		beos_widget_destroy(select_menu);
	
	select_menu = beos_menu_new();
	select_menu_bw = bw;
	select_menu_control = control;

	for (i = 0, option = control->data.select.items; option;
		i++, option = option->next) {
		menu_item = beos_check_menu_item_new_with_label(option->text);
		if (option->selected)
			beos_check_menu_item_set_active(
				beos_CHECK_MENU_ITEM(menu_item), TRUE);
		
		g_signal_connect(menu_item, "toggled",
			G_CALLBACK(nsbeos_select_menu_clicked), (gpointer)i);
		
		beos_menu_shell_append(beos_MENU_SHELL(select_menu), menu_item);
	}
	
	beos_widget_show_all(select_menu);
	
	beos_menu_popup(beos_MENU(select_menu), NULL, NULL, NULL,
			NULL /* data */, 0, beos_get_current_event_time());

#endif
}

void 
gui_window_save_link(struct gui_window *g, const char *url, const char *title)
{
}

/**
 * Send the source of a content to a text editor.
 */

void nsbeos_gui_view_source(struct hlcache_handle *content, struct selection *selection)
{
	char *temp_name;
	bool done = false;
	BPath path;
	status_t err;
	size_t size;
	const char *source = content_get_source_data(content, &size);

	if (!content || !source) {
		warn_user("MiscError", "No document source");
		return;
	}

	/* try to load local files directly. */
	temp_name = url_to_path(nsurl_access(hlcache_handle_get_url(content)));
	if (temp_name) {
		path.SetTo(temp_name);
		BEntry entry;
		if (entry.SetTo(path.Path()) >= B_OK 
			&& entry.Exists() && entry.IsFile())
			done = true;
	}
	if (!done) {
		/* We cannot release the requested filename until after it
		 * has finished being used. As we can't easily find out when
		 * this is, we simply don't bother releasing it and simply
		 * allow it to be re-used next time NetSurf is started. The
		 * memory overhead from doing this is under 1 byte per
		 * filename. */
		const char *filename = filename_request();
		if (!filename) {
			warn_user("NoMemory", 0);
			return;
		}
		path.SetTo(TEMP_FILENAME_PREFIX);
		path.Append(filename);
		BFile file(path.Path(), B_WRITE_ONLY | B_CREATE_FILE);
		err = file.InitCheck();
		if (err < B_OK) {
			warn_user("IOError", strerror(err));
			return;
		}
		err = file.Write(source, size);
		if (err < B_OK) {
			warn_user("IOError", strerror(err));
			return;
		}
		lwc_string *mime = content_get_mime_type(content);
		const char *mime_string = lwc_string_data(mime);
		if (mime)
			file.WriteAttr("BEOS:TYPE", B_MIME_STRING_TYPE, 0LL, 
				mime_string, lwc_string_length(mime) + 1);
		
	}

	entry_ref ref;
	if (get_ref_for_path(path.Path(), &ref) < B_OK)
		return;

	BMessage m(B_REFS_RECEIVED);
	m.AddRef("refs", &ref);

#if 0
	if (selection && selection->defined) {
		int32 line = -1;
		if (content_get_type(content) == CONTENT_HTML) {
			// XXX: use selection, find line in source code
		}
		if (content_get_type(content) == CONTENT_TEXTPLAIN) {
				line = MAKELINE_FROM_IDX(start_idx);
		}
		// not CSS!
		
		if (line > -1)
			message.AddInt32("be:line", line);
	}
#endif

	// apps to try
	const char *editorSigs[] = {
		"application/x-vnd.beunited.pe",
		"application/x-vnd.XEmacs",
		"application/x-vnd.Haiku-StyledEdit",
		"application/x-vnd.Be-STEE",
		"application/x-vnd.yT-STEE",
		NULL
	};
	int i;
	for (i = 0; editorSigs[i]; i++) {
		team_id team = -1;
		{
			BMessenger msgr(editorSigs[i], team);
			if (msgr.SendMessage(&m) >= B_OK)
				break;
		}
		
		err = be_roster->Launch(editorSigs[i], (BMessage *)&m, &team);
		if (err >= B_OK)
			break;
	}
}

/**
 * Broadcast an URL that we can't handle.
 */

void gui_launch_url(const char *url)
{
	status_t status;
	// try to open it as an URI
	BString mimeType = "application/x-vnd.Be.URL.";
	BString arg(url);
	mimeType.Append(arg, arg.FindFirst(":"));

	// special case, text/x-email is used traditionally
	// use it instead
	if (arg.IFindFirst("mailto:") == 0)
		mimeType = "text/x-email";

	// the protocol should be alphanum
	// we just check if it's registered
	// if not there is likely no supporting app anyway
	if (!BMimeType::IsValid(mimeType.String()))
		return;
	char *args[2] = { (char *)url, NULL };
	status = be_roster->Launch(mimeType.String(), 1, args);
	if (status < B_OK)
		warn_user("Cannot launch url", strerror(status));
}


/**
 * Display a warning for a serious problem (eg memory exhaustion).
 *
 * \param  warning  message key for warning message
 * \param  detail   additional message, or 0
 */

void warn_user(const char *warning, const char *detail)
{
	LOG(("warn_user: %s (%s)", warning, detail));
	BAlert *alert;
	BString text(warning);
	if (detail)
		text << ":\n" << detail;
#if 0
	alert = new BAlert("NetSurf Warning", text.String(), "Ok", NULL, NULL, 
		B_WIDTH_AS_USUAL, B_WARNING_ALERT);
	alert->Go();
#else
	alert = new BAlert("NetSurf Warning", text.String(), "Debug", "Ok", NULL, 
		B_WIDTH_AS_USUAL, B_WARNING_ALERT);
	if (alert->Go() < 1)
		debugger("warn_user");
#endif
}

void die(const char * const error)
{
	fprintf(stderr, "%s", error);
	BAlert *alert;
	BString text("Cannot continue:\n");
	text << error;
#if 0
	alert = new BAlert("NetSurf Error", text.String(), "Ok", NULL, NULL, 
		B_WIDTH_AS_USUAL, B_STOP_ALERT);
	alert->Go();
#else
	alert = new BAlert("NetSurf Error", text.String(), "Debug", "Ok", NULL, 
		B_WIDTH_AS_USUAL, B_STOP_ALERT);
	if (alert->Go() < 1)
		debugger("die");
#endif
	exit(EXIT_FAILURE);
}

void gui_cert_verify(const char *url, const struct ssl_cert_info *certs, 
		unsigned long num, nserror (*cb)(bool proceed, void *pw),
		void *cbpw)
{
	CALLED();
#if 0 /* GTK */
	nsbeos_create_ssl_verify_window(bw, c, certs, num);
#endif
}

static void nsbeos_create_ssl_verify_window(struct browser_window *bw,
		hlcache_handle *c, const struct ssl_cert_info *certs,
		unsigned long num)
{
	CALLED();
#if 0 /* GTK */
	GladeXML *x = glade_xml_new(glade_file_location, NULL, NULL);
	beosWindow *wnd = beos_WINDOW(glade_xml_get_widget(x, "wndSSLProblem"));
	beosButton *accept, *reject;
	void **session = calloc(sizeof(void *), 4);
	
	session[0] = bw;
	session[1] = strdup(c->url);
	session[2] = x;
	session[3] = wnd;
	
	accept = beos_BUTTON(glade_xml_get_widget(x, "sslaccept"));
	reject = beos_BUTTON(glade_xml_get_widget(x, "sslreject"));
	
	g_signal_connect(G_OBJECT(accept), "clicked",
			G_CALLBACK(nsbeos_ssl_accept), (gpointer)session);
	g_signal_connect(G_OBJECT(reject), "clicked",
			G_CALLBACK(nsbeos_ssl_reject), (gpointer)session);
	
	beos_widget_show(beos_WIDGET(wnd));	
#endif
}

#if 0 /* GTK */
static void nsbeos_ssl_accept(beosButton *w, gpointer data)
{
	void **session = data;
	struct browser_window *bw = session[0];
	char *url = session[1];
	GladeXML *x = session[2];
	beosWindow *wnd = session[3];
	
  	urldb_set_cert_permissions(url, true);
	browser_window_go(bw, url, 0, true);	
	
	beos_widget_destroy(beos_WIDGET(wnd));
	g_object_unref(G_OBJECT(x));
	free(url);
	free(session);
}

static void nsbeos_ssl_reject(beosButton *w, gpointer data)
{
	void **session = data;
	GladeXML *x = session[2];
	beosWindow *wnd = session[3];
		
	beos_widget_destroy(beos_WIDGET(wnd));
	g_object_unref(G_OBJECT(x));
	free(session[1]);
	free(session);
}
#endif

utf8_convert_ret utf8_to_local_encoding(const char *string, size_t len,
		char **result)
{
	assert(string && result);

	if (len == 0)
		len = strlen(string);

	*result = strndup(string, len);
	if (!(*result))
		return UTF8_CONVERT_NOMEM;

	return UTF8_CONVERT_OK;
}

utf8_convert_ret utf8_from_local_encoding(const char *string, size_t len,
		char **result)
{
	assert(string && result);

	if (len == 0)
		len = strlen(string);

	*result = strndup(string, len);
	if (!(*result))
		return UTF8_CONVERT_NOMEM;

	return UTF8_CONVERT_OK;
}

char *path_to_url(const char *path)
{
	int urllen = strlen(path) + FILE_SCHEME_PREFIX_LEN + 1;
	char *url = (char *)malloc(urllen);

	if (url == NULL) {
		return NULL;
	}

	if (*path == '/') {
		path++; /* file: paths are already absolute */
	} 

	snprintf(url, urllen, "%s%s", FILE_SCHEME_PREFIX, path);

	return url;
}

char *url_to_path(const char *url)
{
	char *url_path = curl_unescape(url, 0);
	char *path;

	/* return the absolute path including leading / */
	path = strdup(url_path + (FILE_SCHEME_PREFIX_LEN - 1));
	curl_free(url_path);

	return path;
}

bool cookies_update(const char *domain, const struct cookie_data *data)
{
	return true;
}

static void *myrealloc(void *ptr, size_t len, void *pw)
{
	if (len == 0) {
		free(ptr);
		return NULL;
	}

	return realloc(ptr, len);
}

/**
 * Return the filename part of a full path
 *
 * \param path full path and filename
 * \return filename (will be freed with free())
 */

char *filename_from_path(char *path)
{
	char *leafname;

	leafname = strrchr(path, '/');
	if (!leafname)
		leafname = path;
	else
		leafname += 1;

	return strdup(leafname);
}

/**
 * Add a path component/filename to an existing path
 *
 * \param path buffer containing path + free space
 * \param length length of buffer "path"
 * \param newpart string containing path component to add to path
 * \return true on success
 */

bool path_add_part(char *path, int length, const char *newpart)
{
	if(path[strlen(path) - 1] != '/')
		strncat(path, "/", length);

	strncat(path, newpart, length);

	return true;
}
