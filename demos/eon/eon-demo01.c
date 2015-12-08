/* To compile:
 * gcc -Wall eon-demo01.c -o eon-demo01 `pkg-config --cflags --libs eon egueb-dom ecore efl-egueb`
 */
#include "Eon.h"
#include "Efl_Egueb.h"
#include "Egueb_Smil.h"
#include "Egueb_Svg.h"
#include "Ecore.h"

#define STRINGIFY(src) #src

static char *_xml = 
#include "eon-demo01.eon"
;

static void _on_close(Egueb_Dom_Event *ev, void *data)
{
	ecore_main_loop_quit();
}

#if 0
/*----------------------------------------------------------------------------*
 *                              Entry interface                              *
 *----------------------------------------------------------------------------*/
static int _eon_theme_mars_entry_version_get(void)
{
	return EON_THEME_ELEMENT_ENTRY_VERSION;
}

static void _eon_theme_mars_entry_dtor(void *data)
{
	Eon_Theme_Mars_Entry *thiz = data;

	/* attributes */
	egueb_dom_node_unref(thiz->cursor_visible);
	egueb_dom_node_unref(thiz->border_color);
	/* private */
	enesim_renderer_unref(thiz->cursor);
	enesim_renderer_unref(thiz->underline);
	enesim_renderer_unref(thiz->entry);
	enesim_renderer_unref(thiz->entry_clip);
	enesim_renderer_unref(thiz->blur);
	enesim_renderer_unref(thiz->proxy);
	free(thiz);
}

static const char * _eon_theme_mars_entry_tag_name_get(void)
{
	return "entry";
}

static Eina_Bool _eon_theme_mars_entry_process(void *data)
{
	Eon_Theme_Mars_Entry *thiz;
	Enesim_Renderer *text;
	Eina_Rectangle geom;
	Eina_Bool enabled;
	Enesim_Argb argb;
	Enesim_Color color;
	Enesim_Color border_color;
	int cursor_visible;
	int cursor_index;
	int cursor_start;

	thiz = data;
	/* get the final attributes */
	egueb_dom_attr_final_get(thiz->border_color, &argb);
	border_color = enesim_color_argb_from(argb);
	egueb_dom_attr_final_get(thiz->cursor_visible, &cursor_visible);
	/* get the inherited members */
	enabled = eon_theme_widget_enabled_get(thiz->n);
	eon_theme_renderable_geometry_get(thiz->n, &geom);
	/* set the rectangle area */
	enesim_renderer_rectangle_position_set(thiz->entry_clip, geom.x, geom.y);
	enesim_renderer_rectangle_size_set(thiz->entry_clip, geom.w, geom.h);
	text = eon_theme_element_entry_text_renderer_get(thiz->n);
	enesim_renderer_shape_fill_renderer_set(thiz->entry_clip,
			enesim_renderer_ref(text));
	enesim_renderer_origin_set(text, geom.x + EON_THEME_MARS_MARGIN,
			geom.y + EON_THEME_MARS_MARGIN);

	/* set the underline */
	enesim_renderer_shape_stroke_color_set(thiz->underline, border_color);
	enesim_renderer_line_coords_set(thiz->underline, geom.x,
			geom.y + geom.h, geom.x + geom.w, geom.y + geom.h);

	/* set the cursor */
	/* TODO no need to calculate this every time, move this calc to the base
	 * theme class */
	cursor_index = eon_theme_element_entry_cursor_get(thiz->n);
	cursor_start = geom.x + EON_THEME_MARS_MARGIN;
	if (!enesim_renderer_text_span_glyph_index_at(text, cursor_index, &cursor_start, NULL))
	{
		enesim_renderer_text_span_glyph_index_at(text, cursor_index - 1, NULL, &cursor_start);
	}
	enesim_renderer_unref(text);
	enesim_renderer_shape_stroke_color_set(thiz->cursor, border_color);
	enesim_renderer_line_coords_set(thiz->cursor, cursor_start, geom.y, cursor_start, geom.y + geom.h);
	enesim_renderer_visibility_set(thiz->cursor, cursor_visible);
	
	/* apply the blur value */
	if (!enabled)
	{
		enesim_renderer_blur_source_renderer_set(thiz->blur,
				enesim_renderer_ref(thiz->entry));
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->blur));
	}
	else
	{
		enesim_renderer_blur_source_renderer_set(thiz->blur, NULL);
		enesim_renderer_proxy_proxied_set(thiz->proxy,
				enesim_renderer_ref(thiz->entry));
	}
	enesim_renderer_origin_set(thiz->blur, geom.x, geom.y);

	return EINA_TRUE;
}

static Enesim_Renderer * _eon_theme_mars_entry_renderer_get(void *data)
{
	Eon_Theme_Mars_Entry *thiz = data;
	return enesim_renderer_ref(thiz->proxy);
}

