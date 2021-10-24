//
//  MazeEnv_Test.m
//  Shortcut_Maze_Test
//
//  Created by Xiaoyue Ding on 10/20/21.
//

#import <XCTest/XCTest.h>
#import "GridWorld.hpp"
#import "MazeEnv.hpp"

@interface MazeEnv_Test : XCTestCase {
@private
    GridWorld* test_grid_world;
    MazeEnv* test_maze_env;
}

@end

@implementation MazeEnv_Test

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
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    delete test_maze_env;
    delete test_grid_world;
    test_maze_env = nullptr;
    test_grid_world = nullptr;
}

- (void)testGetAvailableMoves {
    std::vector<std::tuple<int, int>> got_moves (test_maze_env->getAvailableMoves(std::make_tuple(1, 8)));
    XCTAssertTrue(std::find(got_moves.begin(), got_moves.end(), std::make_tuple(-1, 0)) != got_moves.end());
    XCTAssertTrue(std::find(got_moves.begin(), got_moves.end(), std::make_tuple(0, -1)) != got_moves.end());
    XCTAssertTrue(std::find(got_moves.begin(), got_moves.end(), std::make_tuple(1, 0)) == got_moves.end());
    XCTAssertTrue(std::find(got_moves.begin(), got_moves.end(), std::make_tuple(0, 1)) == got_moves.end());
    
    test_grid_world->changeGrid(2, 8);
    got_moves = test_maze_env->getAvailableMoves(std::make_tuple(1, 8));
    XCTAssertTrue(std::find(got_moves.begin(), got_moves.end(), std::make_tuple(-1, 0)) != got_moves.end());
    XCTAssertTrue(std::find(got_moves.begin(), got_moves.end(), std::make_tuple(0, -1)) != got_moves.end());
    XCTAssertTrue(std::find(got_moves.begin(), got_moves.end(), std::make_tuple(1, 0)) != got_moves.end());
    XCTAssertTrue(std::find(got_moves.begin(), got_moves.end(), std::make_tuple(0, 1)) == got_moves.end());
}

- (void)testGetMazeResponse {
    MazeResponse test_response;
    XCTAssertThrows(test_response = test_maze_env->getMazeResponse(std::make_tuple(1, 8), std::make_tuple(1, 0)));
    
    test_response = test_maze_env->getMazeResponse(std::make_tuple(4, 8), std::make_tuple(1, 0));
    XCTAssertEqual(test_response.next_state, std::make_tuple(5, 8));
    XCTAssertEqual(test_response.reward, 1);
    XCTAssertTrue(test_response.finished);
    
    test_grid_world->changeGrid(2, 8);
    test_response = test_maze_env->getMazeResponse(std::make_tuple(1, 8), std::make_tuple(1, 0));
    XCTAssertEqual(test_response.next_state, std::make_tuple(2, 8));
    XCTAssertEqual(test_response.reward, 0);
    XCTAssertFalse(test_response.finished);
}

@end
