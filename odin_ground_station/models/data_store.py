from collections import deque
import numpy as np

class TelemetryStore:
    # Fatores de conversão para reverter a quantização aplicada em borda
    SCALES = {
        'time': 1000.0,       # ms -> s
        'accel': 100.0,       # m/s² * 100 -> m/s²
        'gyro': 10.0,         # deg/s * 10 -> deg/s
        'temp': 100.0,        # C * 100 -> C
        'gps_coord': 1e7,     # deg * 1e7 -> deg
        'altitude': 10.0,     # m * 10 -> m
        'speed': 10.0         # m/s * 10 -> m/s
    }

    def __init__(self, max_points=1000):
        """
        max_points: Limite do buffer circular para os gráficos na GUI.
        Impede o estouro de memória durante os ensaios de voo longos do Pegazuls.
        """
        self.max_points = max_points
        
        # Buffers IMU
        self.time_imu = deque(maxlen=max_points)
        self.accel_x = deque(maxlen=max_points)
        self.accel_y = deque(maxlen=max_points)
        self.accel_z = deque(maxlen=max_points)
        self.gyro_x = deque(maxlen=max_points)
        self.gyro_y = deque(maxlen=max_points)
        self.gyro_z = deque(maxlen=max_points)
        
        # Buffers Barômetro
        self.time_baro = deque(maxlen=max_points)
        self.pressure = deque(maxlen=max_points)
        self.temperature = deque(maxlen=max_points)
        
        # Estado atual (Latest values) para os Displays/LCDs da View
        self.latest_state = {
            'lat': 0.0,
            'lon': 0.0,
            'alt_msl': 0.0,
            'ground_speed': 0.0,
            'satellites': 0,
            'command_state': 0
        }

    def add_imu(self, msg: dict):
        """Aplica as escalas e armazena pacote CROW_IMU"""
        t = msg['time_boot_ms'] / self.SCALES['time']
        self.time_imu.append(t)
        
        self.accel_x.append(msg['accel_x'] / self.SCALES['accel'])
        self.accel_y.append(msg['accel_y'] / self.SCALES['accel'])
        self.accel_z.append(msg['accel_z'] / self.SCALES['accel'])
        
        self.gyro_x.append(msg['gyro_x'] / self.SCALES['gyro'])
        self.gyro_y.append(msg['gyro_y'] / self.SCALES['gyro'])
        self.gyro_z.append(msg['gyro_z'] / self.SCALES['gyro'])

    def add_baro(self, msg: dict):
        """Aplica as escalas e armazena pacote CROW_BARO"""
        t = msg['time_boot_ms'] / self.SCALES['time']
        self.time_baro.append(t)
        
        # Assumindo que pressure_delta já esteja em Pascal, sem escala no XML
        self.pressure.append(msg['pressure_delta']) 
        self.temperature.append(msg['temperature'] / self.SCALES['temp'])

    def add_gps(self, msg: dict):
        """Atualiza o estado de navegação (apenas valor mais recente necessário)"""
        self.latest_state['lat'] = msg['latitude'] / self.SCALES['gps_coord']
        self.latest_state['lon'] = msg['longitude'] / self.SCALES['gps_coord']
        self.latest_state['alt_msl'] = msg['altitude_msl'] / self.SCALES['altitude']
        self.latest_state['ground_speed'] = msg['ground_speed'] / self.SCALES['speed']
        self.latest_state['satellites'] = msg['satellites']

    def update_command_state(self, msg: dict):
        """Atualiza o estado do sistema baseado nos comandos do piloto"""
        self.latest_state['command_state'] = msg['command']

    # Métodos de Consumo para a View
    
    def get_accel_data(self):
        return (
            np.array(self.time_imu), 
            np.array(self.accel_x), 
            np.array(self.accel_y), 
            np.array(self.accel_z)
        )

    def get_gyro_data(self):
        return (
            np.array(self.time_imu), 
            np.array(self.gyro_x), 
            np.array(self.gyro_y), 
            np.array(self.gyro_z)
        )

    def get_baro_data(self):
        return (
            np.array(self.time_baro), 
            np.array(self.pressure), 
            np.array(self.temperature)
        )