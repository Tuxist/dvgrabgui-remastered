--------------------------------------------------------------------------------
               DVGrabGUI v0.9.4 (c) 2006, 2007, 2008 Bastien Montagne.

Auteur:  Bastien Montagne <montagne29@wanadoo.fr>,
         basé sur dvgrab, par Dan Dennedy et l'équipe de Kino.
Licence: Distribué sous la licence GNU GPL, v2 ou toute version ultérieure.

Ecrivez moi pour toute remarque/suggestion...
Soyez sympas, c'est mon premier programme!
--------------------------------------------------------------------------------

I   - Aide de démarrage
      a) installation
      b) notes de mise-à-jour
      c) usage

II  - Liste todo / Bugs connus

III - Historique

---------------------

I - Aide de démarrage:

  a) Installation:
    J'ai juste écrit un petit script scons pour compiler/installer ce programme,
    vous aurez donc besoin:
        - du programme Scons;
        - de la bibliothèque wxWidgets, v.2.6.x / 2.8.x, dans une version dynamique,
          unicode (et non-monolithique), avec les fichiers d'includes (packages
          dev-);
        - des libdv, libiec61883, libraw1394, libavc1394, libz, libdl, libm,
          avec leurs fichiers includes (packages dev-).
    Si l'une des bibliothèques ci-dessus est installée dans un emplacement
    non-standard, vous devrez modifier son chemin dans le script SConstruct, sauf
    pour wxWidgets:
    	vous avez l'option 'wxLib' pour le chemin des bibliothèques,
    	          l'option 'wxInclude' pour le chemin des principaux includes,
    	          l'option 'wxSetupInclude' pour le chemin du fichier 'wx/setup.h'.

    Allez dans le dossier où vous avez décompressé l'archive, et tapez:
        $ scons
        $ su
        # scons install

    Le nom de l'exécutable est de type 'dvgrabgui(-dbg)-wx-2.x', en fonction de
    la version de wxWidgets utilisée, et d'une compilation normale ou 'debug'
    (voyez ci-dessous).
    
    Note: Avec les systèmes récents (par ex. Debian Etch, Fedora 7, 8, etc.), vous
          devez exécuter dvgrabgui en tant que 'root' (les utilisateurs normaux
          n'ont semble-t-il plus accès au firewire...).
    
    Pour désinstaller, tapez:
        $ su
        # scons -c install
    Notez que les fichiers (de configuration) crées pour chaque utilisateur ayant
    lancé ce programme (dans '/root/.dvgrabgui/*' et '/home/<user>/.dvgrabgui/*')
    ne sont pas supprimés: vous devez effacer ces dossiers vous-même.

    Par défaut, dvgrabgui utilise wxWidgets 2.6.x: si vous voulez utiliser
    wx-2.8.x, utilisez la cible 'wx-2.8':
        $ scons wx-2.8
        $ su
        # scons install wx-2.8
    et pour la désinstallation,
        $ su
        # scons -c install wx-2.8

    Si vous voulez construire un dvgrabgui non-optimisé avec ses symboles de
    débugage, utilisez la cible 'debug':
        $ scons debug [wx-2.x]
        $ su
        # scons install debug [wx-2.x]
    et pour la désinstallation,
        $ su
        # scons -c install debug [wx-2.x]
    
    Notez que vous pouvez construire et installer à la fois une version avec
    wx-2.6 et wx-2.8, et à la fois une version normale et 'debug' de dvgrabgui
    (chaque cible étant construite dans un dossier séparé, par défaut, comme
    'build(-dbg)-wx-2.x', avec un nom d'exécutable différent, comme
    'dvgrabgui(-dbg)-wx-2.x')!

    Vous pouvez faire plusieurs compilations en donnant plusieurs répertoires
    via l'option 'buildDir'.

    Les tests 'configure-like' (vérifiant la présence des bibliothèques nécessaires)
    ne sont par défaut effectués qu'une fois s'ils sont passés avec succès; vous
    pouvez forcer (ou désactiver) ces tests par l'option 'checks'.

    Pour les autres options (de compilation, d'installation), tapez:
        $ scons --help

    Notez que ces options sont sauvegardées dans un fichier, vous n'avez donc
    pas besoin de les retaper à chaque fois si vous devez compiler plusieurs
    fois! Pour les ré-initialiser, supprimez simplement le(s) fichier(s)
    'user_options_wx-2.6.py' et/ou 'user_options_wx-2.8.py' et/ou
    'user_options_debug_wx-2.6.py' et/ou 'user_options_debug_wx-2.8.py',
    en fonction de la (des) cible(s) utilisée(s)/que vous voulez réinitialiser.

    Pour ceux intéressés par le code, avant de la lire, vous pouvez construire
    la doc générée par doxygen en tapant:
        $ doxygen
    puis ouvrez ./doxydoc/html/index.html !

  b) Notes de mise-à-jour:
    Vous ne devriez pas avoir de problèmes: l'installation de cette version
    devrait simplement écraser la précédente! Notez, cependant, que dvgrabgui
    est maintenant INSTALLE PAR DEFAUT DANS '/USR/LOCAL/', et plus dans '/usr/',
    pour être plus "linuxement-correct"! Vous devriez en conséquence, pour
    écraser une précédente installation par défaut, régler l'option 'baseDir'
    à '/usr'.
    Les fichiers de configuration sous '~/.dvgrabgui' seront automatiquement
    mis à jour au premier lancement de dvgrabgui 0.9.4.

  c) Usage:
    J'ai écrit ce programme car j'avais besoin d'un outil de capture dv dont je
    puisse contrôler les fonctions REC/PAUSE avec les boutons de la souris;
    l'usage typique est sur un portable:
        - Branchez la caméra dv et la souris externe (collez un morceau de
          Scotch opaque sur la LED);
        - Lancez DVGrabGUI, ajustez les réglages et démarrez le thread de capture;
        - Avec le touch-pad, placez le curseur de la souris dans la fenêtre
          vidéo, refermez l'écran (pour économiser les batteries!);
        - Passez en mode Pause (bouton droit + bouton gauche);
        - Pour enregistrer, pressez le bouton gauche (il y a un 'beep' sonore
          tant que vous enregistrez), pressez le bouton droit pour revenir en
          Pause;
        - Un nouveau fichier vidéo est créé à chaque fois que vous pressez le
          bouton gauche;
        - Pour quitter le mode Pause, re-pressez les boutons droit + gauche;
    Ceci à été imaginé pour pouvoir "oublier" le portable dans un sac à dos
    (par exemple) pendant le tournage, et pour conserver une certaine mobilité.
    Vous pouvez enregistrer vos réglages favoris dans des fichiers, ou les
    activer par défaut.
    Cette nouvelle version a aussi été conçue pour être utilisée avec un petit
    écran tactile (il y a un mode de l'interface dédié aux petits écrans - 800x600).

    Voyez l'aide (F1 dans l'interface) pour plus de détails!    

