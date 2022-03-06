#include "ray.h"

static pparm_t tpln_parse[ ] =
{
{"xdir",3, sizeof(double),"%lf", 0},
{"dimensions",2, sizeof(double), "%lf", 0},
{"altmaterial",1, sizeof(char[NAME_LEN]), "%s", 0},
};
#define NUM_ATTRS (sizeof(tpln_parse) / sizeof(pparm_t))

tplane_t::tplane_t(FILE *in, model_t *model, int attrmax) : plane_t(in, model, 2)
{

	int mask;
	char attrname[NAME_LEN];
	//vec_t projxdir;
	vec_t projxdir;
	
	strcpy(obj_type, "tplane");
	/* The parser is fairly generic but the address of where to */
	/* put the data must be updated for each new object */
	//fscanf(in, "%s", attrname);
	//assert(strcmp("tiled_plane", attrname));
	//fscanf(in,"%s", attrname);
	tpln_parse[0].loc = &xdir;
	tpln_parse[1].loc = dims;
	tpln_parse[2].loc = attrname;
	mask = parser(in, tpln_parse, NUM_ATTRS, attrmax);
	assert(mask == 7);

	altmat = material_getbyname(model, attrname);

	vec_project(&last_normal, &xdir, &projxdir);
	vec_unit(&xdir, &xdir);

	vec_copy(&projxdir, &rot.row[0]);
	vec_copy(&normal, &rot.row[2]);
	vec_cross(&rot.row[2], &rot.row[0], &rot.row[1]);

}



void tplane_t::getdiffuse(drgb_t *value)
{
	if(select()==0)
		object_t::getdiffuse(value);
	else
		altmat->material_getdiffuse(value);
}

void tplane_t::getambient(drgb_t *value)
{
	if(select()==0)
		object_t::getambient(value);
	else
		altmat->material_getambient(value);
}

int tplane_t::select()
{
	vec_t newloc = {0.0, 0.0, 0.0};
	vec_xform(&rot,&last_hitpt, &newloc);
	int numInt = 0;
	
	int x_ndx = (newloc.x + 10000)/ dims[0];
	int z_ndx = (newloc.y + 10000)/ dims[1];

	
	
	

	if((x_ndx + z_ndx) % 2 == 0)
	{	
		return 1;
	}
	else
	{		
		return 0;
	}
}

void tplane_t::printer(FILE *out)
{
	plane_t::printer(out);


	fprintf(out, "%-11s%5.1lf %5.1lf\n", "Dimensions", dims[0], 
							   dims[1]);
	fprintf(out, "%-12s%-11s\n", "Altmaterial", altmat->material_getname()); 

}
