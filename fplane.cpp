#include "ray.h"

static pparm_t fpln_parse[ ] =
{
{"xdir",3, sizeof(double),"%lf", 0},
{"dimensions",2, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(fpln_parse) / sizeof(pparm_t))
fplane_t::fplane_t(FILE *in, model_t *model, int attrmax) : plane_t(in, model, 2)
{

	int mask;
	vec_t projxdir;

	strcpy(obj_type, "plane");
	fpln_parse[0].loc = &xdir;
	fpln_parse[1].loc = dims;
	mask = parser(in, fpln_parse, NUM_ATTRS, attrmax);
	assert(mask == 3);

	//assert(5 == 0);
	vec_project(&last_normal, &xdir, &projxdir);
	vec_unit(&projxdir, &projxdir);

	vec_copy(&projxdir, &rot.row[0]);
	vec_copy(&normal, &rot.row[2]);
	vec_cross(&rot.row[2], &rot.row[0], &rot.row[1]);
}

void fplane_t::printer(FILE *out)
{
	plane_t::printer(out);

	fprintf(out, "%-11s%5.1lf%5.1lf%5.1lf\n", "xdir", xdir.x, xdir.y, xdir.z);
	//fprintf(out, "%-11s%5.1lf%5.1lf%5.1lf\n", "projxdir", projxdir.x, projxdir.y, projxdir.z);
	fprintf(out, "%-11s%5.1lf %5.1lf\n", "Dimensions", dims[0], 
							   dims[1]);
	fprintf(out, "%-11s \n%5.1lf%5.1lf%5.1lf\n", "rotation matrix", rot.row[0].x, 
								      rot.row[0].y, 
								      rot.row[0].z);
	fprintf(out, "%5.1lf%5.1lf%5.1lf\n", rot.row[1].x, rot.row[1].y, rot.row[1].z);
	fprintf(out, "%5.1lf%5.1lf%5.1lf\n", rot.row[2].x, rot.row[2].y, rot.row[2].z);
}

double fplane_t::hits(
	vec_t *base, /* ray base */
	vec_t *dir) /* unit direction vector */
{
	vec_t newloc;
	double t;

	t = plane_t::hits(base, dir);
	if(t == -1)
		return t;	

	vec_diff(&point, &last_hitpt, &newloc);
	vec_xform(&rot, &newloc, &newloc);
	
	if(0 <= newloc.x && newloc.x <= dims[0])
		if(0<=newloc.y && newloc.y <= dims[1])
		{	
			rothit = newloc;
			return t;
		}
			
	return -1;
	
}

void  fplane_t::get_dims(double *)
{

}


void  fplane_t::get_rothit(vec_t *)
{

}
