from PyQt6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QGridLayout, QLabel, QPushButton, QGroupBox
from PyQt6.QtCore import Qt
import pyqtgraph as pg

class DashboardScreen(QWidget):
    def __init__(self):
        super().__init__()
        # Configuração global de aparência do PyQtGraph (Fundo escuro, linhas claras)
        pg.setConfigOption('background', '#121212')
        pg.setConfigOption('foreground', '#d3d3d3')
        
        self._setup_ui()

    def _setup_ui(self):
        main_layout = QVBoxLayout()
        self.setLayout(main_layout)

        header_layout = QHBoxLayout()
        
        self.btn_stop = QPushButton("Encerrar Voo e Salvar")
        self.btn_stop.setFixedSize(200, 40)
        self.btn_stop.setStyleSheet("""
            QPushButton { background-color: #dc3545; color: white; font-weight: bold; border-radius: 4px; }
            QPushButton:hover { background-color: #c82333; }
        """)
        
        # Displays numéricos formatados
        self.lbl_gps = QLabel("GPS: Aguardando Fix...")
        self.lbl_speed = QLabel("Velocidade: 0.0 m/s")
        self.lbl_alt = QLabel("Altitude: 0.0 m")
        self.lbl_sats = QLabel("Sats: 0")
        self.lbl_cmd = QLabel("Comando: IDLE")
        
        for lbl in [self.lbl_gps, self.lbl_speed, self.lbl_alt, self.lbl_sats, self.lbl_cmd]:
            lbl.setStyleSheet("font-size: 14px; font-weight: bold; color: #00ffcc; padding: 5px;")
            header_layout.addWidget(lbl)
            
        header_layout.addStretch()
        header_layout.addWidget(self.btn_stop)
        main_layout.addLayout(header_layout)

        grid = QGridLayout()
        main_layout.addLayout(grid)

        # 1. Gráfico de Aceleração
        self.plot_accel = self._create_plot("Aceleração Linear (m/s²)", "Tempo (s)", "m/s²")
        self.curve_accel_x = self.plot_accel.plot(pen=pg.mkPen('r', width=2), name="X")
        self.curve_accel_y = self.plot_accel.plot(pen=pg.mkPen('g', width=2), name="Y")
        self.curve_accel_z = self.plot_accel.plot(pen=pg.mkPen('b', width=2), name="Z")
        grid.addWidget(self.plot_accel, 0, 0)

        # 2. Gráfico de Giroscópio
        self.plot_gyro = self._create_plot("Velocidade Angular (°/s)", "Tempo (s)", "°/s")
        self.curve_gyro_x = self.plot_gyro.plot(pen=pg.mkPen('r', width=2), name="X")
        self.curve_gyro_y = self.plot_gyro.plot(pen=pg.mkPen('g', width=2), name="Y")
        self.curve_gyro_z = self.plot_gyro.plot(pen=pg.mkPen('b', width=2), name="Z")
        grid.addWidget(self.plot_gyro, 1, 0)

        # 3. Gráfico de Pressão Barométrica
        self.plot_baro = self._create_plot("Pressão Diferencial (Pa)", "Tempo (s)", "Pa")
        self.curve_baro = self.plot_baro.plot(pen=pg.mkPen('y', width=2), name="Pressão")
        grid.addWidget(self.plot_baro, 0, 1)

        # 4. Caixa de Status Ambiental (Ocupa o quadrante inferior direito)
        env_group = QGroupBox("Dados Ambientais (Tempo Real)")
        env_group.setStyleSheet("color: white; font-weight: bold;")
        env_layout = QVBoxLayout()
        self.lbl_temp = QLabel("Temperatura interna: 0.0 °C")
        self.lbl_temp.setStyleSheet("font-size: 18px; color: #ff9900;")
        env_layout.addWidget(self.lbl_temp)
        env_group.setLayout(env_layout)
        grid.addWidget(env_group, 1, 1, alignment=Qt.AlignmentFlag.AlignTop)

    def _create_plot(self, title, xlabel, ylabel):
        """Fábrica de PlotWidgets com otimizações nativas de renderização."""
        plot = pg.PlotWidget(title=title)
        plot.showGrid(x=True, y=True, alpha=0.3)
        plot.setLabel('left', ylabel)
        plot.setLabel('bottom', xlabel)
        plot.addLegend(offset=(10, 10))
        
        # Otimizações críticas para telemetria de alta frequência
        plot.setClipToView(True)
        plot.setDownsampling(mode='peak')
        
        return plot


    def update_plots(self, accel, gyro, baro):
        """Recebe arrays NumPy e repinta as curvas utilizando .setData() O(1)"""
        t_imu, ax, ay, az = accel
        if len(t_imu) > 0:
            self.curve_accel_x.setData(t_imu, ax)
            self.curve_accel_y.setData(t_imu, ay)
            self.curve_accel_z.setData(t_imu, az)

        t_imu_g, gx, gy, gz = gyro
        if len(t_imu_g) > 0:
            self.curve_gyro_x.setData(t_imu_g, gx)
            self.curve_gyro_y.setData(t_imu_g, gy)
            self.curve_gyro_z.setData(t_imu_g, gz)

        t_baro, press, temp = baro
        if len(t_baro) > 0:
            self.curve_baro.setData(t_baro, press)
            # Atualiza o label com a última temperatura registrada no array
            self.lbl_temp.setText(f"Temperatura interna: {temp[-1]:.1f} °C")

    def update_displays(self, state):
        """Atualiza os displays estáticos (Strings)"""
        # Formatação de strings para evitar tremulação na UI com casas decimais infinitas
        self.lbl_gps.setText(f"GPS: {state['lat']:.5f}, {state['lon']:.5f}")
        self.lbl_alt.setText(f"Altitude MSL: {state['alt_msl']:.1f} m")
        self.lbl_speed.setText(f"Velocidade: {state['ground_speed']:.1f} m/s")
        self.lbl_sats.setText(f"Sats: {state['satellites']}")
        
        # Mapeamento do CROW_COMMAND_TYPE (XML)
        cmd_map = {0: "IDLE", 1: "START_REC", 2: "STOP_REC"}
        cmd_str = cmd_map.get(state['command_state'], "UNKNOWN")
        self.lbl_cmd.setText(f"Comando: {cmd_str}")