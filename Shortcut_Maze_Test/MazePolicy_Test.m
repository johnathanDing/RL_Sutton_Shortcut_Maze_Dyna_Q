//
//  MazePolicy_Test.m
//  Shortcut_Maze_Test
//
//  Created by Xiaoyue Ding on 10/23/21.
//

#import <XCTest/XCTest.h>
#import "GridWorld.hpp"
#import "MazeEnv.hpp"
#import "MazePolicy.hpp"

@interface MazePolicy_Test : XCTestCase {
@private
    GridWorld* test_grid_world;
    MazeEnv* test_maze_env;
    MazePolicy* test_maze_policy;
}

@end

@implementation MazePolicy_Test

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
    test_maze_policy = new MazePolicy(*test_maze_env, 0.1, 0.1, 0.95, 0.1);
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    delete test_maze_policy;
    delete test_maze_env;
    delete test_grid_world;
    
    test_maze_policy = nullptr;
    test_maze_env = nullptr;
    test_grid_world = nullptr;
}

- (void)testUpdateGetStateActionVal_DynaQ {
    std::tuple<int, int> test_state_1{0, 3}, test_state_2{4, 8};
    std::tuple<int, int> test_move_1{1, 0}, test_move_2{1, 0};
    MazeResponse test_response_1(test_maze_env->getMazeResponse(test_state_1, test_move_1));
    test_maze_policy->updateStateActionVal_DynaQ(test_state_1, test_move_1,
                                                 test_response_1.next_state, test_response_1.reward);
    XCTAssertEqual(test_maze_policy->getStateActionVal_DynaQ(test_state_1, test_move_1), 0.0);
    
    MazeResponse test_response_2(test_maze_env->getMazeResponse(test_state_2, test_move_2));
    test_maze_policy->updateStateActionVal_DynaQ(test_state_2, test_move_2,
                                                 test_response_2.next_state, test_response_2.reward);
    XCTAssertEqual(test_maze_policy->getStateActionVal_DynaQ(test_state_2, test_move_2), 0.1);
}

- (void)testUpdateGetStateActionVal_DynaQ_Plus {
    std::tuple<int, int> test_state_1{0, 3}, test_state_2{4, 8};
    std::tuple<int, int> test_move_1{1, 0}, test_move_2{1, 0};
    
    MazeResponse test_response_1(test_maze_env->getMazeResponse(test_state_1, test_move_1));
    
    test_maze_policy->updateStateActionVal_DynaQ_Plus(test_state_1, test_move_1,
                                                      test_response_1.next_state, test_response_1.reward,
                                                      0, false);
    XCTAssertEqual(test_maze_policy->getStateActionVal_DynaQ_Plus(test_state_1, test_move_1), 0.0);
    
    test_maze_policy->updateStateActionVal_DynaQ_Plus(test_state_1, test_move_1,
                                                      test_response_1.next_state, test_response_1.reward,
                                                      100, true);
    XCTAssertEqual(test_maze_policy->getStateActionVal_DynaQ_Plus(test_state_1, test_move_1), 0.1);
    
    MazeResponse test_response_2(test_maze_env->getMazeResponse(test_state_2, test_move_2));
    
    test_maze_policy->updateStateActionVal_DynaQ_Plus(test_state_2, test_move_2,
                                                      test_response_2.next_state, test_response_2.reward,
                                                      0, false);
    XCTAssertEqual(test_maze_policy->getStateActionVal_DynaQ_Plus(test_state_2, test_move_2), 0.1);
    
    test_maze_policy->updateStateActionVal_DynaQ_Plus(test_state_2, test_move_2,
                                                      test_response_2.next_state, test_response_2.reward,
                                                      100, true);
    XCTAssertEqualWithAccuracy(test_maze_policy->getStateActionVal_DynaQ_Plus(test_state_2, test_move_2), 0.29, 1e-10);
}

