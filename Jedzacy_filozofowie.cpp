#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>

// Enum for philosopher states
enum class Current_State {
    Thinking,
    Eating,
    Hungry,
};

// Philosopher class
class Philosopher {
private:
    int id;
    Current_State state;
    std::mt19937 rng;
    std::uniform_int_distribution<int> think_dist;
    std::uniform_int_distribution<int> eat_dist;

public:
    Philosopher(int philosopherId)
        : id(philosopherId),
        state(Current_State::Thinking),
        rng(std::random_device{}()),
        think_dist(1000, 3000),  
        eat_dist(1000, 2000)    
    {}

    // Dining function
    void dine() {
        for (int i = 0; i < 5; ++i) {  // Each philosopher eats 5 times 
            think();
            getHungry();
            eat();
        }
    }

    // Thinking function
    void think() {
        setState(Current_State::Thinking);
        reportState();
        std::this_thread::sleep_for(std::chrono::milliseconds(think_dist(rng)));
    }

    // Becoming hungry function
    void getHungry() {
        setState(Current_State::Hungry);
        reportState();
    }

    // Eating function
    void eat() {
        setState(Current_State::Eating);
        reportState();
        std::this_thread::sleep_for(std::chrono::milliseconds(eat_dist(rng)));
    }

    void setState(Current_State newState) {
        state = newState;
    }

    void reportState() {
        std::string stateStr;
        switch (state) {
        case Current_State::Thinking:
            stateStr = "thinking";
            break;
        case Current_State::Hungry:
            stateStr = "hungry";
            break;
        case Current_State::Eating:
            stateStr = "eating";
            break;
        }
        std::cout << "Philosopher " << id << " is " << stateStr << std::endl;
    }
};

int main(int argc, char* argv[]) {
    int numPhilosophers = 5;

    std::cout << "Dining Philosophers Problem Simulation" << std::endl;
    std::cout << "Number of philosophers: " << numPhilosophers << std::endl;

    std::vector<Philosopher> philosophers;
    for (int i = 0; i < numPhilosophers; i++) {
        philosophers.emplace_back(i);
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < numPhilosophers; i++) {
        threads.emplace_back(&Philosopher::dine, &philosophers[i]);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Simulation ended" << std::endl;
    return 0;
}