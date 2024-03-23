# Use the official Ubuntu base image
FROM ubuntu:latest
 
# Install necessary dependencies
RUN apt-get update && \
    apt-get install -y build-essential libgrpc++-dev libprotobuf-dev protobuf-compiler protobuf-compiler-grpc
 
# Set the working directory in the container
WORKDIR /app
 
# Copy the server source code and generated files to the container
COPY server.cpp server.proto ./
 
# Generate protobuf files
RUN protoc --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` server.proto

# Compile the game server
RUN g++ -c server.cpp -o server.o && \
	g++ -c server.grpc.pb.cc -o server.grpc.pb.o && \
	g++ -c server.pb.cc -o server.pb.o && \
	g++ server.o server.grpc.pb.o server.pb.o -lgrpc++ -lprotobuf -lpthread -o server
 
# Expose the gRPC server port
EXPOSE 50051
 
# Run the game server when the container starts
CMD ["./server"]

