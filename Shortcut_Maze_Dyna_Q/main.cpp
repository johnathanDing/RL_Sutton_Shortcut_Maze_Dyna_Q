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
    MazePolicy maze_policy(maze_env, 0.1, 0.1, 0.95, 0.1);
    MazeModel maze_model(maze_env);
    MazeVisualizer maze_visual(grid_world);
    
    // Dyna-Q planning step number
    int n_planning(50);
    
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
    
    // Declare state-action variables for real experience
    std::tuple<int, int> curr_state_DynaQ, curr_move_DynaQ, curr_state_DynaQ_Plus, curr_move_DynaQ_Plus;
    // Initialize the response structs to be goal state, for easy transition at the start of training
    MazeResponse maze_response_DynaQ{{grid_world.getGoalPos()}, 1, true},
                 maze_response_DynaQ_Plus{{grid_world.getGoalPos()}, 1, true};
    
    // Declare simulated experience
    MazePastExp rand_simulation_DynaQ, rand_simulation_DynaQ_Plus;
    
    while (time_step < time_limit) {
        // Open up the shortcut at time stamp = 3000
        if (time_step == 3000) {
            grid_world.changeGrid(2, 8);
            maze_policy.reAcquireStateActionSpace_DynaQ(std::tuple<int, int> {1, 8});
            maze_policy.reAcquireStateActionSpace_DynaQ(std::tuple<int, int> {3, 8});
            maze_policy.reAcquireStateActionSpace_DynaQ_Plus(std::tuple<int, int> {1, 8}, time_step);
            maze_policy.reAcquireStateActionSpace_DynaQ_Plus(std::tuple<int, int> {3, 8}, time_step);
            maze_model.reacquireModel_DynaQ_Plus(std::tuple<int, int> {1, 8});
            maze_model.reacquireModel_DynaQ_Plus(std::tuple<int, int> {3, 8});
        }
        
        /// Real Exprience
        // If a Dyna-Q episode is just finished, start a new one
        if (maze_response_DynaQ.finished) {
            curr_state_DynaQ = grid_world.getStartPos();
            std::cout << "Dyna-Q episode time stamp: " << time_step << std::endl;
        }
        // If a Dyna-Q+ episode is just finished, start a new one
        if (maze_response_DynaQ_Plus.finished) {
            curr_state_DynaQ_Plus = grid_world.getStartPos();
            std::cout << "Dyna-Q+ episode time stamp: " << time_step << std::endl;
        }
        // Get the actions according to soft policy
        curr_move_DynaQ = maze_policy.getSoftPolicy_DynaQ(curr_state_DynaQ);
        curr_move_DynaQ_Plus = maze_policy.getSoftPolicy_DynaQ_Plus(curr_state_DynaQ_Plus, time_step, true);
        // Get the response from state-action
        maze_response_DynaQ = maze_env.getMazeResponse(curr_state_DynaQ, curr_move_DynaQ);
        maze_response_DynaQ_Plus = maze_env.getMazeResponse(curr_state_DynaQ_Plus, curr_move_DynaQ_Plus);
        // Update the state-aciton values
        maze_policy.updateStateActionVal_DynaQ(curr_state_DynaQ, curr_move_DynaQ,
                                               maze_response_DynaQ.next_state, maze_response_DynaQ.reward);
        // No bonus reward for Dyna-Q+ during real experience
        maze_policy.updateStateActionVal_DynaQ_Plus(curr_state_DynaQ_Plus, curr_move_DynaQ_Plus,
                                                    maze_response_DynaQ_Plus.next_state, maze_response_DynaQ_Plus.reward,
                                                    time_step, false);
        // Update the planning model
        maze_model.memorizeStateAction_DynaQ(curr_state_DynaQ, curr_move_DynaQ,
                                             maze_response_DynaQ.next_state, maze_response_DynaQ.reward);
        maze_model.memorizeStateAction_DynaQ_Plus(curr_state_DynaQ_Plus, curr_move_DynaQ_Plus,
                                                  maze_response_DynaQ_Plus.next_state, maze_response_DynaQ_Plus.reward);
        
        // Calculate the training progress and record in file
        cum_reward_DynaQ += maze_response_DynaQ.reward;
        cum_reward_DynaQ_Plus += maze_response_DynaQ_Plus.reward;
        training_file << time_step << " " << cum_reward_DynaQ << " " << cum_reward_DynaQ_Plus << std::endl;
        
        /// Planning using simulated experience
        for (int i(0); i<n_planning; ++i) {
            // Get random past experience
            rand_simulation_DynaQ = maze_model.getPastResponse_DynaQ();
            rand_simulation_DynaQ_Plus = maze_model.getPastResponse_DynaQ_Plus();
            // Update state-action value with the simulated step
            maze_policy.updateStateActionVal_DynaQ(rand_simulation_DynaQ.prev_state, rand_simulation_DynaQ.prev_move,
                                                   rand_simulation_DynaQ.result_state, rand_simulation_DynaQ.result_reward);
            maze_policy.updateStateActionVal_DynaQ_Plus(rand_simulation_DynaQ_Plus.prev_state,
                                                        rand_simulation_DynaQ_Plus.prev_move,
                                                        rand_simulation_DynaQ_Plus.result_state,
                                                        rand_simulation_DynaQ_Plus.result_reward, time_step, false);
        }
        
        // Prepare for next step
        curr_state_DynaQ = maze_response_DynaQ.next_state;
        curr_state_DynaQ_Plus = maze_response_DynaQ_Plus.next_state;
        
        // Increment the time step
        ++ time_step;
    }
    
    // Close recording file
    training_file.close();
    // Print total training time
    clock_t total_time(clock() - start_time);
    std::cout << "Total training time is: " << static_cast<double>(total_time)/CLOCKS_PER_SEC << " sec." << "\n";
    
    // Generate a fully-trained, Dyna-Q greedy policy to visualize result
    std::vector<std::tuple<int, int>> episode_DynaQ, episode_DynaQ_Plus;
    // Reset time step
    time_step = 0;
    // Get the starting states
    curr_state_DynaQ = grid_world.getStartPos();
    curr_state_DynaQ_Plus = grid_world.getStartPos();
    curr_move_DynaQ = maze_policy.getGreedyPolicy_DynaQ(curr_state_DynaQ);
    curr_move_DynaQ_Plus = maze_policy.getGreedyPolicy_DynaQ_Plus(curr_move_DynaQ_Plus, time_step, false);
    // Get the response from state-action
    maze_response_DynaQ.finished = false;
    maze_response_DynaQ_Plus.finished = false;
    
    // Generate a single greedy episode, Dyna-Q
    while (!maze_response_DynaQ.finished) {
        maze_response_DynaQ = maze_env.getMazeResponse(curr_state_DynaQ, curr_move_DynaQ);
        episode_DynaQ.push_back(curr_state_DynaQ);
        curr_state_DynaQ = maze_response_DynaQ.next_state;
        curr_move_DynaQ = maze_policy.getGreedyPolicy_DynaQ(curr_state_DynaQ);
    }
    episode_DynaQ.push_back(curr_state_DynaQ);
    
    // Print out the episode length of Dyna-Q
    std::cout << "Total length of Dyna-Q episode is: " << episode_DynaQ.size() << "\n";
    // Visualize the greedy episode of Dyna-Q
    maze_visual.drawMazeEpisode(episode_DynaQ);
    
    // Generate a single greedy episode, Dyna-Q+
    while (!maze_response_DynaQ_Plus.finished) {
        maze_response_DynaQ_Plus = maze_env.getMazeResponse(curr_state_DynaQ_Plus, curr_move_DynaQ_Plus);
        episode_DynaQ_Plus.push_back(curr_state_DynaQ_Plus);
        curr_state_DynaQ_Plus = maze_response_DynaQ_Plus.next_state;
        curr_move_DynaQ_Plus = maze_policy.getGreedyPolicy_DynaQ_Plus(curr_state_DynaQ_Plus, time_step, false);
        ++ time_step;
    }
    episode_DynaQ_Plus.push_back(curr_state_DynaQ_Plus);
    
    // Print out the episode length of Dyna-Q+
    std::cout << "Total length of Dyna-Q+ episode is: " << episode_DynaQ_Plus.size() << "\n";
    // Visualize the greedy episode of Dyna-Q+
    maze_visual.drawMazeEpisode(episode_DynaQ_Plus);
    
    return 0;
}
