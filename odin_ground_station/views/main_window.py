from PyQt6.QtWidgets import QMainWindow, QStackedWidget, QFileDialog, QMessageBox
from PyQt6.QtCore import pyqtSignal

from .start_screen import StartScreen
from .dashboard_screen import DashboardScreen

class MainApplicationWindow(QMainWindow):
    # Sinais estritos esperados pelo MainController
    start_flight_requested = pyqtSignal()
    stop_flight_requested = pyqtSignal()
    save_flight_requested = pyqtSignal(str) 

    def __init__(self):
        super().__init__()
        self.setWindowTitle("Crow Telemetry Ground Station")
        self.resize(1280, 720) # Resolução base segura para dashboards

        self.stacked_widget = QStackedWidget()
        self.setCentralWidget(self.stacked_widget)

        # Instanciação estática das sub-views
        self.start_screen = StartScreen()
        self.dashboard_screen = DashboardScreen()

        self.stacked_widget.addWidget(self.start_screen)
        self.stacked_widget.addWidget(self.dashboard_screen)

        self._setup_internal_routing()

    def _setup_internal_routing(self):
        """Acopla os eventos nativos das telas filhas aos sinais expostos ao Controller."""
        self.start_screen.btn_start.clicked.connect(self.start_flight_requested.emit)
        self.dashboard_screen.btn_stop.clicked.connect(self.stop_flight_requested.emit)

    # API de Controle de Estado
    
    def show_dashboard(self):
        self.stacked_widget.setCurrentWidget(self.dashboard_screen)

    def show_start_screen(self):
        self.stacked_widget.setCurrentWidget(self.start_screen)

    def is_dashboard_active(self):
        """Evita ciclos de CPU repintando gráficos se a tela não estiver visível."""
        return self.stacked_widget.currentWidget() == self.dashboard_screen

    # API de Injeção de Dados
    
    def update_plots(self, accel, gyro, baro):
        self.dashboard_screen.update_plots(accel, gyro, baro)

    def update_displays(self, state):
        self.dashboard_screen.update_displays(state)

    # Interações de Sistema Operacional
    
    def prompt_save_dialog(self):
        """Trava a thread principal aguardando I/O do usuário."""
        # Se cancelar, o QFileDialog retorna path vazio (''). O Controller já trata isso.
        path, _ = QFileDialog.getSaveFileName(
            self, 
            "Salvar Telemetria Consolidada", 
            "telemetria_voo.csv", 
            "CSV Files (*.csv)"
        )
        self.save_flight_requested.emit(path)

    def show_error_message(self, message):
        QMessageBox.critical(self, "Erro Crítico de Sistema", message)