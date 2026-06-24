from PyQt6.QtCore import QObject, QTimer, pyqtSlot

class MainController(QObject):
    def __init__(self, view, worker, store, logger, gui_update_ms=50):
        """
        view: MainApplicationWindow (A implementar)
        worker: TelemetryWorker (Thread UDP)
        store: TelemetryStore (Buffers)
        logger: TelemetryLogger (Escrita em disco)
        gui_update_ms: Taxa de atualização do dashboard em milissegundos (50ms = 20Hz)
        """
        super().__init__()
        self.view = view
        self.worker = worker
        self.store = store
        self.logger = logger
        
        # Timer para atualizar a interface gráfica em uma taxa fixa
        self.gui_timer = QTimer(self)
        self.gui_timer.timeout.connect(self.update_dashboard)
        self.gui_update_ms = gui_update_ms

        self.setup_connections()

    def setup_connections(self):
        """Mapeia os sinais da View e do Worker para os Slots lógicos do Controller."""
        
        # Sinais da View (Eventos de Usuário)
        # Assumimos que a View terá estes sinais customizados implementados
        self.view.start_flight_requested.connect(self.start_flight)
        self.view.stop_flight_requested.connect(self.stop_flight)
        self.view.save_flight_requested.connect(self.save_flight)

        # Sinais do Worker (Eventos de Rede)
        self.worker.imu_received.connect(lambda msg: self.process_telemetry('CROW_IMU', msg))
        self.worker.baro_received.connect(lambda msg: self.process_telemetry('CROW_BARO', msg))
        self.worker.gps_received.connect(lambda msg: self.process_telemetry('CROW_GPS', msg))
        self.worker.cmd_received.connect(lambda msg: self.process_telemetry('CROW_COMMAND', msg))
        
        self.worker.error_occurred.connect(self.handle_worker_error)

    @pyqtSlot()
    def start_flight(self):
        """Inicializa a cadeia de aquisição de dados e altera a View."""
        self.logger.start_new_log()
        self.worker.start()  # Inicia a QThread
        self.gui_timer.start(self.gui_update_ms)
        self.view.show_dashboard()

    @pyqtSlot()
    def stop_flight(self):
        """Interrompe a aquisição e solicita o salvamento na View."""
        self.worker.stop()
        self.gui_timer.stop()
        
        # A View deve abrir o QFileDialog e emitir 'save_flight_requested' com o path escolhido
        self.view.prompt_save_dialog()

    @pyqtSlot(str)
    def save_flight(self, destination_path):
        """Consolida o log no destino e reseta a interface."""
        if destination_path:
            success = self.logger.save_log_to_destination(destination_path)
            if not success:
                self.view.show_error_message("Falha ao salvar o arquivo de telemetria.")
        else:
            # Se o usuário cancelar o diálogo de salvamento, interrompemos o log e descartamos o temp
            self.logger.stop_log()
            
        self.view.show_start_screen()

    def process_telemetry(self, msg_type: str, msg_dict: dict):
        """
        Gargalo de I/O em memória. Roteia os dados brutos para o disco e os normaliza para a View.
        Executado na thread principal a cada sinal emitido pelo Worker.
        """
        # 1. Loga o dado bruto IMEDIATAMENTE (disco)
        self.logger.log_message(msg_type, msg_dict)
        
        # 2. Atualiza o DataStore (escala em float na memória)
        if msg_type == 'CROW_IMU':
            self.store.add_imu(msg_dict)
        elif msg_type == 'CROW_BARO':
            self.store.add_baro(msg_dict)
        elif msg_type == 'CROW_GPS':
            self.store.add_gps(msg_dict)
        elif msg_type == 'CROW_COMMAND':
            self.store.update_command_state(msg_dict)

    @pyqtSlot()
    def update_dashboard(self):
        """
        Chamado pelo QTimer a ~20Hz. Extrai os arrays do DataStore e comanda a View a redesenhar.
        """
        if not self.view.is_dashboard_active():
            return

        # Puxa arrays do Store
        t_imu, ax, ay, az = self.store.get_accel_data()
        _, gx, gy, gz = self.store.get_gyro_data()
        t_baro, press, temp = self.store.get_baro_data()
        state = self.store.latest_state

        # Empurra para a View (A View deve ter um método para receber e repintar isso)
        self.view.update_plots(
            accel=(t_imu, ax, ay, az),
            gyro=(t_imu, gx, gy, gz),
            baro=(t_baro, press, temp)
        )
        self.view.update_displays(state)

    @pyqtSlot(str)
    def handle_worker_error(self, error_msg):
        """Tratamento de exceções vindas da thread UDP."""
        self.view.show_error_message(f"Erro de Rede: {error_msg}")
        self.stop_flight()