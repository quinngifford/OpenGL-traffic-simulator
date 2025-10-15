Ran traffic simulation (with OpenGL graphics for fun) to test light cycle algorithms because I got mad at the lights in Bellevue
This project was made in July 2023, only the repo has been polished since

Light cycle modes (1 intersection)
- Fixed Time Cycle (Disgusting): Just waits a certain amount of time on each light cycle, does not take any car input
- Single Sensor Activation: Each lane has one sensor that detects when a vehicle is waiting. The light stays red until a car triggers the sensor, then switches to green for a short time.
- Double Sensor Activation: Uses two sensors per lane — one near the stop line and one farther back — to track approaching and clearing vehicles. The light stays green as long as cars are detected and switches only when traffic clears.
- Queue Length Control: Continuously measures how many cars are lined up at each light. The green phase lasts longer for lanes with longer queues and shorter for lighter traffic.
- Machine Learning? (would implement if I were to continue this project)

![download (1)](https://github.com/user-attachments/assets/cff1a61c-7cad-4539-a3a6-37990ddbfd64)

Findings:
| Control Type / Algorithm | Avg. Wait Time (s) | Max Queue Length (cars) | Throughput (vehicles/hr) | Congestion Level
|---------------------------|-------------------|------------------------|--------------------------:|------------------|
| **Fixed-Time Cycle**      | 43.6              | 19                     | 870                       | High             |
| **Single Sensor**         | 32.4              | 13                     | 1,010                     | Moderate/High    | 
| **Double Sensor**         | 24.7              | 9                      | 1,180                     | Moderate         | 
| **Queue-Length Control**  | 21.2              | 8                      | 1,240                     | Low-Moderate     |
