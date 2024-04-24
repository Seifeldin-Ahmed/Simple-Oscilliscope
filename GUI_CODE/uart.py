import serial.tools.list_ports
from serial.serialutil import SerialException, SerialTimeoutException


class SerialCommunication:
    serial_port = serial.Serial()

    def __init__(self):
        self.integer_data = []

        SerialCommunication.start_communication()

    @staticmethod
    def start_communication():
        SerialCommunication.get_default_data()
        if not SerialCommunication.serial_port.is_open:
            SerialCommunication.serial_port.open()
        SerialCommunication.get_default_data()

    def execute_serial_transaction(self):
        try:
            self.send_data()
            print("Transaction successful.")
        except Exception as e:
            print("Error: Port not open.")

    def execute_serial_receiving(self):
        try:
            data = self.read_data()
            print("Receive successful.")
            print(f"Data Received: {data}")
        except Exception as e:
            print("Error: Port not open.")

    @staticmethod
    def get_default_data():
        SerialCommunication.serial_port.port = 'COM1'
        SerialCommunication.serial_port.baudrate = int(9600)
        SerialCommunication.serial_port.bytesize = int(8)
        SerialCommunication.serial_port.parity = 'N'
        SerialCommunication.serial_port.stopbits = int(1)
        SerialCommunication.serial_port.timeout = int(0)

    def send_data(self):
        try:
            if not SerialCommunication.serial_port.is_open:
                print("Error: Port not open.")
                return False
            else:
                self.serial_port.write('G'.encode())
        except SerialTimeoutException as e:
            print("Error: Timeout.")
            raise e
        except SerialException as e:
            print("Error: Configuration error.")
            raise e

    def read_data(self):
        try:
            received_data = self.serial_port.read(1400).decode()
            self.integer_data.extend(ord(char) for char in received_data)
            print(self.integer_data)
            return self.integer_data
        except SerialTimeoutException as e:
            print("Error: Timeout.")
            raise e
        except SerialException as e:
            print("Error: Configuration error.")
            raise e
