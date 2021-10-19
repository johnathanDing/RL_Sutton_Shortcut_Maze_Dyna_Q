//
//  GridWorld.cpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/19/21.
//

#include "GridWorld.hpp"

GridWorld::GridWorld(std::vector<std::vector<int>> input_grid, std::tuple<int, int> input_start,
                     std::tuple<int, int> input_goal) :
grid_world(input_grid),
start_pos(input_start),
goal_pos(input_goal)
{
    std::cout << "Grid World initialized!" << "\n";
};


std::vector<std::vector<int>> GridWorld::getGrid() const
{
    return grid_world;
};


std::tuple<int, int> GridWorld::getStartPos() const
{
    return start_pos;
};


std::tuple<int, int> GridWorld::getGoalPos() const
{
    return goal_pos;
};
