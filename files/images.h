#ifndef DATA_LIST_H
#define DATA_LIST_H

#include "globals.h"

 /* global variable represents the head of our data image */
extern struct image_node_t* data_head;
 /** 
 * calculates the size of image
 */
int get_data_size();

 /** 
 * inserts byte in data image
 * @param buffer - individual byte inside data image
 * @return size of data image
 */
void insert_data_image(unsigned char buffer);

 /** 
 * calculates the size of image
 * @return size of data image
 */
void reverse_data_image();

 /** 
 * frees data image
 */
void free_data_image();

 /* global variable represents the head of our code image */
extern struct image_node_t* code_head;

 /** 
 * calculates the size of image
 * @return size of code image
 */
int get_code_size();

 /** 
 * inserts byte in code image
 * @param buffer - individual byte inside code image
 */
void insert_code_image(unsigned char buffer);

 /**
 * updates individual element inside code image
 * @param buffer - individual byte inside code image
 * @param index - where should we update
 */
void update_code_image(unsigned char buffer, int index);

 /**
 * reverses the code image linked-list
 */
void reverse_code_image();

 /**
 * frees code image
 */
void free_code_image();

#endif