//#include <>

struct Stack {
  int size;
  int data[1024];
  int local_variable[255];
  int constant[255];
};

struct Stack stack;

void push(uint32_t value) {
  stack.data[stack.size] = value;
  stack.size++;
}
uint32_t top() {
  return stack.data[stack.size-1];
}

uint32_t pop() {
  uint32_t x = top();
  stack.size--;
  return x;
}



int size() {
  return stack.size;
}
