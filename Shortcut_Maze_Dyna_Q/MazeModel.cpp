//
//  MazeModel.cpp
//  Shortcut_Maze_Dyna_Q
//
//  Created by Xiaoyue Ding on 10/24/21.
//

#include "MazeModel.hpp"

MazeModel::MazeModel(const MazeEnv& input_maze_env) :
maze_env(input_maze_env)
{
    std::cout << "MazeModel initialized!" << "\n";
};


void MazeModel::memorizeStateAction_DynaQ(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move,
                                          std::tuple<int, int> next_state, double reward)
{
    // If the state is new, add it into model
    if (state_action_space_DynaQ.find(curr_state) == state_action_space_DynaQ.end()) {
        state_action_space_DynaQ.insert(std::pair<std::tuple<int, int>, std::vector<std::tuple<int, int>>>
                                        (curr_state,std::vector<std::tuple<int, int>>()));
        state_action_response_DynaQ.insert(std::pair<std::tuple<int, int>,
                                           std::vector<std::tuple<double, std::tuple<int, int>>>>
                                           (curr_state, std::vector<std::tuple<double, std::tuple<int, int>>>()));
    }
    // Iterator to the current action
    std::vector<std::tuple<int, int>>::iterator iter_move(std::find(state_action_space_DynaQ.at(curr_state).begin(),
                                                                    state_action_space_DynaQ.at(curr_state).end(),
                                                                    curr_move));
    // If the action is new to the state model, add it in
    if (iter_move == state_action_space_DynaQ.at(curr_state).end()) {
        state_action_space_DynaQ.at(curr_state).push_back(curr_move);
        state_action_response_DynaQ.at(curr_state).push_back(std::tuple<double, std::tuple<int, int>> {reward, next_state});
    }
    // If the action has been seen, overwrite the previous model with new results
    else {
        int idx_move(static_cast<int>(std::distance(state_action_space_DynaQ.at(curr_state).begin(), iter_move)));
        state_action_response_DynaQ.at(curr_state).at(idx_move) = std::tuple<double, std::tuple<int, int>> {reward, next_state};
    }
};


void MazeModel::memorizeStateAction_DynaQ_Plus(std::tuple<int, int> curr_state, std::tuple<int, int> curr_move,
                                               std::tuple<int, int> next_state, double reward)
{
    // If the state is new, add it into model
    if (state_action_space_DynaQ_Plus.find(curr_state) == state_action_space_DynaQ_Plus.end()) {
        std::vector<std::tuple<int, int>> all_available_action(maze_env.getAvailableMoves(curr_state));
        state_action_space_DynaQ_Plus.insert(std::pair<std::tuple<int, int>, std::vector<std::tuple<int, int>>>
                                             (curr_state, all_available_action));
        state_action_response_DynaQ_Plus.insert(std::pair<std::tuple<int, int>,
                                                std::vector<std::tuple<double, std::tuple<int, int>>>>
                                                (curr_state,
                                                 std::vector<std::tuple<double, std::tuple<int, int>>>
                                                 (static_cast<int>(all_available_action.size()),
                                                  std::tuple<double, std::tuple<int, int>>{0, curr_state})));
    }
    // Iterator to the current action
    std::vector<std::tuple<int, int>>::iterator iter_move(std::find(state_action_space_DynaQ_Plus.at(curr_state).begin(),
                                                                    state_action_space_DynaQ_Plus.at(curr_state).end(),
                                                                    curr_move));
    // Current action must be already in the model
    int idx_move(static_cast<int>(std::distance(state_action_space_DynaQ_Plus.at(curr_state).begin(), iter_move)));
    state_action_response_DynaQ_Plus.at(curr_state).at(idx_move) = std::tuple<double, std::tuple<int, int>>{reward, next_state};
};


