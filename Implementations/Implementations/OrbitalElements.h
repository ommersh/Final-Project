class OrbitalElements {
public:
    double a;       // semi-major axis (AU)
    double e;       // eccentricity
    double i;       // inclination (degrees)
    double omega;   // longitude of ascending node (degrees)
    double w;       // argument of perihelion (degrees)
    double M0;      // mean anomaly at epoch (degrees)
    double epoch;   // epoch (Julian Date)
    double n;       // mean motion (degrees/day)
    double tp;      // time of perihelion passage (Julian Date)

    OrbitalElements(double a_, double e_, double i_, double omega_, double w_, double M0_, double epoch_, double n_, double tp_)
        : a(a_), e(e_), i(i_), omega(omega_), w(w_), M0(M0_), epoch(epoch_), n(n_), tp(tp_) {}
};
