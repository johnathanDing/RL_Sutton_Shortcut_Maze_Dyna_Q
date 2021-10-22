//
//  MazePolicy.cpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/21/21.
//

#include "MazePolicy.hpp"

MazePolicy::MazePolicy(const MazeEnv& input_maze_env, double input_kappa,
                       double input_epsilon, double input_gamma, double input_alpha) :
maze_env(input_maze_env),
kappa(input_kappa),
epsilon_soft(input_epsilon),
gamma(input_gamma),
alpha(input_alpha)
{
    std::cout << "MazePolicy initialized!" << "\n";
};



