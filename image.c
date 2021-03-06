/*  image.c  */

/*  functions:
    	ray_trace() - code to add
		make_pixel() - complete function provided
		make_row() - code to add
		image_create() - code to add
*/
/*
Matthew Higgins
mrhiggi
cpsc102 002
MP1
*/
#include "ray.h"

static int count = 0;
//=============================================================================
// ******* ray_trace( ) *******
// 
// 
//=============================================================================
void ray_trace (
   model_t *model,
   vec_t *base,                // view point
   vec_t *dir,                 // unit direction vector
   drgb_t *pix,                // pixel return location
   double total_dist,          // distance ray has travelled so far
   object_t *last_hit)         // most recently hit object
{
	object_t *closest = NULL;
	double specref = 0.0, trans = 0.0;
	vec_t ref_dir;
	vec_t normal;
	vec_t hitpt;

	if (total_dist > MAX_DIST)
		return;

	double mindist;
	drgb_t thispix = {0.0, 0.0, 0.0};

	closest = find_closest_object(model, base, dir, last_hit, &mindist);

	if(closest != NULL)
	{
		closest->getambient(&thispix);
		add_illumination(model, base, closest, &thispix);
		closest->getspecular(&specref);
		closest->gettrans(&trans);
		closest->getlast_normal(&normal);
		closest->getlast_hitpt(&hitpt);
		
	}	

	total_dist += mindist;

	pix_scale(1/total_dist, &thispix, &thispix);

	
	if(specref != 0.0)
	{
		drgb_t specint = {0.0, 0.0, 0.0};

		vec_reflect(&normal, dir, &ref_dir);
		ray_trace(model, &hitpt, &ref_dir, &specint, total_dist, closest);
		pix_scale(specref, &specint, &specint);
		pix_sum(&specint, &thispix, &thispix);
		
	}

	if(trans != 0.0)
	{
		drgb_t transint = {0.0, 0.0, 0.0};
		drgb_t diffcolor = {0.0, 0.0, 0.0};

		ray_trace(model, &hitpt, dir, &transint, total_dist, closest);
		pix_scale(1.0 - trans, &thispix, &thispix);
		pix_scale(trans, &transint, &transint);
		add_illumination(model, &hitpt, closest, &diffcolor);
		pix_prod(&transint, &diffcolor, &transint);
		pix_sum(&transint, &thispix, &thispix);
		
	}
	
	pix_sum(&thispix, pix, pix);
}


//=============================================================================
// ******* make_pixel( ) *******
// 
// 
//=============================================================================
void make_pixel(model_t  *model, int  x, int  y) {
	vec_t raydir;
	vec_t view;
	drgb_t pix = {0.0, 0.0, 0.0};
	camera_t *cam = model->cam;
	int i;

	

	
	cam->getviewpt(&view);
	
   /*  The ray_trace function determines the pixel color in */
   /*  d_rgb units.   The last two parameters are used ONLY */
   /*  in the case of specular (bouncing) rays              */
	for(i = 0; i < AA_SAMPLES; i++)
	{
	cam->getdir(x, y, &raydir);
	ray_trace(model, &view, &raydir, &pix, 0.0, NULL);
	}
	pix_scale(1.0 / AA_SAMPLES, &pix, &pix);
	cam->store_pixel(x, y, &pix);

	return;
}


//=============================================================================
// ******* make_row( ) *******
// 
// 
//=============================================================================
void make_row(model_t  *model, int  y) 
{
   int x;
   camera_t *cam = model->cam;

   /*  for each pixel in the row, invoke make_pixel() */
   /*  to paint the pixel.                            */
	//  CODE HERE   
	for(x = 0; x < cam->getxdim(); x++)
	{
		make_pixel(model, x, y);
	}


}


//=============================================================================
// ******* image_create( ) *******
// 
// 
//=============================================================================
void image_create(model_t  *model, FILE  *out) 
{
   int y;
   camera_t *cam = model->cam;

   /*  fire rays through each pixel in the window; */
   /*  for each row, invoke make_row() to paint    */
   /*  the row of pixels.                          */
   //  CODE HERE 
	for(y = 0; y < cam->getydim(); y++)
		make_row(model, y);



   /*  write the image                           */
   cam->camera_write_image(out);
}

