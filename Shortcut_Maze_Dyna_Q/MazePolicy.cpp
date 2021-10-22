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


void MazePolicy::updateStateActionVal_DynaQ(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move, std::tuple<int, int> next_state, double reward)
{
    // Initialize current state-action space if state is missing
    if (state_action_space_DynaQ.find(curr_state) == state_action_space_DynaQ.end()) {
        std::vector<std::tuple<int, int>> all_available_actions (maze_env.getAvailableMoves(curr_state));
        state_action_space_DynaQ.insert(std::pair<std::tuple<int, int>, std::vector<std::tuple<int, int>>>
                                        (curr_state, all_available_actions));
        state_action_val_DynaQ.insert(std::pair<std::tuple<int, int>, std::vector<double>>
                                      (curr_state, std::vector<double> (static_cast<int>(all_available_actions.size()), 0.0)));
    }
    
    // Find the position of current action under current state
    std::vector<std::tuple<int, int>>::iterator iter_curr_move;
    iter_curr_move = std::find(state_action_space_DynaQ.at(curr_state).begin(),
                               state_action_space_DynaQ.at(curr_state).end(), curr_move);
    int idx_curr_move = static_cast<int>(std::distance(state_action_space_DynaQ.at(curr_state).begin(), iter_curr_move));
    
    // Find the max state-action value of next state (Q-learning). If next state is not initialized yet, just use default 0.
    double max_next_val;
    if (state_action_space_DynaQ.find(next_state) == state_action_space_DynaQ.end()) {
        max_next_val = 0.0;
    }
    else {
        max_next_val = *std::max_element(state_action_val_DynaQ.at(next_state).begin(),
                                         state_action_val_DynaQ.at(next_state).end());
    }
    
    // Do the update
    state_action_val_DynaQ.at(curr_state)[idx_curr_move] += alpha * (reward + gamma * max_next_val -
                                                                     state_action_val_DynaQ.at(curr_state)[idx_curr_move]);
};


void MazePolicy::updateStateActionVal_DynaQ_Plus(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move, std::tuple<int, int> next_state, double reward, int time_stamp, bool bonus_reward)
{
    // Initialize current state-action space if state is missing
    if (state_action_space_DynaQ_Plus.find(curr_state) == state_action_space_DynaQ_Plus.end()) {
        std::vector<std::tuple<int, int>> all_available_actions (maze_env.getAvailableMoves(curr_state));
        state_action_space_DynaQ_Plus.insert(std::pair<std::tuple<int, int>, std::vector<std::tuple<int, int>>>
                                             (curr_state, all_available_actions));
        state_action_val_DynaQ_Plus.insert(std::pair<std::tuple<int, int>, std::vector<double>>
                                           (curr_state,
                                            std::vector<double> (static_cast<int>(all_available_actions.size()), 0.0)));
        state_action_time_DynaQ_Plus.insert(std::pair<std::tuple<int, int>, std::vector<int>>
                                            (curr_state,
                                             std::vector<int> (static_cast<int>(all_available_actions.size()), time_stamp)));
    }
    
    // Find the position of current action under current state
    std::vector<std::tuple<int, int>>::iterator iter_curr_move;
    iter_curr_move = std::find(state_action_space_DynaQ_Plus.at(curr_state).begin(),
                               state_action_space_DynaQ_Plus.at(curr_state).end(), curr_move);
    int idx_curr_move = static_cast<int>(std::distance(state_action_space_DynaQ_Plus.at(curr_state).begin(), iter_curr_move));
    
    // Find the max state-action value of next state (Q-learning). If next state is not initialized yet, just use default 0.
    double max_next_val;
    if (state_action_space_DynaQ_Plus.find(next_state) == state_action_space_DynaQ_Plus.end()) {
        max_next_val = 0.0;
    }
    else {
        max_next_val = *std::max_element(state_action_val_DynaQ_Plus.at(next_state).begin(),
                                         state_action_val_DynaQ_Plus.at(next_state).end());
    }
    
    // Depending on flag bonus_reward (whether in real or simulated experience), choose whether to include bonus reward in update
    if (bonus_reward) {
        state_action_val_DynaQ_Plus.at(curr_state)[idx_curr_move] += alpha * (reward
                                  + kappa*sqrt(time_stamp - state_action_time_DynaQ_Plus.at(curr_state)[idx_curr_move])
                                  + gamma * max_next_val - state_action_val_DynaQ_Plus.at(curr_state)[idx_curr_move]);
    }
    else {
        state_action_val_DynaQ_Plus.at(curr_state)[idx_curr_move] += alpha * (reward
                                  + gamma * max_next_val - state_action_val_DynaQ_Plus.at(curr_state)[idx_curr_move]);
    }
    
    // Update the time stamp of current action
    state_action_time_DynaQ_Plus.at(curr_state)[idx_curr_move] = time_stamp;
};


double MazePolicy::getStateActionVal_DynaQ(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move) const
{
    double curr_val;
    // If this state is never seen before, return default 0
    if (state_action_space_DynaQ.find(curr_state) == state_action_space_DynaQ.end()) {
        curr_val = 0.0;
    }
    // If seen this state before
    else {
        // Get the constant iterator
        std::vector<std::tuple<int, int>>::const_iterator iter_curr_move
        (std::find(state_action_space_DynaQ.at(curr_state).begin(),
                   state_action_space_DynaQ.at(curr_state).end(), curr_move));
        // If never seen this move in current state, also return default 0
        if (iter_curr_move == state_action_space_DynaQ.at(curr_state).end()) {
            curr_val = 0.0;
        }
        // If seen state-action pair before, return the stored value
        else {
            int idx_curr_move
            (static_cast<int>(std::distance(state_action_space_DynaQ.at(curr_state).begin(), iter_curr_move)));
            curr_val = state_action_val_DynaQ.at(curr_state)[idx_curr_move];
        }
    }
    
    return curr_val;
};


double MazePolicy::getStateActionVal_DynaQ_Plus(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move) const
{
    double curr_val;
    // If this state is never seen before, return default 0
    if (state_action_space_DynaQ_Plus.find(curr_state) == state_action_space_DynaQ_Plus.end()) {
        curr_val = 0.0;
    }
    // If seen this state before
    else {
        // Get the constant iterator
        std::vector<std::tuple<int, int>>::const_iterator iter_curr_move
        (std::find(state_action_space_DynaQ_Plus.at(curr_state).begin(),
                   state_action_space_DynaQ_Plus.at(curr_state).end(), curr_move));
        // If never seen this move in current state, also return default 0
        if (iter_curr_move == state_action_space_DynaQ_Plus.at(curr_state).end()) {
            curr_val = 0.0;
        }
        // If seen state-action pair before, return the stored value
        else {
            int idx_curr_move
            (static_cast<int>(std::distance(state_action_space_DynaQ_Plus.at(curr_state).begin(), iter_curr_move)));
            curr_val = state_action_val_DynaQ_Plus.at(curr_state)[idx_curr_move];
        }
    }
    
    return curr_val;
};
