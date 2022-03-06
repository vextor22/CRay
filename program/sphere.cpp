/*
Matthew Higgins 
4/25/2014
section 2
Babu
*/
#include "ray.h"

static pparm_t sph_parse[ ] =
{
{"center",3, sizeof(double),"%lf", 0},
{"radius",1, sizeof(double),"%lf", 0},
{"scale",3, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(sph_parse) / sizeof(pparm_t))

sphere_t::sphere_t(FILE *in, model_t *model, int attrmax): object_t(in, model)
{
	int mask;

	strcpy(obj_type, "sphere");
	/* The parser is fairly generic but the address of where to */
	/* put the data must be updated for each new object */
	sph_parse[0].loc = &center;
	sph_parse[1].loc = &radius;
	sph_parse[2].loc = &scale;
	mask = parser(in, sph_parse, NUM_ATTRS, attrmax);
	assert(mask == 3);
}

double sphere_t::hits(vec_t *base, vec_t *dir)
{
	//sphere_t *sph;
	assert (cookie == OBJ_COOKIE);

	vec_t new_center, new_base,temp,hit, normal;
	
	double a, b, c;
	double discr, dist;

	// extract sph pointer from object structure 
	// see notes for sphere_hits function
   // --- CODE GOES HERE ---
	
	vec_diff(&(center),&(center), &new_center);
	vec_diff(&(center), base, &new_base);
	
	a = vec_dot(dir, dir);
	b = 2*vec_dot(&new_base, dir);
	c = vec_dot(&new_base, &new_base) - pow(radius, 2.0);

	discr = (b*b) - (4 * a * c);

	if(discr <= 0)
		return -1.0;
	else
	{
		dist = (-b - sqrt(discr))/(2*a);
	}

	vec_scale(dist, dir, &temp);
	vec_sum(&temp, base, &hit);

	vec_copy(&hit, &last_hitpt);
	
	vec_diff(&center, &hit, &temp);
	vec_unit(&temp, &normal);
	vec_copy(&normal, &last_normal);	
	
	// return distance
	return dist;
}
void sphere_t::printer(FILE *out)
{
	

   //  ---  CODE GOES HERE ---
	// Print generic attributes
	object_t::printer(out);

	// Recover sph pointer from object_t and print
	// center and radius in usual format
	fprintf(out,"%-11s%5.1lf%5.1lf%5.1lf\n","center", center.x, center.y,
					    center.z);
	fprintf(out,"%-11s%5.1lf\n", "radius", radius);
}
