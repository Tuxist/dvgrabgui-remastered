--------------------------------------------------------------------------------
               DVGrabGUI v0.9.4 (c) 2006, 2007, 2008 Bastien Montagne.

Autor:    Bastien Montagne <montagne29@wanadoo.fr>,
          basado en dvgrab, de Dan Dennedy y el equipo de Kino.
Licencia: Distribuido bajo la licencia GNU GPL, v2 o toda otra versión ulterior.

Escríbame para cualquier observación/sugerencia...
Sea simpático, ¡es mi primer programa!
--------------------------------------------------------------------------------

I   - Ayuda
      a) instalación
      b) notas de 'puesta al día'
      c) empleo

II  - Lista todo / Bugs conocidos

III - Historial

---------------------

I - Ayuda:

  a) Instalación:
    Escribí un pequeño script scons para construir/instalar este programa,
    entonces necesitará:
        - El programa Scons;
        - La biblioteca wxWidgets, v.2.6.x / 2.8.x, en una versión dinámica, unicode y
          no-monolítica, con los archivos 'includes' (dev- packages);
        - Las libdv, libiec61883, libraw1394, libavc1394, libz, libdl, libm,
          con sus archivos 'includes' (dev- packages).
    Si cualquiera de las bibliotecas susodichas está instalada en un directorio
    no-estándar, tendrá que modificar su camino en el script SConstruct, excepto
    para wxWidgets:
    	hay la opción 'wxLib' para su camino de bibliotecas,
    	    la opción 'wxInclude' para su camino de includes ,
    	    la opción 'wxSetupInclude' para su camino del archivo 'wx/setup.h'.

    Vaya al directorio en el que decompreso el archivo, y escriba:
        $ scons
        $ su
        # scons install

    El nombre del programa es de tipo 'dvgrabgui(-dbg)-wx-2.x', dependiendo de
    la versión de wxWidgets utilizada, y de una compilación normal o 'debug'
    (vea más abajo).
    
    Nota: Con los sistemas recientes (por ej. Debian Etch, Fedora 7, 8, etc.),
          tiene que ejecutar dvgrabgui como 'root' (parece que los utilizadores
          normales ya no tienen acceso al firewire...).
    
    Para desinstalar, escriba:
        $ su
        # scons -c install
    Anote que los ficheros (de configuración) creados para cada utilizador que
    utilizó este programa (como '/root/.dvgrabgui/*' y '/home/<user>/.dvgrabgui/*')
    no están borrados: tiene que hacerlo ello mismo (? = "yourself"!)

    En rebeldía, dvgrabgui está construido con wxWidgets 2.6.x: si quiere utilizar
    wx-2.8.x, utilice el blanco 'wx-2.8':
        $ scons wx-2.8
        $ su
        # scons install wx-2.8
    y para desinstalar,
        $ su
        # scons -c install wx-2.8

    Si quiere construir un dvgrabgui no-optimizado con debug símbolos, utilice
    el blanco 'debug':
        $ scons debug [wx-2.x]
        $ su
        # scons install debug [wx-2.x]
    y para desinstalar,
        $ su
        # scons -c install debug [wx-2.x]

    ¡Anote que puede construir e instalar dvgrabgui a la vez con wx-2.6 y wx-2.8, y en
    versión normal y 'debug' (cada blanco es construido en un diferente directorio
    en rebeldía, como 'build(-dbg)-wx-2.x', con un diferente nombre, como
    'dvgrabgui(-dbg)-wx-2.x')!

    Puede hacer varias construcciones del programa dando varios caminos via la
    opción 'buildDir'.

    Las pruebas 'como-configure' (comprobando las bibliotecas necesitadas) sólo
    son efectuadas una vez si salen bien; puede forzarlas (o desactivarlas) con la
    opción 'checks'.

    Para las otras opciones (de construcción, de instalación), escriba:
        $ scons --help
    Fíjese bien: las opciones de construcción están escritas en un archivo,
    entonces si tiene que compilar varias veces, ¡no es necesario volver en
    escribirlas cada vez! Para reinicializar estas opciones, borre simplemente
    el(los) fichero(s) 'user_options_wx-2.6.py' y/o 'user_options_wx-2.8.py'
    y/o 'user_options_debug_wx-2.6.py' y/o 'user_options_debug_wx-2.8.py',
    dependiendo de el(los) blanco(s) utilizados/que quiere reinicializar.

	 Para los que estén interesados en el código, antes de leerlo, puede construir
	 la doc generada por doxygen, al escribir:
        $ doxygen
    ¡y abra ./doxydoc/html/index.html!

  b) Notas de 'puesta al día':
    No debería tener problemas: ¡la instalación debería simplemente reemplazar
    los ficheros existentes! Anote, sin embargo, que dvgrabgui ya es INSTALADO
    EN REBELDÍA EN '/USR/LOCAL/', y no más en '/usr', para ser más 'linuxamente-
    correcto'! En consecuencia, debería ajustar la opción 'baseDir' a '/usr' si
    quiere reemplazar la vieja instalación.
    El fichero de configuración en '~/.dvgrabgui' será puesto automáticamente al
    día al primer lanzamiento del nuevo dvgrabgui 0.9.4.

  c) Empleo:
    Escribí este programa porque necesitaba un instrumento para grabar dv, cuya
    función Grabar/Pausa pudiera controlar con los botones del ratón; El uso
    típico es con un PC portátil:
        - Conecte la cámara dv y el ratón externo (Colgué un trozo de cinta
          adhesiva opaca sobre la LED);
        - Lance DVGrabGUI, ajuste los reglajes y lance el thread de captura;
        - Con el touch-pad, ponga el cursor del ratón sobre la ventana de vídeo,
          cierre la pantalla (¡para reservar las baterías!);
        - Pase al modo "pausa" (botones de derecha + izquierda);
        - Para grabar, pulse el botón de izquierda (hay un "beep" sonoro durante
          toda la grabación), pulse el botón de derecha para pausar;
        - Se crea un nuevo archivo vídeo cada vez que pulsa el botón de izquierda;
        - Para dejar el modo "pausa", pulse de nuevo botones de derecha + de
          izquierda;
    Ha sido imaginado para "olvidar" el PC portátil en una mochila (por ejemplo)
    al filmar, y así conservar cierta movilidad.
    Puede registrar sus reglajes preferidos en un archivo, o hacerlos en rebeldía.
    Esta nueva versión también ha sido concebida para las pequeñas pantallas táctil
    (hay un modo de GUI especial para pequeñas pantallas - 800x600).

    ¡Vea la ayuda (F1 desde el GUI) para más detalles!

