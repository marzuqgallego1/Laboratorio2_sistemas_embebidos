# Laboratorio2_sistemas_embebidos
juego simon dice realizado con una matriz de leds 8x8 bicolor
Notas iniciales:
Se utilizo una ESP32, una matrriz de leds 8x8 bicolor de anodo comun y 3 notones para la realizacion de este proyecto.

Descripcion del juego:
Es un juego de memoria en el que se debe seguir un patron de colores mostrado por la matriz de leds
El juego comienza en posicion de pausa, en donde se veran 2 columnas encendidas con los lesd verde y rojo. Cuando se presiona
el boton de inicio se genera un patron aleatoreo de 5 posiciones en donde puede aparecer luz verde o luz roja, posterior a eso
el jugador debera ingresar el patron oprimiendo los correspondientes botones (rojo o verde) en un tiempo de 10 segundos, si el paton es incorrecto o se acaba el tiempo antes de que el jugador termine de ingresar el patron entonces el juego volvera a la posicion de pausa. Si por el contrario el patron es correcto, entonces el juego continuara, se creara y se mostrara un nuevo patron pero ahora con una posicion extra,es decir, si antes eran 5 ahora seran 6, esto sera así hasta llegar a 10 posiciones, apartir de alli no se haran incrementos en el patron pero si continuara el juego. 
