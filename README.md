# Chess-engine-
Wasm compiled cpp engine
El ajedrez estará formado por dos partes principales: el motor y la interfaz.
# Interfaz
Se encarga de guardar el estado de las posiciones de las piezas para mostrarlas graficamente y hacer queries al motor para consultar decisiones. Estas son: Consultar movimientos posibles de las piezas (incluido casos especiales como enroque y coronación), mover piezas (y comer a otra si se da el caso).
# Motor
Se encarga de procesar la logica del ajedrez y guardar el estado de cada pieza: su posición, posibles movimientos.
