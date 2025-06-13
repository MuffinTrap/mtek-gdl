#pragma once

/** This header file allows to use mtek-gdl like CToy
*/

/* system */

#define CTOY_CHAR_MAX 256 // maximum characters per update
#define CTOY_PEN_DATA_MAX 256 // maximum pen tablet data per update

#ifdef __cplusplus
extern "C"
{
#endif

/* Rocket */

unsigned short add_to_rocket(const char *name);
float get_from_rocket(unsigned short id);
void set_BPM(double val);
void set_RPB(double val);

/* Time */
unsigned long ctoy_t(void); // return ctoy current tick (number of updates since ctoy started)
double ctoy_get_time(void); // return ctoy current time (seconds elapsed since ctoy started)
void ctoy_sleep(long sec, long nsec); // suspend execution for second + nanosecond intervals
int ctoy_argc(void); // return standard argc
char **ctoy_argv(void); // return standard argv

/* window managment */



int ctoy_window_width(void); // return window's width
int ctoy_window_height(void); // return windows's height
void ctoy_window_size(int width, int height); // set window's size
void ctoy_window_title(const char *title); // set window's title
void ctoy_window_fullscreen(int state); // set window's fullscreen mode

/* frame buffer */
/* Forward declare*/
struct m_image;
int ctoy_frame_buffer_width(void); // return frame buffer width (use that for glViewport)
int ctoy_frame_buffer_height(void); // return frame buffer height (use that for glViewport)
void ctoy_render_image(struct m_image *src); // render an image to the frame buffer
void ctoy_swap_buffer(struct m_image *src); // swap current buffer with optional image (use NULL for GLES swap)

/* keyboard events */
int ctoy_key_press(int key); // return 1 on key press, or return 0
int ctoy_key_release(int key); // return 1 on key release, or return 0
int ctoy_key_pressed(int key); // return 1 if key is currently pressed, or return 0
int ctoy_get_chars(unsigned int dest[CTOY_CHAR_MAX]); // return number of characters typed and get a copy

/* mouse events */
float ctoy_mouse_x(void); // return mouse x position
float ctoy_mouse_y(void); // return mouse y position
int ctoy_mouse_button_press(int button); // return 1 on mouse-button press, or return 0
int ctoy_mouse_button_release(int button); // return 1 on mouse-button release, or return 0
int ctoy_mouse_button_pressed(int button); // return 1 if mouse-button is currently pressed, or return 0

/* scroll events */
float ctoy_scroll_x(void); // return scroll x position
float ctoy_scroll_y(void); // return scroll y position

/* joysticks events */
int ctoy_joystick_present(int joy); // return 1 if joystick is present, or return 0
int ctoy_joystick_axis_count(int joy); // return joystick's number of axis
int ctoy_joystick_button_count(int joy); // return joystick's number of buttons
int ctoy_joystick_button_press(int joy, int button); // return 1 on joystick-button press, or return 0
int ctoy_joystick_button_release(int joy, int button); // return 1 on joystick-button release, or return 0
int ctoy_joystick_button_pressed(int joy, int button); // return 1 if joystick-button is currently pressed, or return 0
float ctoy_joystick_axis(int joy, int axis); // return joystick axis's value

/* pen tablet events */
//struct ctoy_pen_data {float x, y, z, pressure, pitch, yaw, roll;};
//int ctoy_get_pen_data(struct ctoy_pen_data dest[CTOY_PEN_DATA_MAX]); // return number of pen data events and get a copy

/* persistent memory */
void ctoy_register_memory(void *memory); // register global memory pointer
void *ctoy_retrieve_memory(void); // return previously registered global memory pointer


#ifdef __cplusplus
}
#endif
