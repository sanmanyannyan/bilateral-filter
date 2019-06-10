#include <cassert>
#include <cmath>

#include "bilateral-filter.h"


bilateralFilterF32::bilateralFilterF32() : m_kernel_size(3), m_sigma_color(1.f), m_sigma_space(1.f), kernel_center(1) {
	size_t allocation_kernel_size = static_cast<size_t>(m_kernel_size);
	exp_weights = new float[allocation_kernel_size * allocation_kernel_size];
	compute_exp_weight();
}

bilateralFilterF32::bilateralFilterF32(int32_t kernel_size, float sigma_color, float sigma_space) : m_kernel_size(kernel_size), m_sigma_color(sigma_color), m_sigma_space(sigma_space), kernel_center((kernel_size - 1) / 2) {
	size_t allocation_kernel_size = static_cast<size_t>(m_kernel_size);
	exp_weights = new float[allocation_kernel_size * allocation_kernel_size];
	compute_exp_weight();
}

void bilateralFilterF32::set_kernel_size(int32_t kernel_size) {
	assert(kernel_size > 0);
	if(m_kernel_size < kernel_size){
		delete[] exp_weights;
		size_t allocation_kernel_size = static_cast<size_t>(m_kernel_size);
		exp_weights = new float[allocation_kernel_size * allocation_kernel_size];
	}
	m_kernel_size = kernel_size;
	kernel_center = kernel_size >> 1;
	compute_exp_weight();
}

void bilateralFilterF32::set_sigma_color(float sigma_color) {
	assert(sigma_color != 0.f);
	m_sigma_color = sigma_color;
	compute_exp_weight();
}

void bilateralFilterF32::set_sigma_space(float sigma_space) {
	assert(sigma_space != 0.f);
	m_sigma_space = sigma_space;
}

int32_t bilateralFilterF32::get_kernel_size() {
	return m_kernel_size;
}

float bilateralFilterF32::get_sigma_color() {
	return m_sigma_color;
}

float bilateralFilterF32::get_sigma_space() {
	return m_sigma_space;
}

void bilateralFilterF32::filter(float *dst, const float* src, int32_t width, int32_t height, int32_t y_disp) {
	const float pixel_exp_denom = -2.f * m_sigma_space * m_sigma_space;
	for (int32_t y = 0 ; y < height ; ++y){
		for(int32_t x = 0 ; x < width ; ++x){
			float weight_sum = 0.f;
			float pixel_val = 0.f;
			const float center_val = src[y * y_disp + x];
			for (int32_t ky = -kernel_center ; ky < m_kernel_size - kernel_center ; ++ky) {
				for (int32_t kx = -kernel_center ; kx < m_kernel_size - kernel_center ; ++kx) {
					const float ref_pixel_val = src[(y + ky) * y_disp + x + kx];
					const float sqre_pixel_dif = (ref_pixel_val - center_val) * (ref_pixel_val - center_val);
					const float weight = exp_weights[(ky + kernel_center) * m_kernel_size + kx + kernel_center]
						           * std::exp(sqre_pixel_dif * pixel_exp_denom);
					pixel_val += weight * ref_pixel_val;
					weight_sum += weight;
				}
			}
			dst[y * y_disp + x] = pixel_val / weight_sum;
		}
	}	
}

void bilateralFilterF32::compute_exp_weight(){
	const float rcp_exp_arg_denom = -1.f / 2. * m_sigma_color * m_sigma_color;
	int32_t kernel_idx = 0;
	for(int32_t y = -kernel_center; y < m_kernel_size - kernel_center; ++y) {
		for(int32_t x = -kernel_center; x < m_kernel_size - kernel_center; ++x) {
			float exp_arg = (x * x + y * y) * rcp_exp_arg_denom;
			exp_weights[kernel_idx++] = std::exp(exp_arg);
		}
	}
	assert(kernel_idx == m_kernel_size * m_kernel_size);
}

