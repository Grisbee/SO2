import socket
import threading


class CustomLock:
    """
    Custom implementation of a lock for thread synchronization.
    """

    def __init__(self):
        self.locked = False
        self.event = threading.Event()
        self.lock_operation = threading.Lock()

    def acquire(self):
        """Acquire the lock, blocking if necessary."""
        while True:
            with self.lock_operation:
                if not self.locked:
                    self.locked = True
                    return
            self.event.wait(0.05)
            self.event.clear()

    def release(self):
        """Release the lock."""
        with self.lock_operation:
            self.locked = False
            self.event.set()


class ChatServer:
    """
    Chat server that handles multiple client connections.
    """

    def __init__(self, host='localhost', port=8888):
        self.host = host
        self.port = port
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.clients = []
        self.messages = []

        self.clients_lock = CustomLock()
        self.messages_lock = CustomLock()

    def start(self):
        """Start the chat server."""
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)
        print(f"Server started on {self.host}:{self.port}")

        try:
            while True:
                client_socket, address = self.server_socket.accept()
                print(f"New connection from {address}")

                # Create a new thread for each client
                client_thread = threading.Thread(
                    target=self.handle_client,
                    args=(client_socket, address)
                )
                client_thread.daemon = True
                client_thread.start()
        except KeyboardInterrupt:
            print("Server shutting down...")
        finally:
            self.server_socket.close()

    def handle_client(self, client_socket, address):
        """
        Handle communication with a client.
        Each client runs in its own thread.
        """
        self.clients_lock.acquire()
        self.clients.append(client_socket)
        self.clients_lock.release()

        welcome_msg = f"Welcome to the chat! {len(self.clients)} users online.\n"
        client_socket.send(welcome_msg.encode('utf-8'))

        try:
            while True:
                data = client_socket.recv(1024)
                if not data:
                    break

                message = data.decode('utf-8')
                formatted_message = f"[{address[0]}:{address[1]}]: {message}\n"

                # Add message to history (critical section)
                self.messages_lock.acquire()
                self.messages.append(formatted_message)
                self.messages_lock.release()

                # Broadcast message to all clients (critical section)
                self.clients_lock.acquire()
                disconnected_clients = []
                for client in self.clients:
                    try:
                        client.send(formatted_message.encode('utf-8'))
                    except:
                        # Mark client for removal
                        disconnected_clients.append(client)

                # Remove disconnected clients
                for client in disconnected_clients:
                    if client in self.clients:
                        self.clients.remove(client)
                self.clients_lock.release()

        except Exception as e:
            print(f"Error handling client {address}: {e}")
        finally:
            # Remove client from the list (critical section)
            self.clients_lock.acquire()
            if client_socket in self.clients:
                self.clients.remove(client_socket)
            self.clients_lock.release()
            client_socket.close()
            print(f"Connection from {address} closed")


if __name__ == "__main__":
    server = ChatServer()
    server.start()