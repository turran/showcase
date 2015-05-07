/* To compile:
 * gcc -Wall egueb-svg-demo02.c -o egueb-svg-demo02 `pkg-config --cflags --libs egueb-smil egueb-svg egueb-dom ecore efl-egueb`
 * Idea taken from http://bl.ocks.org/mbostock/5249328
 */

#include "Efl_Egueb.h"
#include "Egueb_Smil.h"
#include "Egueb_Svg.h"
#include "Ecore.h"

static Egueb_Dom_Node * _create_in_anim(void)
{
	Egueb_Dom_Node *anim;
	Egueb_Smil_Duration dur;
	Egueb_Dom_String *value;

	anim = egueb_smil_animate_new();
	/* default clock of 0.5s */
	dur.type = EGUEB_SMIL_DURATION_TYPE_CLOCK;
	dur.data.clock = EGUEB_SMIL_CLOCK_SECONDS * 0.5;
	value = egueb_dom_string_new_with_static_string("opacity");
	egueb_smil_animation_attribute_name_set(anim, value);
	value = egueb_dom_string_new_with_static_string("0");
	egueb_smil_animate_base_from_set(anim, value); 
	value = egueb_dom_string_new_with_static_string("1");
	egueb_smil_animate_base_to_set(anim, value); 
	egueb_smil_animation_dur_set(anim, &dur);

	return anim;
}

static Egueb_Dom_Node * _create_out_anim(void)
{
	Egueb_Dom_Node *anim;
	Egueb_Smil_Duration dur;
	Egueb_Dom_String *value;

	anim = egueb_smil_animate_new();
	/* default clock of 0.5s */
	dur.type = EGUEB_SMIL_DURATION_TYPE_CLOCK;
	dur.data.clock = EGUEB_SMIL_CLOCK_SECONDS * 0.5;
	value = egueb_dom_string_new_with_static_string("opacity");
	egueb_smil_animation_attribute_name_set(anim, value);
	value = egueb_dom_string_new_with_static_string("1");
	egueb_smil_animate_base_from_set(anim, value); 
	value = egueb_dom_string_new_with_static_string("0");
	egueb_smil_animate_base_to_set(anim, value); 
	egueb_smil_animation_dur_set(anim, &dur);

	return anim;
}

static Eina_Bool _hex_is_selected(Egueb_Dom_Node *hex)
{
	Egueb_Svg_Paint_Animated apaint;
	/* if the hex is red, set it back to pink and add again the animations */
	egueb_svg_element_fill_get(hex, &apaint);
	if (apaint.base.color.r == 0xff && apaint.base.color.g == 0xc0 &&
			apaint.base.color.b == 0xcb)
		return EINA_FALSE;
	else
		return EINA_TRUE;
}

static void _on_hex_mouse_click(Egueb_Dom_Event *ev, void *data)
{
	Egueb_Dom_Node *hex;
	Egueb_Dom_Node *anim;
	Egueb_Svg_Paint paint;

	hex = egueb_dom_event_target_get(ev);
	if (!_hex_is_selected(hex))
	{
		Egueb_Dom_Node *attr;

		paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
		egueb_svg_color_components_from(&paint.color, 0xff, 0x00, 0x00);
		egueb_svg_element_fill_set(hex, &paint);
		/* remove the in anim */
		anim = egueb_dom_node_child_first_get(hex);
		egueb_dom_node_child_remove(hex, anim, NULL);
		/* set the max opacity in case the anim has not finished */
		/* given that setting the opacity like this modifies the BASE
		 * attribute, we need to set the ANIMATED value */
		attr = egueb_dom_element_attribute_node_get(hex, EGUEB_SVG_NAME_OPACITY);
		egueb_dom_attr_unset(attr, EGUEB_DOM_ATTR_TYPE_ANIMATED);
		egueb_dom_node_unref(attr);
		egueb_svg_element_opacity_set(hex, 1);
	}
	else
	{
		paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
		egueb_svg_color_components_from(&paint.color, 0xff, 0x00, 0x00);
		egueb_svg_element_fill_set(hex, &paint);
		anim = _create_in_anim();
		egueb_dom_node_child_append(hex, anim, NULL);
	}
	egueb_dom_node_unref(hex);
}

