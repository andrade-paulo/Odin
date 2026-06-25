import sys
from PyQt6.QtWidgets import QApplication

# Importação das camadas MVC
from models.telemetry_worker import TelemetryWorker
from models.data_store import TelemetryStore
from models.logger import TelemetryLogger
from views.main_window import MainApplicationWindow
from controllers.main_controller import MainController

def main():
    app = QApplication(sys.argv)
    
    app.setStyle("Fusion") 
    worker = TelemetryWorker(udp_ip="0.0.0.0", udp_port=14550)
    store = TelemetryStore(max_points=1000)
    logger = TelemetryLogger()

    view = MainApplicationWindow()

    controller = MainController(
        view=view, 
        worker=worker, 
        store=store, 
        logger=logger, 
        gui_update_ms=50
    )

    view.show()

    sys.exit(app.exec())

if __name__ == "__main__":
    main()