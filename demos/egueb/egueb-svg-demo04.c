/* To compile:
 * gcc -Wall egueb-svg-demo04.c -o egueb-svg-demo04 `pkg-config --cflags --libs egueb-smil egueb-svg egueb-dom ecore efl-egueb`
 * Idea taken from http://codepen.io/osublake/pen/gpGEye/
 */
#include "Efl_Egueb.h"
#include "Egueb_Smil.h"
#include "Egueb_Svg.h"
#include "Ecore.h"

static Egueb_Dom_Node *stems;
static Egueb_Dom_Node *leaves;

int leafCount = 30;
int plants = 10;
int centerX = 275;
int offsetX = 175;


typedef struct _Point
{
	double x;
	double y;
} Point;

#if 0
class EventManager {

  constructor() {      
    this.eventLookup = {};
  }

  off(event, callback) {
    var listeners = this.eventLookup[event];                
    if (event === "*") this.eventLookup = {}; 
    else if(!callback) this.eventLookup[event] = []; 
    else _.remove(listeners, { callback });
  }

  on(event, callback, scope) {
    var listeners = this.eventLookup[event];
    if (!listeners) this.eventLookup[event] = listeners = [];  
    listeners.push({ callback, scope });      
    return () => _.remove(listeners, { callback });
  }

  once(event, callback, scope) {      
    var on = (...data) => {
      this.off(event, on);
      callback.apply(scope, data);
    };
    return this.on(event, on);
  }

  fire(event, ...data) {
    var listeners = this.eventLookup[event];
    if (!listeners) return;
    listeners.forEach(list => {
      try {
        return list.callback.apply(list.scope, data);
      } catch(e) {
        return _.isError(e) ? e : new Error(e);
      }
    });      
  }
}

var events = new EventManager();

var ns = "http://www.w3.org/2000/svg";

var stems  = $("#stems");
var leaves = $("#leaves");
var svg    = $("svg");

function generate() {
  
  leaves.empty();
  stems.empty();
      
  _.times(plants, createPlant);
    
  stems.children().each(function() {
    
    var tween = TweenMax.to(this, _.random(2, 4), {
      drawSVG: true,    
      delay: _.random(2),
      onStart: () => TweenLite.set(this, { opacity: 1 }),
      onUpdate: () => events.fire(this.id, tween.progress())      
    });
  });  
}

#endif

static int _random2(int low, int high)
{
	return rand() % (high - low + 1) + low;
}

static int _random1(int high)
{
	return rand() % high;
}

static Egueb_Dom_Node * _create_path(Egueb_Dom_Node *parent)
{
	Egueb_Dom_Node *path;
	path = egueb_svg_element_path_new();
	egueb_dom_node_child_append(parent, egueb_dom_node_ref(path), NULL);
	return path;
}

static void _create_leaf(Point *p, Point *scale, int height, Egueb_Dom_Node *grow)
{
	Egueb_Dom_Node *leaf = _create_path(leaves);
	Egueb_Svg_Paint paint;
	Enesim_Matrix m1, m2;

	/* set the d attribute */
	{
		Egueb_Dom_String *val;
		Egueb_Dom_String *attr;

		val = egueb_dom_string_new_with_static_string("M0,0 Q5,-5 10,0 5,5 0,0z");
		attr = egueb_dom_string_new_with_static_string("d");
		egueb_dom_element_attribute_set(leaf, attr, val, NULL);
		egueb_dom_string_unref(attr);
		egueb_dom_string_unref(val);
	}
	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	egueb_svg_color_components_from(&paint.color, 0, _random2(110, 160), 0x00);
	egueb_svg_element_fill_set(leaf, &paint);

	enesim_matrix_identity(&m2);

	enesim_matrix_translate(&m1, p->x, p->y);
	enesim_matrix_compose(&m2, &m1, &m2);

	enesim_matrix_scale(&m1, scale->x, scale->y);
	enesim_matrix_compose(&m2, &m1, &m2);

	enesim_matrix_rotate(&m1, _random1(3.14) - 3.14);
	enesim_matrix_compose(&m2, &m1, &m2);

	egueb_svg_renderable_transform_set(leaf, &m2);

	egueb_dom_node_unref(leaf);
} 

static Eina_List * _create_points(void)
{
	Eina_List *ret;
	Point *p, *prev;;
	int x = _random2(centerX - offsetX, centerX + offsetX);
	int y = 400;
	int dy = 5;
	double offset = 0.007;
	int count = _random2(leafCount, 65);
	int i;

	p = prev = malloc(sizeof(Point));
	p->x = x;
	p->y = y;
	ret = eina_list_append(NULL, p);

	for (i = 1; i <= count; i++)
	{
		p = malloc(sizeof(Point));
		p->x = prev->x + i * offset * (_random1(21) - 10);
		p->y = 395 - dy * i;

		ret = eina_list_append(ret, p);
		prev = p;
	}
	return ret;
}

