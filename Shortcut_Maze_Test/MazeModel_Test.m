//
//  MazeModel_Test.m
//  Shortcut_Maze_Test
//
//  Created by Xiaoyue Ding on 10/26/21.
//

#import <XCTest/XCTest.h>
#import "GridWorld.hpp"
#import "MazeEnv.hpp"
#import "MazeModel.hpp"

@interface MazeModel_Test : XCTestCase {
@private
    GridWorld* test_grid_world;
    MazeEnv* test_maze_env;
    MazeModel* test_maze_model;
}

@end

@implementation MazeModel_Test

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
    std::vector<std::vector<int>> test_input_grid (6, std::vector<int> (9, 0));
    for (int i(1); i<9; ++i) {
        test_input_grid[2][i] = 1;
    }
    std::tuple<int, int> test_start {0, 3};
    std::tuple<int, int> test_goal {5, 8};
    
    test_grid_world = new GridWorld(test_input_grid, test_start, test_goal);
    test_maze_env = new MazeEnv(*test_grid_world);
    test_maze_model = new MazeModel(*test_maze_env);
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    delete test_maze_model;
    delete test_maze_env;
    delete test_grid_world;
    
    test_maze_model = nullptr;
    test_maze_env = nullptr;
    test_grid_world = nullptr;
}

- (void)testMemorizeGetStateAction_DynaQ {
    std::tuple<int, int> test_state_1{1, 8};
    std::tuple<int, int> test_move_1{1, 0}, test_move_2{-1, 0}, test_move_3{0, -1};
    
    MazeResponse test_response_2(test_maze_env->getMazeResponse(test_state_1, test_move_2)),
                 test_response_3(test_maze_env->getMazeResponse(test_state_1, test_move_3));
    
    test_maze_model->memorizeStateAction_DynaQ(test_state_1, test_move_2, test_response_2.next_state, test_response_2.reward);
    
    MazePastExp test_past_response;
    std::vector<std::tuple<int, int>> test_prev_states, test_prev_moves, test_result_states;
    for (int i(0); i<100; ++i) {
        test_past_response = test_maze_model->getPastResponse_DynaQ();
        test_prev_states.push_back(test_past_response.prev_state);
        test_prev_moves.push_back(test_past_response.prev_move);
        test_result_states.push_back(test_past_response.result_state);
    }
    
    XCTAssertTrue(std::find(test_prev_states.begin(), test_prev_states.end(), test_state_1) != test_prev_states.end());
    
    XCTAssertTrue(std::find(test_prev_moves.begin(), test_prev_moves.end(), test_move_1) == test_prev_moves.end());
    XCTAssertTrue(std::find(test_prev_moves.begin(), test_prev_moves.end(), test_move_2) != test_prev_moves.end());
    XCTAssertTrue(std::find(test_prev_moves.begin(), test_prev_moves.end(), test_move_3) == test_prev_moves.end());
    
    XCTAssertTrue(std::find(test_result_states.begin(), test_result_states.end(), test_response_2.next_state)
                  != test_result_states.end());
    XCTAssertTrue(std::find(test_result_states.begin(), test_result_states.end(), test_response_3.next_state)
                  == test_result_states.end());
}


- (void)testMemorizeGetStateAction_DynaQ_Plus {
    std::tuple<int, int> test_state_1{1, 8};
    std::tuple<int, int> test_move_1{1, 0}, test_move_2{-1, 0}, test_move_3{0, -1};
    
    MazeResponse test_response_2(test_maze_env->getMazeResponse(test_state_1, test_move_2)),
                 test_response_3(test_maze_env->getMazeResponse(test_state_1, test_move_3));
    
    test_maze_model->memorizeStateAction_DynaQ_Plus(test_state_1, test_move_2, test_response_2.next_state,
                                                    test_response_2.reward);
    
    MazePastExp test_past_response;
    std::vector<std::tuple<int, int>> test_prev_states, test_prev_moves, test_result_states;
    for (int i(0); i<100; ++i) {
        test_past_response = test_maze_model->getPastResponse_DynaQ_Plus();
        test_prev_states.push_back(test_past_response.prev_state);
        test_prev_moves.push_back(test_past_response.prev_move);
        test_result_states.push_back(test_past_response.result_state);
    }
    
    XCTAssertTrue(std::find(test_prev_states.begin(), test_prev_states.end(), test_state_1) != test_prev_states.end());
    
    XCTAssertTrue(std::find(test_prev_moves.begin(), test_prev_moves.end(), test_move_1) == test_prev_moves.end());
    XCTAssertTrue(std::find(test_prev_moves.begin(), test_prev_moves.end(), test_move_2) != test_prev_moves.end());
    XCTAssertTrue(std::find(test_prev_moves.begin(), test_prev_moves.end(), test_move_3) != test_prev_moves.end());
    
    XCTAssertTrue(std::find(test_result_states.begin(), test_result_states.end(), test_response_2.next_state)
                  != test_result_states.end());
    XCTAssertTrue(std::find(test_result_states.begin(), test_result_states.end(), test_response_3.next_state)
                  == test_result_states.end());
}


- (void)testReAcquireStateAction_DynaQ_Plus {
    std::tuple<int, int> test_state_1{1, 8};
    std::tuple<int, int> test_move_1{1, 0}, test_move_2{-1, 0}, test_move_3{0, -1};
    
    MazeResponse test_response_2(test_maze_env->getMazeResponse(test_state_1, test_move_2)),
                 test_response_3(test_maze_env->getMazeResponse(test_state_1, test_move_3));
    
    test_maze_model->memorizeStateAction_DynaQ_Plus(test_state_1, test_move_2, test_response_2.next_state,
                                                    test_response_2.reward);
    
    test_grid_world->changeGrid(2, 8);
    test_maze_model->reacquireModel_DynaQ_Plus(test_state_1);
    
    MazePastExp test_past_response;
    std::vector<std::tuple<int, int>> test_prev_states, test_prev_moves, test_result_states;
    for (int i(0); i<100; ++i) {
        test_past_response = test_maze_model->getPastResponse_DynaQ_Plus();
        test_prev_states.push_back(test_past_response.prev_state);
        test_prev_moves.push_back(test_past_response.prev_move);
        test_result_states.push_back(test_past_response.result_state);
    }
    
    XCTAssertTrue(std::find(test_prev_states.begin(), test_prev_states.end(), test_state_1) != test_prev_states.end());
    
    XCTAssertTrue(std::find(test_prev_moves.begin(), test_prev_moves.end(), test_move_1) != test_prev_moves.end());
    XCTAssertTrue(std::find(test_prev_moves.begin(), test_prev_moves.end(), test_move_2) != test_prev_moves.end());
    XCTAssertTrue(std::find(test_prev_moves.begin(), test_prev_moves.end(), test_move_3) != test_prev_moves.end());
    
    XCTAssertTrue(std::find(test_result_states.begin(), test_result_states.end(), test_response_2.next_state)
                  != test_result_states.end());
    XCTAssertTrue(std::find(test_result_states.begin(), test_result_states.end(), test_response_3.next_state)
                  == test_result_states.end());
    
    
}

@end
