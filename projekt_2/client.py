import socket
import threading
import sys


class ChatClient:
    """
    Chat client that can send and receive messages.
    """

    def __init__(self, host='localhost', port=8888):
        self.host = host
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        """Connect to the chat server."""
        try:
            self.socket.connect((self.host, self.port))
            print(f"Connected to server at {self.host}:{self.port}")

            receive_thread = threading.Thread(target=self.receive_messages)
            receive_thread.daemon = True
            receive_thread.start()

            self.send_messages()

        except Exception as e:
            print(f"Error connecting to server: {e}")
        finally:
            self.socket.close()

    def receive_messages(self):
        """Receive and display messages from the server."""
        try:
            while True:
                data = self.socket.recv(1024)
                if not data:
                    break
                print(data.decode('utf-8'), end='')
        except Exception as e:
            print(f"Error receiving messages: {e}")

    def send_messages(self):
        """Send messages to the server."""
        try:
            print("Type your messages (press Ctrl+C to exit):")
            while True:
                message = input()
                self.socket.send(message.encode('utf-8'))
        except KeyboardInterrupt:
            print("Disconnecting from server...")
        except Exception as e:
            print(f"Error sending messages: {e}")


if __name__ == "__main__":
    host = sys.argv[1] if len(sys.argv) > 1 else 'localhost'
    port = int(sys.argv[2]) if len(sys.argv) > 2 else 8888

    client = ChatClient(host, port)
    client.connect()