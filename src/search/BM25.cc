#include <cmath>


double BM25OneTokenScore(unsigned tf, unsigned int n_qi, unsigned int dl, double avgdl)
{
	unsigned int total_doc = 101712;
	double score = 0.0;

	double k1 = 1.2; //2.0
	double b = 0.75;
	double wi = log( (total_doc - n_qi + 0.5) / (n_qi + 0.5));
	double ri =  tf*(k1 + 1) / (tf + k1*(1 - b + b * dl/avgdl));
	score = wi * ri;
	return score;
}
//res += (K1 + 1) * fij / (K1 * ((1 - b) + b * doclen.get(doc) / avglen) + fij) * math.log((N + 0.5) / (wordindata.get(word, 0) + 0.5))
