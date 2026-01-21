#include <menu.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <Exit.h>
#include <String.h>
Exit_class Exit {};
void exit_proc( int signal)
{
	cout << "Exit class " << endl;
	exit( signal );
}
/**************************************************
 https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/menus.html
 *************************************************/
#include <Composer/Lineedit.h>
Kbd_base Kbd {};
#include <Table.h>
int main()
{
/*	Lineedit_class Edit { "Line Edit > ", &Kbd };
	vector_str_t text = { "Line1", "Line2", "Line3", "Line4", "Line5"};
	Edit.Text( text );
	cout << show_str_items( text );
	*/
	Table_class T {};
	T.TestTable();
	return 0;
	// Initialize curses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Create the menu
    // Initialize menu items
    ITEM *items[4];
    items[0] = new_item("Option 1", "Description for Option 1");
    items[1] = new_item("Option 2", "Description for Option 2");
    items[2] = new_item("Option 3", "Description for Option 3");
    items[3] = nullptr; // Null-terminate the item list

    MENU* menu = new_menu(items);


    // Display the menu
    post_menu(menu);
    refresh();

    int ch;
    while ((ch = getch()) != 'q') { // Press 'q' to quit
        switch (ch) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case 10: // Enter key
                mvprintw(LINES - 2, 0, "You selected: %s", item_name(current_item(menu)));
                refresh();
                break;
        }
    }

    // Cleanup
    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; i < 3; ++i) {
        free_item(items[i]);
    }
    endwin();

    return 0;
}
