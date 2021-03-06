	
#include <iostream>
#include <cstdint>
#include <cstdlib>

extern "C" {
  void copy_asm( uint32_t const * i_a,
                 uint64_t       * o_b );
  void copy_c( uint32_t const * i_a,
               uint64_t       * o_b );
}

int main() {
  uint32_t l_a[7] = { 1, 21, 43, 78, 89, 91, 93 };
  uint64_t l_b[7] = { 0 };



  copy_asm( l_a,
            l_b );

for(int i = 0; i<7; i++){
	std::cout << l_a[i] << "/" << l_b[i] << std::endl;
}


  copy_c( l_a,
           l_b );

for(int i = 0; i<7; i++){
        std::cout << l_a[i] << "/" << l_b[i] << std::endl;
}

  delete(l_a);
  delete(l_b);


  return EXIT_SUCCESS;
}
