from PyQt6.QtWidgets import QWidget, QVBoxLayout, QPushButton, QLabel
from PyQt6.QtCore import Qt

class StartScreen(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        layout.setAlignment(Qt.AlignmentFlag.AlignCenter)

        title = QLabel("Sistema de Telemetria - Pegazuls AeroDesign")
        title.setStyleSheet("font-size: 28px; font-weight: bold; margin-bottom: 20px;")
        title.setAlignment(Qt.AlignmentFlag.AlignCenter)

        self.btn_start = QPushButton("Registrar Novo Voo")
        self.btn_start.setFixedSize(250, 60)
        self.btn_start.setStyleSheet("""
            QPushButton {
                font-size: 16px;
                font-weight: bold;
                background-color: #28a745;
                color: white;
                border-radius: 5px;
            }
            QPushButton:hover { background-color: #218838; }
        """)

        layout.addWidget(title)
        layout.addWidget(self.btn_start, alignment=Qt.AlignmentFlag.AlignCenter)
        
        self.setLayout(layout)