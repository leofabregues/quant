#include <iostream>
#include <cmath>     // Pour les fonctions mathématiques : exp, log, sqrt
#include <algorithm> // Pour std::max
#include <iomanip>   // Pour un affichage propre des prix (2 ou 4 décimales)

/**
 * FONCTION DE RÉPARTITION DE LA LOI NORMALE N(x)
 * C'est la base de Black-Scholes. Elle calcule la probabilité qu'une 
 * variable aléatoire normale soit inférieure à x.
 */
double normal_cdf(double x) {
    // Utilisation de la fonction d'erreur complémentaire (erfc) pour la précision
    return 0.5 * std::erfc(-x * std::sqrt(1/0.5));
}

/**
 * CLASSE EUROPEAN_OPTION
 * On utilise la Programmation Orientée Objet (POO) pour encapsuler les données.
 */
class EuropeanOption {
private:
    // Paramètres du modèle (Mnémonique : S, K, r, v, T)
    double S; // Prix actuel de l'action (Spot)
    double K; // Prix d'exercice (Strike)
    double r; // Taux d'intérêt sans risque (ex: 0.03 pour 3%)
    double v; // Volatilité du sous-jacent (ex: 0.20 pour 20%)
    double T; // Temps restant jusqu'à l'échéance (en années)

    // d1 et d2 sont des variables intermédiaires de la formule de Black-Scholes
    double d1() const {
        return (std::log(S / K) + (r + 0.5 * v * v) * T) / (v * std::sqrt(T));
    }

    double d2() const {
        return d1() - v * std::sqrt(T);
    }

public:
    // Constructeur : permet de créer l'option avec ses paramètres
    EuropeanOption(double _S, double _K, double _r, double _v, double _T) 
        : S(_S), K(_K), r(_r), v(_v), T(_T) {}

    // --- CALCUL DES PRIX ---

    double price_call() const {
        return S * normal_cdf(d1()) - K * std::exp(-r * T) * normal_cdf(d2());
    }

    double price_put() const {
        // On utilise la parité Call-Put ou la formule directe
        return K * std::exp(-r * T) * normal_cdf(-d2()) - S * normal_cdf(-d1());
    }

    // --- CALCUL DES GRECQUES (Indispensable pour un stage !) ---

    // Le Delta mesure combien le prix de l'option bouge si l'action bouge de 1€
    double delta_call() const {
        return normal_cdf(d1());
    }

    double delta_put() const {
        return normal_cdf(d1()) - 1.0;
    }
};

int main() {
    // 1. Initialisation des paramètres
    // Imaginons une option sur l'action LVMH ou Apple
    double spot = 100.0;   
    double strike = 105.0; 
    double rate = 0.03;    // 3%
    double vol = 0.25;     // 25%
    double time = 1.0;     // 1 an

    // 2. Création de l'objet
    EuropeanOption option(spot, strike, rate, vol, time);

    // 3. Affichage des résultats dans la console
    std::cout << "========================================" << std::endl;
    std::cout << "   MOTEUR DE VALORISATION QUANTITATIVE  " << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Configuration de l'affichage (2 chiffres après la virgule)
    std::cout << std::fixed << std::setprecision(2);
    
    std::cout << "Prix du Call : " << option.price_call() << " EUR" << std::endl;
    std::cout << "Delta du Call: " << option.delta_call() << " (Sensibilite)" << std::endl;
    
    std::cout << "----------------------------------------" << std::endl;
    
    std::cout << "Prix du Put  : " << option.price_put() << " EUR" << std::endl;
    std::cout << "Delta du Put : " << option.delta_put() << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;

}