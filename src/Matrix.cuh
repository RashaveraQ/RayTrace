#pragma once

struct	sp;

#ifdef __CUDACC__
#define TARGET __device__ __host__
#else
#define TARGET
#endif // __CUDACC__

struct matrix
{
private:
	int		m_Width;
	int		m_Height;
	float	m_data[4][4];
	bool	m_is_enabled_idata;
	float	m_idata[4][4];
public:
	TARGET matrix(int = 4, int = 4);
	TARGET matrix(const matrix&);
	TARGET matrix(const sp&, float d = 1.0);
	TARGET matrix(int, int, float data[4][4]);
	TARGET virtual ~matrix();
	TARGET int	get_width() const { return m_Width; }
	TARGET int	get_height() const { return m_Height; }
	TARGET matrix	operator=(const matrix&);
	TARGET matrix	operator+(const matrix&) const;
	TARGET matrix	operator-(const matrix&) const;
	TARGET matrix	operator*(const matrix&) const;
	TARGET matrix	operator*(float) const;
	TARGET matrix	operator/(float) const;
	TARGET float	d() const;			// 行列式
	TARGET float	M(int, int) const;	// 小行列式
	TARGET matrix	Inv();				// 逆行列
	//void	print() const;
	TARGET float get_data(int gyo, int retu) const { return m_data[gyo - 1][retu - 1]; }
	TARGET void set_data(int gyo, int retu, float value);
};

#undef TARGET

