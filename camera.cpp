#include "ray.h"

static pparm_t cam_parse[ ] =
{
{"pixeldim",2, sizeof(int),"%d", 0},
{"worlddim",2, sizeof(double), "%lf", 0},
{"viewpoint", 3, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(cam_parse) / sizeof(pparm_t))

camera_t::camera_t(FILE *in)
{

	
	char buf[256];
	int mask = 0;	

	
	assert(fscanf(in, "%s", name) == 1);
	
	fscanf(in, "%s", buf);
	assert(buf[0] == '{');

	cookie = CAM_COOKIE;

	cam_parse[0].loc = &pixel_dim;
	cam_parse[1].loc = &world_dim;
	cam_parse[2].loc = &view_point;

	mask = parser(in, cam_parse, NUM_ATTRS, 0);

	assert(mask == 7);
	
	pixmap = (irgb_t *)malloc(sizeof(irgb_t)*pixel_dim[X]*pixel_dim[Y]);
	
}

void camera_t::camera_print(FILE *out)
{

	assert(cookie == CAM_COOKIE);
	//prints all strings in field 12 wide, left aligned
	//all floats in field of 5, with 1 decimal, ints without the decimal
	fprintf(out, "%-12s%-12s\n", "camera", name);
	fprintf(out, "%-12s%5d%5d\n", "pixeldim", pixel_dim[X], pixel_dim[Y]);
	fprintf(out, "%-12s%5.1lf%5.1lf\n", "worlddim", world_dim[X], world_dim[Y]);
	fprintf(out, "%-12s%5.1lf%5.1lf%5.1lf\n", "viewpoint", view_point.x, view_point.y,
							    view_point.z);	

}

void camera_t::getdir(int x, int y, vec_t *dir)
{
	//declare world variables for converted pixel coordinates
	assert(cookie == CAM_COOKIE);
	double worldX, worldY, dx, dy;
	dx = x;
	dy = y;
	
	if (AA_SAMPLES > 1)
	{
		dx = randomize(dx);
		dy = randomize(dy);
	}

	//convert x and y to worldX and worldY
	worldX = (1.0*dx/pixel_dim[0])*world_dim[0];
	worldY = (1.0*dy/pixel_dim[1])*world_dim[1];

	//allocate and 0 a placeholder vector
	vec_t *vec = (vec_t *)malloc(sizeof(vec_t));
	memset(vec, 0, sizeof(vec_t));

	//store components of connecting 
	vec->z = -view_point.z;
	vec->x =  worldX - view_point.x;
	vec->y =  worldY - view_point.y;
	
	//make connecting vector into a unit vector, put unit vector into uvec
	vec_unit(vec, dir);
}

double randomize(double input)
{
	double randy = random();
	randy /= 2147483647;
	randy -= .5;
	input += randy;
	return input;
	
}

unsigned char camera_t::get_rgb(float pixel_color)
{
	//scales pixel_color and uses .5 to account for rounding
	float new_value = pixel_color * 255 + .5;

	//clamps range of rgb values
	if(new_value < 0.0)
		return(unsigned char)0;
	else if(new_value>255.0)
		return(unsigned char)255;
	else
		return (unsigned char)new_value;

}


void camera_t::store_pixel(int x, int y, drgb_t *pix)
{
	assert(cookie == CAM_COOKIE);
	
	int row, col;
	
	col = x;
	row = pixel_dim[Y] - y - 1;

	irgb_t *ipix;

	ipix = pixmap + row*pixel_dim[X] + col;

	ipix->r = get_rgb(pix->r);
	ipix->g = get_rgb(pix->g);
	ipix->b = get_rgb(pix->b);
}



void camera_t::camera_write_image(FILE *out)
{
	//prints out header of ppm image
	fprintf(out,"P6\n");
	fprintf(out, "%d %d 255\n", pixel_dim[X], pixel_dim[Y]);
	//prints out body of image from pixmap
	fwrite(pixmap, sizeof(irgb_t), pixel_dim[X]*pixel_dim[Y], out);
}

int camera_t::getydim(void)
{
	return(pixel_dim[1]);
}

int camera_t::getxdim(void)
{
	return(pixel_dim[0]);
}

void camera_t::getviewpt(vec_t *view)
{
	*view = view_point;
}
