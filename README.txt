Installation des sdk/librairies pour Benchmark
----------------------------------------------

Pour pouvoir develloper avec Benchmark, voud devez realiser les points suivants suivant votre configuration

	Windows :

		OpenMPI :

			prendre le dernier .msi disponible sur ce lien : http://www.mpich.org/downloads/versions/
			et l'installer
			
        OpenCL
        
            pour utiliser OpenCl sous Windows, vous devez installer CUDA si vous avez une carte graphique NVIDIA disponible ici : https://developer.nvidia.com/cuda-downloads
            si vous avez une carte graphique AMD, voud devez installer le sdk AMD pour OpenCL : http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/ 

	Unix :

		OpenMPI :

			Telechager la derniere version de la bibliotheque sur : https://www.open-mpi.org/software/ompi/v2.0/
			ouvrir un terminal et activer le mode : su
			exraire les fichier a l'aide de : tar -xzf Téléchargements/openmpi-2.0.1.tar.gz (le fichier s'apelle comme ca dans mon cas)
			une fois extrait ce placer dans le dossier openmpi-2.0.1 (dans mon cas) avec : cd openmpi-2.0.1
			y cree un fichier build a l'aide de : mkdir build
			se placée dedans a l'aide de : cd build
			executer la commande : ../configure --prexif=/usr/local
			executer la comande : make -j3
			executer : make install

		OpenCL

			pour utiliser OpenCl sous Linux, vous devez installer CUDA si vous avez une carte graphique NVIDIA disponible ici : https://developer.nvidia.com/cuda-downloads