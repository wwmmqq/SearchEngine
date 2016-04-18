#include <cmath>


double BM25OneTokenScore(unsigned tf, unsigned int D_N, double avgdl)
{
	double score = 0.0;

	double wi = log( (D_N-tf+0.5) / (tf+0.5) );
	double k1 = 1.2; //2.0
	double b = 0.75;
	double ri =  tf*(k1 + 1) / (k1 + k1*(1 - b + b * D_N/avgdl));
	score = wi * ri;
	return score;
}

