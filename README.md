# MOVIECOLOR
La cosa esta de pillar una peli y ver el color medio de los fotogramas


Está chulo te lo prometo, pero tienes que hacer una cosa. 
Instalar ffmpeg y python, con Pillow

Método de uso
  1. Sacar los fotogramas del video con ffmpeg: 
      ffmpeg -i %VIDEO% -vf "select=eq(pict_type\,I)" -vsync vfr %thumb%04d.jpg% -hide_banner
  
  2. Ejecutar el programa: 
      python main.py %directorio de fotogramas% %archivo de salida%
   
Tarda que flipas, pero acaba estando guay. Por ahora el tamaño de la foto será tanta altura como fotogramas 
haya y anchura de ratio 9:16
  
