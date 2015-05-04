/* To compile:
 * gcc -Wall egueb-svg-demo02.c -o egueb-svg-demo02 `pkg-config --cflags --libs egueb-smil egueb-svg egueb-dom ecore efl-egueb`
 * Idea taken from http://bl.ocks.org/mbostock/5249328
 */

#include "Efl_Egueb.h"
#include "Egueb_Smil.h"
#include "Egueb_Svg.h"
#include "Ecore.h"

int main(void)
{
	Efl_Egueb_Window *w;
	Egueb_Dom_Node *doc;
	Egueb_Dom_Node *svg;
	Egueb_Dom_Node *rect;
	Egueb_Dom_Node *patt;
	Egueb_Dom_Node *path;
	Egueb_Svg_Length width, height;
	Egueb_Svg_Length sw;
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
	paint.type = EGUEB_SVG_PAINT_TYPE_NONE;
	egueb_svg_element_fill_set(path, &paint);
	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	egueb_svg_color_components_from(&paint.color, 0, 0, 0);
	egueb_svg_element_stroke_set(path, &paint);
	egueb_svg_length_set(&sw, 0.5, EGUEB_SVG_LENGTH_UNIT_PX);
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
