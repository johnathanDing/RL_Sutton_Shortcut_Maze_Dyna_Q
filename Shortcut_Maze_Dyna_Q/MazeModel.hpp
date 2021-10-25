//
//  MazeModel.hpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/24/21.
//

#ifndef MazeModel_hpp
#define MazeModel_hpp

#include "MazeEnv.hpp"
#include <vector>
#include <tuple>
#include <map>
#include <time.h>
#include <random>
#include <iterator>
#include <iostream>

/// Struct storing past experience
struct MazePastExp
{
    // Previsouly visited state, action, and resultant state
    std::tuple<int, int> prev_state, prev_move, result_move;
    // Previous resultant reward
    int result_reward;
};

/// Class that stores past real experience and simuates experience for planning
class MazeModel
{
private:
    const MazeEnv& maze_env;
    // State-to-action space map for storing Dyna-Q model
    std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>> state_action_space_DynaQ;
    // State-to-(reward, next state) tuple map for storing Dyna-Q model
    std::map<std::tuple<int, int>, std::vector<std::tuple<double, std::tuple<int, int>>>> state_action_response_DynaQ;
    
    // State-to-action space map for storing Dyna-Q+ model
    std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>> state_action_space_DynaQ_Plus;
    // State-to-(reward, next state) tuple map for storing Dyna-Q+ model
    std::map<std::tuple<int, int>, std::vector<std::tuple<double, std::tuple<int, int>>>> state_action_response_DynaQ_Plus;
    
public:
    /// Constructor of the MazeModel class
    /// @param input_maze_env Input constant reference to a MazeEnv class instance
    MazeModel(const MazeEnv& input_maze_env);
    
    /// Memorizes the current experience in MazeModel, according to Dyna-Q
    /// @param curr_state Current state in experience
    /// @param curr_move Current aciton in experience
    /// @param next_state Next state in experience
    /// @param reward Reward of current experience
    void memorizeStateAction_DynaQ(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move,
                                   std::tuple<int, int> next_state, int reward);
    
    /// Memorizes the current experience in MazeModel, according to Dyna-Q+
    /// @param curr_state Current state in experience
    /// @param curr_move Current aciton in experience
    /// @param next_state Next state in experience
    /// @param reward Reward of current experience
    void memorizeStateAction_DynaQ_Plus(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move,
                                        std::tuple<int, int> next_state, int reward);
    
    /// Get a random past experience from the model, according to Dyna-Q
    MazePastExp getPastResponse_DynaQ() const;
    
    /// Get a random past experience from the model, according to Dyna-Q+
    MazePastExp getPastResponse_DynaQ_Plus() const;
};

#endif /* MazeModel_hpp */
