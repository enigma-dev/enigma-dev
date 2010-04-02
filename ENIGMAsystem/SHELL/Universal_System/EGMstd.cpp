/*********************************************************************************\
*                                                                             **
* Copyright (C) 2008 Josh Ventura                                             **
*                                                                             **
* This file is a part of the ENIGMA Development Environment.                  **
*                                                                             **
*                                                                             **
* ENIGMA is free software: you can redistribute it and/or modify it under the **
* terms of the GNU General Public License as published by the Free Software   **
* Foundation, version 3 of the license or any later version.                  **
*                                                                             **
* This application and its source code is distributed AS-IS, WITHOUT ANY      **
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
* details.                                                                    **
*                                                                             **
* You should have recieved a copy of the GNU General Public License along     **
* with this code. If not, see <http://www.gnu.org/licenses/>                  **
*                                                                             **
* ENIGMA is an environment designed to create games and other programs with a **
* high-level, fully compilable language. Developers of ENIGMA or anything     **
* associated with ENIGMA are in no way responsible for its users or           **
* applications created by its users, or damages caused by the environment     **
* or programs made in the environment.                                        **
*                                                                             **
\*********************************************************************************/

#include <math.h>
#include <string>
#include <stdio.h>
#include "var_cr3.h"
#include "EGMstd.h"
#define MANZ(x) (*(long long*)&(x)&0x07FFFFFFFFFFFFFF)
std::string string(std::string val){return val;}
std::string string(double val){
	char retstr[41],*a=retstr+sprintf(retstr,"%.2f",val)-1;
	if(*a=='0'){
		*a=0;
		if(*--a=='0'){
			*a=0;
			if(*--a=='.') *a=0;
		}
	}
	return retstr;
}
std::string string(var& val){return val.values[0]->type?(std::string)val:string((double)val);}
std::string string(enigma::variant& val){return val.type?(std::string)val:string((double)val);}
//Here begins ds_ stuff. Should go elsewhere, but that involves poking the compiler
//I never spent too much time with ds_. Therefore my memory is fuzzy of the implementation details
//I'll therefore not strictly follow the id patterns. I think ids didn't repeat. They will now
//Idea: Use var itself to implement these. Idea is to circumvent leak issues
/*
#include <vector>
struct ds_list{
	ds_list*next;
};
//ds_list**ds_lists=new ds_list*[1];
double ds_list_create();

struct ds_stack{
	ds_stack*n;
	void*d;
};
std::vector<ds_stack*>ds_stacks;
double ds_stack_create();
double ds_stack_destroy(double id);
double ds_stack_clear(double id);
double ds_stack_copy(double id,double src);
double ds_stack_size(double id);
double ds_stack_push(double id,var& val);
double ds_stack_pop(double id);
double ds_stack_top(double id);
double ds_stack_write(double id);
double ds_stack_read(double id,std::string str);

double ds_stack_create(){
	unsigned int i=0;
	while(ds_stacks[i]) i++;
	ds_stacks[i]=new ds_stack;
	ds_stacks[i]->n->n=ds_stacks[i]->sz=0;
	return i;
}
double ds_stack_destroy(double id){
	if(ds_stacks[i]){
		delete ds_stacks[i];
		ds_stacks[i]=0;
	}
	return 0;
}
double ds_stack_clear(double id){
	ds_stack*n=ds_stacks[id],*nn;
	while(n){
		nn=n;
		n=n->n;
		delete nn;
	}
	return 0;
}
double ds_stack_copy(double id,double src);
double ds_stack_size(double id){
	ds_stack*n=ds_stacks[id];
	unsigned int sz=0;
	while(n){
		sz++;
		n=n->n;
	}
	return sz;
}
double ds_stack_push(double id,var& val);
double ds_stack_pop(double id);
double ds_stack_top(double id);
double ds_stack_write(double id);
double ds_stack_read(double id,std::string str);

struct ds_queue{};
double ds_queue_create();

struct ds_map{};
double ds_map_create();

struct ds_priority{};
double ds_priority_create();

struct ds_grid{};
double ds_grid_create();
*/

