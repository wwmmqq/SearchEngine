#include "BM25.h"
#include <cmath>


double BM25_W(int tf, int D_N)
{
	return log( (D_N-tf+0.5) / (tf+0.5) );
}

double BM25_R(int tf, int D_N, double avgdl)
{
	double k1 = 1.2; //2.0
	double b = 0.75;
	return tf*(k1 + 1) / (k1 + k1*(1 - b + b * D_N/avgdl))
}

double BM25OneTokenScore(std::string token)
{
	////
	//double wi = BM25_W(tf, D_N);
	//double ri = BM25_R(tf, D_N, avgdl);

	//return wi*ri;
	return 0.0;
}

double score(std::string &q)
{
	double result = 0.0;

	std::vector<std::string> token;
	// todo 

	n = token.size();
	for(int i = 0; i < n; ++i) {
		int tf = 0;
		int D_N = 0;
		double avgdl = 1.0;

		double wi = BM25_W(tf, D_N);
		double ri = BM25_R(tf, D_N, avgdl);
		result += wi*ri;
	}
	return result;
}