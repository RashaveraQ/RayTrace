#pragma once

struct	Sp;

struct Matrix
{
private:
	int		m_Width;
	int		m_Height;
	float	m_data[4][4];
	Matrix* m_pInvMatrix;
public:
	__device__ __host__ Matrix(int = 4, int = 4);
	__device__ __host__ Matrix(const Matrix&);
	__device__ __host__ Matrix(const Sp&, float d = 1.0);
	__device__ __host__ Matrix(int, int, float data[4][4]);
	__device__ __host__ virtual ~Matrix();
	__device__ __host__ int	get_width() const { return m_Width; }
	__device__ __host__ int	get_height() const { return m_Height; }
	__device__ __host__ Matrix	operator=(const Matrix&);
	__device__ __host__ Matrix	operator+(const Matrix&) const;
	__device__ __host__ Matrix	operator-(const Matrix&) const;
	__device__ __host__ Matrix	operator*(const Matrix&) const;
	__device__ __host__ Matrix	operator*(float) const;
	__device__ __host__ Matrix	operator/(float) const;
	__device__ __host__ float	d() const;			// çsóÒéÆ
	__device__ __host__ float	M(int, int) const;	// è¨çsóÒéÆ
	__device__ __host__ Matrix	Inv(void);				// ãtçsóÒ
	//void	print() const;
	__device__ __host__ double get_data(int gyo, int retu) const { return m_data[gyo - 1][retu - 1]; }
	__device__ __host__ void set_data(int gyo, int retu, float value);
};

