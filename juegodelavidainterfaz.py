import numpy as np
import sys
import matplotlib
matplotlib.use('Qt5Agg')
from PyQt6 import QtCore, QtWidgets
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg
from matplotlib.figure import Figure
import random
from scipy.signal import convolve2d
import serial
import threading
from playsound import playsound  # Necesitarás instalar esta biblioteca

class MplCanvas(FigureCanvasQTAgg):
    def __init__(self, parent=None, width=5, height=4, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(111)
        super(MplCanvas, self).__init__(fig)

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.sc = MplCanvas(self, width=5, height=4, dpi=100)
        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(self.sc)
        widget = QtWidgets.QWidget()
        widget.setLayout(layout)
        self.setCentralWidget(widget)
        N = 100  # Tamaño de la grilla NxN
        self.grid = np.random.choice([0, 100], N * N, p=[0.8, 0.2]).reshape(N, N)
        self.timer = QtCore.QTimer()
        self.timer.setInterval(100)
        self.timer.timeout.connect(self.update_plot)
        self.timer.start()
        self.timer_celulas = QtCore.QTimer()
        self.timer_celulas.setInterval(10000)  # 10 segundos
        self.timer_celulas.timeout.connect(self.contador_cell)
        self.timer_celulas.start()
        
        self.serial_port = serial.Serial('COM6', 9600, timeout=1)  # Puerto serial
        self.serial_lock = threading.Lock()
        self.reset_requested = False

        self.umbral_frio = 10
        self.umbral_calor = 20

    def update_plot(self):
        if self.reset_requested:
            self.reset_game()
            self.reset_requested = False
            return

        kernel = np.array([[1, 1, 1],
                        [1, 0, 1],
                        [1, 1, 1]])
        convolved = convolve2d(self.grid, kernel, mode='same', boundary='wrap')

        birth = (convolved == 300) & (self.grid == 0)  # Células muertas con 3 vecinos vivos
        survive = ((convolved == 200) | (convolved == 300)) & (self.grid == 100)  # Células vivas con 2 o 3 vecinos vivos

        self.grid[:, :] = 0  # Resetea la grilla
        self.grid[birth | survive] = 100  # Actualiza con las nuevas células vivas

        # Aplicar reglas de puntos de vida
        self.grid[survive] -= 30  # Reducir puntos de vida para células vivas con 2 o 3 vecinos vivos
        self.grid[birth] = 100  # Renacer células muertas con 100 puntos de vida

        # Ajustar valores mínimos de puntos de vida
        self.grid[self.grid < 0] = 0

        # Generar matriz de colores
        colors = self.map_colors(self.grid)

        self.sc.axes.cla()  # Limpia el gráfico
        self.sc.axes.imshow(colors)  # Muestra la grilla con colores
        self.sc.draw()  # Redibuja el gráfico

        # Reproducir audio y reiniciar el juego si todas las células mueren
        if np.sum(self.grid) == 0:
           playsound('C:/Users/josea/Downloads/aaaa.mp3')  # Reemplaza 'game_over_sound.mp3' con la ruta de tu archivo de audio

        temperatura = self.read_temperature_from_serial()

        if temperatura is not None:
            if temperatura < self.umbral_frio:
                self.grid[self.grid < 100] += 10
            elif temperatura > self.umbral_calor:
                self.grid[self.grid > 0] -= 20

        self.send_data_to_serial()

    def read_temperature_from_serial(self):
        # Leer temperatura desde el puerto serial y convertirla a un valor numérico
        temperatura_str = self.serial_port.readline().decode('utf-8').strip()
        if temperatura_str.startswith('a - '):
            temperatura = int(temperatura_str[2:])
            return temperatura
        return None

    def send_data_to_serial(self):
            # Enviar información sobre células y temperatura al puerto serial
            sobrepoblacion = np.sum((self.grid > 300) & (self.grid == 100))
            bajapoblacion = np.sum((self.grid < 200) & (self.grid == 100))
            estabilidad = np.sum(((self.grid == 200) | (self.grid == 300)) & (self.grid == 100))
            celulasvivas = np.sum(self.grid == 100)

            mensaje = f'{sobrepoblacion}, {bajapoblacion}, {estabilidad}, {celulasvivas}\n'
            print(mensaje)
            self.serial_port.write(mensaje.encode())
    
    def contador_cell(self):
        celulasvivas = np.sum(self.grid == 100)
        self.serial_port.write(f'celulas vivas: {celulasvivas}\n'.encode())

    def reset_game(self):
        N = 100  # Tamaño de la grilla NxN
        self.grid = np.random.choice([0, 100], N * N, p=[0.8, 0.2]).reshape(N, N)

    def map_colors(self, grid):
        colors = np.zeros(grid.shape + (3,), dtype=np.uint8)
        colors[grid == 0] = [255, 255, 255]  # Blanco para puntos de vida 0
        colors[(grid >= 1) & (grid < 20)] = [255, 0, 0]  # Rojo para puntos de vida de 1 a 19
        colors[(grid >= 20) & (grid < 60)] = [0, 0, 255]  # Amarillo para puntos de vida de 20 a 59
        colors[(grid >= 60) & (grid <= 100)] = [0, 255, 0]  # Verde para puntos de vida de 60 a 100
        return colors

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
    w.show()
    
    def handle_serial_data():
        while True:
            try:
                if w.serial_port.inWaiting() > 0:
                    line = w.serial_port.readline().decode('utf-8').strip()
                    print(f"Temperatura: {line}")  # Esto te mostrará exactamente qué está recibiendo Python
                    if line == "reset":
                        print("Señal de reinicio recibida")
                        w.reset_requested = True
            except Exception as e:
                print(f"Error al recibir datos: {e}")
                
    serial_thread = threading.Thread(target=handle_serial_data)
    serial_thread.daemon = True
    serial_thread.start()
    
    app.exec()
