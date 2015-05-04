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
	Egueb_Svg_Paint paint;

	if (!efl_egueb_init())
		return -1;

	doc = egueb_svg_document_new();
	svg = egueb_svg_element_svg_new();
	/* create our SVG */
	egueb_svg_length_set(&width, 100, EGUEB_SVG_LENGTH_UNIT_PERCENT);
	egueb_svg_length_set(&height, 100, EGUEB_SVG_LENGTH_UNIT_PERCENT);
	/* create the background pattern */
	patt = egueb_svg_element_pattern_new();
	egueb_svg_element_id_set(patt, egueb_dom_string_new_with_static_string("patt"));
	path = egueb_svg_element_path_new();
	egueb_dom_node_child_append(svg, patt, NULL);
	
	/* create the background */
	rect = egueb_svg_element_rect_new();
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