- (void)testGetGreedyPolicy_DynaQ {
    std::tuple<int, int> test_state_1{4, 8};
    std::tuple<int, int> test_move_1{1, 0}, test_move_2{-1, 0}, test_move_3{0, -1};
    
    MazeResponse test_response_1(test_maze_env->getMazeResponse(test_state_1, test_move_1)),
                 test_response_2(test_maze_env->getMazeResponse(test_state_1, test_move_2)),
                 test_response_3(test_maze_env->getMazeResponse(test_state_1, test_move_3));
    
    test_maze_policy->updateStateActionVal_DynaQ(test_state_1, test_move_1, test_response_1.next_state, test_response_1.reward);
    test_maze_policy->updateStateActionVal_DynaQ(test_state_1, test_move_2, test_response_2.next_state, test_response_2.reward);
    test_maze_policy->updateStateActionVal_DynaQ(test_state_1, test_move_3, test_response_3.next_state, test_response_3.reward);
    
    XCTAssertEqual(test_maze_policy->getGreedyPolicy_DynaQ(test_state_1), std::make_tuple(1, 0));
}

- (void)testGetGreedyPolicy_DynaQ_Plus {
    std::tuple<int, int> test_state_1{4, 8};
    std::tuple<int, int> test_move_1{1, 0}, test_move_2{-1, 0}, test_move_3{0, -1};
    
    MazeResponse test_response_1(test_maze_env->getMazeResponse(test_state_1, test_move_1)),
                 test_response_2(test_maze_env->getMazeResponse(test_state_1, test_move_2)),
                 test_response_3(test_maze_env->getMazeResponse(test_state_1, test_move_3));
    
    test_maze_policy->updateStateActionVal_DynaQ_Plus(test_state_1, test_move_1,
                                                      test_response_1.next_state, test_response_1.reward,
                                                      0, false);
    test_maze_policy->updateStateActionVal_DynaQ_Plus(test_state_1, test_move_2,
                                                      test_response_2.next_state, test_response_2.reward,
                                                      101, true);
    XCTAssertEqual(test_maze_policy->getGreedyPolicy_DynaQ_Plus(test_state_1, 101, false), std::make_tuple(-1, 0));
    
    test_maze_policy->updateStateActionVal_DynaQ_Plus(test_state_1, test_move_1,
                                                      test_response_1.next_state, test_response_1.reward,
                                                      999, false);
    XCTAssertEqual(test_maze_policy->getGreedyPolicy_DynaQ_Plus(test_state_1, 1001, true), std::make_tuple(0, -1));
}

- (void)testReAcquireStateActionSpace_DynaQ {
    std::tuple<int, int> test_state_1{1, 8};
    std::tuple<int, int> test_move_1{-1, 0}, test_move_2{0, -1}, test_move_3{1, 0};
    
    MazeResponse test_response_1(test_maze_env->getMazeResponse(test_state_1, test_move_1)),
                 test_response_2(test_maze_env->getMazeResponse(test_state_1, test_move_2));
    
    test_maze_policy->updateStateActionVal_DynaQ(test_state_1, test_move_1,
                                                 test_response_1.next_state, test_response_1.reward);
    
    XCTAssertThrows(test_maze_policy->getStateActionVal_DynaQ(test_state_1, test_move_3));
    
    test_grid_world->changeGrid(2, 8);
    test_maze_policy->reAcquireStateActionSpace_DynaQ(test_state_1);
    XCTAssertEqual(test_maze_policy->getStateActionVal_DynaQ(test_state_1, test_move_3), 0.0);
}

- (void)testReAcquireStateActionSpace_DynaQ_Plus {
    std::tuple<int, int> test_state_1{1, 8};
    std::tuple<int, int> test_move_1{-1, 0}, test_move_2{0, -1}, test_move_3{1, 0};
    
    MazeResponse test_response_1(test_maze_env->getMazeResponse(test_state_1, test_move_1)),
                 test_response_2(test_maze_env->getMazeResponse(test_state_1, test_move_2));
    
    test_maze_policy->updateStateActionVal_DynaQ_Plus(test_state_1, test_move_1,
                                                      test_response_1.next_state, test_response_1.reward,
                                                      0, false);
    
    XCTAssertThrows(test_maze_policy->getStateActionVal_DynaQ_Plus(test_state_1, test_move_3));
    
    test_grid_world->changeGrid(2, 8);
    test_maze_policy->reAcquireStateActionSpace_DynaQ_Plus(test_state_1, 101);
    XCTAssertEqual(test_maze_policy->getStateActionVal_DynaQ_Plus(test_state_1, test_move_3), 0.0);
}

@end
