/* To compile:
 * gcc -Wall egueb-svg-demo01.c -o egueb-svg-demo01 `pkg-config --cflags --libs egueb-smil egueb-svg egueb-dom ecore efl-egueb`
 * Idea taken from http://bl.ocks.org/mbostock/1062544
 */
#include "Efl_Egueb.h"
#include "Egueb_Smil.h"
#include "Egueb_Svg.h"
#include "Ecore.h"

int i = 0;

/*
 * TODO: this might go into enesim someday
 */
static void _hsv_to_rgb(uint8_t h, uint8_t s, uint8_t v,
		uint8_t *r, uint8_t *g, uint8_t *b)
{
	uint8_t region, remainder, p, q, t;

	if (s == 0)
	{
		*r = v;
		*g = v;
		*b = v;
		return;
	}

	region = h / 43;
	remainder = (h - (region * 43)) * 6; 

	p = (v * (255 - s)) >> 8;
	q = (v * (255 - ((s * remainder) >> 8))) >> 8;
	t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

	switch (region)
	{
		case 0:
		*r = v; *g = t; *b = p;
		break;

		case 1:
		*r = q; *g = v; *b = p;
		break;

		case 2:
		*r = p; *g = v; *b = t;
		break;

		case 3:
		*r = p; *g = q; *b = v;
		break;

		case 4:
		*r = t; *g = p; *b = v;
		break;

		default:
		*r = v; *g = p; *b = q;
		break;
	}
}

/* once the animation stops we need to remove the element from the tree
 * and finally destroy it
 */
static void _on_anim_end(Egueb_Dom_Event *ev, void *data)
{
	Egueb_Dom_Node *anim;
	Egueb_Dom_Node *svg;
	Egueb_Dom_Node *parent;

	anim = egueb_dom_event_target_get(ev);
	parent = egueb_dom_node_parent_get(anim);
	svg = egueb_dom_event_target_current_get(ev);
	egueb_dom_node_child_remove(svg, parent, NULL);
	egueb_dom_node_unref(svg);
}

static void _on_rect_mouse_move(Egueb_Dom_Event *ev, void *data)
{
	Egueb_Dom_Node *circle;
	Egueb_Dom_Node *parent;
	Egueb_Dom_Node *anim;
	Egueb_Dom_String *value;
	Egueb_Svg_Length cx, cy, rad, sw;
	Egueb_Svg_Paint paint;
	Egueb_Smil_Duration dur;
	int mx, my;
	uint8_t r, g, b;

	mx = egueb_dom_event_mouse_client_x_get(ev);
	my = egueb_dom_event_mouse_client_y_get(ev);
	/* Create a new circle on x,y position */
	circle = egueb_svg_element_circle_new();
	egueb_svg_length_set(&cx, mx, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_length_set(&cy, my, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_length_set(&rad, 0.1, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_length_set(&sw, 2.5, EGUEB_SVG_LENGTH_UNIT_PX);
	egueb_svg_element_circle_cx_set(circle, &cx);
	egueb_svg_element_circle_cy_set(circle, &cy);
	egueb_svg_element_circle_r_set(circle, &rad);
	paint.type = EGUEB_SVG_PAINT_TYPE_NONE;
	egueb_svg_element_fill_set(circle, &paint);
	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	/* set the color based on the previous color */
	i = (i + 1) % 255;
	_hsv_to_rgb(i, 255, 127, &r, &g, &b); 
	egueb_svg_color_components_from(&paint.color, r, g, b);
	egueb_svg_element_stroke_set(circle, &paint);
	egueb_svg_element_stroke_width_set(circle, &sw);
	/* make sure to not trigger any event on the circles */
	egueb_svg_element_pointer_events_set(circle, EGUEB_SVG_POINTER_EVENTS_NONE);
	/* animations */
	/* default clock of 0.5s */
	dur.type = EGUEB_SMIL_DURATION_TYPE_CLOCK;
	dur.data.clock = EGUEB_SMIL_CLOCK_SECONDS * 0.5;
	/* add an animation for the opacity */
	anim = egueb_smil_animate_new();
	value = egueb_dom_string_new_with_static_string("opacity");
	egueb_smil_animation_attribute_name_set(anim, value);
	value = egueb_dom_string_new_with_static_string("1");
	egueb_smil_animate_base_from_set(anim, value); 
	value = egueb_dom_string_new_with_static_string("0");
	egueb_smil_animate_base_to_set(anim, value); 
	egueb_smil_animation_dur_set(anim, &dur);
	egueb_dom_node_child_append(circle, anim, NULL);
	/* add an animation for the radius */
	anim = egueb_smil_animate_new();
	value = egueb_dom_string_new_with_static_string("r");
	egueb_smil_animation_attribute_name_set(anim, value);
	value = egueb_dom_string_new_with_static_string("0.1");
	egueb_smil_animate_base_from_set(anim, value); 
	value = egueb_dom_string_new_with_static_string("100");
	egueb_smil_animate_base_to_set(anim, value); 
	egueb_smil_animation_dur_set(anim, &dur);
	egueb_dom_node_child_append(circle, anim, NULL);

	/* add the circle after the rect */
	parent = egueb_dom_event_target_current_get(ev);
	egueb_dom_node_child_append(parent, circle, NULL);
	egueb_dom_node_unref(parent);
}

int main(void)
{
	Efl_Egueb_Window *w;
	Egueb_Dom_Node *doc;
	Egueb_Dom_Node *svg;
	Egueb_Dom_Node *rect;
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
	/* add our rectangle */
	rect = egueb_svg_element_rect_new();
	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	egueb_svg_color_components_from(&paint.color, 0x22, 0x22, 0x22);
	egueb_svg_element_fill_set(rect, &paint);
	egueb_svg_element_rect_width_set_simple(rect, &width);
	egueb_svg_element_rect_height_set_simple(rect, &height);
	/* register the mousemove event */
	egueb_dom_node_event_listener_add(svg, EGUEB_DOM_EVENT_MOUSE_MOVE,
			_on_rect_mouse_move, EINA_FALSE, NULL);
	/* register the anim end event */
	egueb_dom_node_event_listener_add(svg, EGUEB_SMIL_EVENT_END,
			_on_anim_end, EINA_TRUE, NULL);
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
