#include <ijvm.h>
#include <stdlib.h>
#include "blocks.h"
#include "stack.h"

int counter = 0;
bool control;

int convert() {
  byte_t b1, b2;
  b1 = get_number();
  b2 = get_number();
  int result = b2 | b1 << 8;
  return result;
}

void display_memory(char *label, byte_t *a, int num_bytes, int bytes_per_row) {
	int i, k = 0;

	printf("%s:\n", label);
	for (i = 0 ; i < num_bytes ; ++i) {
		printf("0x%02x", a[i]);
		if (++k >= bytes_per_row) {
			k = 0;
			printf("\n");
		} else {
			printf(" ");
		}
	}
	if (k != 0) {
		printf("\n");
	}
}

int init_ijvm(char *binary_file) {
  int buffer;
  stack.size = 0;
  FILE *fp;
  fp = fopen(binary_file, "rb");

  fread(&buffer, sizeof(uint32_t), 1, fp);
  buffer = swap_uint32(buffer);
  if(buffer == MAGIC_NUMBER) {
    fread(&constant_pool_origin, sizeof(uint32_t), 1, fp);
    constant_pool_origin = swap_uint32(constant_pool_origin);

    fread(&constant_pool_size, sizeof(uint32_t), 1, fp);
    constant_pool_size = swap_uint32(constant_pool_size);

    if(constant_pool_size != 0) {
      constant_pool_data = malloc(constant_pool_size);
      fread(constant_pool_data, sizeof(uint32_t), (constant_pool_size/4), fp);
    }

    fread(&text_origin, sizeof(uint32_t), 1, fp);
    text_origin = swap_uint32(text_origin);

    fread(&text_sz, sizeof(uint32_t), 1, fp);
    text_sz = swap_uint32(text_sz);

    if(text_sz != 0) {
      text = malloc(text_sz);
      fread(text, sizeof(uint8_t), text_sz, fp);
    }
    display_memory("Text Memory", text, text_sz,4);

    control =true;
    return 0;
  }
    else {
      return -1;
    }
}

void destroy_ijvm() {
  free(text);
  counter =0;
  control = true;
  constant_pool_origin=0;
  constant_pool_size=0;
  //word_t *constant_pool_data=0;

  text_origin=0;
  text_sz=0;
}

void run() {
  while ((counter >= text_sz || counter<0) && control) {
    step();
  }

}

void set_input(FILE *fp) {
FILE *out;
}



void set_output(FILE *file) {
  out = file;
}

int get_program_counter(void) {
  return counter;
}


byte_t *get_text(void) {
  return text;
}

byte_t get_instruction(void) {
  return text[counter];
}

word_t tos(){
  return top();
}

word_t get_local_variable(int i) {
  return stack.local_variable[i];
}
word_t get_constant(int i) {
  return stack.constant[i];
}

int get_number() {
  counter++;
   char x = text[counter];
  return x;
}

bool step(void) {
    switch (get_instruction()) {

      uint32_t value2;
      short s_var1;

      case 0x10:
      variable1 = get_number();
      printf("BIPUSH %d\n",variable1 );
      push(variable1);
      break;

      case 0x60:
      printf("IADD\n");
      variable1 = pop();
      variable2 = pop();
      push(variable1+variable2);
      break;

      case 0x64:
      printf("ISUB\n");
      variable1 = pop();
      variable2 = pop();
      push(variable2-variable1);
      break;

      case 0x7E:
      printf("IAND\n");
      variable1 = pop();
      variable2 = pop();
      push(variable1&variable2);
      break;

      case 0xB0:
      printf("IOR\n");
      variable1 = pop();
      variable2 = pop();
      push(variable1|variable2);
      break;

      case 0x5F:
      printf("SWAP\n");
      variable1 = pop();
      variable2 = pop();
      push(variable1);
      push(variable2);
      break;

      case 0x57:
      printf("POP\n");
      variable1 = pop();
      break;

      case 0xA7:
      printf("%s ","GOTO");
      s_var1 = convert();
      printf("%d\n", s_var1);
      counter = counter + s_var1 - 3;
      // printf("COUNTER: %d\n", counter);
      break;

      case 0xFD:
      printf("%s\n","OUT");
      variable1 = pop();
      fprintf(out, "%d\n", variable1);
      break;

      case 0xFF:
      printf("%s\n","HALT");
      control =false;
      break;

      case 0x59:
      printf("%s\n","DUP");
      variable1= top();
      push(variable1);
      break;

      case 0x99: // Not working bc offset wrong, wrong place, tos not correct
      s_var1= convert();
      printf("IFEQ %02X\n",s_var1);
      variable2= pop();
      if(variable2 == 0) {
        counter = counter + s_var1 - 3;
      }
      break;

      case 0x9B:
      printf("%s\n","IFLT");
      variable1 = pop();
      s_var1 = convert();
      if(variable1 < 0) {
        counter = counter + s_var1 - 3;
      }
      break;

      case 0x9F:
      printf("%s\n","IF_ICMPEQ");
      variable1 = pop();
      variable2 = pop();
      s_var1 = convert();
      if(variable1 == variable2) {
        counter = counter + s_var1 - 3;
      }
      break;

      case 0x13:
      printf("%s\n","LDC_W");
      s_var1 = convert();
      word_t w = constant_pool_data[s_var1];
      w = swap_uint32(w);
      push(w);
      break;

      case 0x15:
      printf("%s\n","ILOAD");
      variable1 = get_number();
      push(stack.local_variable[variable1]);
      break;

      case 0x36:
      printf("%s\n","ISTORE");
      variable1 = get_number();
      variable2 = pop();
      stack.local_variable[variable1]= variable2;
      break;

      case 0x00:
      printf("%s\n","NOP");
      break;

      case 0xFE:
      printf("%s\n","Error: Simulator will now stop" );
      control =false;
      break;

      default:
      printf("%s", "VARIABLE ");
      printf("%x\n", text[counter]);
  }
  counter++;
  return true;
}

int text_size(void) {
  return text_sz;
}
