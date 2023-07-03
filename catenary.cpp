#include <vector>
#include <stddef.h>
#include <cmath>
#include <iostream>
#include <random>
#include <fstream>



using scalar = double;



struct Vec2D
{
    scalar x;
    scalar y;
};


scalar norm(const Vec2D& v){
    return (v.x * v.x + v.y * v.y);
}


scalar getResidual(const std::vector<Vec2D>& forces){
    
    scalar res = 0;

    for (size_t i = 1; i < forces.size() - 1; ++i){
        res += norm(forces[i]);
    }

    return res;
}


void updateForces(const std::vector<Vec2D>& positions,
                  const std::vector<scalar>& mass,
                  const scalar K,
                  const scalar gravity,
                  std::vector<Vec2D>& forces){

    for (int i = 1; i < forces.size() - 1; ++i){
        forces[i].x = (positions[i - 1].x - 2 *positions[i].x  + positions[i + 1].x ) * K;

        forces[i].y = (positions[i - 1].y - 2*positions[i].y  + positions[i + 1].y ) * K - mass[i] * gravity;

    }
              
}

std::vector<Vec2D> getDerivatives(const std::vector<Vec2D>& positions,
                  const std::vector<scalar>& mass,
                  const scalar K,
                  const scalar gravity) {

std::vector<Vec2D> derivatives;

 for (int i = 1; i < positions.size() - 1; ++i){
    derivatives.emplace_back(Vec2D{-4 * (positions[i - 1].x - 2 *positions[i].x  + positions[i + 1].x ) * K, 
                                   -4 * ( (positions[i - 1].y - 2 *positions[i].y  + positions[i + 1].y ) * K - mass[i] * gravity)});
 }
    return derivatives;
}
int main(){
    scalar L = 100;
    scalar total_mass = 2750;
    size_t n_mass = 25;
    size_t nodal_mass = total_mass/n_mass;
    scalar increment = L/(n_mass - 1);
    scalar K_mat = 16239.8;
    scalar K = K_mat/increment;
    scalar gravity = 9.81;
    scalar tol = 1e-5;
    scalar incr = 0.000005;

    std::vector<Vec2D> coords(n_mass);
    std::vector<Vec2D> forces(n_mass);
    std::vector<scalar> mass(n_mass);



   
    for (size_t i = 0; i < n_mass; ++i){
        coords[i].x = i * increment;
        coords[i].y = 0.1;
        mass[i] = nodal_mass;
        forces[i].x = 0;
        forces[i].y = 0;
    }

    updateForces(coords, mass, K, gravity, forces);

    scalar res = getResidual(forces);
    std::vector<Vec2D> der;
    der.reserve(coords.size() - 2);

    unsigned int iteration = 0;
    while (res > tol){
        der = getDerivatives(coords, mass, K, gravity);
        for (size_t i = 1; i < coords.size() - 1; ++i){
            coords[i].x +=  - incr * der[i - 1].x;
            coords[i].y +=  - incr * der[i - 1].y;
        }

        updateForces(coords, mass, K, gravity, forces);
        res = getResidual(forces);
        std::cout << "Iteration: " << iteration << " residual: " << res << std::endl;
        if (iteration % 100 == 0){
            std::ofstream results;
            std::string frameName = "res_" + std::to_string(iteration) + ".csv";
            results.open(frameName);
            results << "x" << "," << "y" << '\n';
            for (size_t i = 0; i < coords.size(); ++i){
                results << coords[i].x << "," << coords[i].y << '\n';
            }
            results.close();
        }
        iteration++;

    }
   
    
      

   scalar length = 0;

   for (size_t i = 1; i < coords.size() ; ++i) {
    Vec2D diff{coords[i].x - coords[i-1].x, 
               coords[i].y - coords[i-1].y};

    length += sqrt(norm(diff));
   }

   std::cout << "Final length:" << length <<  std::endl;
}