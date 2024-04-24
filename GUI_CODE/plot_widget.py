from PyQt5.QtGui import QFont
from PyQt5.QtWidgets import QWidget, QLabel, QHBoxLayout, QFormLayout, QVBoxLayout
import pyqtgraph as pg


class PlotWidget(QWidget):

    def __init__(self,channel_name):
        super().__init__()

        self.channel_name = channel_name

        self.init_ui()
        self.set_size()

    def init_ui(self):

        self.create_widgets()
        self.create_layouts()
        self.plot_appearance()

    def create_widgets(self):
        self.channel_label = QLabel(f'Channel {self.channel_name}')
        self.frequancy_label = QLabel("Frequancy: ")
        self.peak_volatage_label = QLabel("Peak Voltage: ")
        self.average_volatage_label = QLabel("Average Voltage: ")
        self.plot_widget = pg.PlotWidget()

    def create_layouts(self):
        self.label_layout = QFormLayout()

        v_spacing = 15
        h_spacing = 1

        self.label_layout.setVerticalSpacing(v_spacing)
        self.label_layout.setHorizontalSpacing(h_spacing)

        layout1 = QHBoxLayout()
        layout1.addWidget(self.plot_widget)

        layout2 = QHBoxLayout()
        layout2.addWidget(self.frequancy_label)
        layout2.addWidget(self.peak_volatage_label)
        layout2.addWidget(self.average_volatage_label)

        self.v_layout = QVBoxLayout()
        self.v_layout.addLayout(layout1)
        self.v_layout.addLayout(layout2)

        self.setLayout(self.v_layout)
        self.edit_labels()

    def edit_labels(self):
        self.frequancy_label.setFixedSize(250, 30)
        self.peak_volatage_label.setFixedSize(250, 30)
        self.average_volatage_label.setFixedSize(250, 30)

        self.font = QFont("Times New Roman", 16, QFont.Bold)
        self.frequancy_label.setFont(self.font)
        self.peak_volatage_label.setFont(self.font)
        self.average_volatage_label.setFont(self.font)

    def plot_appearance(self):
        self.plot_widget.setBackground((255, 255, 255))

        self.plot_widget.setTitle(f'Channel {self.channel_name} Data', color='k')

        self.plot_widget.setLabel('bottom', 'Time', color='k')
        self.plot_widget.setLabel('left', 'Amplitude', color='k')

        pen = pg.mkPen(color=(0, 120, 255), width=2)
        self.plot_widget.plot(pen=pen, symbol='o', symbolSize=5)
        self.plot_widget.getAxis("bottom").setTextPen((0, 0, 0))
        self.plot_widget.getAxis("left").setTextPen((0, 0, 0))

        self.plot_widget.showGrid(x=True, y=True, alpha=0.7)
        self.plot_widget.addLegend()

        self.plot_widget.setMouseEnabled(x=True, y=True)

    def set_size(self):
        self.plot_widget.setMinimumWidth(700)
        self.plot_widget.setMaximumWidth(1300)

        self.plot_widget.setMinimumHeight(200)
        self.plot_widget.setMaximumHeight(400)

    def update_labels(self, frequency, peak_voltage, average_voltage):
        self.frequancy_label.setText(f"Frequancy: {frequency}k")
        self.peak_volatage_label.setText(f"Peak Voltage: {peak_voltage}v")
        self.average_volatage_label.setText(f"Average Voltage: {average_voltage}v")

    def clear(self):
        self.plot_widget.setData([], [])