---------------------

II - Liste todo:

    * Cela devrait être la dernière version de ce programme: je prévois d'en
      développer un nouveau, basé sur GStreamer - il devrait être plus facile à
      maintenir/faire évoluer!

   - Bugs connus:

    * Sur les systèmes récents, vous devez utiliser dvgrabgui comme "root" (les
      périphériques firewire semblent inutilisables par les utilisateurs
      normaux?!?).
    * Si vous utilisez une version récente de votre OS (comme Fedora 7 ou 8),
      le programme risque de ne pas fonctionner du tout (comme dvgrab): il y a
      des problèmes avec le système firewire!
      Cela semble avoir été résolu dans Fedora 9 (noyau 2.6.25).
      Notez que cette version a été développée avec Debian Etch (4.0).
    * Si, lors du démarrage du thread de capture, il s'arrête imédiatement, et
      il y a les messages suivants:
      "ERREUR: source DV spécifiée non valide" et
      "ERREUR: Impossible d'initialiser le lecteur DV",
      il y a un problème avec votre libavc1394: vous devez sélectionnez le
      périphérique dv dans la boîte de dialogue "Options". ("choice box" "GUID")
    * Si, pendant le démarrage du thread de capture, le programme se termine sans
      autre avertissement, essayez de désactiver la "check box" "Aff. vidéo"
      (il y a un problème avec libdv!).
    * Si, pendant l'enregistrement, le programme se termine sans autre
      avertissement, essayez de désactiver la "check box" "Son beep"... ou
      supprimez simplement le fichier "/usr/share/dvgrabgui/beep.wav"!

---------------------

III - Historique:

    * v.0.9 (11/2006):
      Première release.

    * v.0.9.1 (12/2006):
      Ajouté le support multi-langues, et l'application immédiate des raccourcis
      clavier lors du chargement d'un nouveau fichier de configuration.
      Corrigé le léger usage continuel du processeur dû au rafraîchissement de
      l'interface.
      Codé l'onglet Raccourcis de la boîte de dialogue Options.

    * v.0.9.2 (12/2006):
      Plus de problèmes avec dv_decode_full_frame dans la nouvelle libdv
      (nouvelle Fedora Core, v6).
      Réglé le problème de l'option "Nbr max de f. à sauver".
      Ajouté un contrôle sur les polices de la GUI.
      Ajouté un contrôle sur le "beep" sonore lors de l'enregistrement.

    * v.0.9.2b (01/2007):
      Réglé le problème de compilation avec la nouvelle wxWidgets 2.8.

    * v.0.9.2c (02/2007):
      Légères modifications, meilleur gestion de l'absence du fichier de
      configuration par défaut.
      Première version disponible en RPM.

    * v.0.9.3 (04/2008):
      Grosses modifications dans la GUI.
      Utilisable avec wxWidgets 2.6.x et 2.8.x.
      Réglé le problème du son "beep" (wxWidgets semble avoir des problèmes à
      jouer un son en boucles!).
      Notez que le code de capture n'a pas changé (pas d'update d'après dvgrab 3).
      Package DEB.
      Devrait être la dernière release.

    * v.0.9.4 (08/2008):
      Modifications mineures dans le système de compilation/installation.
      Correction d'un bug dans la GUI (sous Fedora 9 / wxWidgets 2.8.7): les
      contrôles créés avant le wxStaticBoxSizer dans lequel ils vont être inclus
      ne sont pas du tout affichés (bien qu'ils occupent de l'espace, et
      réagissent comme attendu quand ils sont cliqués!).
