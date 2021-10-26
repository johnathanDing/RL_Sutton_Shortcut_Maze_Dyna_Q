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



