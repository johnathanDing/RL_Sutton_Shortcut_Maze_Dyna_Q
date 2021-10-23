//
//  MazeEnv.hpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/19/21.
//

#ifndef MazeEnv_hpp
#define MazeEnv_hpp

#include "GridWorld.hpp"
#include <vector>
#include <tuple>
#include <iostream>

/// Constainer of environment response information
struct MazeResponse
{
    // Next state after current state and action
    std::tuple<int, int> next_state;
    // Reward for the current action
    int reward;
    // Whether the current state has reached goal position
    bool finished;
};

/// Maze Environment class. Can returns available movements and envrionment responses given agent states and actions.
class MazeEnv
{
private:
    // A constant reference to a GridWorld class instance
    const GridWorld& grid_world;
    // For easy access to the goal position
    std::tuple<int, int> goal_pos;
    // For easy access to the grid world dimensions
    int m, n;
    
public:
    /// Constructor of the MazeEnv class
    /// @param input_grid_world Constant reference to an input GridWorld class instance
    MazeEnv(const GridWorld& input_grid_world);
    
    /// Returns all currently available moves for the given state
    /// @param curr_state State being inquired
    std::vector<std::tuple<int, int>> getAvailableMoves(std::tuple<int, int> curr_state) const;
    
    /// Returns the environment response given current state and action
    /// @param curr_state Current state inquired
    /// @param curr_move Current action inquired
    MazeResponse getMazeResponse(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move) const;
};

#endif /* MazeEnv_hpp */
