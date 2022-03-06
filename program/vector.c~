/**  vector.c   **/

#include "vector.h"

/*  This file contains function definitions for    */
/*  for 3D vector operations                       */

/* Compute the sum of two vectors */
/* v3 = v2 + v1; */
void vec_sum(
vec_t *v1,   /* addend */
vec_t *v2,   /* addend */
vec_t *v3)   /* result */
{
	(*v3).x = (*v1).x + (*v2).x;
	(*v3).y = (*v1).y + (*v2).y;
	(*v3).z = (*v1).z + (*v2).z;
}

/* Compute the difference of two vectors */
/* v3 = v2 - v1 */
void vec_diff(
vec_t *v1,   /* subtrahend */
vec_t *v2,   /* minuend    */
vec_t *v3)   /* result     */
{
	(*v3).x = (*v2).x - (*v1).x;
	(*v3).y = (*v2).y - (*v1).y;
	(*v3).z = (*v2).z - (*v1).z;
}

/* Compute the componentwise product of two vectors */
/* v3 = v2 * v1; */
void vec_mult( 
vec_t *v1,   /* Input vector 1 */
vec_t *v2,   /* Input vector 2 */
vec_t *v3)   /* result         */
{
	(*v3).x = (*v1).x * (*v2).x;
	(*v3).y = (*v1).y * (*v2).y;
	(*v3).z = (*v1).z * (*v2).z;
}

/* Return the (dot) inner product of two input vectors     */
double vec_dot(
vec_t *v1,   /* Input vector 1 */
vec_t *v2)   /* Input vector 2 */
{
	double dotprod = 0;
	dotprod = ((*v1).x * (*v2).x) + ((*v1).y * (*v2).y) + ((*v1).z * (*v2).z);
	return dotprod;
}

/* Return length of a 3d vector */
double vec_len(
vec_t *v1)  /* Vector whose length is desired */
{
	double length;
	length = sqrt(pow((*v1).x, 2) + pow((*v1).y, 2) + pow((*v1).z, 2));
	return length;
}

/* Scale a 3d vector by multiplying each element by scale factor */
void vec_scale(
double fact,   /* Scale factor */
vec_t *v1,      /* Input vector */
vec_t *v2)      /* Output vector */
{
	(*v2).x = (*v1).x * fact;
	(*v2).y = (*v1).y * fact;
	(*v2).z = (*v1).z * fact;
}

/* Construct a unit vector v2 in direction of input vector v1 */
void vec_unit(
vec_t *v1,    /* Input vector      */
vec_t *v2)    /* output unit vec   */
{
	double scale = 1.0 / vec_len(v1);
	vec_scale(scale, v1, v2);
}

/* Copy vector v1 to v2 another */
void vec_copy(
vec_t *v1,   /* input vector */
vec_t *v2)   /* output vector */
{
	(*v2).x = (*v1).x;
	(*v2).y = (*v1).y;
	(*v2).z = (*v1).z;
}

/* Read in values of vector from file */
/* Return the # of items read	*/
int vec_read(
FILE *in,     /* input file  */
vec_t *v1)    /* vector from input file */
{
	int valuesRead;
	float x = 0, y = 0, z = 0;
	valuesRead = fscanf(in, "%f %f %f", &x, &y, &z);

	(*v1).x = x;
	(*v1).y = y;
	(*v1).z = z;

	if (valuesRead != 3)
	{
		printf("3 Values not read, exiting.");
		exit(0);
	}
	return valuesRead;
}

/* Print values of vector to file */
void vec_print(
FILE *out,     /* output file */
char *label,   /* label string */
vec_t *v1)      /* vector to print */
{
	fprintf(out,"%s %8.3f%8.3f%8.3f\n", label, (*v1).x, (*v1).y, (*v1).z);

}

void vec_cross(
  vec_t *v1,    /* Left input vector  */
  vec_t *v2,    /* Right input vector */
  vec_t *v3)    /* Output vector      */ 
{
	vec_t new1;
	new1.x = (v1->y*v2->z)-(v1->z*v2->y);
	new1.y = (v1->z*v2->x)-(v1->x*v2->z);
	new1.z = (v1->x*v2->y)-(v1->y*v2->x);
	
	vec_copy(&new1, v3);
}

/* project a vector onto a plane */
void vec_project(
  vec_t *n,       /* plane normal     */
  vec_t *v,       /* input vector     */
  vec_t *p)       /* projected vector */ 
{
	vec_t new1;
	new1.x = v->x - vec_dot(n,v)*n->x;
	new1.y = v->y - vec_dot(n,v)*n->y;
	new1.z = v->z - vec_dot(n,v)*n->z;
	vec_copy(&new1, p);

	//vec_unit(*unit);
	//double dot = sqrt((v->x * n->x) + (v->y *n->y) + (v->z * n->z));
	//p->x = v->x - dot*n->x;	
	//p->y = v->y - dot*n->y;
	//p->z = v->z - dot*n->z;	

}

/* reflect a vector from a surface plane */
void vec_reflect(
  vec_t *n,       /* surface normal      */
  vec_t *w,       /* incoming ray vector */
  vec_t *v)       /* reflected vector    */
{
	vec_t u, U, N;
	u.x = -(w->x);
	u.y = -(w->y);
	u.z = -(w->z);
	vec_unit(&u, &U);
	vec_unit(n, &N);
	
	v->x = 2*N.x*vec_dot(&U,&N) - U.x;
	v->y = 2*N.y*vec_dot(&U,&N) - U.y;
	v->z = 2*N.z*vec_dot(&U,&N) - U.z;
}

/* Apply transform matrix to vector */
void vec_xform(
  mtx_t *m,
  vec_t *v1,
  vec_t *v2)
{
	vec_t new1;
	new1.x = vec_dot(&(m->row[0]),v1);
	new1.y = vec_dot(&(m->row[1]),v1);
	new1.z = vec_dot(&(m->row[2]),v1);

	vec_copy(&new1, v2);
}

/* Compute the transpose of a matrix */
void mtx_transpose(
  mtx_t *m1,
  mtx_t *m2)
{
mtx_t *m3 = (mtx_t *)malloc(sizeof(mtx_t));

	m3->row[0].x = m1->row[0].x;
	m3->row[1].x = m1->row[0].y;
	m3->row[2].x = m1->row[0].z;
	
	m3->row[0].y = m1->row[1].x;
	m3->row[1].y = m1->row[1].y;
	m3->row[2].y = m1->row[1].z;

	m3->row[0].z = m1->row[2].x;
	m3->row[1].z = m1->row[2].y;
	m3->row[2].z = m1->row[2].z;

	m2->row[0] = m3->row[0];
	m2->row[1] = m3->row[1];
	m2->row[2] = m3->row[2];
} 