static void _on_hex_mouse_in(Egueb_Dom_Event *ev, void *data)
{
	Egueb_Dom_Node *hex;

	/* add the in color animation */
	hex = egueb_dom_event_target_get(ev);
	if (!_hex_is_selected(hex))
	{
		Egueb_Dom_String *id;
		Egueb_Dom_Node *anim;

		id = egueb_svg_element_id_get(hex);
		//printf("mouse in %s\n", egueb_dom_string_string_get(id));
		egueb_dom_string_unref(id);
		anim = _create_in_anim();
		egueb_dom_node_child_append(hex, anim, NULL);
	}
	egueb_dom_node_unref(hex);
}

static void _on_hex_mouse_out(Egueb_Dom_Event *ev, void *data)
{
	Egueb_Dom_Node *hex;

	hex = egueb_dom_event_target_get(ev);
	if (!_hex_is_selected(hex))
	{
		Egueb_Dom_Node *anim;
		Egueb_Dom_String *id;

		/* stop the current animation */
		anim = egueb_dom_node_child_first_get(hex);
		egueb_smil_animation_element_end(anim);
		egueb_dom_node_child_remove(hex, anim, NULL);
		//egueb_dom_node_unref(anim);
		/* add the out color animation */
		id = egueb_svg_element_id_get(hex);
		//printf("mouse out %s\n", egueb_dom_string_string_get(id));
		egueb_dom_string_unref(id);
		anim = _create_out_anim();
		egueb_dom_node_child_append(hex, anim, NULL);
	}
	egueb_dom_node_unref(hex);
}

