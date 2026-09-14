#include <iostream>
#include <vector>
#include <random> // Pour les nombres aléatoires
#include <cmath>

int main() {
    double S = 100.0;    // Prix initial
    double mu = 0.05;    // Rendement attendu (5%)
    double sigma = 0.20; // Volatilité (20%)
    double dt = 1.0/252.0; // Un pas de temps (1 jour)
    int days = 252;

    // Générateur de nombres aléatoires (Loi Normale Standard N(0,1))
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    std::cout << "Simulation du prix sur 252 jours..." << std::endl;

    for (int i = 0; i < days; ++i) {
        // La formule de la marche aléatoire
        double B = dist(gen);
        S = S * std::exp((mu - 0.5 * sigma * sigma) * dt + sigma * std::sqrt(dt) * B);

        if (i % 20 == 0) { // On affiche le prix tous les 20 jours
            std::cout << "Jour " << i << " : " << S << " EUR" << std::endl;
        }
    }

    return 0;
}