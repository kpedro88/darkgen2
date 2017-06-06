#!/bin/bash -e

case `uname` in
	Linux) ECHO="echo -e" ;;
	*) ECHO="echo" ;;
esac

usage() {
	$ECHO "setup.sh [options]"
	$ECHO
	$ECHO "Options:"
	$ECHO "-p            \tinstall Pythia 8.226"
	$ECHO "-e            \tlink Pythia 8.226 with HepMC"
	$ECHO "-d            \tinstall Delphes"
	$ECHO "-m            \tinstall Madgraph5_aMC@NLO 2.5.4"
	$ECHO "-n            \tnumber of cores for Madgraph (default=1)"
	$ECHO "-h            \tdisplay this message and exit"
	exit $1
}

CUR_DIR=`pwd`
INSTALL_PYTHIA=""
INSTALL_HEPMC=""
INSTALL_DELPHES=""
INSTALL_MADGRAPH=""
NUMCORES_MADGRAPH=1
# check arguments
while getopts "pedmnh" opt; do
	case "$opt" in
	p) INSTALL_PYTHIA=yes
	;;
	e) INSTALL_HEPMC=yes
	;;
	d) INSTALL_DELPHES=yes
	;;
	m) INSTALL_MADGRAPH=yes
	;;
	n) NUMCORES_MADGRAPH=$OPTARG
	;;
	h) usage 0
	;;
	esac
done

if [ -z "$INSTALL_PYTHIA" ] && [ -z "$INSTALL_DELPHES" ] && [ -z "$INSTALL_MADGRAPH" ]; then
	usage 1
fi

# setup compiler
source /cvmfs/sft.cern.ch/lcg/views/LCG_87/x86_64-slc6-gcc49-opt/setup.sh

if [ -n "$INSTALL_PYTHIA" ]; then
	cd $CUR_DIR
	# get pythia8 source and compile
	wget -q http://home.thep.lu.se/~torbjorn/pythia8/pythia8226.tgz
	tar -xzf pythia8226.tgz
	export PYTHIA8=$CUR_DIR/pythia8226
	export PYTHIA8DATA=$PYTHIA8/share/Pythia8/xmldoc
	cd pythia8226/

	# link to HepMC
	if [ -n "$INSTALL_HEPMC" ]; then
		ln -s /cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/hepmc/2.06.07-cms/lib/libHepMC.a libHepMC.a
		mkdir HepMC
		cd HepMC
		ln -s /cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/hepmc/2.06.07-cms/include/HepMC/* .
		cd ..
		./configure --with-hepmc2
	fi

	# build
	gmake

	# cleanup
	cd $CUR_DIR
	rm pythia8226.tgz
fi

if [ -n "$INSTALL_DELPHES" ]; then
	cd $CUR_DIR
	# get delphes source and compile
	wget -q http://cp3.irmp.ucl.ac.be/downloads/Delphes-3.4.1.tar.gz
	tar -xzf Delphes-3.4.1.tar.gz
	cd Delphes-3.4.1
	make

	# cleanup
	cd $CUR_DIR
	rm Delphes-3.4.1.tar.gz
fi

if [ -n "$INSTALL_MADGRAPH" ]; then
	cd $CUR_DIR
	# get madgraph source and unpack
	wget -q https://launchpad.net/mg5amcnlo/2.0/2.5.x/+download/MG5_aMC_v2.5.4.tar.gz
	tar -xzf MG5_aMC_v2.5.4.tar.gz

	# configure
	cd MG5_aMC_v2_5_4
	echo "set auto_update 0" > mgconfigscript
	echo "set automatic_html_opening False" >> mgconfigscript
	echo "set output_dependencies internal" >> mgconfigscript
	if [ ${NUMCORES_MADGRAPH} -eq 1 ]; then
		echo "set run_mode 0" >> mgconfigscript
	else
		echo "set run_mode 2" >> mgconfigscript
		echo "set nb_core" ${NUMCORES_MADGRAPH} >> mgconfigscript
	fi
	# MG5 pythia8 interface requires HepMC
	if [ -n "$PYTHIA8" ] && [ -f $PYTHIA8/libHepMC.a ]; then
		# hack to put things where MG5 expects them
		mkdir -p $PYTHIA8/share/Pythia8/examples
		sed 's~HEPMC2_INCLUDE=./~HEPMC2_INCLUDE='$PYTHIA8'~; s~HEPMC2_LIB=./~HEPMC2_LIB='$PYTHIA8~' $PYTHIA8/examples/Makefile.inc > $PYTHIA8/share/Pythia8/examples/Makefile.inc
		echo "set pythia8_path" $PYTHIA8 >> mgconfigscript
		echo "set hepmc_path" $PYTHIA8/HepMC >> mgconfigscript
		echo "install mg5amc_py8_interface" >> mgconfigscript
	fi
	echo "save options" >> mgconfigscript
	./bin/mg5_aMC mgconfigscript

	# cleanup
	cd $CUR_DIR
	rm MG5_aMC_v2.5.4.tar.gz
fi
