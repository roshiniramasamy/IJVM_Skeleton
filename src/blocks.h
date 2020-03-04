#include <ijvm.h>
//#include <stdlib.h>

uint32_t constant_pool_origin;
uint32_t constant_pool_size;
word_t *constant_pool_data;

uint32_t text_origin;
uint32_t text_sz;
byte_t *text;

int variable1;
int variable2;
FILE*standardout;

static uint32_t swap_uint32(uint32_t num) {
  return ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000);
 }