---------------------

II - Lista todo:

    * Debería ser la última versión de este programa: preveo escribir un nuevo,
      usando GStreamer - ¡debería ser más fácil de mantener/hacer evolucionar!

   - Bugs conocidos:

    * Sobre sistemas recentes, usted debe ser como "root" para usar dvgrabgui
      (los periféricos firewire parecen inutilizables como regular usuario?!?).
    * Si utilice recientes OS (como Fedora 7 o 8), este programa puede no funcionar
      de ninguna manera (como dvgrab): ¡hay un problema con el sistema de firewire!
      Parece que ha sido resollado con Fedora 9 (kernel 2.6.25).
      Anote que esta versión ha sido escrita con Debian Etch (4.0).
    * Si, al lanzar el thread de captura, se para inmediatamente, y hay los mensages
      "ERROR: DV fuente especificada inválida" y
      "ERROR: Imposible inicializar el lector DV",
      hay un problema con su libavc1394: tendrá que elegir el periférico dv en
      la caja de dialogo "Opciones". ("choice box" "GUID")
    * Si, al lanzar el thread de captura, el programa para de repente, intente
      desactivar la 'check box' "Most. vídeo" (hay un problema con su libdv!).

---------------------

III - Historial:

    * v.0.9 (11/2006):
      Primera release.

    * v.0.9.1 (12/2006):
      Añadido el soporte multi-lenguas, e inmediato efecto sobre los atajos
      cuando se carga un nuevo archivo de reglajes.
      Corregido el pequeño uso continuo del procesador debido a la regeneración
      de la interfaz.
      Uñero de atajos escrito en la caja de diálogo de Opciones.

    * v.0.9.2 (12/2006):
      No más problemas con dv_decode_full_frame en la nueva versión de libdv
      (nueva Fedora Core, v6).
      Arreglado el problema con la opción "Num max i. de registrar:".
      Añadido control sobre tipos del GUI.
      Añadido control sobre el sonido "beep" durante la grabación.

    * v.0.9.2b (01/2007):
      Arreglado el problema de construcción con la nueva wxWidgets 2.8.

    * v.0.9.2c (02/2007):
      Ligeras modificaciones, mejora gestión de la ausencia del archivo de
      configuración en rebeldía.
      Primera versión disponible en RPM.

    * v.0.9.3 (04/2008):
      Mayores modificaciones del GUI.
      Se construye a la vez con wxWidgets 2.6.x y 2.8.x.
      Arreglado el problema con el sonido "beep" (wxWidgets parece tener
      problemas con leer sonidos en bucles).
      Anote que el código de captura no fue modificado (es decir que no ha sido puesto 
      al día con dvgrab 3).
      DEB "paquete" (? = "packages")
      Debería ser la última release.

    * v.0.9.4 (08/2008):
      Minores modificaciones en el sistema de compilación/instalación.
      Corrección de un bug en la GUI (en Fedora 9 / wxWidgets 2.8.7): los
      controles creados antes del wxStaticBoxSizer en el que van a ser incluidos
      no son dibujados (aunque ocupen espacio, y acatan como lo deben cuando
      están "cliqueados"!).
