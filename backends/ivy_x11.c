#include "ivy.h"

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct {
	Display *dsp;
	Window wnd;
	GC gc;
	XImage *img;
	Atom wnd_close_atom;
} window_native_t;

 int _get_mapped_mods(uint32_t state);
 int _get_mapped_key(int key);

void* wnd_create_native(window_context_t *wnd, u32_t width, u32_t height, const char *title)
{
	window_native_t *native = IVY_MALLOC(sizeof(window_native_t));
	if (!native) {
		return NULL;
		WARN("IVY_X11: Unable to allocate memory");
	}

	native->dsp = XOpenDisplay(NULL);
	if (!native->dsp) {
		return NULL;
		WARN("IVY_X11: Unable to open display");
	}

	int screen = DefaultScreen(native->dsp);
	Window root_wnd = RootWindow(native->dsp, screen);

	int wx = 0;
	int wy = 0;

	native->wnd = XCreateSimpleWindow(native->dsp, root_wnd, wx, wy, width, height, 0, WhitePixel(native->dsp, screen), BlackPixel(native->dsp, screen));

	if (!native->wnd) {
		return NULL;
		WARN("IVY_X11: Unable to create window");
	}

	if (!XSetStandardProperties(native->dsp, native->wnd, title, "X11 Window", None, NULL, 0, NULL)) {
		return NULL;
		WARN("IVY_X11: Unable to set window properties");
	}

	native->gc = XCreateGC(native->dsp, native->wnd, 0, 0);
	if (!native->gc) {
		return NULL;
		WARN("IVY_X11: Unable to Create Graphics Context");
	}

	long event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask;
	XSelectInput(native->dsp, native->wnd, event_mask);
	XMapRaised(native->dsp, native->wnd);

	XSync(native->dsp, native->wnd);

	Visual *visual = DefaultVisual(native->dsp, 0);
	native->img = XCreateImage(native->dsp, visual, 24, ZPixmap, 0, (char *)wnd->pixels.buffer, width, height, 32, 0);

	native->wnd_close_atom = XInternAtom(native->dsp, "WM_DELETE_WINDOW", 0);
	return native;			
}

int wnd_update_native(window_context_t *wnd)
{
	window_native_t* native = wnd->native;
	pixel_array_t* pixels = &wnd->pixels;
	XPutImage(native->dsp, native->wnd, native->gc, native->img, 0, 0, 0, 0, pixels->width, pixels->height);
	XFlush(native->dsp);
	XEvent ev;
	while (XPending(native->dsp)) {
		XNextEvent(native->dsp, &ev);
		switch (ev.type) {
		case KeyPress: {
			// On key press
			{
				wnd->mods = _get_mapped_mods(ev.xkey.state);
				int key = XkbKeycodeToKeysym(native->dsp, ev.xkey.keycode, 0, 0);
				key = _get_mapped_key(key);
				wnd->keys[key] = 1;
				wnd->on_key_press(wnd, key, wnd->mods);
			}
			// on Text input
			{
				char buf[64];
				u64_t sym;
				int len_read = XLookupString(&ev.xkey, buf, 64 - 1, &sym, NULL);
				buf[len_read] = '\0';
				INFO("%d, [%s], [%zu]", len_read, buf, sizeof(sym));
				wnd->on_text_input(wnd, buf, len_read);
			}
		} break;
		case KeyRelease: {
			wnd->mods = _get_mapped_mods(ev.xkey.state);
			int key = XkbKeycodeToKeysym(native->dsp, ev.xkey.keycode, 0, 0);
			key = _get_mapped_key(key);
			wnd->keys[key] = 0;
			wnd->on_key_release(wnd, key, wnd->mods);
		} break;
		case ButtonPress: {
			wnd->mods = _get_mapped_mods(ev.xbutton.state);
			int key = _get_mapped_key(ev.xbutton.button);
			wnd->keys[key] = 1;
			wnd->on_key_press(wnd, key, wnd->mods);
		} break;
		case ButtonRelease: {
			wnd->mods = _get_mapped_mods(ev.xbutton.state);
			int key = _get_mapped_key(ev.xbutton.button);
			wnd->keys[key] = 0;
			wnd->on_key_release(wnd, key, wnd->mods);
		} break;
		case MotionNotify: {
			if (wnd->first_mouse) {
				wnd->p_mouse_x = ev.xmotion.x;
				wnd->p_mouse_y = ev.xmotion.y;
			}
			wnd->mouse_x = ev.xmotion.x;
			wnd->mouse_y = ev.xmotion.y;
			wnd->on_mouse_move(wnd, ev.xmotion.x, ev.xmotion.y, wnd->p_mouse_x, wnd->p_mouse_y);
			wnd->p_mouse_x = ev.xmotion.x;
			wnd->p_mouse_y = ev.xmotion.y;
		} break;
		case EnterNotify: {
			wnd->p_mouse_x = wnd->mouse_x = ev.xcrossing.x;
			wnd->p_mouse_y = wnd->mouse_y = ev.xcrossing.y;
			wnd->on_mouse_enter(wnd, wnd->mouse_x, wnd->mouse_y, wnd->p_mouse_x, wnd->p_mouse_y);
		} break;
		case LeaveNotify: {
			wnd->p_mouse_x = wnd->mouse_x = ev.xcrossing.x;
			wnd->p_mouse_y = wnd->mouse_y = ev.xcrossing.y;
			wnd->on_mouse_leave(wnd, wnd->mouse_x, wnd->mouse_y, wnd->p_mouse_x, wnd->p_mouse_y);
		} break;
		case ConfigureNotify: {
			if (ev.xconfigure.window == native->wnd) {
				int width = ev.xconfigure.width;
				int height = ev.xconfigure.height;
				if (width != pixels->width || height != pixels->height) {
					
					gfx_resize(pixels, width, height);
					
					// XDestroyImage also frees the pixel buffer it stores
					// Thats why we have to manually free it like so
					IVY_FREE(native->img);
					
					Visual *visual = DefaultVisual(native->dsp, 0);
					native->img = XCreateImage(native->dsp, visual, 24, ZPixmap, 0, (char *)pixels->buffer, width, height, 32, 0);
					
					wnd->on_window_resize(wnd, width, height);
				}
			}
		} break;
		case Expose: {
			// no need to do it as we are redrawing every frame
			// XClearWindow(native->dsp, native->wnd);
		} break;
		case ClientMessage: {
			if ((Atom)ev.xclient.data.l[0] == native->wnd_close_atom) {
				wnd->is_closed = 1;
				return 1;
			}
		} break;
		default:
			 break;
		} // switch(ev.type)
	}
	return wnd->is_closed;
	
}

void wnd_destroy_native(window_context_t *wnd)
{
	window_native_t* native = wnd->native;
	IVY_FREE(native->img);
	IVY_FREE(native->gc);
	XCloseDisplay(native->dsp);
}
