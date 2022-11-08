#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "gnotesc.h"


void gtk_notepad_show_about_box(void)
{
	GtkWidget *dialog = gtk_about_dialog_new();
	GtkAboutDialog *dlg = GTK_ABOUT_DIALOG(dialog);
	gtk_about_dialog_set_program_name(dlg, "GTK Notepad");
	gtk_about_dialog_set_version(dlg, "0.1");
	gtk_about_dialog_set_copyright(dlg, "© Hoang Van Gioi");
	gtk_about_dialog_set_comments(dlg, "GTK Notepad is a text editing "
				      "program written in C and GTK. "
				      "\nOriginal by Hoang Van Gioi");;
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

/* Callbacks */

void gtk_notepad_text_changed(void)
{
	modified = TRUE;
	gtk_statusbar_update_lncol();
}

void gtk_notepad_cut(void)
{
	gtk_text_buffer_cut_clipboard(buffer, clipboard, TRUE);
}

void gtk_notepad_copy(void)
{
	gtk_text_buffer_copy_clipboard(buffer, clipboard);
}

void gtk_notepad_paste(void)
{
	gtk_text_buffer_paste_clipboard(buffer, clipboard, NULL, TRUE);
}

void gtk_notepad_delete(void)
{
	gtk_text_buffer_delete_selection(buffer, TRUE, TRUE);
}

void gtk_notepad_select_all(void)
{
	GtkTextIter start, end;
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	gtk_text_buffer_select_range(buffer, &start, &end);
}

void gtk_text_view_toggle_wrapping(void)
{
	GtkWrapMode mode;
	if (wrapping)
		mode = GTK_WRAP_NONE;
	else
		mode = GTK_WRAP_CHAR;

	wrapping = !wrapping;

	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textarea), mode);
}

void gtk_statusbar_update_lncol(void)
{
	char *msg;
	int row, col;
	GtkTextIter iter;

	GtkStatusbar *gstatusbar = GTK_STATUSBAR(statusbar);

	gtk_statusbar_pop(gstatusbar, 0);

	gtk_text_buffer_get_iter_at_mark(buffer, &iter,
					 gtk_text_buffer_get_insert
					 (buffer));

	row = gtk_text_iter_get_line(&iter);
	col = gtk_text_iter_get_line_offset(&iter);

	msg = g_strdup_printf("Ln: %d Col: %d", row + 1, col + 1);

	gtk_statusbar_push(gstatusbar, 0, msg);

	g_free(msg);

}

/* Other functions */

