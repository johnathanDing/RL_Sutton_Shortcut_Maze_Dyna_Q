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
                                                                     state_action_val_DynaQ.at(curr_state).at(idx_curr_move));
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
                                  + kappa*sqrt(time_stamp - state_action_time_DynaQ_Plus.at(curr_state).at(idx_curr_move))
                                  + gamma * max_next_val - state_action_val_DynaQ_Plus.at(curr_state).at(idx_curr_move));
    }
    else {
        state_action_val_DynaQ_Plus.at(curr_state)[idx_curr_move] += alpha * (reward
                                  + gamma * max_next_val - state_action_val_DynaQ_Plus.at(curr_state).at(idx_curr_move));
    }
    
    // Update the time stamp of current action
    state_action_time_DynaQ_Plus.at(curr_state).at(idx_curr_move)= time_stamp;
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

        int idx_curr_move
        (static_cast<int>(std::distance(state_action_space_DynaQ.at(curr_state).begin(), iter_curr_move)));
        curr_val = state_action_val_DynaQ.at(curr_state).at(idx_curr_move);
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

        int idx_curr_move
        (static_cast<int>(std::distance(state_action_space_DynaQ_Plus.at(curr_state).begin(), iter_curr_move)));
        curr_val = state_action_val_DynaQ_Plus.at(curr_state).at(idx_curr_move);
    }
    
    return curr_val;
};


std::tuple<int, int> MazePolicy::getGreedyPolicy_DynaQ(std::tuple<int, int> curr_state) const
{
    std::tuple<int, int> greedy_move;
    
    // If state has not been seen, return a random policy
    if (state_action_val_DynaQ.find(curr_state) == state_action_val_DynaQ.end()) {
        greedy_move = getRandomPolicy_DynaQ(curr_state);
    }
    // If seen, return greedy policy as usual
    else {
        std::vector<double>::const_iterator iter_max_val;
        iter_max_val = std::max_element(state_action_val_DynaQ.at(curr_state).begin(),
                                        state_action_val_DynaQ.at(curr_state).end());
        int idx_max_val (static_cast<int>(std::distance(state_action_val_DynaQ.at(curr_state).begin(), iter_max_val)));
        greedy_move = state_action_space_DynaQ.at(curr_state).at(idx_max_val);
    }
    
    return greedy_move;
};


std::tuple<int, int> MazePolicy::getGreedyPolicy_DynaQ_Plus(std::tuple<int, int> curr_state,
                                                            int time_stamp, bool bonus_reward) const
{
    std::tuple<int, int> greedy_move;
    
    // If state has not been seen, return a random policy
    if (state_action_val_DynaQ_Plus.find(curr_state) == state_action_val_DynaQ_Plus.end()) {
        greedy_move = getRandomPolicy_DynaQ_Plus(curr_state);
    }
    // If seen, return greedy policy as follows
    else {
        
        int idx_max_val;
        // If time interval bonus reward should be considered in action selection
        if (bonus_reward) {
            std::vector<double>::iterator iter_max_val;
            // Get a new vector combining SA value and time reward
            std::vector<double> sa_val_with_reward;
            for (int i(0); i<static_cast<int>(state_action_val_DynaQ_Plus.at(curr_state).size()); ++i) {
                sa_val_with_reward.push_back(state_action_val_DynaQ_Plus.at(curr_state)[i] +
                                             kappa * sqrt(time_stamp - state_action_time_DynaQ_Plus.at(curr_state).at(i)));
                
            }
            iter_max_val = std::max_element(sa_val_with_reward.begin(), sa_val_with_reward.end());
            idx_max_val = static_cast<int>(std::distance(sa_val_with_reward.begin(), iter_max_val));
        }
        // If bonus reward is not considered
        else {
            std::vector<double>::const_iterator iter_max_val;
            iter_max_val = std::max_element(state_action_val_DynaQ_Plus.at(curr_state).begin(),
                                            state_action_val_DynaQ_Plus.at(curr_state).end());
            idx_max_val = static_cast<int>(std::distance(state_action_val_DynaQ_Plus.at(curr_state).begin(), iter_max_val));
        }
        greedy_move = state_action_space_DynaQ_Plus.at(curr_state).at(idx_max_val);
    }
    
    return greedy_move;
};