static void _eon_theme_mars_entry_padding_get(void *data,
		Eon_Box *padding)
{
	padding->left = EON_THEME_MARS_MARGIN;
	padding->right = EON_THEME_MARS_MARGIN;
	padding->top = EON_THEME_MARS_MARGIN;
	padding->bottom = EON_THEME_MARS_MARGIN;
}

static Eon_Theme_Element_Entry_Descriptor _descriptor = {
	/* .version_get		= */ _eon_theme_mars_entry_version_get,
	/* .ctor 		= */ eon_theme_mars_entry_new,
	/* .dtor 		= */ _eon_theme_mars_entry_dtor,
	/* .tag_name_get	= */ _eon_theme_mars_entry_tag_name_get,
	/* .process 		= */ _eon_theme_mars_entry_process,
	/* .renderer_get	= */ _eon_theme_mars_entry_renderer_get,
	/* .padding_get		= */ _eon_theme_mars_entry_padding_get,
};

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Egueb_Dom_Node * eon_theme_mars_entry_new(void)
{
	Eon_Theme_Mars_Entry *thiz;
	Egueb_Dom_Node *n;
	Egueb_Dom_String *s;
	Enesim_Renderer_Compound_Layer *l;

	thiz = calloc(1, sizeof(Eon_Theme_Mars_Entry));
	thiz->entry_clip = enesim_renderer_rectangle_new();
	enesim_renderer_shape_draw_mode_set(thiz->entry_clip,
			ENESIM_RENDERER_SHAPE_DRAW_MODE_FILL);

	thiz->underline = enesim_renderer_line_new();
	enesim_renderer_shape_draw_mode_set(thiz->underline,
		ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE);
	enesim_renderer_shape_stroke_weight_set(thiz->underline, EON_THEME_MARS_BORDER);

	thiz->cursor = enesim_renderer_line_new();
	enesim_renderer_shape_draw_mode_set(thiz->cursor,
		ENESIM_RENDERER_SHAPE_DRAW_MODE_STROKE);
	enesim_renderer_shape_stroke_weight_set(thiz->cursor, EON_THEME_MARS_BORDER);

	/* the real entry */
	thiz->entry = enesim_renderer_compound_new();
	enesim_renderer_compound_background_enable_set(thiz->entry, EINA_TRUE);
	enesim_renderer_compound_background_color_set(thiz->entry, 0x00000000);
	
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_FILL);
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->entry_clip));
	enesim_renderer_compound_layer_add(thiz->entry, l);
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->underline));
	enesim_renderer_compound_layer_add(thiz->entry, l);
	l = enesim_renderer_compound_layer_new();
	enesim_renderer_compound_layer_rop_set(l, ENESIM_ROP_BLEND);
	enesim_renderer_compound_layer_renderer_set(l,
			enesim_renderer_ref(thiz->cursor));
	enesim_renderer_compound_layer_add(thiz->entry, l);

	/* the blur effect for disabled entrys */
	thiz->blur = enesim_renderer_blur_new();
	enesim_renderer_blur_radius_x_set(thiz->blur, 3);
	enesim_renderer_blur_radius_y_set(thiz->blur, 3);

	thiz->proxy = enesim_renderer_proxy_new();
	enesim_renderer_proxy_proxied_set(thiz->proxy, enesim_renderer_ref(thiz->entry));

	n = eon_theme_element_entry_new(&_descriptor, thiz);
	/* the attributes */
	s = egueb_dom_string_new_with_static_string("border-color");
	thiz->border_color = egueb_css_attr_color_new(s, NULL, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	s = egueb_dom_string_new_with_static_string("cursor-visible");
	thiz->cursor_visible = egueb_dom_attr_boolean_new(s, EINA_TRUE,
			EINA_TRUE, EINA_FALSE);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->border_color), NULL);
	egueb_dom_element_attribute_node_set(n, egueb_dom_node_ref(thiz->cursor_visible), NULL);
	thiz->n = n;

	return n;
}

	_namespace = eon_theme_namespace_register("http://www.eon.org/theme/mars");
	if (!_namespace)
		return EINA_FALSE;
	eon_theme_namespace_element_add(_namespace, "button",
			eon_theme_mars_button_new);
	eon_theme_namespace_element_add(_namespace, "entry",
			eon_theme_mars_entry_new);
#endif

int main(void)
{
	Egueb_Dom_Window *w;
	Egueb_Dom_Node *doc = NULL;
	Enesim_Stream *stream;

	if (!efl_egueb_init())
		return -1;

	stream = enesim_stream_buffer_new(_xml, strlen(_xml), NULL);
	egueb_dom_parser_parse(stream, &doc);
	/* create our doc with the xml egueb-demo01.eon */
	w = efl_egueb_window_auto_new(doc, 0, 0, -1, -1);
	egueb_dom_event_target_event_listener_add(
			EGUEB_DOM_EVENT_TARGET_CAST(w),
			EGUEB_DOM_EVENT_WINDOW_CLOSE,
			_on_close, EINA_FALSE, NULL);

	/* begin */
	ecore_main_loop_begin();

	egueb_dom_window_unref(w);
	efl_egueb_shutdown();
	return 0;
}
