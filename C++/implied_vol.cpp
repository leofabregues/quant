#include <iostream>
#include <cmath>
#include <iomanip>

// On réutilise notre logique de base
double normal_pdf(double x) {
    return (1.0 / std::sqrt(2.0 * M_PI)) * std::exp(-0.5 * x * x);
}

double normal_cdf(double x) {
    return 0.5 * std::erfc(-x * std::sqrt(0.5));
}

class ImpliedVolEngine {
public:
    double S, K, r, T, market_price;

    ImpliedVolEngine(double _S, double _K, double _r, double _T, double _mp)
        : S(_S), K(_K), r(_r), T(_T), market_price(_mp) {}

    // Calcul du prix et du Vega pour une volatilité donnée
    double bs_price(double sigma) const {
        double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
        double d2 = d1 - sigma * std::sqrt(T);
        return S * normal_cdf(d1) - K * std::exp(-r * T) * normal_cdf(d2);
    }

    double bs_vega(double sigma) const {
        double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
        return S * std::sqrt(T) * normal_pdf(d1);
    }

    // L'algorithme de Newton-Raphson
    double find_vol() {
        double sigma = 0.5; // Première estimation (50%)
        double epsilon = 1e-6; // Précision voulue
        int max_iter = 100;

        for (int i = 0; i < max_iter; ++i) {
            double price = bs_price(sigma);
            double vega = bs_vega(sigma);
            double diff = price - market_price;

            if (std::abs(diff) < epsilon) return sigma; // On a trouvé !

            sigma = sigma - diff / vega; // On ajuste
        }
        return sigma;
    }
};

int main() {
    // Exemple : Si l'action est à 100, le strike à 100, et que le Call vaut 10€ sur le marché
    ImpliedVolEngine engine(100.0, 100.0, 0.05, 1.0, 10.0);
    
    double vol = engine.find_vol();
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "La volatilite implicite est de : " << vol * 100 << "%" << std::endl;

    return 0;
}