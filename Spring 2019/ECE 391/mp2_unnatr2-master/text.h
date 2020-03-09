/*
 * tab:4
 *
 * text.h - font data and text to mode X conversion utility header file
 *
 * "Copyright (c) 2004-2009 by Steven S. Lumetta."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:        Steve Lumetta
 * Version:       2
 * Creation Date: Thu Sep  9 22:08:16 2004
 * Filename:      text.h
 * History:
 *    SL    1    Thu Sep  9 22:08:16 2004
 *        First written.
 *    SL    2    Sat Sep 12 13:40:11 2009
 *        Integrated original release back into main code base.
 */

#ifndef TEXT_H
#define TEXT_H

/* The default VGA text mode font is 8x16 pixels. */
#define FONT_WIDTH 8
#define FONT_HEIGHT 16

/* Width and height of Text Block */
#define X_NUMBER 12
#define Y_NUMBER 8

/* Status bar global definitions */
#define STATUS_BAR_X_DIM 320                                  // Width of screen
#define STATUS_BAR_Y_DIM (FONT_HEIGHT + 2)                    // 16 + 2 = 18
#define STATUS_BAR_SIZE (STATUS_BAR_X_DIM * STATUS_BAR_Y_DIM) // Number of pixels the status bar occupies (320 * 18) = 5760
#define STATUS_BAR_ADDR_SIZE (STATUS_BAR_SIZE / 4)            // Number of addresses the status bar occupies (320 * 18) / 4 = 1440

/* Global buffer to save pixel values for status bar*/
unsigned char buffer[STATUS_BAR_SIZE];

/* Standard VGA text font. */
extern unsigned char font_data[256][16];

/* Routine to convert a string into graphic data that can be used to display on the screen */
void text_to_graphic(const char *str, int level);

/* Routine to convert a string into graphic data that can be used to display on the screen */
void text_to_graphic_fruit(char *str, unsigned char *buf);

#endif /* TEXT_H */
