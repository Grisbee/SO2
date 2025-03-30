
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <random>
#include <string>

// Custom mutex implementation
class CustomMutex {
private:
    std::atomic<bool> locked;  

public:
    CustomMutex() : locked(false) {}

    void lock() {
        bool expected = false;
        while (!locked.compare_exchange_weak(expected, true)) {
            expected = false;
            std::this_thread::yield();
        }
    }
    bool try_lock() {
        bool expected = false;
        return locked.compare_exchange_strong(expected, true);
    }

    void unlock() {
        locked.store(false);
    }
};

class ConsoleLock {
private:
    CustomMutex mutex;

public:
    void print(const std::string& message) {
        mutex.lock();
        std::cout << message << std::endl;
        mutex.unlock();
    }
};

// Synchronizing the console 
ConsoleLock console;

// Enum for philosopher state
enum class Current_State {
    Thinking,
    Eating,
    Hungry,
};

// Class representing a philosopher
class Philosopher {
private:
    int id;
    int num_philosophers;
    Current_State state;
    std::vector<CustomMutex>& forks;

    std::mt19937 rng;
    std::uniform_int_distribution<int> think_dist;
    std::uniform_int_distribution<int> eat_dist;

// Trying to pick up the forks
    int leftFork() const {
        return id;
    }

    int rightFork() const {
        return (id + 1) % num_philosophers;
    }

public:
    Philosopher(int philosopherId, int total_philosophers, std::vector<CustomMutex>& forks_ref)
        : id(philosopherId),
        num_philosophers(total_philosophers),
        state(Current_State::Thinking),
        forks(forks_ref),
        rng(std::random_device{}()),
        think_dist(1000, 3000),
        eat_dist(1000, 2000)
    {}

    void dine() {
        for (int i = 0; i < 5; ++i) {  
            think();
            getHungry();
            pickUpForks();
            eat();
            putDownForks();
        }
    }

    // Thinking function 
    void think() {
        setState(Current_State::Thinking);
        reportState();
        std::this_thread::sleep_for(std::chrono::milliseconds(think_dist(rng)));
    }

    // Hunger reporting function
    void getHungry() {
        setState(Current_State::Hungry);
        reportState();
    }

    // Function for picking up forks (deadlock resolved with assymetry: the last philosopher picks up the forks in a reversed manner)
    void pickUpForks() {
        if (id == num_philosophers - 1) {
            console.print("Philosopher " + std::to_string(id) + " is trying to pick up the right fork");
            forks[rightFork()].lock();

            console.print("Philosopher " + std::to_string(id) + " picked up right fork");
            console.print("Philosopher " + std::to_string(id) + " is trying to pick up the left fork");
            forks[leftFork()].lock();

            console.print("Philosopher " + std::to_string(id) + " picked up left fork");
        }
        else {
            console.print("Philosopher " + std::to_string(id) + " is trying to pick up the left fork");
            forks[leftFork()].lock();

            console.print("Philosopher " + std::to_string(id) + " picked up left fork");
            console.print("Philosopher " + std::to_string(id) + " is trying to pick up the right fork");
            forks[rightFork()].lock();

            console.print("Philosopher " + std::to_string(id) + " picked up right fork");
        }
    }

    // Eating function
    void eat() {
        setState(Current_State::Eating);
        reportState();
        std::this_thread::sleep_for(std::chrono::milliseconds(eat_dist(rng)));
    }

    // Function for putting down forks
    void putDownForks() {
        if (id == num_philosophers - 1) {
            forks[leftFork()].unlock();
            console.print("Philosopher " + std::to_string(id) + " put down left fork");

            forks[rightFork()].unlock();
            console.print("Philosopher " + std::to_string(id) + " put down right fork");
        }
        else {
            forks[rightFork()].unlock();
            console.print("Philosopher " + std::to_string(id) + " put down right fork");

            forks[leftFork()].unlock();
            console.print("Philosopher " + std::to_string(id) + " put down left fork");
        }
    }

    // Functions for modifying the states
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

        console.print("Philosopher " + std::to_string(id) + " is " + stateStr);
    }
};

int main(int argc, char* argv[]) {
    int numPhilosophers = 5; //default 5

    if (argc > 1) {
        try {
            numPhilosophers = std::stoi(argv[1]);
            if (numPhilosophers <= 0) {
                std::cerr << "Number of philosophers must be greater than zero!" << std::endl;
                return 1;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
            return 1;
        }
    }

    std::cout << "Dining Philosophers Problem Simulation" << std::endl;
    std::cout << "Number of philosophers: " << numPhilosophers << std::endl;

    std::vector<CustomMutex> forks(numPhilosophers);

    std::vector<Philosopher> philosophers;
    for (int i = 0; i < numPhilosophers; i++) {
        philosophers.emplace_back(i, numPhilosophers, forks);
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