//
//  MazePolicy.hpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/21/21.
//

#ifndef MazePolicy_hpp
#define MazePolicy_hpp

#include "MazeEnv.hpp"
#include <vector>
#include <tuple>
#include <map>
#include <random>
#include <time.h>
#include <math.h>
#include <iterator>
#include <iostream>

/// Class that computes and returns policy according to Dyna-Q and Dyna-Q+
class MazePolicy
{
private:
    // Constant reference to a maze environment class instance
    const MazeEnv& maze_env;
    // State-to-action space map for Dyna-Q
    std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>> state_action_space_DynaQ;
    // State-to-action value map for Dyna-Q
    std::map<std::tuple<int, int>, std::vector<double>> state_action_val_DynaQ;
    
    // State-to-action space map for Dyna-Q+
    std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>> state_action_space_DynaQ_Plus;
    // State-to-action value map for Dyna-Q+
    std::map<std::tuple<int, int>, std::vector<double>> state_action_val_DynaQ_Plus;
    // State-to-action interval (time step of last pick) for Dyna-Q+
    std::map<std::tuple<int, int>, std::vector<int>> state_action_time_DynaQ_Plus;
    
    // Parameter for time interval bonus reward in DYna-Q+
    double kappa;
    // Epsilon soft policy parameter
    double epsilon_soft;
    // Discount parameter for episodic task
    double gamma;
    // Learning rate
    double alpha;
    
public:
    /// Constructor of MazePolicy class
    /// @param input_maze_env Input constant reference to a MazeEnv class instance
    /// @param input_kappa Input kappa parameter for Dyna-Q+ bonus reward
    /// @param input_epsilon Input epsilon parameter for soft-greedy policy
    /// @param input_gamma Input discount parameter for episodic tasks
    /// @param input_alpha Input learning rate parameter
    MazePolicy(const MazeEnv& input_maze_env, double input_kappa, double input_epsilon, double input_gamma, double input_alpha);
    
    /// Updates the state-action value according to Dyna-Q
    /// @param curr_state Current state to be updated
    /// @param curr_move Current action to be updated
    /// @param next_state Next state transitioned from current state-action
    /// @param reward Reward of current transition
    void updateStateActionVal_DynaQ(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move,
                                    std::tuple<int, int> next_state, double reward);
    
    /// Updates the state-action value according to Dyna-Q+
    /// @param curr_state Current state to be updated
    /// @param curr_move Current action to be updated
    /// @param next_state Next state transitioned from current state-action
    /// @param reward Reward of current transition
    /// @param time_stamp Current time stamp during training. Used for Dyna-Q+ bonus reward
    void updateStateActionVal_DynaQ_Plus(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move,
                                         std::tuple<int, int> next_state, double reward, int time_stamp, bool bonus_reward);
    
    /// Returns the state-action value according to Dyna-Q
    /// @param curr_state Current state inquired
    /// @param curr_move Current action inquired
    double getStateActionVal_DynaQ(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move) const;
    
    /// Returns the state-action value according to Dyna-Q+
    /// @param curr_state Current state inquired
    /// @param curr_move Current action inquired
    double getStateActionVal_DynaQ_Plus(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move) const;
    
    /// Return action following greedy policy according to Dyna-Q
    /// @param curr_state Current state inquired
    std::tuple<int, int> getGreedyPolicy_DynaQ(std::tuple<int, int> curr_state) const;
    
    /// Return action following greedy policy according to Dyna-Q+
    /// @param curr_state Current state inquired
    std::tuple<int, int> getGreedyPolicy_DynaQ_Plus(std::tuple<int, int> curr_state) const;
    
    /// Return action following soft policy according to Dyna-Q
    /// @param curr_state Current state inquired
    std::tuple<int, int> getSoftPolicy_DynaQ(std::tuple<int, int> curr_state) const;
    
    /// Return action following soft policy according to Dyna-Q+
    /// @param curr_state Current state inquired
    std::tuple<int, int> getSoftPolicy_DynaQ_Plus(std::tuple<int, int> curr_state) const;
    
    /// Reacquires all available actions at a state for Dyna-Q. Use after a Maze change
    /// @param curr_state State to be reaquired
    void reAcquireStateActionSpace_DynaQ(std::tuple<int, int> curr_state);
    
    /// Reacquires all available actions at a state for Dyna-Q+. Use after a Maze change
    /// @param curr_state State to be reaquired
    void reAcquireStateActionSpace_DynaQ_Plus(std::tuple<int, int> curr_state);
    
private:
    /// Return a random policy according to Dyna-Q
    /// @param curr_state Current state inquired
    std::tuple<int, int> getRandomPolicy_DynaQ(std::tuple<int, int> curr_state) const;
    
    /// Return a random policy according to Dyna-Q+
    /// @param curr_state Current state inquired
    std::tuple<int, int> getRandomPolicy_DynaQ_Plus(std::tuple<int, int> curr_state) const;
};

#endif /* MazePolicy_hpp */