static Egueb_Dom_Node * _create_hexagon(void)
{
	Egueb_Dom_Node *hex;
	Egueb_Svg_Length sw;
	Egueb_Svg_Paint paint;

	hex = egueb_svg_element_path_new();
	{
		Egueb_Dom_String *val;
		Egueb_Dom_String *attr;

		val = egueb_dom_string_new_with_static_string("M14 33L0 25L0 8L14 0L28 8L28 25L14 33");
		attr = egueb_dom_string_new_with_static_string("d");
		egueb_dom_element_attribute_set(hex, attr, val, NULL);
	}
	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	egueb_svg_color_components_from(&paint.color, 0xff, 0xc0, 0xcb);
	egueb_svg_element_fill_set(hex, &paint);
	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	egueb_svg_color_components_from(&paint.color, 0xcc, 0xcc, 0xcc);
	egueb_svg_element_stroke_set(hex, &paint);
	egueb_svg_element_opacity_set(hex, 0);
	egueb_svg_length_set(&sw, 1, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_element_stroke_width_set(hex, &sw);
	egueb_dom_node_event_listener_add(hex, EGUEB_DOM_EVENT_MOUSE_OVER,
			_on_hex_mouse_in, EINA_TRUE, NULL);
	egueb_dom_node_event_listener_add(hex, EGUEB_DOM_EVENT_MOUSE_OUT,
			_on_hex_mouse_out, EINA_TRUE, NULL);
	egueb_dom_node_event_listener_add(hex, EGUEB_DOM_EVENT_MOUSE_CLICK,
			_on_hex_mouse_click, EINA_TRUE, NULL);
	return hex;
}

static void _on_rect_mouse_move(Egueb_Dom_Event *ev, void *data)
{
	Egueb_Dom_Node *hex;
	Egueb_Dom_Node *rect;
	Egueb_Dom_Node *parent;
	Enesim_Matrix m;
	int mx, my;
	int hx, hy;

	/* calculate the hexagon below the cursor */
	mx = egueb_dom_event_mouse_client_x_get(ev);
	my = egueb_dom_event_mouse_client_y_get(ev);
	/* the hex is of size 28x33 */
	hx = mx / 28;
	hy = my / 25;
	hx = hx * 28;
	hy = hy * 25;
	if (hy % 2)
	{
		if (mx % 28 > 14)
			hx += 14;
		else
			hx -= 14;
	}
	/* create a new path */
	hex = _create_hexagon();
	{
		Egueb_Dom_String *id;
		char *ids;
		asprintf(&ids, "hex-%dx%d", hx, hy);
		id = egueb_dom_string_new_with_string(ids);
		egueb_svg_element_id_set(hex, id);
	}
	enesim_matrix_translate(&m, hx, hy);
	egueb_svg_renderable_transform_set(hex, &m);
	/* add the hex to the svg */
	rect = egueb_dom_event_target_get(ev);
	parent = egueb_dom_node_parent_get(rect);
	egueb_dom_node_child_append(parent, hex, NULL);
	egueb_dom_node_unref(parent);
	egueb_dom_node_unref(rect);	
}

int main(void)
{
	Efl_Egueb_Window *w;
	Egueb_Dom_Node *doc;
	Egueb_Dom_Node *svg;
	Egueb_Dom_Node *rect;
	Egueb_Dom_Node *patt;
	Egueb_Dom_Node *path;
	Egueb_Svg_Length sw;
	Egueb_Svg_Length width, height;
	Egueb_Svg_Paint paint;

	if (!efl_egueb_init())
		return -1;

	doc = egueb_svg_document_new();
	svg = egueb_svg_element_svg_new();
	/* create our SVG */
	egueb_svg_length_set(&width, 100, EGUEB_SVG_LENGTH_UNIT_PERCENT);
	egueb_svg_length_set(&height, 100, EGUEB_SVG_LENGTH_UNIT_PERCENT);
	egueb_svg_element_svg_width_set_simple(svg, &width);
	egueb_svg_element_svg_height_set_simple(svg, &height);
	/* create the background pattern */
	patt = egueb_svg_element_pattern_new();
	egueb_svg_element_id_set(patt, egueb_dom_string_new_with_static_string("pattern0"));
	egueb_svg_length_set(&width, 28, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_length_set(&height, 50, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_element_pattern_width_set(patt, &width);
	egueb_svg_element_pattern_height_set(patt, &height);
	egueb_svg_element_pattern_pattern_units_set(patt, EGUEB_SVG_REFERENCEABLE_UNITS_USER_SPACE_ON_USE);
	/* create the path pattern */
	path = egueb_svg_element_path_new();
	{
		Egueb_Dom_String *val;
		Egueb_Dom_String *attr;

		val = egueb_dom_string_new_with_static_string("M14 33L0 25L0 8L14 0L28 8L28 25L14 33L14 50");
		attr = egueb_dom_string_new_with_static_string("d");
		egueb_dom_element_attribute_set(path, attr, val, NULL);
	}
	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	egueb_svg_color_components_from(&paint.color, 0xff, 0xff, 0xff);
	egueb_svg_element_fill_set(path, &paint);
	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	egueb_svg_color_components_from(&paint.color, 0xcc, 0xcc, 0xcc);
	egueb_svg_element_stroke_set(path, &paint);
	egueb_svg_length_set(&sw, 1, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_element_stroke_width_set(path, &sw);
	egueb_dom_node_child_append(patt, path, NULL);
	egueb_dom_node_child_append(svg, patt, NULL);
	/* create the background */
	rect = egueb_svg_element_rect_new();
	paint.type = EGUEB_SVG_PAINT_TYPE_SERVER;
	paint.uri = egueb_dom_string_new_with_static_string("url(#pattern0)");
	egueb_svg_element_fill_set(rect, &paint);
	egueb_svg_length_set(&width, 100, EGUEB_SVG_LENGTH_UNIT_PERCENT);
	egueb_svg_length_set(&height, 100, EGUEB_SVG_LENGTH_UNIT_PERCENT);
	egueb_svg_element_rect_width_set_simple(rect, &width);
	egueb_svg_element_rect_height_set_simple(rect, &height);
	/* register the mousemove event */
	egueb_dom_node_event_listener_add(rect, EGUEB_DOM_EVENT_MOUSE_MOVE,
			_on_rect_mouse_move, EINA_FALSE, NULL);
	egueb_dom_node_child_append(svg, rect, NULL);
	/* append it as our own topmost element */	
	egueb_dom_node_child_append(doc, svg, NULL);
	/* create a window of size 960x500 */
	w = efl_egueb_window_auto_new(doc, 0, 0, 960, 500);
	ecore_main_loop_begin();

	efl_egueb_window_free(w);
	efl_egueb_shutdown();
	return 0;

}
