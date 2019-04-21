#include <cstdint>

class bilateralFilterF32 {
	int32_t m_kernel_size;
	float m_sigma_color;
	float m_sigma_space;
	float* exp_weights;
	int32_t kernel_center;

	void compute_exp_weight();

public:
	bilateralFilterF32();
	bilateralFilterF32(int32_t kernel_size, float sigma_color, float sigma_space);
	
	void set_kernel_size(int32_t kernel_size);
	void set_sigma_color(float sigma_color);
	void set_sigma_space(float sigma_space);
	int32_t get_kernel_size();
	float get_sigma_color();
	float get_sigma_space();
	void filter(float *dst, const float* src, int32_t width, int32_t height, int32_t y_disp);
};