/*
ds_stack_create() Creates a new stack. The function returns an integer as an id that must be used in all other functions to access the particular stack. You can create multiple stacks.
ds_stack_destroy(id) Destroys the stack with the given id, freeing the memory used. Don't forget to call this function when you are ready with the structure.
ds_stack_clear(id) Clears the stack with the given id, removing all data from it but not destroying it.
ds_stack_copy(id,source) Copies the stack source into the stack with the given id.
ds_stack_size(id) Returns the number of values stored in the stack.
ds_stack_empty(id) Returns whether the stack is empty. This is the same as testing whether the size is 0.
ds_stack_push(id,val) Pushes the value on the stack.
ds_stack_pop(id) Returns the value on the top of the stack and removes it from the stack.
ds_stack_top(id) Returns the value on the top of the stack but does not remove it from the stack.
ds_stack_write(id) Turns the data structure into a string and returns this string. The string can then be used to e.g. save it to a file. This provides an easy mechanism for saving data structures.
ds_stack_read(id,str) Reads the data structure from the given string (as created by the previous call).

ds_queue_create() Creates a new queue. The function returns an integer as an id that must be used in all other functions to access the particular queue. You can create multiple queues.
ds_queue_destroy(id) Destroys the queue with the given id, freeing the memory used. Don't forget to call this function when you are ready with the structure.
ds_queue_clear(id) Clears the queue with the given id, removing all data from it but not destroying it.
ds_queue_copy(id,source) Copies the queue source into the queue with the given id.
ds_queue_size(id) Returns the number of values stored in the queue.
ds_queue_empty(id) Returns whether the queue is empty. This is the same as testing whether the size is 0.
ds_queue_enqueue(id,val) Enters the value in the queue.
ds_queue_dequeue(id) Returns the value that is longest in the queue and removes it from the queue.
ds_queue_head(id) Returns the value at the head of the queue, that is, the value that has been the longest in the queue. (It does not remove it from the queue.)
ds_queue_tail(id) Returns the value at the tail of the queue, that is, the value that has most recently been added to the queue. (It does not remove it from the queue.)
ds_queue_write(id) Turns the data structure into a string and returns this string. The string can then be used to e.g. save it to a file. This provides an easy mechanism for saving data structures.
ds_queue_read(id,str) Reads the data structure from the given string (as created by the previous call).

ds_list_create() Creates a new list. The function returns an integer as an id that must be used in all other functions to access the particular list.
ds_list_destroy(id) Destroys the list with the given id, freeing the memory used. Don't forget to call this function when you are ready with the structure.
ds_list_clear(id) Clears the list with the given id, removing all data from it but not destroying it.
ds_list_copy(id,source) Copies the list source into the list with the given id.
ds_list_size(id) Returns the number of values stored in the list.
ds_list_empty(id) Returns whether the list is empty. This is the same as testing whether the size is 0.
ds_list_add(id,val) Adds the value at the end of the list.
ds_list_insert(id,pos,val) Inserts the value at position pos in the list. The first position is 0, the last position is the size of the list minus 1.
ds_list_replace(id,pos,val) Replaces the value at position pos in the list with the new value.
ds_list_delete(id,pos) Deletes the value at position pos in the list. (Position 0 is the first element.)
ds_list_find_index(id,val) Find the position storing the indicated value. If the value is not in the list -1 is returned.
ds_list_find_value(id,pos) Returns the value stored at the indicated position in the list.
ds_list_sort(id,ascend) Sorts the values in the list. When ascend is true the values are sorted in ascending order, otherwise in descending order.
ds_list_shuffle(id) Shuffles the values in the list such that they end up in a random order.
ds_list_write(id) Turns the data structure into a string and returns this string. The string can then be used to e.g. save it to a file. This provides an easy mechanism for saving data structures.
ds_list_read(id,str) Reads the data structure from the given string (as created by the previous call).

ds_map_create() Creates a new map. The function returns an integer as an id that must be used in all other functions to access the particular map.
ds_map_destroy(id) Destroys the map with the given id, freeing the memory used. Don't forget to call this function when you are ready with the structure.
ds_map_clear(id) Clears the map with the given id, removing all data from it but not destroying it.
ds_map_copy(id,source) Copies the map source into the map with the given id.
ds_map_size(id) Returns the number of key-value pairs stored in the map.
ds_map_empty(id) Returns whether the map is empty. This is the same as testing whether the size is 0.
ds_map_add(id,key,val) Adds the key-value pair to the map.
ds_map_replace(id,key,val) Replaces the value corresponding with the key with a new value.
ds_map_delete(id,key) Deletes the key and the corresponding value from the map. (If there are multiple entries with the same key, only one is removed.)
ds_map_exists(id,key) Returns whether the key exists in the map.
ds_map_find_value(id,key) Returns the value corresponding to the key.
ds_map_find_previous(id,key) Returns the largest key in the map smaller than the indicated key. (Note that the key is returned, not the value. You can use the previous routine to find the value.)
ds_map_find_next(id,key) Returns the smallest key in the map larger than the indicated key.
ds_map_find_first(id) Returns the smallest key in the map.
ds_map_find_last(id) Returns the largest key in the map.
ds_map_write(id) Turns the data structure into a string and returns this string. The string can then be used to e.g. save it to a file. This provides an easy mechanism for saving data structures.
ds_map_read(id,str) Reads the data structure from the given string (as created by the previous call).

ds_priority_create() Creates a new priority queue. The function returns an integer as an id that must be used in all other functions to access the particular priority queue.
ds_priority_destroy(id) Destroys the priority queue with the given id, freeing the memory used. Don't forget to call this function when you are ready with the structure.
ds_priority_clear(id) Clears the priority queue with the given id, removing all data from it but not destroying it.
ds_priority_copy(id,source) Copies the priority queue source into the priority queue with the given id.
ds_priority_size(id) Returns the number of values stored in the priority queue.
ds_priority_empty(id) Returns whether the priority queue is empty. This is the same as testing whether the size is 0.
ds_priority_add(id,val,prio) Adds the value with the given priority to the priority queue.
ds_priority_change_priority(id,val,prio) Changes the priority of the given value in the priority queue.
ds_priority_find_priority(id,val) Returns the priority of the given value in the priority queue.
ds_priority_delete_value(id,val) Deletes the given value (with its priority) from the priority queue.
ds_priority_delete_min(id) Returns the value with the smallest priority and deletes it from the priority queue.
ds_priority_find_min(id) Returns the value with the smallest priority but does not delete it from the priority queue.
ds_priority_delete_max(id) Returns the value with the largest priority and deletes it from the priority queue.
ds_priority_find_max(id) Returns the value with the largest priority but does not delete it from the priority queue.
ds_priority_write(id) Turns the data structure into a string and returns this string. The string can then be used to e.g. save it to a file. This provides an easy mechanism for saving data structures.
ds_priority_read(id,str) Reads the data structure from the given string (as created by the previous call).

ds_grid_create(w,h) Creates a new grid with the indicated width and height. The function returns a integer as an id that must be used in all other functions to access the particular grid.
ds_grid_destroy(id) Destroys the grid with the given id, freeing the memory used. Don't forget to call this function when you are ready with the structure.
ds_grid_copy(id,source) Copies the grid source into the grid with the given id.
ds_grid_resize(id,w,h) Resizes the grid to the new width and height. Existing cells keep their original value.
ds_grid_width(id) Returns the width of the grid with the indicated id.
ds_grid_height(id) Returns the height of the grid with the indicated id.
ds_grid_clear(id,val) Clears the grid with the given id, to the indicated value (can both be a number or a string).
ds_grid_set(id,x,y,val) Sets the indicated cell in the grid with the given id, to the indicated value (can both be a number or a string).
ds_grid_add(id,x,y,val) Add the value to the indicated cell in the grid with the given id. For strings this corresponds to concatenation.
ds_grid_multiply(id,x,y,val) Multiplies the value to the indicated cell in the grid with the given id. Is only valid for numbers.
ds_grid_set_region(id,x1,y1,x2,y2,val) Sets the all cells in the region in the grid with the given id, to the indicated value (can both be a number or a string).
ds_grid_add_region(id,x1,y1,x2,y2,val) Add the value to the cell in the region in the grid with the given id. For strings this corresponds to concatenation.
ds_grid_multiply_region(id,x1,y1,x2,y2,val) Multiplies the value to the cells in the region in the grid with the given id. Is only valid for numbers.
ds_grid_set_disk(id,xm,ym,r,val) Sets all cells in the disk with center (xm,ym) and radius r.
ds_grid_add_disk(id,xm,ym,r,val) Add the value to all cells in the disk with center (xm,ym) and radius r.
ds_grid_multiply_disk(id,xm,ym,r,val) Multiply the value to all cells in the disk with center (xm,ym) and radius r.
ds_grid_set_grid_region(id,source,x1,y1,x2,y2,xpos,ypos) Copies the contents of the cells in the region in grid source to grid id. xpos and ypos indicate the place where the region must be placed in the grid. (Can also be used to copy values from one place in a grid to another.)
ds_grid_add_grid_region(id,source,x1,y1,x2,y2,xpos,ypos) Adds the contents of the cells in the region in grid source to grid id. xpos and ypos indicate the place where the region must be added in the grid. (id and source can be the same.)
ds_grid_multiply_grid_region(id,source,x1,y1,x2,y2,xpos,ypos) Multiplies the contents of the cells in the region in grid source to grid id. xpos and ypos indicate the place where the region must be multiplied in the grid. (id and source can be the same.) Only valid for numbers.
ds_grid_get(id,x,y) Returns the value of the indicated cell in the grid with the given id.
ds_grid_get_sum(id,x1,y1,x2,y2) Returns the sum of the values of the cells in the region in the grid with the given id. Does only work when the cells contain numbers.
ds_grid_get_max(id,x1,y1,x2,y2) Returns the maximum of the values of the cells in the region in the grid with the given id. Does only work when the cells contain numbers.
ds_grid_get_min(id,x1,y1,x2,y2) Returns the minimum of the values of the cells in the region in the grid with the given id. Does only work when the cells contain numbers.
ds_grid_get_mean(id,x1,y1,x2,y2) Returns the mean of the values of the cells in the region in the grid with the given id. Does only work when the cells contain numbers.
ds_grid_get_disk_sum(id,xm,ym,r) Returns the sum of the values of the cells in the disk.
ds_grid_get_disk_min(id,xm,ym,r) Returns the min of the values of the cells in the disk.
ds_grid_get_disk_max(id,xm,ym,r) Returns the max of the values of the cells in the disk.
ds_grid_get_disk_mean(id,xm,ym,r) Returns the mean of the values of the cells in the disk.
ds_grid_value_exists(id,x1,y1,x2,y2,val) Returns whether the value appears somewhere in the region.
ds_grid_value_x(id,x1,y1,x2,y2,val) Returns the x-coordinate of the cell in which the value appears in the region.
ds_grid_value_y(id,x1,y1,x2,y2,val) Returns the y-coordinate of the cell in which the value appears in the region.
ds_grid_value_disk_exists(id,xm,ym,r,val) Returns whether the value appears somewhere in the disk.
ds_grid_value_disk_x(id,xm,ym,r,val) Returns the x-coordinate of the cell in which the value appears in the disk.
ds_grid_value_disk_y(id,xm,ym,r,val) Returns the y-coordinate of the cell in which the value appears in the disk.
ds_grid_shuffle(id) Shuffles the values in the grid such that they end up in a random order.
ds_grid_write(id) Turns the data structure into a string and returns this string. The string can then be used to e.g. save it to a file. This provides an easy mechanism for saving data structures.
ds_grid_read(id,str) Reads the data structure from the given string (as created by the previous call).
*/