MazePastExp MazeModel::getPastResponse_DynaQ() const
{
    MazePastExp past_exp;
    // Static RNG
    static std::mt19937 mersenne_eng(static_cast<std::mt19937::result_type>(std::time(nullptr)));
    // Dynamic RNG for state
    int curr_map_size(static_cast<int>(state_action_space_DynaQ.size()));
    std::uniform_int_distribution<> state_RNG(0, curr_map_size-1);
    
    // Set up constant iterator at the beginning of map
    std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>>::const_iterator iter_state;
    iter_state = state_action_space_DynaQ.begin();
    // Advance the iterator by a random step size
    std::advance(iter_state, state_RNG(mersenne_eng));
    // Get the random key (state)
    std::tuple<int, int> random_state(iter_state->first);
    
    // Dynamic RNG for move
    int curr_move_size(static_cast<int>(state_action_space_DynaQ.at(random_state).size()));
    std::uniform_int_distribution<> move_RNG(0, curr_move_size-1);
    // Get the random action
    int idx_move(move_RNG(mersenne_eng));
    std::tuple<int, int> random_move(state_action_space_DynaQ.at(random_state).at(idx_move));
    
    // Get the response to the random state and action
    std::tuple<double, std::tuple<int, int>> random_response(state_action_response_DynaQ.at(random_state).at(idx_move));
    
    // Store random selection in struct
    past_exp.prev_state = random_state;
    past_exp.prev_move = random_move;
    past_exp.result_reward = std::get<0>(random_response);
    past_exp.result_state = std::get<1>(random_response);
    
    return past_exp;
};


MazePastExp MazeModel::getPastResponse_DynaQ_Plus() const
{
    MazePastExp past_exp;
    // Static RNG
    static std::mt19937 mersenne_eng(static_cast<std::mt19937::result_type>(std::time(nullptr)));
    // Dynamic RNG for state
    int curr_map_size(static_cast<int>(state_action_space_DynaQ_Plus.size()));
    std::uniform_int_distribution<> state_RNG(0, curr_map_size-1);
    
    // Set up constant iterator at the beginning of map
    std::map<std::tuple<int, int>, std::vector<std::tuple<int, int>>>::const_iterator iter_state;
    iter_state = state_action_space_DynaQ_Plus.begin();
    // Advance the iterator by a random step size
    std::advance(iter_state, state_RNG(mersenne_eng));
    // Get the random key (state)
    std::tuple<int, int> random_state(iter_state->first);
    
    // Dynamic RNG for move
    int curr_move_size(static_cast<int>(state_action_space_DynaQ_Plus.at(random_state).size()));
    std::uniform_int_distribution<> move_RNG(0, curr_move_size-1);
    // Get the random action
    int idx_move(move_RNG(mersenne_eng));
    std::tuple<int, int> random_move(state_action_space_DynaQ_Plus.at(random_state).at(idx_move));
    
    // Get the response to the random state and action
    std::tuple<double, std::tuple<int, int>> random_response(state_action_response_DynaQ_Plus.at(random_state).at(idx_move));
    
    // Store random selection in struct
    past_exp.prev_state = random_state;
    past_exp.prev_move = random_move;
    past_exp.result_reward = std::get<0>(random_response);
    past_exp.result_state = std::get<1>(random_response);
    
    return past_exp;
};


void MazeModel::reacquireModel_DynaQ_Plus(std::tuple<int, int> curr_state)
{
    std::vector<std::tuple<int, int>> all_available_action(maze_env.getAvailableMoves(curr_state));
    
    for (std::tuple<int, int> move : all_available_action) {
        if (std::find(state_action_space_DynaQ_Plus.at(curr_state).begin(),
                      state_action_space_DynaQ_Plus.at(curr_state).end(), move) ==
            state_action_space_DynaQ_Plus.at(curr_state).end()) {
            state_action_space_DynaQ_Plus.at(curr_state).push_back(move);
            state_action_response_DynaQ_Plus.at(curr_state).push_back(std::tuple<double, std::tuple<int, int>>{0, curr_state});
        }
    }
};