/* Create and setup a menubar */
void setup_menubar(void)
{
	menubar = gtk_menu_bar_new();

	filemenu = gtk_menu_new();
	file = gtk_menu_item_new_with_mnemonic("_File");
	quit = gtk_menu_item_new_with_mnemonic("_Quit");

	editmenu = gtk_menu_new();
	edit = gtk_menu_item_new_with_mnemonic("_Edit");
	cut = gtk_menu_item_new_with_mnemonic("Cu_t");
	copy = gtk_menu_item_new_with_mnemonic("_Copy");
	paste = gtk_menu_item_new_with_mnemonic("_Paste");
	delete = gtk_menu_item_new_with_mnemonic("_Delete");
	selectall = gtk_menu_item_new_with_mnemonic("_Select All");
	wll = gtk_check_menu_item_new_with_mnemonic("_Wrap long " "lines");

	helpmenu = gtk_menu_new();
	help = gtk_menu_item_new_with_mnemonic("_Help");
	about = gtk_menu_item_new_with_mnemonic("_About");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu),
			      gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), editmenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), cut);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), copy);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), paste);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), delete);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu),
			      gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), selectall);
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu),
			      gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), wll);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), helpmenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu), about);

	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help);

	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 1);

	// Accelerators

	// File menu
	gtk_widget_add_accelerator(quit, "activate", accel, GDK_KEY_q,
				   GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	// Edit menu
	gtk_widget_add_accelerator(cut, "activate", accel, GDK_KEY_x,
				   GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(copy, "activate", accel, GDK_KEY_c,
				   GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(paste, "activate", accel, GDK_KEY_v,
				   GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(delete, "activate", accel,
				   GDK_KEY_Delete, 0, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(selectall, "activate", accel, GDK_KEY_a,
				   GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	// Signals

	// File menu
	g_signal_connect(quit, "activate",
			 (GCallback) gtk_main_quit, NULL);

	// Edit menu
	g_signal_connect(cut, "activate", (GCallback)
			 gtk_notepad_cut, NULL);
	g_signal_connect(copy, "activate", (GCallback)
			 gtk_notepad_copy, NULL);
	g_signal_connect(paste, "activate", (GCallback)
			 gtk_notepad_paste, NULL);
	g_signal_connect(delete, "activate", (GCallback)
			 gtk_notepad_delete, NULL);
	g_signal_connect(selectall, "activate", (GCallback)
			 gtk_notepad_select_all, NULL);
	g_signal_connect(wll, "activate", (GCallback)
			 gtk_text_view_toggle_wrapping, NULL);

	// Help menu
	g_signal_connect(about, "activate", (GCallback)
			 gtk_notepad_show_about_box, NULL);
}

void setup_textarea(void)
{
	scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_AUTOMATIC);

	textarea = gtk_text_view_new();
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(textarea), 5);
	gtk_text_view_set_top_margin(GTK_TEXT_VIEW(textarea), 2);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(textarea), 5);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(textarea), 2);
	gtk_text_view_set_monospace(GTK_TEXT_VIEW(textarea), TRUE);

	gtk_container_add(GTK_CONTAINER(scrolledwindow), textarea);

	gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 0);

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textarea));

	g_signal_connect(buffer, "changed",
			 (GCallback) gtk_notepad_text_changed, NULL);
	g_signal_connect(buffer, "mark_set",
			 (GCallback) gtk_statusbar_update_lncol, NULL);

	gtk_text_view_toggle_wrapping();
}

void setup_statusbar(void)
{
	statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, FALSE, 0);

	gtk_statusbar_update_lncol();
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	atom = gdk_atom_intern("CLIPBOARD", TRUE);
	clipboard = gtk_clipboard_get(atom);	// get primary clipboard

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gwindow = GTK_WINDOW(window);

	gtk_window_set_decorated(gwindow, FALSE);

	GtkCssProvider *provider;

	provider = gtk_css_provider_new();

	gtk_css_provider_load_from_data(provider,
									"#window {"
										"background-color: #33aaaa;"
										"border-radius: 3px;"
									"}menubar {"
										"background-color: #33aaaa;"
										"color: #aa1199;"
									"}statusbar {"
										"background-color: #ee7afa;"
										"color: #ffffff;"
										"font-size: small;"
										"margin: -6px -10px -6px -10px;"
									"}", -1, NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
												GTK_STYLE_PROVIDER(provider),
												GTK_STYLE_PROVIDER_PRIORITY_FALLBACK);

	gtk_window_set_title(gwindow, title);
	gtk_window_set_default_size(gwindow, 640, 480);
	gtk_window_set_position(gwindow, GTK_WIN_POS_CENTER);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_valign(vbox, GTK_ALIGN_CENTER);
	gtk_widget_set_halign(vbox, GTK_ALIGN_CENTER);
	gtk_widget_set_size_request(vbox, 640, 480);
  	gtk_widget_set_name(GTK_WIDGET(vbox), "window");
  	gtk_container_add(GTK_CONTAINER(window), vbox);

	accel = gtk_accel_group_new();
	gtk_window_add_accel_group(gwindow, accel);

	setup_menubar();
	setup_textarea();
	setup_statusbar();

	g_signal_connect(window, "destroy", (GCallback) gtk_main_quit, NULL);
	gtk_widget_show_all(window);
	gtk_main();

	return EXIT_SUCCESS;
}
