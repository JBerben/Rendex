#include <keyboard.h>
#include <display.h>
#include <clock.h>
#include <stdlib.h>
#include <string.h>
#include "includes/fxlib.h"

#include "game.h"
#include "definitions.h"

#include <stdbool.h>

/**
 * DEFINES
 */
#define OPTION_CHOICE 30 + 8 * option_choice
#define MAX_RAW_FILE_NAME 9
#define MAX_FILE_NAME 13

/*
* The extern image and font custom structs tell the compiler
* that these aren't variables written into the code, but will
* be found in the binary when everything gets linked.  
* Needs to have "assets_ + type eg font_ + font name. shown at top of font.bmp"
*/

/**
 * FUNCTION PROTOTYPES
 */
// MENU
void draw_menu(void);
void menu_tooltip(int option_choice);
void menu_select(int option_choice);
// INPUT
void reset_kboard_rr(void);
// 3D
void render_cube(bool initialize);
void load_file(bool initialize);
void loading_bar(void);
void render_misc(bool initialize);
// CLOCK


/**
 * GLOBAL VARIABLES
 */
extern font_t assets_font_island;
extern image_t assets_title;

static int option_choice = 0;
static bool in_menu = true;
static bool in_applet = false;

/* Where the magic happens! */
int main(void) {
    text_configure(&assets_font_island, color_black);

    /* Start drawing title screen. */
    while (in_menu) {

        draw_menu();        

        /* Menu entry selection. */
        switch (getkey()) {
            case KEY_DOWN:
            if (option_choice < 2) option_choice++;
            break;

            case KEY_UP:
            if (option_choice) option_choice--;
            break;

            case KEY_EXE:
            in_menu = false;
            menu_select(option_choice);
            break;

            case KEY_EXIT:
            in_menu = false;
            return 0;
            break;
        }
    }

    return 0;
}

/****
 * MENU FUNCTIONS
 */
/**
 * Draws up a nice starting menu.
 */
void draw_menu(void) {
    dclear();
    
    dimage(1, 1, &assets_title);
    dtext(9, 30, "Render Cube");
    dtext(9, 38, "Load File");
    dtext(9, 46, "Render Misc");
    dline(77, 29, 77, 54, color_black);
    // x01 is the custom glyph in font for > arrow.
    dtext(1, OPTION_CHOICE, "\x01");
    /* Renders the tool-tip text depending on option hover */
    menu_tooltip(option_choice);

    dupdate();
}

/**
 * Renders the tool-tip message related to the
 * menu option currently selected.
 */
void menu_tooltip(int option_choice) {
    switch (option_choice) {
        case 0:
        dtext(84, 30, "Renders a");
        dtext(84, 36, "cube and");
        dtext(84, 42, "it's mesh.");
        break;

        case 1:
        dtext(84, 30, "Front end");
        dtext(84, 36, "of file");
        dtext(84, 42, "loading!");   
        break;

        case 2:
        dtext(84, 30, "Solely for");
        dtext(84, 36, "testing");
        dtext(84, 42, "purposes.");
        break;
    }
}

void menu_select(int option_choice) {

    if (option_choice == 0) {
        render_cube(true);
    } else if (option_choice == 1) {
        load_file(true);
        //loading_bar();
    } else if (option_choice == 2) {
        render_misc(true);
    }
}

/****
 * INPUT FUNCTIONS
 */
void reset_kboard_rr(void) {
    keyboard_setRepeatRate(625, 125);
}

/****
 * 3D FUNCTIONS
 */
void render_cube(bool initialize) {
    if (initialize) {
        initializeGame();
        initialize = false;
    }
    dclear();
    updateGame();
    dupdate();
    // After we are done.
    reset_kboard_rr();
    in_menu = true;
}

// Backend of file loading.
void load_file(bool initialize) {
    dclear();

    /**
     * File I/O variables.
     */
    char data[] = {'I', ' ', 'L', 'o', 'v', 'e', ' ', 'Y', 'o', 'u', '!', 0};
    const void* data_buffer = data;

    char read_data[12];
    void* read_ptr = read_data;

    FONTCHARACTER file_name[] = {'\\','\\','f','l','s','0','\\','T','E','X','T' ,0}; 
    int fsize = 32;
    int file_handle = 0;

    // Waits for user input.
    while (getkey_opt(getkey_none, 125) != KEY_AC_ON) {
        dupdate();
        dtext(1, 1, "AC BUTTON TO START...");
    }
    dclear();

    Bfile_DeleteFile(file_name);

    // File creation.
    file_handle = Bfile_CreateFile(file_name, fsize);
    if (file_handle >= 0) {
        file_handle = Bfile_OpenFile(file_name, _OPENMODE_READWRITE);
        Bfile_WriteFile(file_handle, data_buffer, fsize);
        if (file_handle < 0) {
            dtext(1, 10, "FILE WRITING ERROR :(");
            dupdate();
            sleep_ms(200);
            dclear();
            in_menu = true;
            return;
        }
        dclear();
    }

    loading_bar();

    /* Waiting for input to start file reading. */
    short loading_dots = 0;
    while (getkey_opt(getkey_none, 125) != KEY_EXE) {
        dclear();

        if (loading_dots == 0) {
            dtext(12, 8, "PRESS EXE TO READ");
        }
        if (loading_dots == 1) {
            dtext(12, 8, "PRESS EXE TO READ.");
        }
        if (loading_dots == 2) {
            dtext(12, 8, "PRESS EXE TO READ..");
        }
        if (loading_dots == 3) {
            dtext(12, 8, "PRESS EXE TO READ...");
        }
        loading_dots = (loading_dots + 1) % 4;

        dupdate();
    }

    // File Reading.
    dclear();
    Bfile_ReadFile(file_handle, read_ptr, 12, 0);
    
    // Display contents.
    while (getkey_opt(getkey_none, 125) != KEY_EXIT) {
        dclear();
        dtext(32, 1, "FILE CONTENTS:");
        dprint(1, 50, "HANDLE: %d", file_handle);
        dprint(1, 28, "%s", (char*)read_ptr);
        dupdate();
    }

    // Close file and return to menu.
    dclear();
    Bfile_CloseFile(file_handle);
  
    in_menu = true;
}

/**
 * Used atm to test a flashy loading bar for future use.
 */
void loading_bar() {
    const int MAX_SEGMENTS = 60;
    const int MAX_PROGRESS = 100;
    double progress = 0.0;
    int segments_drawn = 1;
    
    while (progress <= MAX_PROGRESS) {
        dclear();
        
        drect(32, 28, 96, 38, color_black);
        drect(34, 30, 94, 36, color_white);
        
        sleep_ms(25);
        
        drect(34, 30, 34 + segments_drawn, 36, color_black);
        
        progress += 1.67;
        segments_drawn ++;
        
        dupdate();
    }

    dclear();

}

/**
 * Learning how to control Gint's gray engine better...
 */
void render_misc(bool initialize) {
    
    in_menu = true;
}


/****
 * CLOCK FUNCTIONS
 */