static void _create_plant(void)
{
	Point *p;
	Egueb_Dom_Node *stem = _create_path(stems);
	Eina_List *points = _create_points();
	Eina_List *l;
	int length = eina_list_count(points);
	int height = ((Point *)eina_list_nth(points, length - 1))->y;
	int i;
	char *d_str;

	d_str = strdup("M");
	EINA_LIST_FOREACH(points, l, p)
	{
		char *p_str;
		asprintf(&p_str, "%s %f,%f", d_str, p->x, p->y);
		free(d_str);
		d_str = p_str;
	}

	/* set the d attribute */
	{
		Egueb_Dom_String *val;
		Egueb_Dom_String *attr;

		val = egueb_dom_string_new_with_string(d_str);
		attr = egueb_dom_string_new_with_static_string("d");
		egueb_dom_element_attribute_set(stem, attr, val, NULL);
		egueb_dom_string_unref(attr);
		egueb_dom_string_unref(val);
	}

	for (i = 0; i < leafCount; i++)
	{
		p = eina_list_nth(points, length - 1 - i);
		Point scale = { 1 + (0.1 * i), 1 + (0.05 * i) };
		_create_leaf(p, &scale, height, stem);
	}
	egueb_dom_node_unref(stem);
}

static void _on_close(Egueb_Dom_Event *ev, void *data)
{
	ecore_main_loop_quit();
}

int main(void)
{
	Egueb_Dom_Window *w;
	Egueb_Dom_Node *doc;
	Egueb_Dom_Node *svg;
	Egueb_Dom_Node *rect;
	Egueb_Dom_Node *g;
	int i;

	Egueb_Svg_Length width, height;
	Egueb_Svg_Paint paint;

	if (!efl_egueb_init())
		return -1;

	doc = egueb_svg_document_new();
	svg = egueb_svg_element_svg_new();
	/* create our SVG */
	// TODO <svg viewBox="0 0 550 400" preserveAspectRatio="xMidYMax meet">
	egueb_svg_length_set(&width, 100, EGUEB_SVG_LENGTH_UNIT_PERCENT);
	egueb_svg_length_set(&height, 100, EGUEB_SVG_LENGTH_UNIT_PERCENT);
	egueb_svg_element_svg_width_set_simple(svg, &width);
	egueb_svg_element_svg_height_set_simple(svg, &height);
	/* add our rectangle */
	rect = egueb_svg_element_rect_new();
	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	egueb_svg_color_components_from(&paint.color, 0xff, 0xff, 0xff);
	egueb_svg_element_fill_set(rect, &paint);
	egueb_svg_element_rect_width_set_simple(rect, &width);
	egueb_svg_element_rect_height_set_simple(rect, &height);
	/* append it as our own topmost element */
	egueb_dom_node_child_append(svg, rect, NULL);
	/* add the group of stems */
	g = stems = egueb_svg_element_g_new();
	paint.type = EGUEB_SVG_PAINT_TYPE_NONE;
	egueb_svg_element_fill_set(g, &paint);
	paint.type = EGUEB_SVG_PAINT_TYPE_COLOR;
	egueb_svg_color_components_from(&paint.color, 0x00, 0xff, 0x00);
	egueb_svg_element_stroke_set(g, &paint);
	egueb_svg_element_id_set(g, egueb_dom_string_new_with_static_string("stems"));
	egueb_dom_node_child_append(svg, g, NULL);
	/* add the group of leaves */ 
	g = leaves = egueb_svg_element_g_new();
	egueb_svg_element_id_set(g, egueb_dom_string_new_with_static_string("leaves"));
	egueb_dom_node_child_append(svg, g, NULL);

	egueb_dom_node_child_append(doc, svg, NULL);
	/* create a window of size 960x500 */
	w = efl_egueb_window_auto_new(doc, 0, 0, 550, 400);
	egueb_dom_event_target_event_listener_add(
			EGUEB_DOM_EVENT_TARGET_CAST(w),
			EGUEB_DOM_EVENT_WINDOW_CLOSE,
			_on_close, EINA_FALSE, NULL);

	for (i = 0; i < plants; i++)
	{
		_create_plant();
	}
	/* begin */
	ecore_main_loop_begin();

	egueb_dom_window_unref(w);
	efl_egueb_shutdown();
	return 0;
}
