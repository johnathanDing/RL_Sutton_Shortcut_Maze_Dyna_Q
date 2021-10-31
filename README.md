# Sutton's Shortcut Maze with Dyna-Q and Dyna-Q+


      Before shortcut opens        Dyna-Q after shortcut opens    Dyna-Q+ after shortcut opens

<p float="center">
  <img src="./Examples/No_Shortcut_Recording.gif" width="32%" />
  <img src="./Examples/Dyna-Q_Recording.gif" width="32%" />
  <img src="./Examples/Dyna-Q_Plus_Recording.gif" width="32%" />
</p>

This problem aims to compare and discuss the control methods of Dyna-Q and Dyna-Q+, and their effects on training under various heurisitics and parameter settings.

This solution was written in C++ as main language, on Xcode 13.X platform. Visualization of race track requires installation of the SFML packages.
Unit testings were carried out using XCTest as well. All modules have tests except the visualization module, which cannot be supported by XCTest.

## Problem Statement:
A maze grid world is set up, initially as shown below on left. The agent The optical path would need to go around the left side of the barrier (upper left). After 3000 time steps, however, a shorter path is opened up along the right side, without disturbing the longer path (upper right).

![Maze problem statement](./Examples/Problem_Statement.png)

We would like to try out both Dyna-Q and Dyna-Q+ control methods on this problem, to observe and compare their training results and experiment on their heuristics and parameters.

## Solution Overview:
For the Dyna-Q control method, we straightforwardly follow the algorithm layed out in Sutton's book. After each training step in real experience, we also apply 50 steps of planning, continuing the state-action value updates in the background under simulated experience. The simulated experience are random fragments from all past real experiences "remembered" by an environment model, both deepening the results from real experience, and also back-propagating the training progress of later time steps towards earlier steps, making highly-efficient usage of all real interaction experience with the actual environment.

The Dyna-Q+ control method takes advantage of the high degree of freedom in planning, adding heuristics into the planning steps. One obvious such heuristics is to aribitrarily add a small bonus reward to actions that have not been picked for a long time, even including actions that have never been tried before, since models corresponding to these actions have the highest chance of having changed. The addition of such bonus reward encourages the exploration of not just untried actions, but also rarely visited environment models. 

Depending on how the heurisitic is implemented, Dyna-Q+ can even encourage long sequences of rarely visited state-actions to explore entirely new solutions. Such feature is required to find the new shortcut in maze for our problem. Otherwise, the trained greedy policy will stick to the original longer path without even realizing the opening of a new shorter path. We will demonstrate and discuss all above situations under both control models and various hyperparameter settings.

## Essential Modules
Due to the highly standard structure of RL moduels used in our solution, we'll skip the description of code blocks in our discussion. The audience are welcome to look into the well-documented header files in code folder to gain further insights of the solution structure.

## Result discussion
### 1. The sweetspot solution
Shown below is the training analysis for both Dyna-Q control, and Dyna-Q+ control under its "sweetspot" parameter (kappa for bonus reward) setting. Here our discount factor in training is gamma = 0.95, learning rate alpha = 0.1, soft policy factor epsilon = 0.1, with Dyna-Q+'s kappa = 5e-4. Each step of real experience is followed by 50 steps of randomly sampled planning steps.

<p float="center">
  <img src="./Examples/1_Sweetspot_Solution.jpg" width="48%" />
  <img src="./Examples/1_Sweetspot_Slope.jpg" width="48%" /> 
</p>

As can be seen in the left plot, the Dyna-Q+ agent was able to finish the first episode much sooner (319 steps) compared to the Dyna-Q agent's 642 steps, thanks to the artificial bonus reward encouraging more exploration of the policy and environment in the planning iterations. Afte the first episode, Dyna-Q agent quickly found the optimal solution as well, and maintained the optimal policy indefinitely. Meanwhile, although the Dyna-Q+ agent found the optimal path much quicker, the exploratory nature of the agent kept trying out sub-optimal paths, making the slope of accumulationg of rewards, as shown in the right plot, slower than Dyna-Q. Until after certain iterations, no bonus rewards can interfere with the optimal state-action values anymore, and the two agents align.

The drastic contrast between the Dyna-Q and Dyna-Q+ agents are most prominant after the maze opens up a shortcut at time step of 3000. Since the Dyna-Q agent has no motivation to explore long-unvisited state-actions, it has no means to detect the shortcut, except the usual soft-policy exploration, which has close-to-zero probability of finding out the entire shortcut within limited amount of time. In the constrast, Dyna-Q+ agent's model immediately includes the new actions made available by the shortcut opening, and starts trying it out in real experience after the background planning artifically made the new paths look promising. Such behavior can be observed in the slope in the right plot, where a strong dip in slope at time = 3308 signatures the agent's journey into the new path. Although this slows down the accumulation of rewards temporarily, the agent is able to find the new shortcut whitin 100 steps and switched the overall policy to the new optimal, thus accumulating total rewards at the speed of the new shortcut.

Our parameter settings are the key to the successful experiment discussed above. Our choice of kappa = 5e-4 means that a time interval of ((0.95^16)/5e-4)^2 = 774850 steps would be needed to make our artificial bonus reward exceed the optimal policy at starting position, for the long path at the beginning setup of the maze. If kappa is set as too small, the aritifical bonus would not be enough to encourage the agent trying out entirely new paths. If set too large, the strong desire to explore would leave the agent wandering in the maze, sometimes even without finding the optical path. Another alternative to our current bonus reward treatment, as suggested in Sutton's exercise question, is to only use the bonus reward during greedy action selection, without actually apply the bonus to state-action values. We show and discuss the result of each case below.

### 2. Low bonus parameter
Shown in the below plots are the results of setting kappa = 2e-4, with all other system parameters identical to the 1st result. As observed, even the Dyna-Q+ agent was not able to find the new shortcut path within the limited time. THe only advantage of the Dyna-Q+ heuristics is the quicker finding of first path, compared to the less exploratory Dyna-Q. But apparently, the small amount of bonus is not enough to encourage the exploration of long new paths.

<p float="center">
  <img src="./Examples/2_LowKappa_Solution.jpg" width="48%" />
  <img src="./Examples/2_LowKappa_Slope.jpg" width="48%" /> 
</p>

### 3. Moderately high bonus parameter
Now we switch the bonus parameter to a moderately high value, with kappa = 1e-3.The Dyna-Q+ agent is still able to find the shortcut as shown in case 1, but with apparently more exploratory paths taken along the way. As shown in the slope plot in the right, Dyna-Q+ agent training went through multiple "exploration journeys" even before the shortcut opened up at time step 3000, thus slowing down the accumulation of rewards after both agents found the long path. Such feature also delays the discovery of the actual shortcut after the maze changes, making the turning point of Dyna-Q+ curve much later than the time stamp in case 1.

<p float="center">
  <img src="./Examples/3_ModHighKappa_Solution.jpg" width="48%" />
  <img src="./Examples/3_ModHighKappa_Slope.jpg" width="48%" /> 
</p>

### 4. Too high bonus parameter
As the last data point in our kappa parameter, we look at the consequences of setting the bonus parameter too high. As shown in the plots below, Dyna-Q+ agent spends too much time wandering in the maze. Since the artificial bonus rewards easily exceed the rewards of finding viable maze paths, the agent does not prioritize finding a path at all. This results in extremely low reward accumulation in real experience.

<p float="center">
  <img src="./Examples/4_TooHighKappa_Solution.jpg" width="48%" />
  <img src="./Examples/4_TooHighKappa_Slope.jpg" width="48%" /> 
</p>
