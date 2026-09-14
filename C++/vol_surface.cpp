#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <iomanip>

// --- OUTILS MATHÉMATIQUES ---
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

double normal_pdf(double x) {
    return (1.0 / std::sqrt(2.0 * M_PI)) * std::exp(-0.5 * x * x);
}

double normal_cdf(double x) {
    return 0.5 * std::erfc(-x * std::sqrt(0.5));
}

// --- MOTEUR DE VOLATILITÉ ---
class VolSurfaceEngine {
private:
    double S, r;

public:
    VolSurfaceEngine(double _S, double _r) : S(_S), r(_r) {}

    // Calcul du prix théorique (Black-Scholes)
    double bs_price(double K, double T, double sigma) const {
        if (T <= 0) return std::max(S - K, 0.0);
        double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
        double d2 = d1 - sigma * std::sqrt(T);
        return S * normal_cdf(d1) - K * std::exp(-r * T) * normal_cdf(d2);
    }

    // Calcul du Vega (dérivée par rapport à sigma)
    double bs_vega(double K, double T, double sigma) const {
        double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
        return S * std::sqrt(T) * normal_pdf(d1);
    }

    // Algorithme de Newton-Raphson pour trouver la Vol Implicite
    double find_implied_vol(double K, double T, double market_price) const {
        double sigma = 0.3; // Estimation initiale (30%)
        double epsilon = 1e-7;
        
        for (int i = 0; i < 100; ++i) {
            double price = bs_price(K, T, sigma);
            double vega = bs_vega(K, T, sigma);
            double diff = price - market_price;

            if (std::abs(diff) < epsilon) return sigma;
            if (std::abs(vega) < 1e-10) break; // Évite la division par zéro

            sigma = sigma - diff / vega;
            if (sigma <= 0) sigma = 0.0001; // La vol ne peut pas être négative
        }
        return sigma;
    }
};

int main() {
    double spot = 100.0;
    double rate = 0.02; // Taux 2%

    VolSurfaceEngine engine(spot, rate);

    // Création du fichier CSV
    std::ofstream file("vol_surface.csv");
    file << "Strike,Maturity,ImpliedVol" << std::endl;

    std::cout << "Calcul de la surface en cours..." << std::endl;

    // AXE 1 : Les Strikes (de 80 a 120)
    for (double K = 80; K <= 120; K += 2) {
        // AXE 2 : Les Maturités (de 0.1 a 2.0 ans)
        for (double T = 0.2; T <= 2.0; T += 0.2) {
            
            /* SCÉNARIO RÉEL : Normalement, tu as ici le vrai prix du marché.
               Ici, on simule un "Smile de volatilité" pour que ta surface 
               soit jolie dans Excel : on simule un prix avec une vol plus 
               élevée pour les options en dehors de la monnaie (OTM).
            */
            double real_vol = 0.2 + 0.5 * std::pow((K - spot) / spot, 2) + 0.05 * T;
            double market_price = engine.bs_price(K, T, real_vol);

            // On demande à notre moteur de retrouver cette vol
            double iv = engine.find_implied_vol(K, T, market_price);

            file << K << "," << T << "," << iv << std::endl;
        }
    }

    file.close();
    std::cout << "Termine ! Fichier 'vol_surface.csv' genere." << std::endl;

    return 0;
}