//
//  main.cpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/19/21.
//

#include "GridWorld.hpp"
#include "MazeEnv.hpp"
#include "MazePolicy.hpp"
#include "MazeModel.hpp"
#include "MazeVisualizer.hpp"
#include <vector>
#include <time.h>
#include <iostream>
#include <fstream>

int main() {
    // Set up initial maze
    std::vector<std::vector<int>> input_grid (6, std::vector<int> (9, 0));
    for (int i(1); i<9; ++i) {
        input_grid[2][i] = 1;
    }
    std::tuple<int, int> start_pos {0, 3};
    std::tuple<int, int> goal_pos {5, 8};
    
    // Set up class instances
    GridWorld grid_world(input_grid, start_pos, goal_pos);
    MazeEnv maze_env(grid_world);
    MazePolicy maze_policy(maze_env, 0.01, 0.1, 0.95, 0.1);
    MazeModel maze_model(maze_env);
    MazeVisualizer maze_visual(grid_world);
    
    // Prepare to record training data in a file
    int time_step(0), time_limit(6000), cum_reward_DynaQ(0), cum_reward_DynaQ_Plus(0);
    std::ofstream training_file("Training_statistics.txt");
    if (!training_file) {
        // If somehow the file is not opened
        std::cerr << "Training recording file failed to open!" << "\n";
        return 1;
    }
    training_file << "# Recording training statistics: Cumulative rewards (Dyna-Q and Dyna-Q+) vs time step:" << "\n";
    training_file << "# Time step | Cumulative reward: Dyna-Q | Cumulative reward: Dyna-Q+" << "\n";
    
    // Record training start time
    clock_t start_time(clock());
    
    // Visualize the maze first
    maze_visual.drawMaze();
    
    // Print a message
    std::cout << "Training has started!" << "\n";
    
    // Initialize state-action variables
    std::tuple<int, int> curr_state_DynaQ, curr_move_DynaQ, curr_state_DynaQ_Plus, curr_move_DynaQ_Plus;
    // Initialize the response structs to be goal state, for easy transition at the start of training
    MazeResponse maze_response_DynaQ{{grid_world.getGoalPos()}, {1}, {true}},
                 maze_response_DynaQ_Plus{{grid_world.getGoalPos()}, {1}, {true}};
    
    while (time_step < time_limit) {
        // If a Dyna-Q episode is just finished, start a new one
        if (maze_response_DynaQ.finished) {
            curr_state_DynaQ = grid_world.getStartPos();
            curr_move_DynaQ = maze_policy.getSoftPolicy_DynaQ(curr_state_DynaQ);
        }
        // If a Dyna-Q+ episode is just finished, start a new one
        if (maze_response_DynaQ_Plus.finished) {
            curr_state_DynaQ_Plus = grid_world.getStartPos();
            curr_move_DynaQ_Plus = maze_policy.getSoftPolicy_DynaQ_Plus(curr_state_DynaQ_Plus, time_step);
        }
    }
}
