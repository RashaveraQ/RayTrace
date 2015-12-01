#ifndef __MATRIX_H
#define __MATRIX_H

struct	sp;

struct matrix
{
private:
	int		m_Width;
	int		m_Height;
	double	m_data[4][4];
	matrix* m_pInvMatrix;
public:
	matrix(int=4,int=4);
	matrix(const matrix&);
	matrix(const sp&, double d = 1.0);
	int	get_width() const { return m_Width; }
	int	get_height() const { return m_Height; }
	matrix	operator=(const matrix&);
	matrix	operator+(const matrix&) const;
	matrix	operator-(const matrix&) const;
	matrix	operator*(const matrix&) const;
	matrix	operator*(double) const;
	matrix	operator/(double) const;
	double	d() const;			// s—ñ®
	double	M(int,int) const;	// ¬s—ñ®
	matrix	Inv();				// ‹ts—ñ
	void	print() const;
	//double* operator[](int x) { return m_data[x]; }
	double get_data(int gyo, int retu) const { return m_data[gyo - 1][retu - 1]; }
	void set_data(int gyo, int retu, double value);
};

#endif