std::tuple<int, int> MazePolicy::getSoftPolicy_DynaQ(std::tuple<int, int> curr_state) const
{
    std::tuple<int, int> soft_move;
    // Static RNG engine for soft policy
    static std::mt19937 mersenne_eng(static_cast<std::mt19937::result_type>(std::time(nullptr)));
    static std::uniform_real_distribution<double> soft_RNG(0.0, 1.0);
    
    if (soft_RNG(mersenne_eng) <= epsilon_soft) {
        soft_move = getRandomPolicy_DynaQ(curr_state);
    } else {
        soft_move = getGreedyPolicy_DynaQ(curr_state);
    }
    
    return soft_move;
};


std::tuple<int, int> MazePolicy::getSoftPolicy_DynaQ_Plus(std::tuple<int, int> curr_state, int time_stamp) const
{
    std::tuple<int, int> soft_move;
    // Static RNG for soft policy
    static std::mt19937 mersenne_eng(static_cast<std::mt19937::result_type>(std::time(nullptr)));
    static std::uniform_real_distribution<double> soft_RNG(0.0, 1.0);
    
    if (soft_RNG(mersenne_eng) <= epsilon_soft) {
        soft_move = getRandomPolicy_DynaQ_Plus(curr_state);
    } else {
        // Soft policy is only used in real experience, never in planning. So bonus reward is always false
        soft_move = getGreedyPolicy_DynaQ_Plus(curr_state, time_stamp, false);
    }
    
    return soft_move;
};


void MazePolicy::reAcquireStateActionSpace_DynaQ(std::tuple<int, int> curr_state)
{
    std::vector<std::tuple<int, int>> all_actions_new (maze_env.getAvailableMoves(curr_state));
    for (std::tuple<int, int> move: all_actions_new) {
        if (std::find(state_action_space_DynaQ.at(curr_state).begin(), state_action_space_DynaQ.at(curr_state).end(), move) ==
            state_action_space_DynaQ.at(curr_state).end()) {
            state_action_space_DynaQ.at(curr_state).push_back(move);
            state_action_val_DynaQ.at(curr_state).push_back(0);
        }
    }
};


void MazePolicy::reAcquireStateActionSpace_DynaQ_Plus(std::tuple<int, int> curr_state, int time_stamp)
{
    std::vector<std::tuple<int, int>> all_actions_new (maze_env.getAvailableMoves(curr_state));
    for (std::tuple<int, int> move: all_actions_new) {
        if (std::find(state_action_space_DynaQ_Plus.at(curr_state).begin(),
                      state_action_space_DynaQ_Plus.at(curr_state).end(), move) ==
            state_action_space_DynaQ_Plus.at(curr_state).end()) {
            state_action_space_DynaQ_Plus.at(curr_state).push_back(move);
            state_action_val_DynaQ_Plus.at(curr_state).push_back(0.0);
            state_action_time_DynaQ_Plus.at(curr_state).push_back(time_stamp);
        }
    }
};


std::tuple<int, int> MazePolicy::getRandomPolicy_DynaQ(std::tuple<int, int> curr_state) const
{
    std::tuple<int, int> random_move;
    
    // Static RNG for random policy selection
    static std::mt19937 mersenne_eng(static_cast<std::mt19937::result_type>(std::time(nullptr)));
    
    // State-specific size
    int action_size (static_cast<int>(state_action_space_DynaQ.at(curr_state).size()));
    std::uniform_int_distribution<> action_RNG(0, action_size-1);
    
    random_move = state_action_space_DynaQ.at(curr_state).at(action_RNG(mersenne_eng));
    
    return random_move;
};


std::tuple<int, int> MazePolicy::getRandomPolicy_DynaQ_Plus(std::tuple<int, int> curr_state) const
{
    std::tuple<int, int> random_move;
    
    // Static RNG for random policy selection
    static std::mt19937 mersenne_eng(static_cast<std::mt19937::result_type>(std::time(nullptr)));
    
    // State-specific size
    int action_size (static_cast<int>(state_action_space_DynaQ_Plus.at(curr_state).size()));
    std::uniform_int_distribution<> action_RNG(0, action_size-1);
    
    random_move = state_action_space_DynaQ_Plus.at(curr_state).at(action_RNG(mersenne_eng));
    
    return random_move;
};
