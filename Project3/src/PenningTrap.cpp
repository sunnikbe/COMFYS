#include "PenningTrap.hpp"
#include "Particle.hpp"

// Constructor
PenningTrap::PenningTrap(double B0_in, double V0_in, double d_in)
{
  B0_ = B0_in;
  V0_ = V0_in;
  d_ = d_in;
}

// Methods returning member variable values
// Add a new particle to the Penning trap
void PenningTrap::add_particle(Particle particle_in)
{
  particles_.push_back(particle_in);
}

// External electric field at point r
arma::vec PenningTrap::external_E_field(arma::vec r)
{
  arma::vec tmp = {1,1,-2};
  return V0_/(d_*d_)*r%tmp;
}

// External magneticfield at point r
arma::vec PenningTrap::external_B_field(arma::vec v)
{
  arma::vec B = {0,0,B0_};
  return arma::cross(v,B);
}

// force on particle i from particle j
arma::vec PenningTrap::force_particle(int i, int j)
{
  double k_e = 1.38935333*std::pow(10,5);
  double q_j = particles_.at(j).charge();
  arma::vec r_i = particles_.at(i).position();
  arma::vec r_j = particles_.at(j).position();
  return k_e*q_j*(r_i-r_j)/std::pow(norm(r_i-r_j),3);
}

// total force on particle i from external forces
arma::vec PenningTrap::total_force_external(int i)
{
  double q_i = particles_.at(i).charge();
  arma::vec r_i = particles_.at(i).position();
  arma::vec v_i = particles_.at(i).velocity();
  return q_i*external_E_field(r_i) + q_i*external_B_field(v_i);
}

// total force on particle i from other particles
arma::vec PenningTrap::total_force_particle(int i)
{
  arma::vec F_particle = {0,0,0};
  // need more work
  for (int j = 0; j < particles_.size(); j++)
  {
    if (j != i)
    {
      F_particle += force_particle(i,j);
    }
  }
  return F_particle;
}

// Total force on particle i
arma::vec PenningTrap::total_force(int i)
{
  return total_force_external(i) + total_force_particle(i);
}
