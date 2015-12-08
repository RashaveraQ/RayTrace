#ifndef __MATRIX_H
#define __MATRIX_H

struct	sp;

struct matrix
{
private:
	int		m_Width;
	int		m_Height;
	float	m_data[4][4];
	matrix* m_pInvMatrix;
public:
	matrix(int=4,int=4);
	matrix(const matrix&);
	matrix(const sp&, float d = 1.0f);
	virtual ~matrix();
	int	get_width() const { return m_Width; }
	int	get_height() const { return m_Height; }
	matrix	operator=(const matrix&);
	matrix	operator+(const matrix&) const;
	matrix	operator-(const matrix&) const;
	matrix	operator*(const matrix&) const;
	matrix	operator*(float) const;
	matrix	operator/(float) const;
	float	d() const;			// s—ñ®
	float	M(int,int) const;	// ¬s—ñ®
	matrix	Inv();				// ‹ts—ñ
	void	print() const;
	float get_data(int gyo, int retu) const { return m_data[gyo - 1][retu - 1]; }
	void set_data(int gyo, int retu, float value);
};

#endif
