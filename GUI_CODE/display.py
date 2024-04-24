from PyQt5.QtGui import QGuiApplication, QColor, QFont
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QVBoxLayout, QWidget, QLabel, QHBoxLayout, \
    QScrollArea
import sys
from plot_widget import PlotWidget
from uart import SerialCommunication
from scipy.interpolate import interp1d
import numpy as np

class LogicAnalyzerMainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Oscilloscope")
        self.setGeometry(100, 100, 1100, 900)
        self.center()
        self.serial = SerialCommunication()

        self.initialize_data_lists()
        self.create_widgets()
        self.configure_push_buttons()
        self.connect_signals()
        self.create_layout()

    def center(self):
        q_rect = self.frameGeometry()
        center_point = QGuiApplication.primaryScreen().availableGeometry().center()
        q_rect.moveCenter(center_point)
        self.move(q_rect.topLeft())

    def initialize_data_lists(self):
        self.channel_numbers = 1
        self.time = []
        self.integer_data = []

        for channel in range(1, self.channel_numbers + 1):
            setattr(self, f'x{channel}', [])
            setattr(self, f'y{channel}', [])

    def create_widgets(self):
        self.start_button = QPushButton("Start")
        self.plot_widgets = [PlotWidget(channel) for channel in range(self.channel_numbers)]

    def connect_signals(self):
        self.start_button.clicked.connect(self.start_data_collection)

    def configure_push_buttons(self):
        self.configure_button(self.start_button)

    def configure_button(self, button):
        button.setGeometry(10, 470, 140, 30)
        color = QColor.fromRgb(44, 131, 215)
        font = QFont()
        font.setPointSize(13)
        button.setFont(font)
        button.setStyleSheet(f"QPushButton{{ background-color: {color.name()} ;"
                             "border-style: solid;"
                             "border-width: 5px;"
                             "border-radius: 10px;}"
                             "QPushButton:hover{ background-color: lightgray;}"
                             )

    def create_layout(self):
        plot_layout = QVBoxLayout()

        for plot in self.plot_widgets:
            plot_layout.addWidget(plot)

        button_widget = QWidget()
        button_layout = QHBoxLayout(button_widget)
        button_layout.addWidget(self.start_button)

        main_widget = QWidget()
        main_layout = QVBoxLayout(main_widget)
        main_layout.addLayout(plot_layout)
        main_layout.addWidget(button_widget)

        scroll_area = QScrollArea()
        scroll_area.setWidgetResizable(True)
        scroll_area.setWidget(main_widget)

        self.setCentralWidget(scroll_area)

    def start_data_collection(self):
        try:
            frames = self.serial.read_data()
            self.parse_data(frames)
            print("y1:", self.y1)
            print("x1:", self.x1)
            self.plot_waveforms()
            print("len(y1):", len(self.y1))
            self.update_statistics()
            self.clear_data_lists()
            self.plot_widgets.clear()
            self.serial.send_data()
        except KeyboardInterrupt:
            self.serial.serial_port.close()

    def getpostions(self, ind, l):
        firstindex = l.index(ind)
        for firstindexdash in range(firstindex, len(l)):
            if l[firstindexdash] != ind:
                break
        sublist = l[firstindexdash:]
        secondindex = sublist.index(ind)
        for secondindexdash in range(secondindex, len(sublist)):
            if sublist[secondindexdash] != ind:
                break
        secondindex = secondindex + firstindexdash
        secondindexdash = secondindexdash + firstindexdash - 1
        firstindexdash = firstindexdash - 1
        return (firstindex, firstindexdash, secondindex, secondindexdash)

    def getStatistics(self):
        try:
            Peak_Voltage = str(round(max(self.y1), 2))
            Average_Voltage = str(round(sum(self.y1) / len(self.y1), 2))
            print("Peak Voltage: ", Peak_Voltage, "v")
            print("Average Voltage: ", Average_Voltage, "v")
            l = self.y1
            t = self.x1
            firstindex, firstindexdash, secondindex, secondindexdash = self.getpostions(min(l), l)
            time1 = sum(t[firstindex:firstindexdash + 1]) / len(t[firstindex:firstindexdash + 1])
            time2 = sum(t[secondindex:secondindexdash + 1]) / len(t[secondindex:secondindexdash + 1])
            period = time2 - time1
            Freq = str(round(1 / (period / 1000), 2))
            print("FREQ: ", Freq, "k")
            return Peak_Voltage, Average_Voltage, Freq

        except:
            pass

    def clear_data_lists(self):
        for channel in range(1, self.channel_numbers + 1):
            setattr(self, f'x{channel}', [])
            setattr(self, f'y{channel}', [])

        self.integer_data.clear()

    def plot_waveforms(self):
        f_interp = interp1d(self.x1, self.y1, kind='cubic')
        time_smooth = np.linspace(min(self.x1), max(self.x1), 1000)
        amp_smooth = f_interp(time_smooth)
        for i, plot_widget in enumerate(self.plot_widgets):
            #plot_widget.plot_widget.plot(self.x1, self.y1, pen=(255, 0, 0))
            plot_widget.plot_widget.plot(time_smooth, amp_smooth, pen=(255, 0, 0))
    def parse_data(self, integer_data):
        for i in range(200):
            front = i * 7
            y_value_int = integer_data[front]
            y_value_fra = integer_data[front + 1]
            x_value = self.concatenate_time(integer_data[front + 2], integer_data[front + 3],
                                            integer_data[front + 4], integer_data[front + 5],
                                            integer_data[front + 6])
            x_list = getattr(self, f'x1')
            y_list = getattr(self, f'y1')
            y_value = y_value_int + (y_value_fra/10)
            x_list.append(x_value)
            y_list.append(y_value)

    def concatenate_time(self, most_field, forth_field, third_field, second_field, first_field):
        return (most_field << 25) | (forth_field << 18) | (third_field << 11) | (second_field << 4) | (first_field << 0)

    def update_statistics(self):
        peak_voltage, average_voltage, Freq = self.getStatistics()
        for i, plot_widget in enumerate(self.plot_widgets):
            plot_widget.update_labels(Freq, peak_voltage,average_voltage)



if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = LogicAnalyzerMainWindow()
    window.show()
    sys.exit(app.exec_())
