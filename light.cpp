#include "ray.h"

// for the generalized parsher
static pparm_t lgt_parse[] =
{
   {"emissivity",      3, 8, "%lf", 0},
   {"location",      3, 8, "%lf", 0},
};

#define NUM_ATTRS (sizeof(lgt_parse) / sizeof(pparm_t))

// Create a new light description
light_t::light_t(FILE *in, model_t *model, int attrmax)
{
    char attrname[NAME_LEN];
    int count;
    int mask;

    /* Create a new material structure and initialize it */
    cookie = LGT_COOKIE;

	/* Read the descriptive name of the material */
    /* (dark_red, light_blue, etc.               */
    count = fscanf(in, "%s", name);
    assert(count == 1);

    count = fscanf(in, "%s", attrname);
    assert(*attrname == '{');

    lgt_parse[0].loc = &emissivity;
    lgt_parse[1].loc = &location;
    

    // calls the generic parser
    mask = parser(in, lgt_parse, NUM_ATTRS, 0);

    assert(mask != 0);

    // add to list of materials
    model->lgts->add((void *)this);
}

light_t::~light_t()
{

}

void  light_t::getemiss(drgb_t *drgb)
{
	drgb = &emissivity;
}
void  light_t::getloc(vec_t *vec)
{
	vec = &location;
}
int   light_t::vischeck(vec_t *vec)
{
	return 0;
}
void light_t::illuminate(model_t *model, vec_t *base, object_t *hitobj, drgb_t *pixel)
{
	vec_t dir; // unit direction to light from hitpt
	object_t *obj; // closest object in dir to light
	double close; // dist to closest object in dir to light
	double cos; // of angle between normal and dir to light
	double dist; // distance to the light from hitpoint
	vec_t last_hitpt; // from hitobj argument
	vec_t last_norm;
	drgb_t diffuse = {0.0, 0.0, 0.0};

	drgb_t emiss;
	object_t baseobj = *hitobj;
	double workdist = dist;

	hitobj->getlast_hitpt(&last_hitpt);
	vec_diff(&last_hitpt, &location, &dir);
	dist = vec_len(&dir);
	vec_unit(&dir, &dir);
	
	hitobj->getlast_normal(&last_norm);
	cos = vec_dot(&dir, &last_norm);
	if(cos <= 0)
	{
		return;
	}
	obj = find_closest_object(model, &last_hitpt, &dir, hitobj, &close);
	
	
	if(obj != NULL && (close < dist))
		return;

	hitobj->getdiffuse(&diffuse);

	pix_prod(&diffuse, &emissivity, &diffuse);
	pix_scale(cos/dist, &diffuse, &diffuse);
	pix_sum(&diffuse, pixel, pixel);
	
	add_glint(hitobj, base, &dir, pixel);

}
void light_t::add_glint(object_t *hitobj, 
	vec_t *base,   //base of original ray
 	vec_t *dir,    //unit vector in direction to light
	drgb_t *pixel)
{

	material_t *mat = hitobj->getmat();
	vec_t unit;
	vec_t unitnorm;
	vec_t hitpt;
	vec_t hittolight;
	vec_t hittobase;
	drgb_t emiss;
	double shine = 0, dot, spec;
	
	mat->material_getshine(&shine);
	if(shine == 0.0)
		return;
	hitobj->getlast_hitpt(&hitpt);
	vec_diff(&hitpt, &location, &hittolight);
	vec_diff(&hitpt, base, &hittobase);
	vec_unit(&hittolight, &hittolight);
	vec_unit(&hittobase, &hittobase);


	vec_sum(&hittobase, &hittolight, &unit);
	vec_unit(&unit, &unit);
	
	hitobj->getlast_normal(&unitnorm);
	vec_unit(&unitnorm, &unitnorm);
	
	dot = vec_dot(&unit, &unitnorm);
	dot = pow(dot, shine);

	emiss = emissivity;	
	mat->material_getspecular(&spec);
	pix_scale(dot*spec, &emiss, &emiss);
	pix_sum(&emiss, pixel, pixel);
	

}

void light_t::printer(FILE *out)
{

	fprintf(out, "%-12s %s \n", "Light", name);
	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "Emissivity",
						   emissivity.r,
						   emissivity.g,
						   emissivity.b);
	fprintf(out, "%-12s %5.1lf %5.1lf %5.1lf \n", "Location",
						   location.x,
						   location.y,
						   location.z);
}

void light_list_print(model_t  *model, FILE  *out) {

   light_t *lgt;
   list_t *list = model->lgts;
   list->reset();
   while (list->not_end())
   {
      lgt = (light_t *)list->get_entity();
      lgt->printer(out);
      list->next_link();
   }
}

