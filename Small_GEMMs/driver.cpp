#include "gemm_compiler_32_32_32_32_32_32.hpp"
#include "gemm_ref.hpp"
#include <chrono>
#include <iostream>
#include <libxsmm.h>
#include <numeric>
#include <string>


double measure_GFLOPs(double seconds, unsigned int l_m, unsigned int l_n,
                      unsigned int l_k, unsigned int l_count) {

  return (double)(2 * l_k * l_m * l_n * (double)l_count) / (seconds * 1000000000);
}


void comparison_dimensions(){  
  // comparison gemm_compiler_mnk and gemm_compiler_nkm
  using namespace std::chrono;
  unsigned int l_count = 50000;

  std::vector<float> l_a(32 * 32);
  std::vector<float> l_b(32 * 32);
  std::vector<float> l_c(32 * 32);

  // to increase use iota
  std::fill(l_a.begin(), l_a.end(), 1);
  std::fill(l_b.begin(), l_b.end(), 1);
  std::fill(l_c.begin(), l_c.end(), 0);

  auto start = system_clock::now();

  for (size_t i = 0; i < l_count; i++) {
    gemm_compiler_32_32_32_32_32_32_mnk(l_a.data(), l_b.data(), l_c.data());
  }

  auto end = system_clock::now();
  double seconds = duration<double>(end - start).count();

  std::cout << "gemm_compiler_mnk: "
            << measure_GFLOPs(seconds, 32, 32, 32, l_count) << "\t GFLOP's"
            << '\n'
            << '\n';

  // to increase use iota
  std::fill(l_a.begin(), l_a.end(), 1);
  std::fill(l_b.begin(), l_b.end(), 1);
  std::fill(l_c.begin(), l_c.end(), 0);

  start = system_clock::now();

  for (size_t i = 0; i < l_count; i++) {
    gemm_compiler_32_32_32_32_32_32_nkm(l_a.data(), l_b.data(), l_c.data());
  }

  end = system_clock::now();
  seconds = duration<double>(end - start).count();

  std::cout << "gemm_compiler_nkm: "
            << measure_GFLOPs(seconds, 32, 32, 32, l_count) << "\t GFLOP's"
            << '\n'
            << '\n';

  }
  
int main(int argc, char const *argv[]) {

  std::vector<unsigned int> l_lambda = {4, 8, 12, 16, 24, 32, 48, 64};

  //comparison_dimensions();

  // Performance test of gemm_reff with diffrent matrix sizes
  for (size_t i = 0; i < l_lambda.size(); i++) {

    unsigned int l_m = l_lambda[i];
    unsigned int l_n = l_lambda[i];
    unsigned int l_k = l_lambda[i];

    // To simplify things we assumed that the leading dimension was equal to the
    // number of columns.
    unsigned int l_lda = l_m;
    unsigned int l_ldb = l_k;
    unsigned int l_ldc = l_m;

    std::vector<float> l_a(l_m * l_k);
    std::vector<float> l_b(l_n * l_k);
    std::vector<float> l_c(l_m * l_n);

    // to increase use iota
    std::fill(l_a.begin(), l_a.end(), 1);
    std::fill(l_b.begin(), l_b.end(), 1);
    std::fill(l_c.begin(), l_c.end(), 0);

    unsigned int l_count = 1500000000 / (l_m * l_n * l_k);

    using namespace std::chrono;

    auto start = system_clock::now();

    for (size_t i = 0; i < l_count; i++) {
      gemm_ref(l_a.data(), l_b.data(), l_c.data(), l_m, l_n, l_k, l_lda, l_ldb,
               l_ldc);
    }

    auto end = system_clock::now();

    double seconds = duration<double>(end - start).count();

    std::cout << "Lambda:" << l_lambda[i] << '\n';
    std::cout << "Time:" << seconds << '\n';
    std::cout << "GFLOP's: " << measure_GFLOPs(seconds, l_m, l_k, l_n, l_count)
              << '\n'
              << '\n';
  }

  std::cout<< "using the libxsmm kernel:" << '\n';

  //Benchmark the Libxsmm library with different matrix sizes

  for (size_t i = 0; i < l_lambda.size(); i++) {

    unsigned int l_m = l_lambda[i];
    unsigned int l_n = l_lambda[i];
    unsigned int l_k = l_lambda[i];

    // To simplify things we assumed that the leading dimension was equal to the
    // number of columns.
    unsigned int l_lda = l_m;
    unsigned int l_ldb = l_k;
    unsigned int l_ldc = l_m;

    std::vector<float> l_a(l_m * l_k);
    std::vector<float> l_b(l_n * l_k);
    std::vector<float> l_c(l_m * l_n);

    // to increase use iota
    std::fill(l_a.begin(), l_a.end(), 1);
    std::fill(l_b.begin(), l_b.end(), 1);
    std::fill(l_c.begin(), l_c.end(), 0);

    unsigned int l_count = 15000000000 / (l_m * l_n * l_k);

    using namespace std::chrono;


    libxsmm_mmfunction<float, float> kernel(LIBXSMM_GEMM_FLAG_NONE, l_m, l_n, l_k, 1.0, 1.0);

    assert(kernel);

    auto start = system_clock::now();

    /* kernel multiplies and accumulates matrices: C += Ai * Bi */
    for (size_t i = 0; i < l_count; ++i) {
    
    //kernel(&l_a[l_m * l_k], &l_b[l_k * l_n], &l_c[0]);
    kernel(l_a.data(), l_b.data(), l_c.data());

    }

    auto end = system_clock::now();

    double seconds = duration<double>(end - start).count();

    std::cout << "Lambda:" << l_lambda[i] << '\n';
    std::cout << "Time:" << seconds << '\n';
    std::cout << "GFLOP's: " << measure_GFLOPs(seconds, l_m, l_k, l_n, l_count)
              << '\n'
              << '\n';
  }

  return 0;
}
