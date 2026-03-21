#include <Fourier.h>
//#include <Wavedisplay.h>
//#include <Appsymbols.h>

void exit_proc( int )
{
	exit(0);
}
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
//typedef std::complex<double> Complex;
//typedef std::vector<Complex> CArray;
void fft(cd_vec_t& x) {
   int N = x.size();
   if (N <= 1) return;
   // Split into even and odd elements
   cd_vec_t even(N / 2), odd(N / 2);
   for (int i = 0; i < N / 2; ++i) {
       even[i] = x[i * 2];
       odd[i] = x[i * 2 + 1];
   }
   // Recursive FFT calls
   fft(even);
   fft(odd);
   // Combine results
   for (int k = 0; k < N / 2; ++k) {
       cd_t t = std::polar(1.0, -2 * M_PI * k / N) * odd[k];
       x[k] = even[k] + t;
       x[k + N / 2] = even[k] - t;
   }
}
int main() {
	cd_vec_t data = {1, 2, 3, 4};
   fft(data);
   for (auto& c : data)
       std::cout << c << "\n";
}
