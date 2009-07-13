#include <stdlib.h>
#include <math.h>

#define ENOUGH_SMALL 0.00001

static float func(int d, float *k, float x)
{
	int		i;
	float	ans = 0;

	for ( i = 0; i <= d; i++ ) ans += k[i] * pow(x,i);

	return ans;
}

static int compare( const void *arg1, const void *arg2 )
{
	float *p1, *p2;

	p1 = (float*)arg1;
	p2 = (float*)arg2;

	if ( *p1 == *p2 ) return 0;
	else if ( *p1 > *p2 ) return 1;
	else return -1;
}

static float walk(int d, float *k, float min, float max)
{
	float a, b, f, g, x, y;

	long l = 0;

	if ( (f = func(d,k,max)) == 0.0 ) return max;
	if ( (g = func(d,k,min)) == 0.0 ) return min;

	if ( f * g > 0 ) {
		//MessageBox ( NULL, "Error!", "Solve_Polynomial", MB_OK );
		return 0;
//		exit(0);
	}

	a = min, b = max;
	while (1) {
		if ( fabs( a - b ) < ENOUGH_SMALL )
			break;

		x = (a + b)/ 2;	// CPU error!?
		y = func(d,k,x);

		if ( f > 0 ) {
			if ( func(d,k,b) - y < ENOUGH_SMALL )
				break;
			
			if ( y < 0 )
				a = x;
			else
				b = x;
		} else {
			if ( func(d,k,a) - y < ENOUGH_SMALL )
				break;

			if ( y > 0 )
				a = x;
			else
				b = x;
		}

		if (l++ > 100)
			break;			// for CPU bug
	}
	return x;
}

int Solve_Polynomial(int d, float *k, float min, float max, float *r)
{
	int		i, j, N, ans;
	float	D;
//	float	*K, *R, *y;

	switch ( d )
	{
	case 1:
		if ( k[1] == 0.0 )
			return 0;

		r[0] = - k[0] / k[1];
		return ( min <= r[0] && r[0] <= max ) ? 1 : 0;

	case 2:
		if ( k[2] == 0.0 )
		{
			if ( k[1] == 0.0 ) return 0;

			r[0] = - k[0] / k[1];
			return ( min <= r[0] && r[0] <= max ) ? 1 : 0;
		}

		D = k[1] * k[1] - 4 * k[2] * k[0];
		
		if ( D == 0.0 )
		{
			r[0] = - k[1] / (2.0 * k[2]);
			return ( min <= r[0] && r[0] <= max ) ? 1 : 0;
		}
		else if ( D > 0 )
		{
			r[0] = ( -k[1] + sqrt(D) ) / ( 2.0 * k[2] );
			r[1] = ( -k[1] - sqrt(D) ) / ( 2.0 * k[2] );
			if ( r[0] > r[1] )
				D = r[0], r[0] = r[1], r[1] = D;
			i = (( min <= r[0] && r[0] <= max ) ? 1 : 0 ) + (( min <= r[1] && r[1] <= max ) ? 1 : 0 );
			return i;
		}
		return 0;
	}

//	K = new float [2*d];
//	R = new float [2*d+4];
//	y = new float [2*d+4];
#define MAX_D 4
	float K[2 * MAX_D];
	float R[2 * MAX_D + 4];
	float y[2 * MAX_D + 4];
		
	for ( i = 1; i <= d; i++ )
		K[i-1] = i * k[i] / k[d] / d;

	// 極値の個数を求める。
	N = Solve_Polynomial( d-1, K, min, max, R+1 );
	R[0] = min, R[N+1] = max;

	for ( i = 0; i <= N+1; i++ )
		y[i] = func(d,k,R[i]);

	for ( ans = i = 0; i <= N; i++ )
	{
		// 解が存在しない区間の場合、次へ。
		if ( y[i] * y[i+1] > 0 ) continue;

		r[ans++] = walk(d,k,R[i],R[i+1]);
	}

	qsort((void*)r, (size_t)ans, sizeof( float ), compare );

	for ( i = 0; i < ans - 1; i++ )
	{
		if ( r[i+1] - r[i] <= ENOUGH_SMALL )
		{
			for ( j = i; j < ans - 1; j++ )
				r[j] = r[j+1];
			i--;
			ans--;
		}
	}

//	delete K;
//	delete R;
//	delete y;

	return ans;
}