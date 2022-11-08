#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

char *title = "gnotesc";
int errno;

// Main window
GtkWidget *window, *vbox;
GtkWindow *gwindow;
GtkAccelGroup *accel = NULL;
GtkWidget *statusbar;

GdkAtom atom;
GtkClipboard *clipboard;

// Menubar
GtkWidget *menubar, *sep;

// File menu
GtkWidget *filemenu, *file;
GtkWidget *quit;

// Edit menu
GtkWidget *editmenu, *edit;
GtkWidget *cut, *copy, *paste, *delete;
GtkWidget *selectall;
GtkWidget *wll;

// Help menu
GtkWidget *helpmenu, *help;
GtkWidget *about;

// GtkTextView
GtkWidget *textarea;
GtkTextBuffer *buffer;
GtkWidget *scrolledwindow;
gboolean modified = FALSE;
char wrapping = 1;		// Wrapping will be set to 0 by default

void gtk_statusbar_update_lncol(void);