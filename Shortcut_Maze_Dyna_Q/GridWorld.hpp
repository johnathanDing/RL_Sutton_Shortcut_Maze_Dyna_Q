//
//  GridWorld.hpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/19/21.
//

#ifndef GridWorld_hpp
#define GridWorld_hpp

#include <vector>
#include <tuple>
#include <iostream>

/// Basic class storing grid world layout
class GridWorld
{
private:
    // Stores grid world layout
    std::vector<std::vector<int>> grid_world;
    // Stores the starting position
    std::tuple<int, int> start_pos;
    // Stores the goal position
    std::tuple<int, int> goal_pos;
    
public:
    /// Constructor of GridWorld class
    /// @param input_grid Input 2D vector of grid world layout
    /// @param input_start Input tuple of starting position
    /// @param input_goal Input tuple of goal position
    GridWorld(std::vector<std::vector<int>> input_grid, std::tuple<int, int> input_start, std::tuple<int, int> input_goal);
    
    /// Returns the 2D vector of grid world layout
    std::vector<std::vector<int>> getGrid() const;
    
    /// Returns the tuple of starting position
    std::tuple<int, int> getStartPos() const;
    
    /// Returns the tuple of goal position
    std::tuple<int, int> getGoalPos() const;
};


#endif /* GridWorld_hpp */
