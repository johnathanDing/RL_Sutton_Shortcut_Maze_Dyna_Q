//
//  MazeEnv.cpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/19/21.
//

#include "MazeEnv.hpp"

MazeEnv::MazeEnv(const GridWorld& input_grid_world) :
// Since maze may change in-between episodes, should use reference instead of storing a copy of grid.
grid_world(input_grid_world),
goal_pos(input_grid_world.getGoalPos()),
m(static_cast<int>(input_grid_world.getGrid().size())),
n(static_cast<int>(input_grid_world.getGrid()[0].size()))
{
    std::cout << "MazeEnv initialized!" << "\n";
};


std::vector<std::tuple<int, int>> MazeEnv::getAvailableMoves(std::tuple<int, int> curr_state) const
{
    std::vector<std::tuple<int, int>> all_moves{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}, available_moves;
    
    for (std::tuple<int, int> move : all_moves) {
        int next_i(std::get<0>(curr_state)+std::get<0>(move)),
            next_j(std::get<1>(curr_state)+std::get<1>(move));
        if (next_i >=0 && next_i < m && next_j >= 0 && next_j < n && grid_world.getGrid()[next_i][next_j] == 0) {
            available_moves.push_back(move);
        }
    }
    
    return available_moves;
};


MazeResponse MazeEnv::getMazeResponse(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move) const
{
    MazeResponse response;
    
    int next_i(std::get<0>(curr_state)+std::get<0>(curr_move)),
        next_j(std::get<1>(curr_state)+std::get<1>(curr_move));
    
    // Makes sure the current action is valid
    if (next_i<0 || next_i>=m || next_j<0 || next_j>=n) {
        throw "Next position out of boundary.";
    }
    if (grid_world.getGrid()[next_i][next_j] == 1) {
        throw "Next position hit wall.";
    }

    response.next_state = std::make_tuple(next_i, next_j);
    response.finished = (response.next_state == goal_pos);
    response.reward = (response.finished ? 1 : 0);
    
    return response;
};
