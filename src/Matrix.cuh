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
	__device__ Matrix(int = 4, int = 4);
	__device__ Matrix(const Matrix&);
	__device__ Matrix(const Sp&, float d = 1.0);
	__device__ virtual ~Matrix();
	__device__ int	get_width() const { return m_Width; }
	__device__ int	get_height() const { return m_Height; }
	__device__ Matrix	operator=(const Matrix&);
	__device__ Matrix	operator+(const Matrix&) const;
	__device__ Matrix	operator-(const Matrix&) const;
	__device__ Matrix	operator*(const Matrix&) const;
	__device__ Matrix	operator*(float) const;
	__device__ Matrix	operator/(float) const;
	__device__ float	d() const;			// �s��
	__device__ float	M(int, int) const;	// ���s��
	__device__ Matrix	Inv(void);				// �t�s��
	//void	print() const;
	__device__ double get_data(int gyo, int retu) const { return m_data[gyo - 1][retu - 1]; }
	__device__ void set_data(int gyo, int retu, float value);
};

