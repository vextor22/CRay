/* list.c */

/*
Matthew Higgins 
3/25/2014
lab section 4
*/

#include "list.h"
#include <stdlib.h>
#include <stdio.h>

link_t::link_t(void *newentity)
{

	next = NULL;
	entity = newentity;
}

link_t::link_t(void)
{
	next = NULL;
	entity = NULL;
}

link_t::~link_t(void)
{
   fprintf(stderr, "in link destructor \n");
}

void link_t::set_next(link_t *new_next)
{

	next = new_next;

}

link_t * link_t::get_next()
{

	return next;

}

void * link_t::get_entity()
{

	return entity;

}


list_t::list_t()
{

	first = NULL;
	last = NULL;
	current = NULL;

}

void list_t::add(void *entity)
{

/* Create a new link passing it the entity pointer */
	link_t *new_link;
	new_link = new link_t(entity);



/* Now add the link to the list using generally the */
/* same approach as the C version                   */
	if(first == NULL)
	{
		first = new_link;
		last  = new_link;
		current = new_link;
	}
	else
	{
		last->set_next(new_link);
		current = new_link;
		last = new_link;
	}
}

void list_t::insert(void *entity)
{
	link_t *new_link;
	link_t *traverse;
	new_link = new link_t(entity);
	//traverse = new link_t();

	if(first == NULL)
	{
		first = new_link;
		last  = new_link;
		current = new_link;	
	}
	else if(current == first)
	{
		new_link->set_next(first);
		first = new_link;
		current = new_link;
	}
	else
	{
		traverse = first;
		while(traverse->get_next() != current)
		{
			traverse = traverse->get_next();
		}
		traverse->set_next(new_link);
		new_link->set_next(current);
		current = new_link;
	}

}

void list_t::remove(void)
{
	link_t *traverse;
	//traverse = new link_t();
	
	if(first != NULL)
	{
		if(first == last)
		{
			delete (first);
			first = NULL;
			current = NULL;
			last = NULL;
		}
		else if(current == first)
		{
			first = first->get_next();
			delete(current);
			current = first;
		}
		else
		{
			traverse = first;
			while(traverse->get_next() != current)
			{
				traverse = traverse->get_next();
			}
			traverse->set_next(current->get_next());
			if(current == last)
			{
				last = traverse;
			}
			delete (current);
			current = traverse;
		}
	}
}

void list_t::reset(void)
{
	
	current = first;
}

int list_t::not_end(void)
{

	if(current == NULL)
		return 0;
	else
		return 1;


}

void list_t::next_link(void)
{
   assert(current != NULL);
   current = current->get_next();

}

void *list_t::get_entity(void)
{
   assert(current != NULL);
   void *entity;
   
   entity = current->get_entity();

   return entity;

}

list_t::~list_t()
{
   current = first;
   link_t *kill;
   fprintf(stderr, "in list destructor \n");

/* Delete each link_t in the list */
	
    while(current != NULL){
	kill = current;
	current = current->get_next();
	delete kill;

    }

}


