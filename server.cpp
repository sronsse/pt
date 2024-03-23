#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "server.grpc.pb.h"
 
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using server::GameService;
using server::Empty;
using server::GameState;
 
class GameServiceImpl final : public GameService::Service {
private:
    int gameState;
 
public:
    GameServiceImpl() {
        std::random_device rd;
        gameState = rd() % 100;
    }
 
    Status GetGameState(ServerContext* context, const Empty* request, GameState* response) override {
        response->set_state(gameState);
        return Status::OK;
    }
 
    void RunGameLoop() {
        while (true) {
            // Update game state randomly
            std::random_device rd;
            gameState = rd() % 100;
 
            // Sleep for a short duration to simulate game loop
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};
 
void RunServer() {
    std::string server_address("0.0.0.0:50051");
    GameServiceImpl service;
 
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
 
    std::cout << "Game server listening on " << server_address << std::endl;
 
    // Run game loop in a separate thread
    std::thread gameLoopThread(&GameServiceImpl::RunGameLoop, &service);
 
    server->Wait();
}
 
int main(int argc, char** argv) {
    RunServer();
    return 0;
}

