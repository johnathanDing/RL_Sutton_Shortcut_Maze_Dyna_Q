//
//  MazeModel.cpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/24/21.
//

#include "MazeModel.hpp"

MazeModel::MazeModel(const MazeEnv& input_maze_env) :
maze_env(input_maze_env)
{
    std::cout << "MazeModel initialized!" << "\n";
};


void MazeModel::memorizeStateAction_DynaQ(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move,
                                          std::tuple<int, int> next_state, int reward)
{
    
};
